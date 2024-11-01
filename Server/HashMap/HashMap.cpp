#include "HashMap.hpp"

void HMap::insert(const std::string& key, const std::string& val) {

   size_t hcode = std::hash<std::string>{}(key);
   ht1.h_insert(key, hcode, val, nullptr);

   // use 1.0 as k_max_load_factor
   if (ht1.size_ == ht1.cap_) {
        startResize();
   }
   continueResize();
}

std::optional<std::string> HMap::lookup(const std::string& key) {

   size_t hcode = std::hash<std::string>{}(key);

   std::optional<std::string> res = ht1.h_find(hcode);
   if (res == std::nullopt) {
        return ht2.h_find(hcode);
   }
   return res;
}

bool HMap::del(const std::string& key) {

   size_t hcode = std::hash<std::string>{}(key);
   Node* res = ht1.h_del(hcode);
   if (res == nullptr) {
        res = ht2.h_del(hcode);
   }
   if (res) {
        delete res;
        return true;
   }
   return false;
}

std::vector<std::string> HMap::keys() {

    std::vector<std::string> ht1_keys = ht1.h_keys();
    std::vector<std::string> ht2_keys = ht2.h_keys();
    ht1_keys.insert(ht1_keys.end(), ht2_keys.begin(), ht2_keys.end());
    return ht1_keys;
}

void HMap::startResize() {

    ht2 = std::move(ht1);
    ht1 = HTable(ht2.cap_ * 2);
    resizing_pos = 0;
}

void HMap::continueResize() {

    size_t nwork = 0;

    while (nwork < k_resizing_work && ht2.size_ > 0) {

        Node* node = ht2.tab_[resizing_pos]->next_;
        if(node != nullptr) {
            Node* nodeNew = ht2.h_del(node->hcode_);
            assert(nodeNew != nullptr);
            ht1.h_insert(node->key_, node->hcode_, node->val_, nodeNew);
            ++nwork;
        } else {
            ++resizing_pos;
        }
    }
}