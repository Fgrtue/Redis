#pragma once

struct Conn;

struct TNode {

    TNode();

    TNode(Conn*);

    TNode* next_;
    Conn*  parentConn;
    TNode* prev_;
};

class TList {

public:

    TList() = default;

    Conn* front();

    bool empty();

    void pushBack(TNode*);

    void pop(TNode*);

private:
   
    TNode head;
};