#pragma once

#include <vector>
#include <cstddef>
#include <cstdint>
#include <algorithm>
#include "../HashMap/HashTable.hpp"

struct HNode;

struct HeapNode {

    HNode*   hnode;
    uint64_t expire;

    HeapNode(HNode* hnode, uint64_t exp) 
    : hnode(hnode)
    , expire(exp)
    {}

    bool operator < (const HeapNode& other) const {
        return expire < other.expire;
    }

};

class Heap {

public:

    void insertHeap(HNode*, uint64_t);

    HNode* delHeap();

    void updateHeap(int64_t, uint64_t);

    bool empty();

    HeapNode& operator[] (int64_t);

private:

    int64_t heapParent(int64_t);

    int64_t heapLeft(int64_t);

    int64_t heapRight(int64_t);

    void   heapDown(int64_t);

    void   heapUp(int64_t);

    std::vector<HeapNode> timers_;
};

