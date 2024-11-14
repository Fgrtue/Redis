#include "../include/Heap.hpp"

void Heap::insertHeap(HNode* node, uint64_t expire) {

    timers_.push_back({node, expire});
    node->pos_ = timers_.size() - 1;
    heapUp(node->pos_);
}

HNode* Heap::delHeap() {

    timers_[0].hnode->pos_ = -1;
    timers_.back().hnode->pos_ = 0;
    HNode* node = timers_[0].hnode;
    std::swap(timers_[0], timers_.back());
    timers_.pop_back();
    heapDown(0);
    return node;
}

void Heap::updateHeap(int64_t pos, uint64_t ttl) {

    uint64_t curExp = timers_[pos].expire;
    timers_[pos].expire = ttl;
    if (curExp < ttl) {
        heapDown(pos);
    } else if (ttl < curExp) {
        heapUp(pos);
    }
}

bool Heap::empty() {
    return timers_.empty();
}

int64_t Heap::heapParent(int64_t i) {
    return (i - 1) / 2;
}

int64_t Heap::heapLeft(int64_t i) {
    return (2 * i) + 1;
}

int64_t Heap::heapRight(int64_t i) {
    return (2 * i) + 2;
}

void  Heap::heapDown(int64_t i) {

    int64_t n = timers_.size();
    while (true) {
        int64_t j = i;
        int64_t ch_left = heapLeft(j);
        int64_t ch_righ = heapRight(j);
        if (ch_left < n && timers_[ch_left] < timers_[j]) {
            j = ch_left;
        }
        if (ch_righ < n && timers_[ch_righ] < timers_[j]) {
            j = ch_righ;
        }
        if (j == i) {
            return ;
        }
        std::swap(timers_[i].hnode->pos_, timers_[j].hnode->pos_);
        std::swap(timers_[i], timers_[j]);
        i = j;
    }
}

void  Heap::heapUp(int64_t pos) {
    
    while (pos > 0) {
        int64_t par = heapParent(pos);
        if (timers_[pos] < timers_[par]) {
            std::swap(timers_[pos].hnode->pos_, timers_[par].hnode->pos_);
            std::swap(timers_[pos], timers_[par]);
            pos = par;
        } else {
            return ;    
        }
    }
}

HeapNode& Heap::operator[](int64_t i) {

    return timers_[i];
}
