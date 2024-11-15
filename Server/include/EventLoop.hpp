#pragma once

#include <sys/socket.h>
#include <sys/epoll.h>
#include <vector>
#include <memory>
#include <cstdint>
#include <unistd.h>
#include <string>
#include <fcntl.h>
#include <stdexcept>
#include <iostream>
#include <arpa/inet.h>
#include "Clock.hpp"
#include "EventHandler.hpp"
#include "Conn.hpp"
#include "TList.hpp"

// Manages the core loop and registers event handlers

// 1. Rgisters file descriptors (sockets)
// 2. Uses polling to determine which descriptors are ready
// 3. Delegates events to appropriate handlers

// Basically the class that gets into infinite while loop,
// polls on the file descriptors, and in case an event happens
// handles it with the right function

/*
while work_not_done(fd):
    do_something_to_client(fd)

def do_something_to_client(fd):
    if should_read_from(fd):
        data = read_until_EAGAIN(fd)
        process_incoming_data(data)
    while should_write_to(fd):
        write_until_EAGAIN(fd)
    if should_close(fd):
        destroy_client(fd)
*/

class EventLoop {

public:

    EventLoop(int fd_);

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

    // # 6. Delete this function: no need, since epoll_wait does this automatically
    //   Create epollWait();

    // void updatePfds();

    void createEpollFd();

    void epollAddFd(int, uint32_t, std::string);

    int epollWait();

    // pollFd
    // A simple function for cheching an error

    // # 8. Delete pollCheck

    // void pollCheck();

    void processEvents(int);


    // acceptNewConn
    // safely accept a file descriptor
    // make it non-blocking
    // in case the fd number is too large for the vector
    // resize more space for vector
    // put it into vector;

    void acceptNewConn();

    // Just a funcion for error-prone accept

    int acceptCheck();

    void writeAddr(std::string str, Conn* conn);
    
    // Making the file descriptor non-blocking
    // Do the same fo listening fd

    void makeNonBlock(int fd);

    void connEnd(Conn* conn);

    // Timer jobs

    int nextTimerMS();

    void processTimers();

    std::optional<uint64_t> topTTL();

    void clearTTL();

    int listenfd_;

    int epollfd_;

    int MAX_EVENTS = 64;
    std::vector<struct epoll_event>     events_;

    TList                               timers_;
    std::vector<std::unique_ptr<Conn>>  connections_;

    EventHandler                        eventHandler_;
    struct sockaddr_storage             client_addr_;
};