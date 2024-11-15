#pragma once

#include <string_view>
#include <cmath>

struct AVLNode {

    AVLNode() = default;

    AVLNode(double, const std::string_view, AVLNode*);


    uint32_t            h;
    uint32_t            sz;
    int32_t             diff;
    double              score_;
    std::string_view    key_;
    AVLNode*            left_;
    AVLNode*            right_;
    AVLNode*            parent_;

    bool isEqual(double, double) const;

    bool eq(double score, const std::string_view key) const;

    bool less(double score, const std::string_view key) const;

    bool operator< (const AVLNode& other) const;

    bool operator== (const AVLNode& other) const;

};