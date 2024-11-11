#include "Set.hpp"

void Set::insertSet(const std::string& key, double val) {

    std::optional<const HNode*> found = hmap.lookup(key);
    if (!found.has_value()) {
        hmap.insert(key, val);
        tree.insertTree(val, key);
    } else {
        AVLNode* node = tree.delTree(found.value()->val_, key);
        hmap.insert(key, val);
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

std::vector<std::string> Set::getRange(const std::string& key, double score, int64_t offset, int64_t limit) {

    std::optional<std::vector<std::string>> res = tree.getRange(score, key, offset, limit);
    if (res == std::nullopt) {
        return {};
    }
    return res.value();
}