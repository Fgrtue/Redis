#include "AVL.hpp"

Node::Node(int num, Node* parent) 
: h(1)
, sz(1)
, diff(0)
, num_(num)
, left_(nullptr)
, right_(nullptr)
, parent_(parent)
{}

AVL::AVL() {

    // allocate space for root node
    root_ = new Node(0, nullptr);
}

AVL::~AVL() {

    if (root_->left_ != nullptr) {
        delDFS(root_->left_);
    }
    delete root_;
}

void AVL::delDFS(Node* node) {

    assert(node != nullptr);
    if (node->left_ == nullptr && node->right_ == nullptr) {
        clearNode(node);
        return ;
    } 
    if (node->left_ != nullptr) {
        delDFS(node->left_);
    }

    if (node->right_ != nullptr) {
        delDFS(node->right_);
    }
    clearNode(node);
}

void AVL::clearNode(Node* node) {
    if (node->parent_->left_ == node) {
        node->parent_->left_ = nullptr;
    } else {
        node->parent_->right_ = nullptr;
    }
    delete node;
}

bool AVL::findTree(int num) {

    if (find(num, root_->left_)) {
        return true;
    } 
    return false;
}

Node* AVL::find(int num, Node* cur) {

    if (cur == nullptr) {
        return nullptr;
    }

    // Compates num with cur->
    if (cur->num_ == num) {
        return cur;
    }

    // if <
    // return find(num, cur->left)
    if (num < cur->num_ ) {
        return find(num, cur->left_);
    } else {
        return find(num, cur->right_);
    }
}

// Inserts new node with num value
//      if num exists does nothing
//      else allocates new element

void AVL::insertTree(int num) {

    if (root_->left_ == nullptr) {
        root_->left_ = new Node(num, root_);
    } else {
        Node* inserted = insert(num, root_->left_);
        if (inserted != nullptr) {
            fixImbalance(inserted);
        }
    }
}

Node* AVL::insert(int num, Node* cur) {

    // if num == cur.ind
    //     return nullptr; 
    if (cur->num_ == num) {
        return nullptr;
    }

    // in case num < cur->num 
    //      either insert and return this pointer
    //      or proceed to the next level of recursion
    // other case is mirrored
    if (num < cur->num_) {
        if (cur->left_ == nullptr) {
            cur->left_ = new Node(num, cur);
            return cur->left_;
        } else {
            return insert(num, cur->left_);
        }        
    } else if (cur->num_ < num) {
        if (cur->right_ == nullptr) {
            cur->right_ = new Node(num, cur);
            return cur->right_;
        } else {
            return insert(num, cur->right_);
        }
    }
    return nullptr;
}

// Can return null ptr
// handles the case of root_ == nullptr

Node* AVL::delTree(int num) {

    // Try to search for the value firts
    //  if not found: return nullptr
    //  continue otherwise
    Node* found = find(num, root_->left_);
    if (found == nullptr) {
        return nullptr;
    }
    // Use bottom node for the smallest node that changed
    // its height
    Node* bot;
    if (found->right_ == nullptr && found->left_ == nullptr) {
        bot = setNode(found, nullptr);
    } else if (found->right_ == nullptr) {
        bot = setNode(found, found->left_);
        // set left child to the place of the current node
        // save the bot
    } else if (found->left_ == nullptr) {
        bot = setNode(found, found->right_);
        // set right child to the place of the current node
        // save the bot
    } else { 
        bot = extractMin(found);
    } 
    fixImbalance(bot);
    return found;
}

// walk in a while loop until you get up to the root
// recomputeSzDiffH(cur);
// Check left subtree -> right down the value diff
// Check right subtree -> right down the value diff
// Four cases for rotation
// Implement rotation
// recompute SzDiffH(cur)
// assert -1 <= diff && diff <= 1

void AVL::fixImbalance(Node* start) {

    // Iterate through all the nodes
    //    up to the root_
    Node* cur = start;
    while (cur != root_) {
        recomputeParam(cur);

        int32_t leftDiff = 0;
        int32_t rightDiff = 0;
        if (cur->left_ != nullptr) {
            leftDiff = cur->left_->diff;
        }
        if (cur->right_ != nullptr) {
            rightDiff = cur->right_->diff;
        }

        if (cur->diff == -2) {

            if (rightDiff == 1) {
                rotateRight(cur->right_);
            }
            rotateLeft(cur);
        } else if (cur->diff == 2) {

            if (leftDiff == -1) {
                rotateLeft(cur->left_);
            }
            rotateRight(cur);
        }
        cur = cur->parent_;
    }
}

/*
        x [H]                   y [H]
       / \                     / \ 
[H-3] A   y [H-1]    => [H-1] x   C [H-2]
         / \                 / \
  [H-3] B   C [H-2]   [H-3] A   B [H-3]
*/
    
Node* AVL::rotateLeft(Node* x) {

    Node* y = x->right_;
    Node* parent = x->parent_;

    assert(y != nullptr);
    assert(parent != nullptr);

    if (parent->left_ == x) {
        parent->left_ = y;
    } else {
        parent->right_ = y;
    }

    x->right_ = y->left_; 
    if (x->right_ != nullptr) {
        x->right_->parent_ = x;
    }
    // x->left_ = x->left_; 
    x->parent_ = y;
    y->left_ = x;  //
    // y->right_ = y->right_; 
    y->parent_ = parent;
    recomputeParam(x);
    recomputeParam(y);
    return y;
}

