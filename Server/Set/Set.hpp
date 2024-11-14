#pragma once

#include "HashMap/HashMap.hpp"
#include "AVL/AVL.hpp"
#include "../Clock.hpp"
#include "Heap/Heap.hpp"

class Set {

public:

    void insertSet(const std::string& key, double val);

    std::optional<double> findSet(const std::string& key);

    bool delSet(const std::string& key);

    std::vector<std::string_view> getRange(const std::string& key, double score, int64_t offset, int64_t limit);

    void setTTL(const std::string&, uint64_t);
    
    std::optional<int64_t> getTTL(const std::string&);
    
    void cleanHeap();

    std::optional<uint64_t> minHeap();

private:

    HMap    hmap;
    AVL     tree;
    Heap    heap;
};