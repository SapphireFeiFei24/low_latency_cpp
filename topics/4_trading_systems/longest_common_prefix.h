#pragma once


class Solution {
public:
    string longestCommonPrefixOptimal(vector<string>& strs) {
        if (strs.empty()) return ""; // Check validity

        string res = strs[0];

        for (size_t i = 1; i < strs.size() && !res.empty(); ++i) {
            size_t j = 0;
            size_t limit = min(res.size(), strs[i].size());

            while (j < limit && res[j] == strs[i][j]) {
                ++j;
            }
            res.resize(j);
        }

        return res;
    }

    string longestCommonPrefix(vector<string>& strs) {
        // Not Optimal, Resized twice, Not Clean
        if (strs.empty()) return "";

        string res = strs[0];
        for (int i = 1; i < strs.size(); i++) {
            // while (res.size() > strs[i].size()) {
            //     res.pop_back();
            // }

            res.resize(std::min(res.size(), strs[i].size()));
            int j = 0;
            for (; j < res.size(); j++) {
                if (res[j] != strs[i][j]) {
                    break;
                }
            }
            // res = res.substr(0, j); // Extra memory allocation
            res.resize(j);
        }
        return res;
    }
};

