#include "HashMap.hpp"

void HMap::insert(const std::string& key, const std::string val) {

   int64_t hcode = std::hash<std::string>{}(key) & ht1.mask_;
   ht1.h_insert(new Node(hcode, val));

   // use 1.0 as k_max_load_factor
   if (ht1.size_ == ht1.cap_) {
        startResize();
   }
   continueResize();
}

std::optional<std::string> HMap::lookup(const std::string& key) {

   int64_t hcode = std::hash<std::string>{}(key) & ht1.mask_;

   std::optional<std::string> res = ht1.h_find(hcode);
   if (res == std::nullopt) {
        return ht2.h_find(hcode);
   }
   return res;
}

void HMap::del(const std::string key) {

   int64_t hcode = std::hash<std::string>{}(key) & ht1.mask_;
   Node* res = ht1.h_del(hcode);
   if (res == nullptr) {
        res = ht2.h_del(hcode);
   }
   if (res) {
        delete res;
   }
}

void HMap::startResize() {

    ht2 = std::move(ht1);
    ht1.h_resize(2 * ht2.cap_);
    resizing_pos = 0;
}

void HMap::continueResize() {

    size_t nwork = 0;

    while (nwork < k_resizing_work && ht2.size_ > 0) {
        Node* node = ht2.tab_[resizing_pos]->next_;
        if(node != nullptr) {
            ht2.h_del(node->hcode_);
            ht1.h_insert(node);
            ++nwork;
        } else {
            ++resizing_pos;
        }
    }
}