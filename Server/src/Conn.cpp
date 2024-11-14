#include "../include/Conn.hpp"

Conn::Conn(int fd, State st)
: connfd_(fd)
, st_(st)
, timer_(this)
{}

Conn::~Conn() {
    if (connfd_ != -1) {
        close(connfd_);
    } 
}

void Conn::setEnd() {
    st_ = State::END;
}

