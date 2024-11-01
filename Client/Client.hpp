#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>
#include <netdb.h>
#include <stdexcept>
#include <iostream>
#include <cstring>
#include <vector>
#include "Conn/Conn.hpp"

enum class SER {
    NIL,
    ERR,
    STR,
    INT,
    ARR,
};

class Client
{

public:

    // Has to open socket a connect

    Client(const char addr[], const char port[]);

    ~Client();

    // Send command

    bool sendReq(std::vector<std::string>& query);

    // Receive response

    std::string readRes();

    size_t decode(const std::string&, size_t);

private:

    void getAddr(const char addr[], const char port[]);

    bool fillWbuf(std::vector<std::string>& query);

    bool sendWbuf();

    void setSocket();

    bool Socket(struct addrinfo *ai);

    bool Connect(struct addrinfo *ai);

    bool fillRbuff();
    
    Conn conn_;
    struct addrinfo* ai_;
};