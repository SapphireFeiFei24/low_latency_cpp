#pragma once

#include <unordered_map>

/***
 * 2 * Unordered_map + min_freq to Achieve O(1) in complexity
 * TODO: refactor for better readability
 ***/
class LFUCache {

std::unordered_map<int, std::list<std::pair<int, int>>> freq_map_; // key: freq val: list of (key, val)
std::unordered_map<int, std::pair<int, std::list<std::pair<int, int>>::iterator>> key2freq_; // key: key val: (freq, iter)

int capacity_ = 0;
int min_freq_ = 0;

void pop_up(std::pair<int, std::list<std::pair<int, int>>::iterator>& pair) {
        int freq = pair.first;
        auto list_iter = pair.second;

        // move up
        freq_map_[freq+1].splice(freq_map_[freq+1].begin(), freq_map_[freq], list_iter);

        // printf("After move up, size=%d, freq+1=%d first_ele:%d last_ele:%d\n",
        // freq_map_[freq+1].size(),
        // freq+1,
        // freq_map_[freq+1].begin()->first,
        // std::prev(freq_map_[freq+1].end())->first);
        // update key2freq
        pair.first += 1;

        // clear if no other nodes in freq
        if (freq_map_[freq].empty()) {
            freq_map_.erase(freq);
            if (freq == min_freq_) {
                min_freq_++;
            }
        }
}

public:
    LFUCache(int capacity) {
        capacity_ = capacity;
    }

    int get(int key) {
        // printf("Get:%d\n", key);
        auto iter = key2freq_.find(key);
        if (iter == key2freq_.end()) {
            // printf("not found:%d\n", key);
            return -1;
        }
        // found
        auto& val = iter->second;
        // printf("found:%d moving up\n", key);
        pop_up(val);
        return val.second->second;
    }

    void put(int key, int value) {
        if (capacity_ <= 0) {
            return;
        }
        // printf("Put:%d\n", key);
        auto iter = key2freq_.find(key);
        if (iter == key2freq_.end()) {
            if (key2freq_.size() == capacity_) {
                auto& min_freq_list = freq_map_[min_freq_];
                auto last_iter = std::prev(min_freq_list.end());
                int k = last_iter->first;
                // printf("erasing:%d min_freq_:%d\n", key, min_freq_);
                key2freq_.erase(k);
                min_freq_list.pop_back();
                if (min_freq_list.empty()) {
                    freq_map_.erase(min_freq_);
                }
            }
            // create new entry
            // printf("new entry to front:%d\n", key);
            freq_map_[1].push_front(std::make_pair(key, value));
            key2freq_[key] = std::make_pair(1, freq_map_[1].begin());
            min_freq_ = 1;
        } else {
            // update curr val
            auto& val = iter->second;
            auto list_iter = val.second;
            list_iter->second = value;
            // printf("update entry :%d\n", key);
            // move up
            pop_up(val);
        }


    }
};

/**
 * Your LFUCache object will be instantiated and called as such:
 * LFUCache* obj = new LFUCache(capacity);
 * int param_1 = obj->get(key);
 * obj->put(key,value);
 */