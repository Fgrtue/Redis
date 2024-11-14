#include "../include/EventHandler.hpp"
#include <assert.h>
#include <cstring>

void EventHandler::connection_io(Conn* conn) {

    to_initial(conn);

    if (conn_->st_ == Conn::State::REQ) {
        while(try_fill_buffer()) {};
    } else if (conn_->st_ == Conn::State::RES) {
        while(try_flush_buffer()) {};
    } else {
        assert(false);
    }
}

void EventHandler::clearTTL() {
    set.cleanHeap();
}

std::optional<uint64_t> EventHandler::topTTL() {
    return set.minHeap();
}

void EventHandler::to_initial(Conn* conn) {

    rv = 0;
    conn_ = conn;
    r_buf = &conn_->r_buf;
    w_buf = &conn->w_buf;
    out_.clear();
    response_.clear();
    offset_ = 0;
    cmd_.clear();
}

bool EventHandler::try_fill_buffer() {

    read_request();
    if (!check_read()) {
        return false;
    }
    while(try_one_request()) {};
    return conn_->st_ == Conn::State::REQ;
}

// Copy data into the read_buff
void EventHandler::read_request() {

    do {
        rv = r_buf->readNet(conn_->connfd_);
    } while (rv < 0 && errno == EINTR);
}

// Check
// 1. If error occured and error is EAGAIN
// 2. If error occured and we don't know which one was it
// 3. If EOF was read
bool EventHandler::check_read() {

    if (rv < 0 && errno == EAGAIN) 
        return false;
    if (rv < 0) {
        cout << "read error\n";
        conn_->setEnd();
        return false;       
    }
    if (rv == 0) {
        if (r_buf->Size() > 0) {
            cout << "Unexpected EOF\n";
        } else {
            cout << "EOF\n";
        }
        conn_->setEnd();
        return false;
    }
    return true;
}

bool EventHandler::try_one_request() {

    uint64_t len = 0;
    out_.clear();
    if (!check_read_buffer(&len)) {
        return false;
    }
    if (!do_request(len)) {
        conn_->setEnd();
        return false;
    }

    // Start writing Response

    // Start sending Response
    while(try_flush_buffer()) {};
    return (conn_->st_ == Conn::State::REQ);
}

bool EventHandler::check_read_buffer(uint64_t *len) {

    if (r_buf->Size() < 4) {
        return false;
    }
    r_buf->writeToBuff(len, 4);

    // # E1. Exceeded Lenght Buffer
    if (*len > Conn::max_mes) {
        sendErr(1, "Message too long");
        conn_->setEnd();
        return false;
    }
    if (*len > r_buf->Size()) {
        return false;
    }
    return true;
}


bool EventHandler::do_request(uint64_t len) {
// Retreives the message having the length

    // parse requesnt into commands
    // # E2. Bad request
    if (!parseReq(len)) {
        sendErr(2, "Bad request");
        return false;
    } 

    // #2. check if command is keys
    // add do_keys command
    if (cmd_[0] == "GET" && cmd_.size() == 2) {
        do_get();
    } else if (cmd_[0] == "DEL" && cmd_.size() == 2) {
        do_del();
    } else if (cmd_[0] == "SET" && cmd_.size() == 3) {
        do_set();
    } else if (cmd_[0] == "RANGE" && cmd_.size() == 5) {
        do_range();
    // # 1.
    } else if (cmd_[0] == "EXPIRE" && cmd_.size() == 3) {
         do_expire();
    // # 2.
    } else if (cmd_[0] == "TTL" && cmd_.size() == 2) {
         do_ttl(); // --> ttl must be non negative
    } else {
        // E3. Unknown cmd
        outErr(3, "Unknown command");
    }
    return true;
}

bool EventHandler::parseReq(uint64_t len) {

    if (len < 4ul) {
        return false;
    }

    uint32_t n = 0;
    r_buf->writeToBuff(&n, 4);
    len -= 4;
    if (n > 5) {
        return false;
    }
    for (uint32_t count = 0; count < n; ++count) {
        if (static_cast<int64_t>(len) - 4 < 0) {
            return false;
        }
        int32_t sz = 0;
        r_buf->writeToBuff(&sz, 4);
        if (static_cast<int64_t>(len) - 4 - sz < 0) {
            return false;
        }
        std::string command;
        command.resize(sz);
        r_buf->writeToBuff(&command[0], sz);
        cmd_.push_back(command);
        len  = len - 4 - sz;
    }
    if (len != 0) {
        return false;
    }
    return true;
}

void EventHandler::do_range() {

    try {
        double score = std::stod(cmd_[2]);
        int64_t offset = std::stoll(cmd_[3]);
        int64_t limit = std::stoll(cmd_[4]);
        std::vector<std::string_view> keys = set.getRange(cmd_[1], score, offset, limit);
        outArr(keys.size());
        for (size_t ind = 0; ind < keys.size(); ++ind) {
            outStr(keys[ind]);
        }
    } catch (const std::invalid_argument& e) {
        outErr(4, "Range args must be <string> <double> <int64> <int64>");
    }
}

void EventHandler::do_get() {

    std::optional<double> op = set.findSet(cmd_[1]);
    // # Fill NILL
    if (op == std::nullopt) {
        outNil();
    } else {
        outDouble(op.value());
    }
}

void EventHandler::do_del() {

    // # Depending on the result fill INT 0 or 1
    if (set.delSet(cmd_[1])) {
        outInt(1);
    } else {
        outInt(0);
    }
}

