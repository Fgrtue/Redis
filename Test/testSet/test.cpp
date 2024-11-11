#include "../../Server/Set/Set.hpp"
#include <string>

int main() {

    Set set; 

    // Insert new element
    
    // std::cout << "Test 1\n";

    // std::string key1 = "0";
    // double val1 = 1.0;
    // set.insertSet(key1, val1);

    // // Find element

    // std::optional<double> res = set.findSet(key1);

    // if (res != std::nullopt) {
    //     cout << res.value() << "\n";
    // } else {
    //     cout << "nullopt\n";
    // }

    // // Delete element

    // set.delSet(key1);
    // res = set.findSet(key1);

    // if (res != std::nullopt) {
    //     cout << res.value() << "\n";
    // } else {
    //     cout << "nullopt\n";
    // }

    // std::cout << "\n\n";

    // // Insert element1, element2, new version of element 1

    // std::cout << "Test 2\n";

    // set.insertSet(key1, val1);
    // std::string key2 = "key2";
    // double val2 = 2.0;
    // set.insertSet(key2, val2);

    // res = set.findSet(key1);

    // if (res != std::nullopt) {
    //     cout << res.value() << "\n";
    // } else {
    //     cout << "nullopt\n";
    // }

    // res = set.findSet(key2);

    // if (res != std::nullopt) {
    //     cout << res.value() << "\n";
    // } else {
    //     cout << "nullopt\n";
    // }

    // val1 = 0.0; 
    // set.insertSet(key1, val1);
    // set.insertSet(key2, val1);

    // res = set.findSet(key1);

    // if (res != std::nullopt) {
    //     cout << res.value() << "\n";
    // } else {
    //     cout << "nullopt\n";
    // }

    // std::cout << "\n\n";


    // // Find new version of element1

    // // Insert element 1, 2, 3, .., 10 

    // std::cout << "Test 3\n";

    // for (int i = 0; i < 10; ++i) {
    //     std::string key = std::to_string(i);
    //     double val = 1.5 + 1.0;
    //     set.insertSet(key, val);
    // }
    // std::vector<std::string_view> vec = set.getRange("0", 1.5, 1, 15);
    
    // for (int i = 0; i < vec.size(); ++i) {
    //     cout << vec[i] << " ";
    // }

    // std::cout << "\n\n";

     for (int i = 0; i < 10; ++i) {

        std::string key = std::to_string(i);
        double val = i + (1. / (i + 1));
        set.insertSet(key, val);
     }
    
    std::vector<std::string_view> vec = set.getRange("4", 3, 0, 10);
    for (int i = 0; i < vec.size(); ++i) {
        cout << vec[i] << " ";
    }
    std::cout << "\n\n";
}