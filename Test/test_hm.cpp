#include "HashMap.hpp"
#include <iostream>
#include <random>
#include <unordered_map>
#include <assert.h>

using std::cout;

int main() {

    HMap map;
    std::unordered_map<std::string, std::string> std_map;
    std::mt19937 gen(10892);
    std::uniform_int_distribution<int> dist(0, 100);

    for (int i = 0; i < 1000; ++i) {
        int a = dist(gen);
        int b = dist(gen);

        std::string key = std::to_string(a);
        std::string val = std::to_string(i);
        std::string to_del = std::to_string(b);


        map.insert(key, val);
        std_map[key] = val;
        std::optional<std::string> op = map.lookup(key);
        assert(op.value() == std_map[key]);
        map.del(to_del);
        std_map.erase(to_del);
    }
}