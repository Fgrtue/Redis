#pragma once

#include <vector>
#include <cstddef>
#include <cstdint>
#include <algorithm>

struct HNode;

struct HeapNode {

    uint64_t expire;
    HNode*   hnode;

    bool operator < (const HeapNode& other) const {
        return expire < other.expire;
    }

};

class Heap {

public:

    void insertHeap(HNode*);

    void delHeap();

private:

    size_t heapParent(size_t i);

    size_t heapLeft(size_t i);

    size_t heapRight(size_t i);

    void   heapDown();

    void   heapUp();

    const HeapNode& operator[] (size_t i);

    std::vector<HeapNode> timers_;
};

