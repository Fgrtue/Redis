#include "../include/TList.hpp"

TNode::TNode()
: next_(this)
, parentConn(nullptr)
, prev_(this)
{}

TNode::TNode(Conn* conn)
: next_(nullptr)
, parentConn(conn)
, prev_(nullptr)
{}

Conn* TList::front() {
    return head.next_->parentConn;
}

bool TList::empty() {
    return head.next_ == &head;
}

void TList::pushBack(TNode* node) {
    TNode* prev = head.prev_;
    node->next_ = &head;  
    node->prev_ = prev;
    prev->next_ = node;
    head.prev_  = node;
}

void TList::pop(TNode* node) {
    TNode* prev = node->prev_;
    TNode* next = node->next_;
    prev->next_ = next;
    next->prev_ = prev;
    node->next_ = nullptr;
    node->prev_ = nullptr;
}