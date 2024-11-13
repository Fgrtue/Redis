#pragma once

#include <string>
#include <vector>
#include <memory>
#include <optional>
#include <iostream>

// 1. A node for linked list 

using std::cin;
using std::cout;

class HMap;

struct HNode {

    std::string     key_;
    double          val_;
    // size_t          pos_;
    HNode*          next_ = nullptr;
    size_t          hcode_ = 0;

    HNode() = default;

    HNode(const std::string& key, size_t hcode, double val)
    : key_(key)
    , hcode_(hcode)
    , val_(val)
    , next_(nullptr)
    {}
};

// 2. The hash table structure itself

class HTable {

public:
    // 1. Initialize table

    HTable();

    HTable(size_t cap);

    HTable(const HTable& other) = delete;

    HTable& operator = (const HTable& other) = delete;

    HTable(HTable&& other);

    HTable& operator = (HTable&& other);

    ~HTable();

    // 2. Insert the Node right after the first one


        // 1). Having the position in the tab, compute the
        // index in array 
        // 2). Add next element to first one, and giving the ownership
        // from the node to this pointer
        // 3). Increase the size

    const HNode* h_insert(const std::string& key, size_t hcode, double val, HNode* nodeNew);

    // 3. Lookup element 

        // 1). Having the hcode, iterate through the nodes
        // in a cell until you find the one with the same
        // exact key

    std::optional<const HNode*> h_find(size_t hcode) const;

    // 4. Delete

        // 1). Having the hcode iterate through the nodes
        // saving the previos one
        // 2). Once found set the next for prev as next for the
        // cur
        // 3). Take the ownership from prev to the cur node, call
        // destructor for the node

    HNode* h_del(size_t);

    // 5. Resize

    void h_resize(size_t);

    std::vector<std::string> h_keys();

    void print();

    private:

    void clear();

    void del(HNode* prev, HNode* cur); 

    void tabInit();
    
    static constexpr size_t INIT_CAP = 16;
    std::vector<HNode*> tab_;
    size_t mask_; // since len = 2^n, we can compute the index easily using
                 // & operator
    size_t size_;
    size_t cap_;
    friend HMap;
};
