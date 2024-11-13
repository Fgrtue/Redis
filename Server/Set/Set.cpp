#include "Set.hpp"

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

std::optional<double> Set::findSet(std::string& key) {

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

/*

void setTTL(key, ttl) {
    // Looks for an elemen in HMap
    // if it doesn't exist --> returns nothing
    // else
    // Checks its position in heap
    // if pos == -1 --> creates a new item in the heap
    // updates the position to the end of the heap
    // then updates the heap at this position depending on provided timer
}

void clearHeap(); --> must be reached from process timers in event loop
    - in a while loop
    - delete node from the hmap
    - delete node from the AVL tree
    - increase the nworks (don't to more than 2000 deletions)


*/