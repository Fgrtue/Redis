#include "Client.hpp"

// Клиент

// Берет сокет, но вместо bind и listen делает connect

// Пишет запросы в буффер

// Получает ответы из буффера

Client::Client(char *addr, const char port[]) {

    getAddr(addr, port);
    setSocket();
}

Client::~Client() {
    freeaddrinfo(ai_);
}

void Client::getAddr(char *addr, const char port[]) {
    struct addrinfo hints;
    std::memset(&hints, 0, sizeof(hints));
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;
    int status = getaddrinfo(addr, port, &hints, &ai_);

    if ( status != 0) {
        throw std::runtime_error("getaddrinfo failed code" + std::string(gai_strerror(status)));
    }
}

void Client::setSocket() {

    struct addrinfo *ptr;
    for (ptr = ai_; ptr != nullptr; ptr = ptr->ai_next) {

        if (!Socket(ptr) || !Connect(ptr)) {
            continue;
        }
        break;
    }
    if (!ptr) {
        throw std::runtime_error("set socket failed");
    }
}

bool Client::Socket(struct addrinfo* addr) {

    conn_.connfd_ = socket(addr->ai_family, addr->ai_socktype, addr->ai_protocol);
    if (conn_.connfd_ == -1) {
        std::cerr << "Creation failed\n";
        return false;
    }
    return true;
}

bool Client::Connect(struct addrinfo* addr) {
    if (connect(conn_.connfd_, addr->ai_addr, addr->ai_addrlen) == -1) {
        std::cerr << "Connection failed\n";
        close(conn_.connfd_);
        conn_.connfd_ = -1;
        return false;
    }
    return true;
}

bool Client::sendReq(std::vector<std::string>& query) {
    conn_.setWriteBufZero();
    if (!fillWbuf(query) || !sendWbuf()) {
        return false;
    }
    return true;
}

bool Client::fillWbuf(std::vector<std::string>& query) {

    int len = 4;
    int n = query.size();
    for (int i = 0; i < query.size(); ++i) {
        len += 4 + query[i].size();
    }
    if (len > Conn::max_mes) {
        return false;
    }
    std::memcpy(&conn_.w_buf[conn_.w_buf_size], &len, 4);
    conn_.w_buf_size += 4;
    std::memcpy(&conn_.w_buf[conn_.w_buf_size], &n, 4);
    conn_.w_buf_size += 4;
    for (int i = 0; i < query.size(); ++i) {
        size_t sz = query[i].size();
        std::memcpy(&conn_.w_buf[conn_.w_buf_size], &sz, 4);
        conn_.w_buf_size += 4;
        std::memcpy(&conn_.w_buf[conn_.w_buf_size], query[i].data(), 4);
        conn_.w_buf_size += query[i].size();
    }
    return true;
}

bool Client::sendWbuf() {
    // while send is non equal to the size of the buffer
    // continue sending bytes
    while (conn_.w_buf_sent < conn_.w_buf_size) {
        int rv = send(conn_.connfd_, &conn_.w_buf[conn_.w_buf_sent], conn_.w_buf_size - conn_.w_buf_sent, 0);
        if (rv < 0) {
            std::cerr << "client send error";
            return false;
        }
        conn_.w_buf_sent += rv;
    }
    return true;
}

std::string Client::readRes() {

    int len = 0;
    if(!readSafe(&len, sizeof(len))) {
        return "";
    }
    if (len > Conn::max_mes) {
        std::cerr << "Response too large\n";
        return "";
    }
    if (len < 4) {
        std::cerr << "Response too short\n";
        return "";
    }
    if(!readSafe(conn_.r_buf.data(), len)) {
        return "";
    }
    int res_code = 0;
    std::string res;
    res.resize(len);
    std::memcpy(&res_code, conn_.r_buf.data(), 4);
    std::memcpy(&res[0], &conn_.r_buf[4], len - 4);
    return std::to_string(res_code) + " " + res;
}

bool Client::readSafe(void *buf, int len) {
    int total_read = 0;
    while (total_read < len) {
        int remain = len - total_read;
        int rv = recv(conn_.connfd_, static_cast<char*>(buf) + total_read, remain, 0);
        if (rv < 0) {
            std::cerr << "client read error\n";
            return false;
        } else if (rv == 0) {
            std::cerr << "unexpected EOF\n";
            return false;
        }
        total_read += rv;
    }
    return true;
}

