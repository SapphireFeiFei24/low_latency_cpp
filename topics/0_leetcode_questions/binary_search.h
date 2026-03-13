#pragma once
/***
 * Koko eating bananas
 * Binary search on the number of bananas
 * No need to sort or other optimization --> overengineering
 ***/
class Solution {
public:
    int minEatingSpeed(vector<int>& piles, int h) {
        int B = *std::max_element(piles.begin(), piles.end());

        auto get_hours = [&](int k) {
            int64_t hours = 0;
            for (const auto& p : piles) {
                hours += p / k + (p % k == 0? 0: 1);
            }
            return hours;
        };
        int left = 1, right = B;
        while (left < right) {
            int mid = (left + right) / 2;
            if (get_hours(mid) <= h) {
                right = mid;
            } else {
                left = mid + 1;
            }
        }
        return left;
    }
};