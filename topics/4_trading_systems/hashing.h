#pragma once

class Solution {
    using Key = std::array<int, 26>;

    struct KeyHash {
        size_t operator()(const Key& k) const {
            // override operator for hashing
            size_t h = 0;
            for (const int v: k) {
                // 31 is a prime number for better distribution of hashing result. less conflict possibility
                // there'll be overflow, however unsigned overflow is well defined, like modular(signed is not defined)
                h = h * 31 + std::hash<int>{}(v);
            }
            return h;
        }
    };
    Key compress(const std::string& s) {
        Key key{};
        for (const auto& c: s) {
            // int(c) - int('a') no need casting
            ++key[c - 'a'];
        }
        return key;
    }

public:
    vector<vector<string>> groupAnagrams(vector<string>& strs) {
        std::unordered_map<Key, int, KeyHash> memo;
        vector<vector<string>> res;
        for (const auto& s: strs) {
            auto k = compress(s);
            auto iter = memo.find(k);
            if (iter == memo.end()) {
                res.push_back({s}); // emplace back don't take this
                memo[k] = res.size() - 1;
            } else {
                res[iter->second].emplace_back(s);
            }
        }
        return res;
    }
};