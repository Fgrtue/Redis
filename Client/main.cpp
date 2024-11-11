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
        cout << "TEST SET\n\n";
    for (int i = 0; i < 1000; ++i) {
        try {
            Client client(ADDR, PORT);
    
            queries[1] = std::to_string(i);
            double val = i + (1. / (i + 1));
            queries[2] = std::to_string(val);
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

        cout << "TEST GET\n\n";
    // TEST FIND
        queries.resize(2);
        queries[0] = "get";
        for (int i = 100; i < 200; ++i) {
            try {
                Client client(ADDR, PORT);

                queries[1] = std::to_string(i);
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

        cout << "\n\n";
        cout << "TEST DEL\n\n";
        queries.resize(2);
        queries[0] = "del";
        for (int i = 100; i < 200; ++i) {
            try {
                Client client(ADDR, PORT);
        
                queries[1] = std::to_string(i);
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
        cout << "\n\n";

    // TEST RANGE QUERY

    queries.resize(5);
    queries[0] = "range";
    queries[1] = "0";
    queries[2] = "0";
    queries[3] = "0";
    queries[4] = "1000";
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