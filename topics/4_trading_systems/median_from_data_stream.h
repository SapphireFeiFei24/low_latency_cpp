#pragma once
#include <queue>
/***
 * Two version
 * 1. Can only add, and get_median --> Two heaps
 * 2. Sliding window: add + remove, and get_median --> Multiset
 ***/
class MedianFinder {
public:
    MedianFinder() {
    }

    void addNum(int num) {
        // always _low_data.size() <= _high_data.size(), optimize branch
        _low_data.push(num);
        _high_data.push(_low_data.top());
        _low_data.pop();

        if (_high_data.size() > _low_data.size()) {
            // move high data to low data
            _low_data.push(_high_data.top());
            _high_data.pop();
        }

    }

    double findMedian() {
        bool is_even = ((_low_data.size() + _high_data.size()) & 1) == 0;
        if (is_even) {
            // pay attention to overflow
            return (double(_low_data.top()) + double(_high_data.top())) / 2;
        }

        return _low_data.top();
    }

private:
    std::priority_queue<int> _low_data;
    std::priority_queue<int, std::vector<int>, std::greater<int>> _high_data;
};


class Solution {
public:
    vector<double> medianSlidingWindow(vector<int>& nums, int k) {
        std::vector<double> medians;
        medians.reserve(nums.size() - k + 1);

        // init
        std::multiset<int> window(nums.begin(), nums.begin() + k);

        std::multiset<int>::iterator mid = std::next(window.begin(), k / 2); // point to the mid or the second of the mid

        for (size_t i = k;; i++) {
            medians.push_back((double(*mid) + double(*std::next(mid, k % 2 - 1))) * 0.5);

            if (i == nums.size()) {
                break;
            }
            window.insert(nums[i]);

            if (nums[i] < *mid) { // insertion before mid spoiled the idx
                mid--;
            }

            if (nums[i-k] <= *mid) { // delete before mid spoiled the idx
                mid++;
            }
            window.erase(window.lower_bound(nums[i-k]));

        }

        return medians;
    }
};