#pragma once

#include "HashMap/HashMap.hpp"
#include "AVL/AVL.hpp"

class Set {

public:

    void insertSet(const std::string& key, double val);

    std::optional<double> findSet(std::string& key);

    bool delSet(const std::string& key);

    std::vector<std::string_view> getRange(const std::string& key, double score, int64_t offset, int64_t limit);

private:

    HMap    hmap;
    AVL     tree;
};