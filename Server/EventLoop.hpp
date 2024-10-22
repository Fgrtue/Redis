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
#include <iostream>
#include <arpa/inet.h>
#include "EventHandler.hpp"
#include "Conn.hpp"

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

    EventLoop(int fd_, EventHandler&& eventHandler) 
    : listenfd_(fd_)
    , eventHandler_(std::move(eventHandler))
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

    void writeAccepted();
    
    // Making the file descriptor non-blocking
    // Do the same fo listening fd
    // 

    void makeNonBlock(int fd);

    int listenfd_;
    std::vector<struct pollfd> pfds_;
    std::vector<std::unique_ptr<Conn>> connections_;
    struct sockaddr_storage client_addr_;
    EventHandler eventHandler_;
};