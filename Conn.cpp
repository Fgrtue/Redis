#include "Conn.hpp"

Conn::Conn(int fd, State st)
    : connfd_(fd)
    , st_(st)
    {}

Conn::~Conn() {
    if (connfd_ != -1) {
        close(connfd_);
    } 
}

void Conn::setWriteBufZero() {
    w_buf_size = 0;
    w_buf_sent = 0;
}

void Conn::setEnd() {
    st_ = State::END;
}

