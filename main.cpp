#include "socket.hpp"

constexpr const char PORT[] = "53232";

int main() {

    // Add try-catch block
    try {
        ListenSocket listening(PORT);
    } catch (const std::runtime_error& e) {
        std::cerr << e.what() << std::endl;
        exit(1);
    }
}