/*
          x [H]                      y [H]
         / \                        / \ 
[H-1]   y   C [H-3]     =>   [H-3] A   x [H-1]  
       / \                            / \
[H-3] A   B  [H-2]             [H-2] B   C [H-3]
*/

Node* AVL::rotateRight(Node* x) {

    Node* parent = x->parent_;
    Node* y = x->left_;

    assert(y != nullptr);
    assert(parent != nullptr);

    if (parent->left_ == x) {
        parent->left_ = y;
    } else {
        parent->right_ = y;
    }

    // x->right = x->right
    x->left_  = y->right_;
    if (x->left_ != nullptr) {
        x->left_->parent_ = x;
    }
    x->parent_ = y;
    // y->left_ = y->left_; 
    y->right_ = x;
    y->parent_ = parent;
    recomputeParam(x);
    recomputeParam(y);
    return y;
} 

// Walk to the left until you encounter null as a left child
// once you found it change its right child with the node, store its right child
// change then min with the found
// return the right child of the min before we switched min and found

/*

    found                      found                    Min
        \                         \                        \
        found->right              found->right          found->right
            /  \                      /  \                  /  \
        ...     ...    =>           ...   ...     =>      ...   ...
         /                           /                      /
        parent                      parent                parent
        /                            \                      \
       Min                          child                  child
      /  \                           /  \                   /  \
    null  child                    ...  ...               ...  ...
*/
Node* AVL::extractMin(Node* found) {

    Node* rightMin = found->right_;
    while (rightMin->left_ != nullptr) {
        rightMin = rightMin->left_;
    }
    Node* parent = setNode(rightMin, rightMin->right_);
    
    Node* ret = parent == found ? rightMin : parent;
    assert(parent != nullptr);
    // Now extract found and put rightMin instead of it
    swapNodes(found, rightMin);
    setNull(found);
    // std::cout << "Found->num: " << found->num_ << "\n";
    // std::cout << "RightMin: "  << rightMin->num_ << "\n";
    // std::cout << "RightMinL: " << rightMin->left_ << "\n";
    // std::cout << "RightMinR: " << rightMin->right_ << "\n";
    return ret;
}

void AVL::recomputeParam(Node* node) {
    
    int32_t leftH = 0; 
    int32_t rightH = 0;
    int32_t sizeL = 0;
    int32_t sizeR = 0;
    if (node->left_ != nullptr) {
        leftH = node->left_->h;
        sizeL = node->left_->sz;
    }
    if (node->right_ != nullptr) {
        rightH = node->right_->h;
        sizeR = node->right_->sz;
    }
    node->h = 1 + std::max(leftH, rightH);
    node->diff = leftH - rightH;
    node->sz = 1 + sizeL + sizeR;
}

// Takes a node to extract (toExtr)
// and its child (can be null)
// fixes parent, child and sets toExtr to null
/*
         parent         parent              parent
        /               /                     /
    toExtr          toExtr          =>      child
     /  \            /  \                   /  \ 
  null  child    child  null            ...     ...
*/

Node* AVL::setNode(Node* toExtr, Node* child) {
    
    Node* parent = toExtr->parent_;
    if (toExtr->left_ == child) {
        assert(toExtr->right_ == nullptr);
    } else {
        assert(toExtr->left_ == nullptr);
    }

    if (child != nullptr) {
        child->parent_ = parent;
    }

    if (parent->left_ == toExtr) {
        parent->left_ = child;
    } else {
        parent->right_ = child;
    }

    setNull(toExtr);
    return parent;
}

void AVL::swapNodes(Node* a, Node* b) {
    Node* parent = a->parent_;
    b->parent_ = parent;
    b->left_ = a->left_;
    if (b->left_ != nullptr) {
        b->left_->parent_ = b;
    }

    b->right_ = a->right_;
    if (b->right_ != nullptr) {
        b->right_->parent_ = b;
    }

    if (parent->left_ == a) {
        parent->left_ = b;
    } else {
        parent->right_ = b;
    }
}

void AVL::setNull(Node* node) {
    node->left_ = nullptr;
    node->right_ = nullptr;
    node->parent_ = nullptr;
}

void AVL::TestCheck() {
    std::vector<int> values;
    if (root_->left_ == nullptr) {
        std::cout << "root->left NULL\n";
    } else {
        testDFS(values, root_->left_);
    }
    for (int i = 0; i < values.size(); ++i) {
        std::cout << values[i] << " ";
    }
    std::cout << "\n";
}

void AVL::testDFS(std::vector<int>& values, Node* cur) {

    if (cur->left_ != nullptr) {
        assert(cur->left_->num_ < cur->num_);
        testDFS(values, cur->left_);
    }
    values.push_back(cur->num_);
    if (cur->right_ != nullptr) {
        assert(cur->num_ < cur->right_->num_);
        testDFS(values, cur->right_);
    }
    int leftH = 0;
    int rightH = 0;
    if (cur->left_ != nullptr) {
        leftH = cur->left_->h;
    }
    if (cur->right_ != nullptr) {
        rightH = cur->right_->h;
    }

    assert(leftH == rightH || leftH == rightH + 1 || leftH + 1 == rightH);
} 