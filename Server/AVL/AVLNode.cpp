#include "AVLNode.hpp"

constexpr double eps = 1e-9;

Node::Node(double score, std::string_view key, Node* parent) 
: h(1)
, sz(1)
, diff(0)
, score_(score)
, key_(key)
, left_(nullptr)
, right_(nullptr)
, parent_(parent)
{}

bool Node::isEqual(double a, double b) {
    return std::abs(a - b) < eps;
}

bool Node::eq(double score, std::string_view key) {
    return isEqual(this->score_, score) && this->key_ == key; 
}

bool Node::less(double score, std::string_view key) {
    if (isEqual(this->score_, score)) {
        return this->key_ < key;
    }
    return this->score_ < score;
}

bool Node::operator==(const Node& other) {
    return isEqual(score_, other.score_) && key_ == other.key_;
}

bool Node::operator<(const Node& other) {
    if (isEqual(score_, other.score_)) {
        return key_ < other.key_;
    }
    return score_ < other.score_;
}