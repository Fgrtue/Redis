#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>
#include <netdb.h>
#include <stdexcept>
#include <iostream>

class ListenSocket {

public:

    // Must create a Socket with provided Arguments
    // get_addrinfo is the crutial function, which needs the port and
    // ip address (in case this is client socket)
    // arguments are: char* addr, int port
    ListenSocket(const char* port);
    
    // No copy constr

    ListenSocket(const ListenSocket& other) = delete;

    // No copy assign

    ListenSocket& operator = (const ListenSocket& other) = delete;

    // No move constr

    ListenSocket(const ListenSocket&& other) = delete;

    // No move assign

    ListenSocket& operator = (ListenSocket&& other) = delete;

    virtual ~ListenSocket() {
        close(sockfd_);
        freeaddrinfo(ai_);
    } 

    // const get_socket

    int get_socket() const {
        return sockfd_;
    }

private:

    void fillHints(struct addrinfo *hints);

    void recAddrInfo(const char *service, const struct addrinfo *hints);

    void recSocket();

    void createSocket();

    void setSocketOption();

    void bindSocket();

    void sockListen();

    int sockfd_;
    struct addrinfo *ai_;

    static constexpr const int BACKLOG = 10;
};
