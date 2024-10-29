#include "HashTable.hpp"

HTable::HTable()
: size_(0)
, cap_(INIT_CAP)
, mask_(INIT_CAP - 1)
, tab_(INIT_CAP)
{
    tabInit();
}

HTable::HTable(size_t cap)
: size_(0)
, cap_(cap)
, mask_(cap - 1)
, tab_(cap)
{
    tabInit();
}

HTable::HTable(HTable&& other)
: tab_(std::move(other.tab_))
, size_(other.size_)
, cap_(other.cap_)
, mask_(other.mask_)
{
    other.tab_ = std::vector<Node*>();
    other.size_ = 0;
    other.cap_  = INIT_CAP;
    other.mask_ = INIT_CAP - 1;
}
    
HTable& HTable::operator = (HTable&& other){
    if (this != &other) {
        clear();
    }

    this->tab_  = std::move(other.tab_);
    this->size_ = other.size_;
    this->cap_  = other.cap_;
    this->mask_ = other.mask_;

    other.tab_ = std::vector<Node*>();
    other.size_ = 0;
    other.cap_ = INIT_CAP;
    other.mask_ = INIT_CAP - 1;
    return *this;
}

HTable::~HTable() {
    clear();
}

void HTable::h_insert(size_t hcode, const std::string& val, Node* nodeNew) {

    size_t ind = hcode & mask_;
    Node* prev = tab_[ind];
    Node* ptr = tab_[ind]->next_;
    for (; ptr != nullptr; ptr = ptr->next_) {
        if (ptr->hcode_ == hcode) {
            ptr->val_ = val;
            return ;
        }
        prev = ptr;
    }
    if (nodeNew == nullptr) {
        prev->next_ = new Node(hcode, val);
    } else {
        prev->next_ = nodeNew;
    }
    ++size_;
}

std::optional<std::string> HTable::h_find(size_t hcode) const {

    size_t ind = hcode & mask_;
    Node* ptr = tab_[ind]->next_;

    for (; ptr != nullptr; ptr = ptr->next_) {
        if (ptr->hcode_ == hcode) {
            return ptr->val_;
        }
    }
    return std::nullopt;
}

Node* HTable::h_del(size_t hcode) {

    size_t ind = hcode & mask_;
    Node* prev = tab_[ind];
    Node* ptr = tab_[ind]->next_;
    for (; ptr != nullptr; ptr = ptr->next_) {
        if (ptr->hcode_ == hcode) {
            prev->next_ = ptr->next_;
            ptr->next_ = nullptr;
            --size_;
            return ptr;
        }
        prev = ptr;
    }
    return nullptr;
}

void HTable::h_resize(size_t sz){
    tab_.clear();
    tab_.resize(sz);
    cap_ = sz;
    mask_ = cap_ - 1;
    tabInit();
}

void HTable::clear() {

    for (size_t ind = 0; ind < tab_.size(); ++ind) {
        Node* node = tab_[ind];
        while (node != nullptr) {
            Node* next = node->next_;
            delete node;
            node = next;
        }
        tab_[ind] = nullptr;
    }
    size_ = 0;
}

void HTable::del(Node* prev, Node* cur) {
    prev->next_ = cur->next_;
    delete cur;
    --size_;
}

void HTable::tabInit() {
    for (size_t ind = 0; ind < tab_.size(); ++ind) {
        if (tab_[ind] == nullptr) {
            tab_[ind] = new Node;
        }
    }
}

void HTable::print() {

    for (int i = 0; i < tab_.size(); ++i) {
        if (tab_[i] == nullptr) {
        } else {
            Node *ptr = tab_[i]->next_;
            while (ptr != nullptr) {
                cout << ptr << " ";
                ptr = ptr->next_;
            }
        }
    }

}
