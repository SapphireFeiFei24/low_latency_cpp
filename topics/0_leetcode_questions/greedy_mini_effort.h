#pragma once
/***
 * Go for the task that can "save" the most energy
 ***/
class Solution {
public:
    int minimumEffort(vector<vector<int>>& tasks) {
        std::sort(tasks.begin(), tasks.end(), [](const std::vector<int>& a, const std::vector<int>& b) {
            return (a[1] - a[0]) > (b[1] - b[0]);
        });

        int total = 0, curr = 0;
        for (const auto& t: tasks) {
            if (curr < t[1]) {
                total += t[1] - curr;
                curr = t[1];
            }
            curr -= t[0];
        }
        return total;
    }
};