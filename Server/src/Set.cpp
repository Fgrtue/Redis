#include "../include/Set.hpp"

void Set::insertSet(const std::string& key, double val) {

    std::optional<const HNode*> found = hmap.lookup(key);
    if (!found.has_value()) {
        const HNode* inserted = hmap.insert(key, val);
        tree.insertTree(val, inserted->key_);
    } else {
        AVLNode* node = tree.delTree(found.value()->val_, key);
        const HNode* inserted = hmap.insert(key, val);
        node->key_ = inserted->key_;
        node->score_ = val;
        tree.insertTree(node);
    }
}

std::optional<double> Set::findSet(const std::string& key) {

    std::optional<const HNode*> found = hmap.lookup(key);
    if (found == std::nullopt) {
        return std::nullopt;
    }
    return found.value()->val_;

}

bool Set::delSet(const std::string& key) {

    // Del node from HMap
    HNode* HDel = hmap.del(key);
    if (HDel == nullptr) {
        return false;
    }
    AVLNode* AVLDel = tree.delTree(HDel->val_, HDel->key_);
    delete HDel;
    delete AVLDel;
    return true;
}

std::vector<std::string_view> Set::getRange(const std::string& key, double score, int64_t offset, int64_t limit) {

    std::optional<std::vector<std::string_view>> res = tree.getRange(score, key, offset, limit);
    if (res == std::nullopt) {
        return {};
    }
    return res.value();
}

void Set::setTTL(const std::string& key, uint64_t ttl) {
    // Looks for an elemen in HMap
    auto found = hmap.lookup(key);
    uint64_t curTime = getMonotonicTime();
    // Make microseconds from miliseconds
    ttl *= 1000;
    ttl += curTime;
    if (!found.value()) {
    // if it doesn't exist --> returns nothing
        return ;
    } else if (found.value()->pos_ == -1) {
    // if pos == -1 --> creates a new item in the heap
        heap.insertHeap(found.value(), ttl);
    } else {
        heap.updateHeap(found.value()->pos_, ttl);
    }
}

std::optional<int64_t> Set::getTTL(const std::string& key) {

    // Looks for an element in HMap
    auto found = hmap.lookup(key);
    if (!found.has_value()) {
        // if doesn't find returns nullopt
        return std::nullopt;
    } else if (found.value()->pos_ == -1){
        // if finds result but no ttl is set
        return -1;
    } else { 
        // if finds --> returns expire time in pos of the node
        uint64_t curTimeUS = getMonotonicTime();
        uint64_t expireUS = heap[found.value()->pos_].expire;
        uint64_t expireIn = curTimeUS < expireUS ? (expireUS - curTimeUS) / 1000 : 0;
        return  expireIn;
    }
}

void Set::cleanHeap() {
    // checks current time
   uint64_t curTime = getMonotonicTime();
   for (int i = 0; i < 2000 && !heap.empty(); ++i) {
    // if current time is larger than the expire time in the top node
        if (heap[0].expire <= curTime) {
            HNode* hnode = heap.delHeap();
            delSet(hnode->key_);
            continue;            
        }
        break;
    }
}

std::optional<uint64_t> Set::minHeap() {
    if (heap.empty()) {
        return std::nullopt;
    }
    return heap[0].expire;
}
