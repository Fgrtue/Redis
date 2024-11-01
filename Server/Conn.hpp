#pragma once

#include <cstdint>
#include <array>
#include <unistd.h>
#include "CircBuf.hpp"

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
    State st_ = State::REQ;
    CircBuf<4UL + max_mes> r_buf;
    CircBuf<4UL + max_mes> w_buf;
};