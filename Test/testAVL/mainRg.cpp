#include "../../Server/AVL/AVL.hpp"
#include <set>
#include <random>

using std::cout;

int main() {
    

    AVL tree;

    std::string s1 = "a";
    std::string s2 = "b";
    std::string s3 = "c";
    std::string s4 = "d";
    std::string s5 = "e";

    double sc1 = 1.;
    double sc2 = 2.;
    double sc3 = 3.;
    double sc4 = 4.;
    double sc5 = 5.;

    // Insert all the elements

    tree.insertTree(sc1, s1);
    tree.insertTree(sc2, s2);
    tree.insertTree(sc3, s3);
    tree.insertTree(sc4, s4);
    tree.insertTree(sc5, s5);

    
    // Check the tree

    cout << "Testing for all the elements: \n";
    std::cout << tree.findTree(sc1, s1);
    std::cout << tree.findTree(sc2, s2);
    std::cout << tree.findTree(sc3, s3);
    std::cout << tree.findTree(sc4, s4);
    std::cout << tree.findTree(sc5, s5);
    cout << "\n";

    // GetRange: 1. "a"  0. 3

    std::optional<std::vector<std::string_view>> ret1 = tree.getRange(1., "a", 0, 3);

    if (ret1 == std::nullopt) {
        cout << "nullopt\n";
    } else {
        for (int i = 0; i < ret1->size(); ++i) {
            cout << ret1->operator[](i) << " ";
        }
    }
    cout << "\n";

    // GetRange: 3. "d" 1 1

    std::optional<std::vector<std::string_view>> ret2 = tree.getRange(3., "d", 1, 1);

    if (ret2 == std::nullopt) {
        cout << "nullopt\n";
    } else {
        for (int i = 0; i < ret2->size(); ++i) {
            cout << ret2->operator[](i) << " ";
        }
    }
    cout << "\n";

    // GetRange: 3. "d" -2 6

    std::optional<std::vector<std::string_view>> ret3 = tree.getRange(3., "d", -2, 6);

    if (ret3 == std::nullopt) {
        cout << "nullopt\n";
    } else {
        for (int i = 0; i < ret3->size(); ++i) {
            cout << ret3->operator[](i) << " ";
        }
    }
    cout << "\n";
}