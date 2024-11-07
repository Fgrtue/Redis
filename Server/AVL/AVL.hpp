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

    void    insertTree(Node*);

    Node*   delTree(double, std::string_view);

    Node*   delTree(Node*);

    Arr     getRange(double, std::string_view, int64_t, int64_t);

    void    TestCheck();

private:

    Node* upperBound(double, std::string_view);

    Node*   find(double, std::string_view, Node*);

    Node*   insert(double, std::string_view, Node*);

    Node*   insert(Node*, Node*);

    void    fixImbalance(Node*);
    
    Node*   rotateLeft(Node*);

    Node*   rotateRight(Node*);

    void    recomputeParam(Node*);

    Node*   setNode(Node*, Node*);

    Node*   extractMin(Node*);

    void    swapNodes(Node*, Node*);

    void    setNull(Node*);

    void    testDFS(std::vector<int>&, Node*);

    void    delDFS(Node* node);

    void    clearNode(Node* node);


    // Root node is a dummy for starting the search
    Node*   root_; 
};