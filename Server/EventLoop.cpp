#include "EventLoop.hpp"

static constexpr uint64_t idleTimeout = 5000;

static void *get_in_addr(struct sockaddr *sa)
{
    if (sa->sa_family == AF_INET) {
        return &(((struct sockaddr_in*)sa)->sin_addr);
    }
    return &(((struct sockaddr_in6*)sa)->sin6_addr);
}

EventLoop::EventLoop(int fd_) 
: listenfd_(fd_)
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

        int nfds = 0;
        nfds = epollWait();
        processEvents(nfds);
        if (events_.size() == nfds) {
            events_.resize(2 * events_.size());
        }
    }
}

int EventLoop::epollWait() {

    // # 2. nextTimerMS()
    int timeout = nextTimerMS(); 
    int nfds = epoll_wait(epollfd_, events_.data(), events_.size(), timeout);
    if (nfds == -1) {
        std::runtime_error("Epoll wait failed");
    }
    return nfds;
}

void EventLoop::processEvents(int nfds) {

    for (int ind = 0; ind < nfds; ++ind) {
        int fileDesc = events_[ind].data.fd;
        if (fileDesc == listenfd_) {
            acceptNewConn();
        } else {
            Conn* conn = connections_[fileDesc].get();
            timers_.pop(&conn->timer_);
            timers_.pushBack(&conn->timer_);
            conn->startT = getMonotonicTime();
            eventHandler_.connection_io(conn);
            if (conn->st_ == Conn::State::END) {
                writeAddr("Connection end: ", conn);
                connEnd(conn);
            }
        }
    }
    // # 3. process timers
    processTimers();
}   

void EventLoop::acceptNewConn() {

    int fd_n = acceptCheck();
    makeNonBlock(fd_n);
    if (fd_n >= connections_.size()) {
        connections_.resize(fd_n + 1);
    }

    // Add to connection and add to events_
    connections_[fd_n] = std::make_unique<Conn>(fd_n, Conn::State::REQ);
    std::memcpy(&connections_[fd_n]->addr_, &client_addr_, sizeof(client_addr_));
    writeAddr("Connection from: ", connections_[fd_n].get());

    // # 4. Add connection ptr to the back of the queue
    connections_[fd_n]->startT = getMonotonicTime();
    timers_.pushBack(&connections_[fd_n]->timer_);
    epollAddFd(fd_n, EPOLLIN | EPOLLET, "new socket");
}

int EventLoop::acceptCheck() {
    
    socklen_t sz = sizeof(client_addr_);
    int fd_n = accept(listenfd_, (struct sockaddr* )&client_addr_, &sz);
    if (fd_n == -1) {
        throw std::runtime_error("accept failed");
    }
    return fd_n;
}

void EventLoop::writeAddr(std::string str, Conn* conn) {
    socklen_t sz = sizeof(conn->addr_);
    char s[sz];
    inet_ntop(conn->addr_.ss_family, get_in_addr((struct sockaddr *)&conn->addr_), s, sz);
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

void EventLoop::connEnd(Conn* conn) {
    timers_.pop(&conn->timer_);
    connections_[conn->connfd_] = nullptr;
}

int EventLoop::nextTimerMS() {
    // Case when list is empty
    if (timers_.empty()) {
        return 1000;
    }

    // get current time
    uint64_t curTime = getMonotonicTime();
    
    // get time of expiration for the nearest timer
    uint64_t nextTimer = timers_.front()->startT + idleTimeout * 1000;
    
    // in case the timer has already expired -> return 0
    if (nextTimer <= curTime) {
        return 0;
    }

    // otherwise return miliseconds as a differnce in time
    return (static_cast<int>(nextTimer - curTime)) / 1000;

}

void EventLoop::processTimers() {
    // get current time
    uint64_t curTimeUS = getMonotonicTime();
    // continue until the list is not empty

    while (!timers_.empty()) {
    // if the current time + 1000 is less than the time of timer expiration --> we stop
    // otherwise finish connection with printing the appropriate message
        Conn* conn = timers_.front();
        uint64_t nextTimerUS = conn->startT + idleTimeout * 1000;
        if (curTimeUS + 1000 <= nextTimerUS) {
            break ;
        }
        writeAddr("Idle connection end: ", conn);
        connEnd(conn);
    }
}

inline uint64_t EventLoop::getMonotonicTime() {
    auto now = std::chrono::steady_clock::now().time_since_epoch();
    return std::chrono::duration_cast<std::chrono::microseconds>(now).count();
}
