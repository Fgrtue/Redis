#pragma once

#include <cstdint>
#include <array>
#include <unistd.h>

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

    void setWriteBufZero();

    ~Conn();

    int connfd_ = -1;
    State st_ = State::REQ;
    std::array<uint8_t, 4UL + max_mes> r_buf;
    uint32_t r_buf_size = 0;
    std::array<uint8_t, 4UL + max_mes> w_buf;
    uint32_t w_buf_size = 0;
    uint32_t w_buf_sent = 0;
};