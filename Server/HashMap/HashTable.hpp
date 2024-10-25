#pragma once

#include <string>
#include <vector>
#include <memory>
#include <optional>

// 1. A node for linked list 

class HMap;

struct Node {

    std::string  val_;
    Node*        next_ = nullptr;
    int64_t     hcode_ = -1;

    Node(int64_t hcode, const std::string& val)
    : hcode_(hcode)
    , val_(val)
    , next_(nullptr)
    {}
};

// 2. The hash table structure itself

class HTable {

public:
    // 1. Initialize table

    HTable();

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

    void h_insert(Node*);

    // 3. Lookup element 

        // 1). Having the hcode, iterate through the nodes
        // in a cell until you find the one with the same
        // exact key

    std::optional<std::string> h_find(int64_t hcode) const;

    // 4. Delete

        // 1). Having the hcode iterate through the nodes
        // saving the previos one
        // 2). Once found set the next for prev as next for the
        // cur
        // 3). Take the ownership from prev to the cur node, call
        // destructor for the node

    Node* h_del(int64_t);

    // 5. Resize

    void h_resize(size_t);

    private:

    void clear();

    bool del(Node* prev, Node* cur);    
    
    static constexpr size_t INIT_CAP = 16;
    std::vector<std::unique_ptr<Node>> tab_;
    size_t mask_; // since len = 2^n, we can compute the index easily using
                 // & operator
    size_t size_;
    size_t cap_;
    friend HMap;
};
