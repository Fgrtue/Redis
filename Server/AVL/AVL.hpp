#pragma once
#include <iostream>
#include <vector>
#include <assert.h>

struct Node {

    Node(int, Node*);

    uint32_t    h;
    uint32_t    sz;
    int32_t     diff;
    int         num_;
    Node*       left_;
    Node*       right_;
    Node*       parent_;

    void printNode() {
        std::cout << "Num: " << num_ << "\n";
        if (parent_ != nullptr) {
            std::cout << "Parent: " << parent_->num_ << "\n";
        }
        if (left_ != nullptr) {
            std::cout << "Left: " << left_->num_ << "\n";
        }
        if (right_ != nullptr) {
            std::cout << "Right: " << right_->num_ << "\n";
        }
        std::cout << "\n";
    }
};

class AVL {

public:

    AVL();

    ~AVL();

    bool findTree(int);

    void insertTree(int num);

    Node* delTree(int num);

    void TestCheck();

private:

    Node* find(int, Node*);

    Node* insert(int num, Node* cur);

    void fixImbalance(Node*);
    
    Node* rotateLeft(Node*);

    Node* rotateRight(Node*);

    void recomputeParam(Node*);

    Node* setNode(Node*, Node*);

    Node* extractMin(Node*);

    void swapNodes(Node*, Node*);

    void setNull(Node*);

    void testDFS(std::vector<int>&, Node*);

    void delDFS(Node* node);

    void clearNode(Node* node);


    // Root node is a dummy for starting the search
    Node* root_; 
};