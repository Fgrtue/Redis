#include "../include/Socket.hpp"
#include "../include/EventLoop.hpp"

constexpr const char PORT[] = "53232";

int main() {

    // Add try-catch block
    try {
        ListenSocket listening(PORT);
        EventLoop eventLoop(listening.get_socket());
        eventLoop.run();
    } catch (const std::runtime_error& e) {
        std::cerr << e.what() << std::endl;
        exit(1);
    }
}