void EventHandler::do_set() {

    // # Return NILL
    try {
        double score = std::stod(cmd_[2]);
        set.insertSet(cmd_[1], score);
        outNil();
    } catch (const std::invalid_argument& e) {
        outErr(4, "Set args must be <string> <double>");
    }
}

// # 1. Sets a timer on the key
void EventHandler::do_expire() {

    // Tries to convert the string into number
    try {
        int32_t ttl = std::stoi(cmd_[2]);
        if (ttl < 0) {
            outErr(4, "TTL must be positive int");
        }
        set.setTTL(cmd_[1], ttl);
        outNil();
    } catch (const std::invalid_argument& e) {
        outErr(4, "TTL must be positive int");
    }
    // if manages then goes to the function and outrNil
    // otherwise sends an error back
}

// # 2. Returns the timer of the node
void EventHandler::do_ttl() {
    
    // Goes to set, either finds the number or not
    std::optional<int64_t> ttl = set.getTTL(cmd_[1]);
    if (!ttl.has_value()) {
        outNil();
    } else {
        outInt(ttl.value());
    }
}

void EventHandler::sendErr(int32_t err_no, std::string msg) {
    outErr(err_no, msg);
    fill_write_buffer();
    try_flush_buffer();
}

bool EventHandler::try_flush_buffer() {

    conn_->st_ = Conn::State::RES;
    fill_write_buffer();
    write_response();
    if (!check_write()) {
        return false;
    }
    if (response_.size() == offset_) {
        conn_->setEnd();
        return false;
    }
    return true;
}

void EventHandler::fill_write_buffer() {

    if (!out_.empty()) {
        fill_response();
    }
    uint32_t totCap = w_buf->computeCap(w_buf->end_, w_buf->start_);
    uint32_t needSend = response_.size() - offset_;
    totCap = std::min(totCap, needSend);
    w_buf->readFromBuff(&response_[offset_], totCap);
    offset_ += totCap;
}

void EventHandler::fill_response() {
    int32_t wlen = static_cast<int32_t>(out_.size());
    response_.append((char *)&wlen, 4);
    response_.append(out_);
    out_.clear();
}

// sent the data from the write buffer into the socket
void EventHandler::write_response(){

    do {
        rv = w_buf->sendNet(conn_->connfd_);
    } while (rv < 0 && errno == EINTR);
}

bool EventHandler::check_write() {

    if (rv < 0 && errno == EAGAIN) 
        return false;
    if (rv < 0) {
        cout << "write error\n";
        conn_->setEnd();
        return false;
    }
    return true;
}

void EventHandler::outNil() {

    out_.push_back(static_cast<char>(SER::NIL));
}

void EventHandler::outErr(int32_t code, const std::string& msg) {

    out_.push_back(static_cast<char>(SER::ERR));
    out_.append((char *)&code, 4);
    uint32_t len = msg.size();
    out_.append((char *)&len, 4);
    out_.append(msg);
}

void EventHandler::outStr(const std::string_view str) {

    out_.push_back(static_cast<char>(SER::STR));
    uint32_t len = static_cast<uint32_t>(str.size());
    out_.append((char *)&len, 4);
    out_.append(str);
}

void EventHandler::outInt(uint64_t num) {

    out_.push_back(static_cast<char>(SER::INT));
    out_.append((char *)&num, 8);
}

void EventHandler::outDouble(double score) {

    out_.push_back(static_cast<char>(SER::DOUB));
    std::string doub = std::to_string(score);
    uint32_t len = static_cast<uint32_t>(doub.size());
    out_.append((char *)&len, 4);
    out_.append(doub);
}

void EventHandler::outArr(uint64_t n) {
    out_.push_back(static_cast<char>(SER::ARR));
    out_.append((char *)&n, 8);
}

/*

// nil      -> [ mes_len ] [ Ser Code ]
// err      -> [ mes_len ] [ Ser Code ] [ err code ] [ err_mes ]
// int      -> [ mes_len ] [ Ser Code ] [ int ]
// string   -> [ mes_len ] [ Ser Code ] [ str ]
// array    -> [ mes_len ] [ Ser Code ] [ arr_len ] [ Ser Code1 ] [ str/int ] ... [Ser Code n] [ str/len ] 


# 3. Add function for processing different serialization codes 
 1. Nil code
    -> add just ser code
 2. Error code
    -> add ser code
    -> add the code itself
    -> add the len of message
    -> add the message itself
 3. out_str
    -> add ser code
    -> add len of string
    -> add string itself
 4. out_int
    -> add ser code
    -> add the int
 5. out_arr
    -> add ser code
    -> add length of array

# 4. Depending on command we receive one of the codes

    -> Nil code is obtained in case of succes of insert
    -> Err code is obtained in case something went wrong
    -> Int code is obtained in case of deletion (if it happened or not)
    -> Str code is obtained in case of a get
    -> Arr is obtained in case of keys, and followed by the keys

    Once we have the response line we just have to add the lenght for the line
    and then add just the string to our response --> in fact nothing changes!

# 5. We need to add a function for processing key command
    -> it will walk through the hm1 and collect all the keys in this map
    -> it will walk through the hm2 and collect all the keys in it
    -> the keys are put in a vector of intergers, and then returned
    -> then we apply out_arr
    -> and walk through the array, applying out_int

# 6. Change all the places in the code where serialization code must
    be returned

# 7. Add function for decoding the result from the side of the client
    -> when received a buffer, decode the first Ser code
    -> depending on the number apply the function for decoding
    -> let it work recursively
*/