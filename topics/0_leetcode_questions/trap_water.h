#pragma once

/***
 * Three Optimal solutions
 * DP, Two Pointers, Monotonous Stack
 * The water trapped is decided by min(left_max,right_max) - curr_height for each cell
 ***/

class Solution {
public:
    int trap(vector<int>& height) {
        int water = 0;
        int left = 0, right = height.size() - 1;
        int left_max = 0, right_max = 0;
        while (left < right) {
            if (height[left] > height[right]) {
                if (height[right] >= right_max) {
                    right_max = height[right];
                } else {
                    water += right_max - height[right];
                }
                right--;
            } else {
                if (height[left] >= left_max) {
                    left_max = height[left];
                } else {
                    water += left_max - height[left];
                }
                left++;
            }
        }
        return water;
    }
};