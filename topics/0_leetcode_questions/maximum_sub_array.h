#pragma once
/***
 * DP Solution: total max and the max ending with curr idx
 ***/
class Solution {
public:
    int maxSubArray(vector<int>& nums) {
        int curr_max, total_max;
        for (int i = 0; i < nums.size(); i++) {
            curr_max = std::max(curr_max+nums[i], nums[i]);
            total_max = std::max(total_max, curr_max);
        }
        return total_max;
    }
};