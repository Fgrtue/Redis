#include "Client.hpp"
#include <vector>

using std::vector;
using std::cin;
using std::cout;

constexpr const char PORT[] = "53232";

int main(int argc, char* argv[]) {

    if (argc != 2) {
        cout << "Input: serv_IP\n"; 
    }

    try {
        Client client(argv[1], PORT);

        int m;
        std::cin >> m;
        std::vector<std::vector<std::string>> queries(m);
        for (int i = 0; i < m; ++i) {
            int k;
            cin >> k;
            for (int j = 0; j < k; ++j) {
                std::string arg;
                std::cin >> arg;
                queries[i].push_back(arg);
            }
            if (client.sendReq(queries[i])) {
                cout << "Response: " << client.readRes() << "\n";
            }
        }
    } catch (const std::runtime_error& e) {
        std::cerr << e.what() << std::endl;
        exit(1);
    }

}