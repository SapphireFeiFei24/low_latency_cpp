#pragma once
#include <stdexcept>

/***
 * Vector(random) + unordered_map(find/insert/delete)
 * Swap the last ele with the deleted one, remove the last ele, O(1)
 ***/
class RandomizedSet {
    std::vector<int> container_;
    std::unordered_map<int, int> cache_;
    std::mt19937 gen_;

public:
    RandomizedSet() {
        // expensive only init once
        gen_ = std::mt19937(std::random_device{}());
    }

    bool insert(int val) {
        if (cache_.find(val) != cache_.end()) {
            return false;
        }
        container_.emplace_back(val);
        cache_[val] = container_.size() - 1;
        return true;
    }

    bool remove(int val) {
        auto iter = cache_.find(val);
        if (iter == cache_.end()){
            return false;
        }

        int idx = iter->second;
        swap(container_[idx], container_[container_.size()-1]);
        cache_[container_[idx]] = idx;

        // Update first before erase, or else it'll end error
        container_.pop_back();
        cache_.erase(val);
        return true;
    }

    int getRandom() {
        // safe guard in production
        if (container_.empty()) {
            throw std::logic_error("no data");
        }
        std::uniform_int_distribution<int> dist(0, container_.size()-1);
        return container_[dist(gen_)];
    }
};

/**
 * Your RandomizedSet object will be instantiated and called as such:
 * RandomizedSet* obj = new RandomizedSet();
 * bool param_1 = obj->insert(val);
 * bool param_2 = obj->remove(val);
 * int param_3 = obj->getRandom();
 */