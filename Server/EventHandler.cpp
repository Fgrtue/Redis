#include "EventHandler.hpp"
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

void EventHandler::to_initial(Conn* conn) {

    rv = 0;
    wlen_ = 0;
    rescode_ = Res::OK;
    conn_ = conn;
    r_buf = &conn_->r_buf;
    w_buf = &conn->w_buf;
    res_value.clear();
    cmd_.clear();
}

bool EventHandler::try_fill_buffer() {

    // 0. readReq
    read_request();
    if (!check_read()) return false;
    // # 3.r_buf->UpdateEnd(rv); -- No need -> we'll do it in function automatically while reading
    while(try_one_request()) {};
    return conn_->st_ == Conn::State::REQ;
}

// Copy data into the read_buff
void EventHandler::read_request() {
    // ### Add a function for computing cap depending on the size available in buffer

    do {
        // FUNCTION: 
        // r_buf->readNet(conn_->connfd_);
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
        // # 2. conn_->r_buf.Size();
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

    int len = 0;

    if (!check_read_buffer(&len)) {
        return false;
    }
    if (!do_request(len)) {
        conn_->setEnd();
        return false;
    }

    fill_write_buffer(len);

    conn_->st_ = Conn::State::RES;
    while(try_flush_buffer()) {};
    return (conn_->st_ == Conn::State::REQ);
}

bool EventHandler::check_read_buffer(int *len) {

    // # 4. r_buf->Size();
    if (r_buf->Size() < 4) {
        return false;
    }
    // # 5. r_buf->fill_buf(len, 4);
    r_buf->writeToBuff(len, 4);
    if (*len > Conn::max_mes) {
        cout << "message too long\n";
        conn_->setEnd();
        return false;
    }
    // # 5.1 r_buf->Size();
    if (*len > r_buf->Size()) {
        return false;
    }
    return true;
}


// Having the length of the following message
// it must retreive this message
bool EventHandler::do_request(int len) {

    // parse requesnt into commands
    if (!parseReq(len)) {
        cout << "Bad req\n";
        return false;
    } 

    // depending on the string, choose one of 3 options

    if (cmd_[0] == "get" && cmd_.size() == 2) {
        do_get();
    } else if (cmd_[0] == "del" && cmd_.size() == 2) {
        do_del();
    } else if (cmd_[0] == "set" && cmd_.size() == 3) {
        do_set();
    } else {
        rescode_ = Res::ERR;
    }
    return true;
}

bool EventHandler::parseReq(int len) {

    if (len < 4) {
        return false;
    }

    u_int32_t n = 0;
    // # 6. r_buf->write_to_buf(&n, 4);
    r_buf->writeToBuff(&n, 4);
    len -= 4;
    if (n > 3) {
        return false;
    }
    for (int count = 0; count < n; ++count) {
        if (len - 4 < 0) {
            return false;
        }
        int sz = 0;
        // # 7. r_buf->write_to_buf(&sz, 4);
        r_buf->writeToBuff(&sz, 4);
        if (len - 4 - sz < 0) {
            return false;
        }
        // # 8. r_buf->write_to_buf(&string, sz);
        std::string command;
        command.resize(sz);
        r_buf->writeToBuff(&command[0], sz);
        cmd_.push_back(command);
        len  = len - 4 - sz;
    }
    // Find a way to compare how much byte we sent
    if (len != 0) {
        return false;
    }
    return true;
}

void EventHandler::do_get() {

    if (g_map.find(cmd_[1]) == g_map.end()) {
        rescode_ = Res::NX;
    }
    assert(cmd_[1].size() <= Conn::max_mes);
    res_value = g_map[cmd_[1]];
    // # 9. w_buf->read_to_buf(val.data(), val.size()); || save this variable as additional information 
    // std::memcpy(&conn_->w_buf[8], val.data(), val.size());
    wlen_ += res_value.size();
}

void EventHandler::do_del() {

    g_map.erase(cmd_[1]);
}

void EventHandler::do_set() {

    g_map[cmd_[1]] = cmd_[2];
}

void EventHandler::fill_write_buffer(int len) {

    wlen_ += 4;
    int code;
    switch (rescode_)
    {
        case Res::OK:
            code = 0;
            break;
        case Res::ERR:
            code = 1;
            break;
        case Res::NX:
            code = 2;
            break;
    }
    // 10. w_buf->read_to_buf(&wlen, 4)
    w_buf->readFromBuff(&wlen_, 4);
    // 11. w_buf->read_to_buf(&wlen, 4)
    w_buf->readFromBuff(&code, 4);
    // +. w_buf->read_to_buf(&savedinfo, size_info);
    if (res_value.size() != 0) {
        w_buf->readFromBuff(&res_value[0], res_value.size());
    }

    // 12. r_buf->UpdateStart(w_buf->Size());
}

bool EventHandler::try_flush_buffer() {

    write_response();
    if (!check_write()) return false;
    if (w_buf->start_ == w_buf->end_) {
        conn_->setEnd();
        return false;
    }
    return true;
}

// sent the data from the write buffer into the socket
void EventHandler::write_response(){

    // 13. w_buf->senNet(conn_->connfd_);
    static int count = 1; 
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
