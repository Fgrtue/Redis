#pragma once

#include <sys/socket.h>
#include <poll.h>
#include <vector>
#include <memory>
#include <cstdint>
#include <unistd.h>
#include <string>
#include <fcntl.h>
#include <stdexcept>
#include "EventHandler.hpp"

// Manages the core loop and registers event handlers

// 1. Rgisters file descriptors (sockets)
// 2. Uses polling to determine which descriptors are ready
// 3. Delegates events to appropriate handlers

// Basically the class that gets into infinite while loop,
// polls on the file descriptors, and in case an event happens
// handles it with the right function

struct Conn
{
    static constexpr const uint64_t max_mes = 1024UL;

    enum class State {
        REQ,
        RES,
        END
    };

    Conn() = default;

    Conn(int fd, State st)
    : connfd_(fd)
    , st_(st)
    {}

    ~Conn() {
        if (connfd_ != -1) {
            close(connfd_);
        } 
    }

    int connfd_ = -1;
    State st_ = State::REQ;
    std::array<uint8_t, max_mes> r_buf;
    uint32_t r_buf_size = 0;
    std::array<uint8_t, max_mes> w_buf;
    uint32_t w_buf_size = 0;
    uint32_t w_buf_send = 0;
};

class EventLoop {

public:

    EventLoop(int fd_) 
    : listenfd_(fd_)
    {
        makeNonBlock(listenfd_);
    }

    void run();

private:

    // renew the pfds
    // add the listening socket
    // +
    // Walk through connections
    //    for each file descriptor
    //    check what that connection is non null
    //    in this case fill the pollfd structure
    //    and add it to pfds_

    void updatePfds();

    // pollFd
    // A simple function for cheching an error

    void pollCheck();

    void checkReady();

    // acceptNewConn
    // safely accept a file descriptor
    // make it non-blocking
    // in case the fd number is too large for the vector
    // resize more space for vector
    // put it into vector;

    void acceptNewConn();

    // Just a funcion for error-prone accept

    int acceptCheck();
    
    // Making the file descriptor non-blocking
    // Do the same fo listening fd
    // 

    void makeNonBlock(int fd);

    int listenfd_;
    std::vector<struct pollfd> pfds_ = {{listenfd_, POLLIN, 0}};
    std::vector<std::unique_ptr<Conn>> connections_;
    struct sockaddr_storage client_addr_;
    EventHandler eventHandler;
};