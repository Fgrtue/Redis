#pragma once

#include <string_view>
#include <cmath>

struct AVLNode {

    AVLNode() = default;

    AVLNode(double, const std::string&, AVLNode*);


    uint32_t            h;
    uint32_t            sz;
    int32_t             diff;
    double              score_;
    std::string         key_;
    AVLNode*            left_;
    AVLNode*            right_;
    AVLNode*            parent_;

    bool isEqual(double, double);

    bool eq(double score, const std::string& key);

    bool less(double score, const std::string& key);

    bool operator< (const AVLNode& other);

    bool operator== (const AVLNode& other);

};