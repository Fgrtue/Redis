#include "../include/Client.hpp"
// Клиент

// Берет сокет, но вместо bind и listen делает connect

// Пишет запросы в буффер

// Получает ответы из буффера

using std::cout;
using std::cin;

Client::Client(const char addr[], const char port[]) {

    getAddr(addr, port);
    setSocket();
}

Client::~Client() {
    freeaddrinfo(ai_);
}

void Client::getAddr(const char addr[], const char port[]) {
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
    if (!fillWbuf(query) || !sendWbuf()) {
        return false;
    }
    return true;
}

bool Client::fillWbuf(std::vector<std::string>& query) {

    uint32_t len = 4;
    int32_t n = query.size();
    for (size_t i = 0; i < query.size(); ++i) {
        len += 4 + query[i].size();
    }
    if (len > Conn::max_mes) {
        return false;
    }
    conn_.w_buf.readFromBuff(&len, 4);
    conn_.w_buf.readFromBuff(&n, 4);
    for (size_t i = 0; i < query.size(); ++i) {
        int32_t sz = query[i].size();
        conn_.w_buf.readFromBuff(&sz, 4);
        conn_.w_buf.readFromBuff(query[i].data(), sz);
    }
    return true;
}

bool Client::sendWbuf() {
    // while send is non equal to the size of the buffer
    // continue sending bytes
    while (conn_.w_buf.Size()) {
        ssize_t rv = conn_.w_buf.sendNet(conn_.connfd_);
        if (rv < 0) {
            std::cerr << "client send error";
            return false;
        }
    }
    return true;
}

std::string Client::readRes() {

    int len = 0;
    // read the length of message
    ssize_t rv = recv(conn_.connfd_, &len, 4, 0);
    if (rv < 0) {
        cout << "Client read error";
        return "";
    }
    if (rv == 0) {
        cout << "Unexpected EOF";
        return "";
    }
    std::string response;
    response.resize(len);
    int read = 0;
    do {
        rv = recv(conn_.connfd_, &response[read], len - read, 0);
        if (rv < 0) {
            cout << "Client read error";
            return "";
        }
        if (rv == 0) {
            cout << "Unexpected EOF";
            return "";
        }
        read += rv;
    } while (read != len);
    return response;
}

size_t Client::decode(const std::string& res, size_t pos) {

    SER op = static_cast<SER>(res[pos]);
    ++pos;
    if (op == SER::NIL) {
        cout << "nil\n";
    } else if (op == SER::INT) {
        cout << "Int: ";
        int64_t num = 0;
        memcpy(&num, &res[pos], 8);
        pos += 8;
        cout << num << "\n";
    } else if (op == SER::STR) {
        cout << "Str: ";
        int32_t len = 0;
        memcpy(&len, &res[pos], 4);
        pos += 4;
        std::string str;
        str.resize(len);
        memcpy(str.data(), &res[pos], len);
        pos += len;
        cout << str << "\n";
    } else if (op == SER::ERR) {
        cout << "Error code: ";
        uint32_t err_code = 0;
        memcpy(&err_code, &res[pos], 4);
        pos += 4;
        cout << err_code << "\n";
        int32_t len = 0;
        memcpy(&len, &res[pos], 4);
        pos += 4;
        std::string str;
        str.resize(len);
        memcpy(str.data(), &res[pos], len);
        pos += len;
        cout << "Message: " << str << "\n";
    } else if (op == SER::ARR) {
        cout << "Array len: ";
        int64_t len = 0;
        memcpy(&len, &res[pos], 8);
        pos += 8;
        cout << len << "\n";
        for (int i = 0; i < len; ++i) {
            pos = decode(res, pos);
        }
    } else if (op == SER::DOUB) {
        int32_t len = 0;
        memcpy(&len, &res[pos], 4);
        pos += 4;
        std::string str;
        str.resize(len);
        memcpy(str.data(), &res[pos], len);
        pos += len;
        cout << str << "\n";
    }
    return pos;
}

