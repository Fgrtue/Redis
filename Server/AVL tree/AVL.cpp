#include "AVL.hpp"

void AVL::initTree() {

    // Fill the root node
}

void AVL::insert(int num, Node* cur) {

    /*
    
        compare with the cur
        
        in case cur == num
            return cur

        in case num < cur
            1. Check cur->left
            if null -> create new
            else insert(num, cur->left);
        in case num > cur
            2. check cur->right
            if null -> create new
            else insert(num, cur->right)

        
    
    */

    // Compates num with cur->

    // if == 
    // return true;

    // if <
    // return find(num, cur->left)

    // if >
    // return find(num, cur->right)

}

Node* AVL::del(int num, Node* cur) {



}

bool AVL::find(int num, Node* cur) {

    // if cur == nullptr
    // return false;

    // Compates num with cur->

    // if == 
    // return true;

    // if <
    // return find(num, cur->left)

    // if >
    // return find(num, cur->right)

}

void fixImbalance();
    
void rotateLL(Node*);

void rotateRR(Node*);
    
void rotateLR(Node*);

void rotateRL(Node*);