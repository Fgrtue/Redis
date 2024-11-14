#include "../include/AVLNode.hpp"

constexpr double eps = 1e-9;

AVLNode::AVLNode(double score, const std::string_view key, AVLNode* parent) 
: h(1)
, sz(1)
, diff(0)
, score_(score)
, key_(key)
, left_(nullptr)
, right_(nullptr)
, parent_(parent)
{}

bool AVLNode::isEqual(double a, double b) {
    return std::abs(a - b) < eps;
}

bool AVLNode::eq(double score, const std::string_view key) {
    return isEqual(this->score_, score) && this->key_ == key; 
}

bool AVLNode::less(double score, const std::string_view key) {
    if (isEqual(this->score_, score)) {
        return this->key_ < key;
    }
    return this->score_ < score;
}

bool AVLNode::operator==(const AVLNode& other) {
    return isEqual(score_, other.score_) && key_ == other.key_;
}

bool AVLNode::operator<(const AVLNode& other) {
    if (isEqual(score_, other.score_)) {
        return this->key_ < other.key_;
    }
    return this->score_ < other.score_;
}