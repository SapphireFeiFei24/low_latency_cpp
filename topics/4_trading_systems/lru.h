#pragma once
#include <list>
#include <unordered_map>
class LRUCache {
public:
    typedef std::pair<int, int> Node;
    LRUCache(int capacity) {
        capacity_ = capacity;
    }

    int get(int key) {
        auto iter = key2iter_.find(key);
        if (iter == key2iter_.end()) {
            return -1;
        }

        int val = iter->second->second;
        // Extra copy happen if like this
        // data_.emplace_front(*iter->second);
        // data_.erase(iter->second);
        data_.splice(data_.begin(), data_, iter->second);
        return val;
    }

    void put(int key, int value) {
        auto iter = key2iter_.find(key);
        if (iter != key2iter_.end()) {
            // key exist
            iter->second->second = value;
            data_.splice(data_.begin(), data_, iter->second);
        } else {
            data_.emplace_front(key, value);
        }


        key2iter_[key] = data_.begin();

        if ((key2iter_.size()) > capacity_) {
            int expired_key = std::prev(data_.end())->first;
            data_.pop_back();
            key2iter_.erase(expired_key);
        }
    }

private:

    std::list<Node> data_;
    std::unordered_map<int, std::list<Node>::iterator> key2iter_; // unorder for O(1) lookup
    int capacity_;
};

/**
 * Your LRUCache object will be instantiated and called as such:
 * LRUCache* obj = new LRUCache(capacity);
 * int param_1 = obj->get(key);
 * obj->put(key,value);
 */