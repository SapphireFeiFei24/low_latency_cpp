#pragma once

/**
 * Generate list of range + Binary Search
 */
class Solution {
    std::vector<int64_t> _data;
    std::mt19937 gen_;
public:
    Solution(vector<int>& w) {
        gen_ = std::mt19937(std::random_device{}());
        _data.reserve(w.size());
        int value = 0;
        for (int i = 0; i < w.size(); ++i) {
            value += w[i];
            _data.emplace_back(value);

        }
    }

    int pickIndex() {
        std::uniform_int_distribution<int64_t> dist(1, _data.back());
        int val = dist(gen_);
        auto iter = std::lower_bound(_data.begin(), _data.end(), val);
        return iter - _data.begin();
    }
};

/**
 * Your Solution object will be instantiated and called as such:
 * Solution* obj = new Solution(w);
 * int param_1 = obj->pickIndex();
 */