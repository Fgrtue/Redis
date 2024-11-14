#pragma once
#include "HashTable.hpp"
#include <assert.h>

class HMap {


public: 
    // 1. Insert function which handles the resizing

        // 1. Inserts a node in ht1
        // 2. If the capacity was reached
        //     - start resizing
        // 3. Continue resizing

    HMap() = default;

    const HNode* insert(const std::string& key, double val);

    // 2. Lookup handles the lookup for both tables

    std::optional<HNode*> lookup(const std::string& key);

    // 3. Deletion

    HNode* del(const std::string& key); 

    std::vector<std::string> keys();

    private:

    // 4. startResize

    void startResize();

        // - move the table from ht1 to ht2
        // - resizes the vector of ht1 to capacity of ht2 * 2

    // 5. continueResize

    void continueResize();

        // - in a loop from 1 to 128
        // - move the nodes from the second table to the first, until it is non empty
        // - use resizing pos as an iterator
        // - in case size of ht2 becomes 0 --> set resizing pos to 0

    HTable ht1;
    HTable ht2;
    static constexpr size_t k_resizing_work = 128;
    size_t resizing_pos = 0; // needed for pointing which exact position are moving
};