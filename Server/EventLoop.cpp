#include "EventLoop.hpp"

static void *get_in_addr(struct sockaddr *sa)
{
    if (sa->sa_family == AF_INET) {
        return &(((struct sockaddr_in*)sa)->sin_addr);
    }
    return &(((struct sockaddr_in6*)sa)->sin6_addr);
}

void EventLoop::run() {
    while(true) {
        updatePfds();
        checkReady();
        acceptNewConn();
    }
}

void EventLoop::updatePfds() {

    pfds_.clear();
    pfds_.push_back({listenfd_, POLLIN, 0});
    for (auto& conn : connections_) {
        if (conn.get() != nullptr) {
            struct pollfd pfd_new = {};
            pfd_new.fd = conn->connfd_;
            pfd_new.events = conn->st_ == Conn::State::REQ ? POLLIN : POLLOUT;
            pfd_new.events |= POLLERR;
            pfds_.push_back(pfd_new);
        }
    }
}

void EventLoop::checkReady() {

    pollCheck();
    for (int ind = 1; ind < pfds_.size(); ++ind) {
        if (pfds_[ind].revents) {
            Conn* conn = connections_[pfds_[ind].fd].get();
            eventHandler_.connection_io(conn);
            if (conn->st_ == Conn::State::END) {
                connections_[pfds_[ind].fd] = nullptr;
            }
        }
    }
}

void EventLoop::pollCheck() {

    int rv = poll(pfds_.data(), (nfds_t)pfds_.size(), 1000);
    if (rv < 0) {
        throw std::runtime_error("poll failed");
    }
}

void EventLoop::acceptNewConn() {
    if (pfds_[0].revents == 0) return;

    int fd_n = acceptCheck();
    makeNonBlock(fd_n);
    if (fd_n >= connections_.size()) {
        connections_.resize(fd_n + 1);
    }
    connections_[fd_n] = std::make_unique<Conn>(fd_n, Conn::State::REQ);
}

int EventLoop::acceptCheck() {
    
    socklen_t sz = sizeof(client_addr_);
    int fd_n = accept(listenfd_, (struct sockaddr* )&client_addr_, &sz);
    writeAccepted();
    if (fd_n == -1) {
        throw std::runtime_error("accept failed");
    }
    return fd_n;
}

void EventLoop::writeAccepted() {
    socklen_t sz = sizeof(client_addr_);
    char s[sz];
    inet_ntop(client_addr_.ss_family, get_in_addr((struct sockaddr *)&client_addr_), s, sz);
    std::cout << "Connection from: " << s << "\n";
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