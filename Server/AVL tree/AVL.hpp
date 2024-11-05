#pragma once
#include <iostream>


struct Node {

    uint32_t    h;
    uint32_t    sz;
    int         ind;
    Node*       left_;
    Node*       right_;
    Node*       parent;
};

class AVL {

    void initTree();

    void insert(int num, Node* cur);

    Node* del(int num, Node* cur);

    void find(int, Node*);

    private:

    void fixImbalance();
    
    void rotateLL(Node*);

    void rotateRR(Node*);
    
    void rotateLR(Node*);

    void rotateRL(Node*);

    Node** root_; 
};