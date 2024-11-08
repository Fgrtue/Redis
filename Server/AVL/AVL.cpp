#include "AVL.hpp"

static int32_t getDiff(const Node* node);
    
static uint32_t getHeight(const Node* node);

static uint32_t getSz(const Node* node);

AVL::AVL() {

    // allocate space for root node
    root_ = new Node(0, "", nullptr);
}

AVL::~AVL() {

    if (root_->left_ != nullptr) {
        delDFS(root_->left_);
    }
    delete root_;
}

std::optional<std::vector<std::string_view>> AVL::getRange(double score, std::string_view key, int64_t offset, int64_t limit) {

    /*
    // Node* ub = upperBound(score, key);
    if (ub = nullptr) {
        return std::nullopt;
    }
    // Node* off = offset(ub, offset);
    if (off == nullptr) {
        return std::nullopt;
    }
    std::vector<std::string_view> ret;
    // for(int64_t i = 0; i < limit; ++i) {
        ret.push_back(off);
        off = offset(off, 1);
    }
    return ret;
    */
}

// We search for the element that is equal of larger than the
// current
Node* AVL::upperBound(double score, std::string_view key) {

    Node* found = nullptr;
    for (Node* cur = root_->left_; cur != nullptr; ) {
        if (cur->less(score, key)) {
            cur = cur->right_;
        } else {
            found = cur;
            cur = cur->left_;
        }
    } 
    return found;
}

// Implement offset function.


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

bool AVL::findTree(double score, std::string_view key) {

    if (find(score, key, root_->left_)) {
        return true;
    } 
    return false;
}

Node* AVL::find(double score, std::string_view key, Node* cur) {

    if (cur == nullptr) {
        return nullptr;
    }

    // Compates num with cur->
    if (cur->eq(score, key)) {
        return cur;
    }

    // if <
    // return find(num, cur->left)
    if (cur->less(score, key)) {
        return find(score, key, cur->right_);
    } else {
        return find(score, key, cur->left_);
    }
}

// Inserts new node with num value
//      if num exists does nothing
//      else allocates new element

void AVL::insertTree(Node* node) {

    if (root_->left_ == nullptr) {
        root_->left_ = node;
    } else {
        Node* inserted = insert(node, root_->left_);
        if (inserted != nullptr) {
            fixImbalance(inserted);
        }
    }
}

Node* AVL::insert(Node* node, Node* cur) {

    if (*cur == *node) {
        return nullptr;
    }

    if (*cur < *node) {
        if (cur->right_ == nullptr) {
            cur->right_ = node;
            return cur->right_;
        } else {
            return insert(node, cur->right_);
        }
    } else {
        if (cur->left_ == nullptr) {
            cur->left_ = node;
            return cur->left_;
        } else {
            return insert(node, cur->left_);
        }        
    }
    return nullptr;
}

void AVL::insertTree(double score, std::string_view key) {

    if (root_->left_ == nullptr) {
        root_->left_ = new Node(score, key, root_);
    } else {
        Node* inserted = insert(score, key, root_->left_);
        if (inserted != nullptr) {
            fixImbalance(inserted);
        }
    }
}

Node* AVL::insert(double score, std::string_view key, Node* cur) {

    if (cur->eq(score, key)) {
        return nullptr;
    }

    if (cur->less(score, key)) {
        if (cur->right_ == nullptr) {
            cur->right_ = new Node(score, key, cur);
            return cur->right_;
        } else {
            return insert(score, key, cur->right_);
        }
    } else {
        if (cur->left_ == nullptr) {
            cur->left_ = new Node(score, key, cur);
            return cur->left_;
        } else {
            return insert(score, key, cur->left_);
        }        
    }
    return nullptr;
}

// Can return null ptr
// handles the case of root_ == nullptr

Node* AVL::delTree(double score, std::string_view key) {

    // Try to search for the value firts
    //  if not found: return nullptr
    //  continue otherwise
    Node* found = find(score, key, root_->left_);
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

        if (cur->diff == -2) {

            if (getDiff(cur->right_) == 1) {
                rotateRight(cur->right_);
            }
            rotateLeft(cur);
        } else if (cur->diff == 2) {

            if (getDiff(cur->left_) == -1) {
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
    y->left_ = x; 
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
    return ret;
}

void AVL::recomputeParam(Node* node) {
    
    int32_t leftH = getHeight(node->left_) ; 
    int32_t rightH = getHeight(node->right_);

    node->h = 1 + std::max(leftH, rightH);
    node->diff = leftH - rightH;
    node->sz = 1 + getSz(node->left_) + getSz(node->right_);
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
        // assert(cur->left_->score_ < cur->score_);
        testDFS(values, cur->left_);
    }
    values.push_back(cur->score_);
    if (cur->right_ != nullptr) {
        // assert(cur->score_ < cur->right_->score_);
        testDFS(values, cur->right_);
    }
    int leftH = getHeight(cur->left_);
    int rightH = getHeight(cur->right_);

    assert(leftH == rightH || leftH == rightH + 1 || leftH + 1 == rightH);
}

int32_t getDiff(const Node* node) {
    if (node != nullptr) {
        return node->diff;
    }
    return 0;
}
    
uint32_t getHeight(const Node* node) {
    if (node != nullptr) {
        return node->h;
    }
    return 0;
}

uint32_t getSz(const Node* node) {
    if (node != nullptr) {
        return node->sz;
    }
    return 0;
}