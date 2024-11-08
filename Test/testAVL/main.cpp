#include "../../Server/AVL/AVL.hpp"
#include <set>
#include <random>

using std::cout;

int main() {
    
    std::mt19937 gen(4623);
    std::uniform_real_distribution<double> dist(0., 100.);

    for(int i = 0; i < 1000; ++i) {
        AVL tree;
        std::set<double> set;
        cout << "Test " << i << " insert\n"; 
        int val = dist(gen);
        int del = dist(gen);
        // cout << val << " " << val << "\n";
        tree.insertTree(val, "");
        set.insert(val);
        bool findAVL = false;
        bool findSet = false;
        if (set.find(val) != set.end()) {
            findSet = true;
        }
        tree.TestCheck();
        findAVL = tree.findTree(val, "");
        assert(findAVL == findSet);
        findAVL = false;
        findSet = false;
        tree.delTree(del, "");
        set.erase(del);
        cout << "Test " << i << " delete\n";
        if (set.find(del) != set.end()) {
            findSet = true;
        }
        findAVL = tree.findTree(del, "");
        assert(findAVL == findSet);
    }

    AVL tree;
    std::set<double> set;
    for(int i = 0; i < 1000; ++i) {
        
        cout << "Test " << i << " insert\n"; 
        int val = dist(gen);
        // cout << val << " " << val << "\n";
        tree.insertTree(val, "");
        set.insert(val);
        bool findAVL = false;
        bool findSet = false;
        if (set.find(val) != set.end()) {
            findSet = true;
        }
        findAVL = tree.findTree(val, "");
        assert(findAVL == findSet);
        tree.TestCheck();
    }
    for(int i = 0; i < 1000; ++i) {
        cout << "Test " << i << " delete\n";
        int del = dist(gen);
        bool findAVL = false;
        bool findSet = false;
        tree.delTree(del, "");
        set.erase(del);
        if (set.find(del) != set.end()) {
            findSet = true;
        }
        findAVL = tree.findTree(del, "");
        assert(findAVL == findSet);
        tree.TestCheck();
    }
}