#include "HashTable.hpp"

HTable::HTable()
: size_(0)
, cap_(INIT_CAP)
, mask_(INIT_CAP - 1)
{}

HTable::HTable(HTable&& other)
: tab_(std::move(other.tab_))
, size_(other.size_)
, cap_(other.cap_)
, mask_(other.mask_)
{
    tab_.resize(cap_);
}
    
HTable& HTable::operator = (HTable&& other){

    if (this != &other) {
        clear();
    }

    this->tab_  = std::move(other.tab_);
    this->size_ = other.size_;
    this->cap_  = other.cap_;
    this->mask_ = other.mask_;

    other.tab_.resize(INIT_CAP, std::unique_ptr<Node>());
    other.size_ = 0;
    other.cap_ = INIT_CAP;
    other.mask_ = INIT_CAP;
    return *this;
}

HTable::~HTable() {
    clear();
}

void HTable::h_insert(Node* node) {

    Node* ptr = tab_[node->hcode_]->next_;

    for (; ptr != nullptr; ptr = ptr->next_) {

        if (ptr->hcode_ == node->hcode_) {
            ptr->val_ = std::move(node->val_);
            delete node;
            break ;
        }
        if (ptr->next_ == nullptr) {
            break;
        }
    }
    if (ptr == nullptr) {
        ptr->next_ = node;
    }
    ++size_;
}

std::optional<std::string> HTable::h_find(int64_t hcode) const {

    Node* ptr = tab_[hcode]->next_;

    for (; ptr != nullptr; ptr = ptr->next_) {
        if (ptr->hcode_ == hcode) {
            return ptr->val_;
        }
    }
    if (ptr == nullptr) {
        return std::nullopt;
    }
}

Node* HTable::h_del(int64_t hcode) {

    Node* prev = tab_[hcode].get();
    Node* ptr = tab_[hcode]->next_;
    for (; ptr != nullptr; ptr = ptr->next_) {
        if (ptr->hcode_ == hcode) {
            prev->next_ = ptr->next_;
            --size_;
            return ptr;
        }
        prev = ptr;
    }
    return nullptr;

}

void HTable::h_resize(size_t sz){
    tab_.resize(sz);
}

void HTable::clear() {

    for (size_t ind = 0; ind < tab_.size(); ++ind) {
        Node* node = tab_[ind].get();
        while(node->next_ != nullptr) {
            del(node, node->next_);
        }
    }
}

bool HTable::del(Node* prev, Node* cur) {
    prev->next_ = cur->next_;
    delete cur;
    --size_;
}
