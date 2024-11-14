#pragma once

#include <cstdint>
#include <array>
#include <unistd.h>
#include "CircBuf.hpp"
#include "TList.hpp"

struct Conn
{
    static constexpr const uint64_t max_mes = 1024UL;

    enum class State {
        REQ,
        RES,
        END
    };

    Conn() = default;

    Conn(int fd, State st);
    
    void setEnd();

    ~Conn();

    int connfd_ = -1;
    uint64_t startT = 0;
    State st_ = State::REQ;
    TNode    timer_;
    CircBuf<4UL + max_mes> r_buf;
    CircBuf<4UL + max_mes> w_buf;
    struct sockaddr_storage addr_;
};