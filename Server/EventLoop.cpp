#include "EventLoop.hpp"

static void *get_in_addr(struct sockaddr *sa)
{
    if (sa->sa_family == AF_INET) {
        return &(((struct sockaddr_in*)sa)->sin_addr);
    }
    return &(((struct sockaddr_in6*)sa)->sin6_addr);
}


EventLoop::EventLoop(int fd_, EventHandler&& eventHandler) 
: listenfd_(fd_)
, eventHandler_(std::move(eventHandler))
{
        makeNonBlock(listenfd_);
        createEpollFd();
        epollAddFd(listenfd_, EPOLLIN, "listen sock");
}

void EventLoop::createEpollFd() {

    epollfd_ = epoll_create1(0);
    if (epollfd_ == -1) {
        throw std::runtime_error("Epoll create failed");
    }
    events_.resize(MAX_EVENTS);
}

void EventLoop::epollAddFd(int fd, uint32_t option, std::string mes) {

    struct epoll_event ev;
    
    ev.events = option;
    ev.data.fd = fd;
    if (epoll_ctl(epollfd_, EPOLL_CTL_ADD, fd, &ev) == -1) {
        throw std::runtime_error("epoll_ctl: " + mes);
    }
}

void EventLoop::run() {
    while(true) {

        // # 4. Change for epoll_wait
        int nfds = 0;
        nfds = epollWait();
        processEvents(nfds);
        if (events_.size() == nfds) {
            events_.resize(2 * events_.size());
        }

    }
}

int EventLoop::epollWait() {

    int nfds = epoll_wait(epollfd_, events_.data(), events_.size(), -1);
    if (nfds == -1) {
        std::runtime_error("Epoll wait failed");
    }
    return nfds;
}

void EventLoop::processEvents(int nfds) {

    for (int ind = 0; ind < nfds; ++ind) {
        if (events_[ind].data.fd == listenfd_) {
            acceptNewConn();
        } else {
            Conn* conn = connections_[events_[ind].data.fd].get();
            eventHandler_.connection_io(conn);
            if (conn->st_ == Conn::State::END) {
                writeAddr("Connection end: ");
                connections_[events_[ind].data.fd] = nullptr;
            }
        }
    }
}   

void EventLoop::acceptNewConn() {

    int fd_n = acceptCheck();
    makeNonBlock(fd_n);
    if (fd_n >= connections_.size()) {
        connections_.resize(fd_n + 1);
    }
    // Add to connection and add to events_
    connections_[fd_n] = std::make_unique<Conn>(fd_n, Conn::State::REQ);
    epollAddFd(fd_n, EPOLLIN | EPOLLET, "new socket");
}

int EventLoop::acceptCheck() {
    
    socklen_t sz = sizeof(client_addr_);
    int fd_n = accept(listenfd_, (struct sockaddr* )&client_addr_, &sz);
    writeAddr("Connection from: ");
    if (fd_n == -1) {
        throw std::runtime_error("accept failed");
    }
    return fd_n;
}

void EventLoop::writeAddr(std::string str) {
    socklen_t sz = sizeof(client_addr_);
    char s[sz];
    inet_ntop(client_addr_.ss_family, get_in_addr((struct sockaddr *)&client_addr_), s, sz);
    std::cout << str << s << "\n";
}

void EventLoop::makeNonBlock(int fd_n) {
    errno = 0;
    int flags = fcntl(fd_n, F_GETFL, 0);
    if (flags == -1) {
        throw std::runtime_error("retreiving flags failed");
    }
    flags |= O_NONBLOCK;
    int rv = fcntl(fd_n, F_SETFL, flags);
    if (rv == -1) {
        throw std::runtime_error("setting flags failed");
    }
}