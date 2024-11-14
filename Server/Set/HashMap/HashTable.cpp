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
    other.tab_ = std::vector<HNode*>();
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

    other.tab_ = std::vector<HNode*>();
    other.size_ = 0;
    other.cap_ = INIT_CAP;
    other.mask_ = INIT_CAP - 1;
    return *this;
}

HTable::~HTable() {
    clear();
}

const HNode* HTable::h_insert(const std::string& key, size_t hcode, double val, HNode* nodeNew) {

    size_t ind = hcode & mask_;
    HNode* prev = tab_[ind];
    HNode* ptr = tab_[ind]->next_;
    for (; ptr != nullptr; ptr = ptr->next_) {
        if (ptr->hcode_ == hcode) {
            ptr->val_ = val;
            return ptr;
        }
        prev = ptr;
    }
    if (nodeNew == nullptr) {
        prev->next_ = new HNode(key, hcode, val);
    } else {
        prev->next_ = nodeNew;
    }
    ++size_;
    return prev->next_;
}

std::optional<HNode*> HTable::h_find(size_t hcode) const {

    size_t ind = hcode & mask_;
    HNode* ptr = tab_[ind]->next_;

    for (; ptr != nullptr; ptr = ptr->next_) {
        if (ptr->hcode_ == hcode) {
            return ptr;
        }
    }
    return std::nullopt;
}

HNode* HTable::h_del(size_t hcode) {

    size_t ind = hcode & mask_;
    HNode* prev = tab_[ind];
    HNode* ptr = tab_[ind]->next_;
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

void HTable::h_resize(size_t sz) {
    tab_.clear();
    tab_.resize(sz);
    cap_ = sz;
    mask_ = cap_ - 1;
    tabInit();
}

std::vector<std::string> HTable::h_keys() {

    std::vector<std::string> keys;
    keys.reserve(size_);
    for (size_t ind = 0; ind < tab_.size(); ++ind) {

        // Possible seg fault in case tab_[ind] = NULL
        HNode* next = tab_[ind]->next_;
        while (next != nullptr) {
            keys.push_back(next->key_);
            next = next->next_;
        }
    }
    return keys;
}

void HTable::clear() {

    for (size_t ind = 0; ind < tab_.size(); ++ind) {
        HNode* node = tab_[ind];
        while (node != nullptr) {
            HNode* next = node->next_;
            delete node;
            node = next;
        }
        tab_[ind] = nullptr;
    }
    size_ = 0;
}

void HTable::del(HNode* prev, HNode* cur) {
    prev->next_ = cur->next_;
    delete cur;
    --size_;
}

void HTable::tabInit() {
    for (size_t ind = 0; ind < tab_.size(); ++ind) {
        if (tab_[ind] == nullptr) {
            tab_[ind] = new HNode;
        }
    }
}

void HTable::print() {

    for (int i = 0; i < tab_.size(); ++i) {
        if (tab_[i] == nullptr) {
        } else {
            HNode *ptr = tab_[i]->next_;
            while (ptr != nullptr) {
                cout << ptr << " ";
                ptr = ptr->next_;
            }
        }
    }

}
