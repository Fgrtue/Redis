#include "Client.hpp"
#include <vector>

using std::vector;
using std::cin;
using std::cout;

constexpr const char PORT[] = "53232";

int main(int argc, char* argv[]) {

    if (argc != 2) {
        cout << "Input: serv_IP\n";
        exit(0); 
    }

    try {
        Client client(argv[1], PORT);
        std::vector<std::string> queries;
        int k;
        std::cin >> k;
        std::string arg;
        for (int j = 0; j < k; ++j) {
            std::cin >> arg;
            queries.push_back(arg);
        }
        if (client.sendReq(queries)) {
            cout << "Response: " << client.readRes() << "\n";
        } else {
            cout << "Error send req\n";
        }
    } catch (const std::runtime_error& e) {
        std::cerr << e.what() << std::endl;
        exit(1);
    }
}