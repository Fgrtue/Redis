#pragma once
#include "AVLNode.hpp"
#include <iostream>
#include <vector>
#include <assert.h>
#include <optional>

class AVL {

    using Arr = std::optional<std::vector<std::string_view>>; 

public:

    AVL();

    ~AVL();

    bool    findTree(double, std::string_view);

    void    insertTree(double, std::string_view);

    void    insertTree(AVLNode*);

    AVLNode*   delTree(double, std::string_view);

    Arr     getRange(double, std::string_view, int64_t, int64_t);

    void    TestCheck();

private:

    AVLNode*   upperBound(double, std::string_view);

    AVLNode*   offsetNode(AVLNode*, int64_t);

    AVLNode*   find(double, std::string_view, AVLNode*);

    AVLNode*   insert(double, std::string_view, AVLNode*);

    AVLNode*   insert(AVLNode*, AVLNode*);

    void    fixImbalance(AVLNode*);
    
    AVLNode*   rotateLeft(AVLNode*);

    AVLNode*   rotateRight(AVLNode*);

    void    recomputeParam(AVLNode*);

    AVLNode*   setNode(AVLNode*, AVLNode*);

    AVLNode*   extractMin(AVLNode*);

    void    swapNodes(AVLNode*, AVLNode*);

    void    setNull(AVLNode*);

    void    testDFS(std::vector<int>&, AVLNode*);

    void    delDFS(AVLNode* node);

    void    clearNode(AVLNode* node);


    // Root node is a dummy for starting the search
    AVLNode*   root_; 
};