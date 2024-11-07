#pragma once

#include <string_view>
#include <cmath>

struct Node {

    Node(double, std::string_view, Node*);

    uint32_t         h;
    uint32_t         sz;
    int32_t          diff;
    double           score_;
    std::string_view key_;
    Node*            left_;
    Node*            right_;
    Node*            parent_;

    bool isEqual(double, double);

    bool eq(double score, std::string_view key);

    bool less(double score, std::string_view key);

    bool operator< (const Node& other);

    bool operator== (const Node& other);

};