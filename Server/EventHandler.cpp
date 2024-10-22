#include "EventHandler.hpp"
#include <assert.h>
#include <cstring>


void EventHandler::connection_io(Conn* conn) {
    to_initial();
    conn_ = conn;
    if (conn_->st_ == Conn::State::REQ) {
        while(try_fill_buffer()) {};
    } else if (conn_->st_ == Conn::State::RES) {
        while(try_flush_buffer()) {};
    } else {
        assert(false);
    }
}

void EventHandler::to_initial() {

    rv = 0;
    wlen_ = 0;
    rescode_ = Res::OK;
    conn_ = nullptr;
    cmd_.clear();
}

bool EventHandler::try_fill_buffer() {

    read_request();
    if (!check_read()) return false;
    // ### Add a function for updating the end of the buffer
    conn_->r_buf_size += rv;
    while(try_one_request()) {};
    return conn_->st_ == Conn::State::REQ;
}

// Copy data into the read_buff
void EventHandler::read_request() {
    // ### Add a function for computing cap depending on the size available in buffer
    do {
        size_t cap = conn_->r_buf.size() - conn_->r_buf_size;
        rv = recv(conn_->connfd_, &conn_->r_buf[conn_->r_buf_size], cap, 0);
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
        if (conn_->r_buf_size > 0) {
            cout << "Unexpected EOF\n";
        } else {
            cout << "EOF\n";
        }
        conn_->setEnd();
        return false;
    }
    return true;
}

bool EventHandler::try_flush_buffer() {

    write_response();
    if (!check_write()) return false;
    conn_->w_buf_sent += rv;
    assert(conn_->w_buf_sent <= conn_->w_buf_size);
    if (conn_->w_buf_sent == conn_->w_buf_size) {
        conn_->setWriteBufZero();
        conn_->setEnd();
        return false;
    }
    return true;
}

// sent the data from the write buffer into the socket
void EventHandler::write_response(){

    do {
        size_t remain = conn_->w_buf_size - conn_->w_buf_sent;
        rv = send(conn_->connfd_, &conn_->w_buf[conn_->w_buf_sent], remain, 0);
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

bool EventHandler::try_one_request() {

    int len = 0;

    if (!check_read_buffer(&len)) {
        return false;
    }
    uint32_t pos = 4;
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

    // ### Add a fucntion for computing what's the actual size of buffer
    if (conn_->r_buf_size < 4) {
        return false;
    }
    // 
    std::memmove(len, conn_->r_buf.data(), 4);
    if (*len > Conn::max_mes) {
        cout << "message too long\n";
        conn_->setEnd();
        return false;
    }

    if (4 + *len > conn_->r_buf_size) {
        return false;
    }
    return true;
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
    std::memcpy(conn_->w_buf.data(), &wlen_, 4);
    std::memcpy(&conn_->w_buf[4], &code, 4);
    conn_->w_buf_size = wlen_ + 4;
    size_t remain = conn_->r_buf_size - 4 - len;
    
    if (remain) {
        std::memmove(conn_->r_buf.data(), &conn_->r_buf[4 + len], remain);
    }

    conn_->r_buf_size = remain;
}


// Having the length of the following message
// it must retreive this message
bool EventHandler::do_request(int len) {

    // parse requesnt into commands
    if (!parseReq(len, 4U)) {
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

bool EventHandler::parseReq(int len, uint32_t pos) {

    if (len < 4) {
        return false;
    }

    u_int32_t n = 0;
    std::memcpy(&n, &conn_->r_buf[pos], 4);
    if (n > 3) {
        return false;
    }
    pos += 4;
    for (int count = 0; count < n; ++count) {
        if (pos + 4 > len + 4) {
            return false;
        }
        uint32_t sz = 0;
        std::memcpy(&sz, &conn_->r_buf[pos], 4);
        if (pos + 4 + sz > len + 4) {
            return false;
        }
        cmd_.emplace_back(std::string((char *)&conn_->r_buf[pos + 4], sz));
        pos += 4 + sz;
    }
    if (pos != len + 4) {
        return false;
    }
    return true;
}

void EventHandler::do_get() {

    if (g_map.find(cmd_[1]) == g_map.end()) {
        rescode_ = Res::NX;
    }
    assert(cmd_[1].size() <= Conn::max_mes);
    std::string val = g_map[cmd_[1]];
    std::memcpy(&conn_->w_buf[8], val.data(), val.size());
    wlen_ += val.size();
}

void EventHandler::do_del() {

    g_map.erase(cmd_[1]);
}

void EventHandler::do_set() {

    g_map[cmd_[1]] = cmd_[2];
}
