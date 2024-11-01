#include "Client.hpp"
#include <vector>

using std::vector;
using std::cin;
using std::cout;

constexpr const char PORT[] = "53232";
constexpr const char ADDR[] = "127.0.0.1";

int main() {

    // if (argc != 2) {
    //     cout << "Input: serv_IP\n";
    //     exit(0); 
    // }
    std::vector<std::string> queries;
    queries.resize(3);
    queries[0] = "set";
    queries[2] = "val";
    for (int i = 0; i < 1000; ++i) {
        try {
            Client client(ADDR, PORT);
    
            queries[1] = std::to_string(i);

            client.sendReq(queries);
            if (client.sendReq(queries)) {
                std::string response = client.readRes();
                client.decode(response, 0);
            } else {
                cout << "Error send req\n";
            } 

        } catch (const std::runtime_error& e) {
            std::cerr << e.what() << std::endl;
            exit(1);
        }
    }
    queries.resize(1);
    queries[0] = "keys";
    try {
        Client client(ADDR, PORT);
        if (client.sendReq(queries)) {
            std::string response = client.readRes();
            cout << "Response len: " << response.size() << "\n";
            client.decode(response, 0);
        } else {
            cout << "Error send req\n";
        } 
    } catch (const std::runtime_error& e) {
        std::cerr << e.what() << std::endl;
        exit(1);
    }

}