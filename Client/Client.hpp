#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>
#include <netdb.h>
#include <stdexcept>
#include <iostream>
#include <cstring>
#include <vector>
#include "Conn.hpp"

class Client
{

public:

    // Has to open socket a connect

    Client(char* addr, const char port[]);

    ~Client();

    // Send command

    bool sendReq(std::vector<std::string>& query);

    // Receive response

    std::string readRes();

private:

    void getAddr(char *addr, const char port[]);

    bool fillWbuf(std::vector<std::string>& query);

    bool sendWbuf();

    void setSocket();

    bool Socket(struct addrinfo *ai);

    bool Connect(struct addrinfo *ai);

    bool fillRbuff();

    bool readSafe(void *buf, int len);
    
    Conn conn_;
    struct addrinfo* ai_;
};