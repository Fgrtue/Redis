#include "Heap.hpp"

void Heap::insertHeap(HNode* node, uint64_t expire) {

    timers_.emplace_back(expire, node);
    heapUp();
}

HNode* Heap::delHeap() {
    std::swap(timers_[0], timers_.back());
    HNode* node = timers_.back().hnode;
    timers_.pop_back();
    heapDown();
}

size_t Heap::heapParent(size_t i) {
    return (i - 1) / 2;
}

size_t Heap::heapLeft(size_t i) {
    return (2 * i) + 1;
}

size_t Heap::heapRight(size_t i) {
    return (2 * i) + 2;
}

void  Heap::heapDown() {

    size_t n = timers_.size();
    size_t i = 0;
    while (true) {
        size_t j = i;
        size_t ch_left = heapLeft(j);
        size_t ch_righ = heapRight(j);
        if (ch_left < n && timers_[ch_left] < timers_[j]) {
            j = ch_left;
        }
        if (ch_righ < n && timers_[ch_righ] < timers_[j]) {
            j = ch_righ;
        }
        if (j == i) {
            return ;
        }
        timers_[i].hnode->pos_ = j;
        timers_[j].hnode->pos_ = i;
        std::swap(timers_[i], timers_[j]);
        i = j;
    }
}

void  Heap::heapUp() {
    
    size_t pos = timers_.size() - 1;
    while (pos > 0) {
        size_t par = heapParent(pos);
        if (timers_[pos] < timers_[par]) {
            timers_[par].hnode->pos_ = pos;
            timers_[pos].hnode->pos_ = par;
            std::swap(timers_[pos], timers_[par]);
            pos = par;
        } else {
            return ;    
        }
    }
}

