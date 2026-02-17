#pragma once
#include <vector>
#include <unordered_map>
class Solution {
public:
    std::vector<int> twoSum(std::vector<int>& nums, int target) {

        std::unordered_map<int, int> seen;
        seen.reserve(nums.size());
        for (int i = 0; i < int(nums.size()); i++) {
            int k = target - nums[i];
            auto iter = seen.find(k);
            if (iter!= seen.end()) {
                // no need preallocate result
                return {iter->second, i};
            }
            seen[nums[i]] = i;
        }
        // exception hanlding
        throw std::logic_error("No solution");
    }
};