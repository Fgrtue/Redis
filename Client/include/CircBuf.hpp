#pragma once

#include <cstdint>
#include <array>
#include <unistd.h>
#include <cstring>
#include <sys/socket.h>
#include <iostream>

template <std::size_t SIZE>
struct CircBuf {

    uint32_t totalSZ();

    uint32_t Size();

    uint32_t computeCap(uint32_t, uint32_t);

    ssize_t readNet(int);

    ssize_t sendNet(int);

    void updateStart(size_t);

    void updateEnd(size_t);

    void readFromBuff(void*, size_t);

    void writeToBuff(void*, size_t);

    std::array<uint8_t, SIZE> buf_;
    uint32_t start_ = 0;
    uint32_t end_ = 0;
};


template <std::size_t SIZE>
uint32_t CircBuf<SIZE>::totalSZ() {

    return SIZE;
}

template <std::size_t SIZE>
uint32_t CircBuf<SIZE>::Size() {

    if (start_ <= end_) {
        return end_ - start_;
    } else {
        return buf_.size() - start_ + end_;
    }
}

template <std::size_t SIZE>
uint32_t CircBuf<SIZE>::computeCap(uint32_t a, uint32_t b) {

    if (a < b) {
        return b - a;
    } else {
        return buf_.size() - a;
    }
}

template <std::size_t SIZE>
ssize_t CircBuf<SIZE>::readNet(int fd) {

    // 1. compute cap(end_, start_)
    int cap = computeCap(end_, start_);
    // 2. read cap
    int rv = recv(fd, &buf_[end_], cap, 0);
    // 3. if sent was larger than 0, update the end
    if (rv > 0) {
        updateEnd(rv);
    }
    return rv;
    // 4. return the number of bytes read
}

template <std::size_t SIZE>
ssize_t CircBuf<SIZE>::sendNet(int fd) {

    // 1. compute cap(start_, end_)
    int cap = computeCap(start_, end_);
    // 2. send cap, save the number of sent bytes
    int rv = send(fd, &buf_[start_], cap, 0);
    // 3. if sent was larger than 0, update the start
    if (rv > 0) {
        updateStart(rv);
    }
    // 4. return the number of read 
    return rv;
}

template <std::size_t SIZE>
void CircBuf<SIZE>::updateStart(size_t sz) {

    start_ = (start_ + sz) % SIZE;
}

template <std::size_t SIZE>
void CircBuf<SIZE>::updateEnd(size_t sz) {

    end_ = (end_ + sz) % SIZE;
}

template <std::size_t SIZE>
void CircBuf<SIZE>::readFromBuff(void* data, size_t n) {

    // 1. compute cap(end_, start_)
    uint8_t* buf = static_cast<uint8_t*>(data);
    uint32_t cap = computeCap(end_, start_);
    // 2. if cap >= n
    //    2.1. memcpy cap elements from buf to our buf_
    if (cap < n) {
        std::memcpy(&buf_[end_], buf, cap);
        updateEnd(cap);
        std::memcpy(&buf_[end_], buf + cap, n - cap);
        updateEnd(n - cap);
    } else {
        // 3. else copy cap elements first
        std::memcpy(&buf_[end_], buf, n);
        updateEnd(n);
    }
}

template <std::size_t SIZE>
void CircBuf<SIZE>::writeToBuff(void* data, size_t n) {

    // 1. compute cap(start_, end_)
    uint8_t* buf = static_cast<uint8_t*>(data);
    int cap = computeCap(start_, end_);
    // 2. if cap >= n
    //    2.1. memcpy cap elements from buf to our buf_
    if (cap < n) {
        std::memcpy(buf, &buf_[start_], cap);
        updateStart(cap);
        std::memcpy(buf + cap, &buf_[start_], n - cap);
        updateStart(n - cap);
    } else {
        // 3. else copy n elements first
        std::memcpy(buf, &buf_[start_], n);
        updateStart(n);
    }
    // 4. copy n - cap elements after
    // 5. update start by n
}