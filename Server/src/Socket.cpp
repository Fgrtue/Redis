#include "../include/Socket.hpp"

ListenSocket::ListenSocket(const char* port) {

    struct addrinfo hints = {};
    
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_flags = AI_PASSIVE;
    recAddrInfo(port, &hints);
    recSocket();
    sockListen();
}

void ListenSocket::recAddrInfo(const char *service, 
                 const struct addrinfo *hints) {
    int status = getaddrinfo(nullptr, service, hints, &ai_);
    if (status != 0) {
        throw std::runtime_error("getaddrinfo failed: code " + std::string(gai_strerror(status)));
    }
}

void ListenSocket::recSocket() {

    struct addrinfo *ptr;
    for (ptr = ai_; ptr != nullptr; ptr = ptr->ai_next) {
        try {
            createSocket();
            setSocketOption();
            bindSocket();
            break;
        } catch (const std::runtime_error& e) {
            std::cerr << e.what() << std::endl;
            if (std::string(e.what()).find("setsockopt failed") != std::string::npos) {
                throw std::runtime_error("setsockopt failed");
            }
        }
    }
    if (ptr == nullptr) {
        throw std::runtime_error("connection failed");
    }
}

void ListenSocket::createSocket() {

    sockfd_ = socket(ai_->ai_family, ai_->ai_socktype, ai_->ai_protocol);
    if (sockfd_ == -1) {
        throw std::runtime_error("socket creation failure");
    }
}

void ListenSocket::setSocketOption() {

    int yes = 1;
    if (setsockopt(sockfd_, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(int)) == -1) {
        throw std::runtime_error("setsockopt failed");
    }
}

void ListenSocket::bindSocket() {

    if (bind(sockfd_, ai_->ai_addr, ai_->ai_addrlen) == -1) {
        close(sockfd_);
        throw std::runtime_error("bind failed");
    }
}

void ListenSocket::sockListen() {

    if (listen(sockfd_, BACKLOG) == -1) {
        throw std::runtime_error("listen failed");
    }
}
