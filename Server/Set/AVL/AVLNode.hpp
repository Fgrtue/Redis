#pragma once

#include <string_view>
#include <cmath>

struct AVLNode {

    AVLNode(double, std::string_view, AVLNode*);

    uint32_t         h;
    uint32_t         sz;
    int32_t          diff;
    double           score_;
    std::string_view key_;
    AVLNode*            left_;
    AVLNode*            right_;
    AVLNode*            parent_;

    bool isEqual(double, double);

    bool eq(double score, std::string_view key);

    bool less(double score, std::string_view key);

    bool operator< (const AVLNode& other);

    bool operator== (const AVLNode& other);

};