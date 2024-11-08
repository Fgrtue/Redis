#include "Set.hpp"

void Set::insetSet(const std::string& key, double val) {

    // find key in Hmap
    std::optional<const HNode&> found = hmap.lookup(key);
    const HNode& inserted = hmap.insert(key, val);
    if (found == std::nullopt) {
        tree.insertTree(val, inserted.key_);
    } else {
        AVLNode* node = tree.delTree(found->val_, inserted.key_);
        node->score_ = val;
        tree.insertTree(node);
    }
}

std::optional<double> Set::findSet(std::string& key) {

    std::optional<const HNode&> found = hmap.lookup(key);
    if (found == std::nullopt) {
        return std::nullopt;
    }
    return found->val_;

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