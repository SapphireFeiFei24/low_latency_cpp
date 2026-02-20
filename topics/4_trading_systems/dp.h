class Solution {
   static constexpr int MOD = 1'000'000'007;

public:
    int numRollsToTarget(int n, int k, int target) {

        std::vector<int> dp(target + 1);
        for (int i = 1; i <= std::min(k, target); ++i) {
            dp[i] = 1;
        }

        for (int i = 1; i < n; ++i) {
            std::vector<int> new_dp(target + 1); // new alloc every time, can use std::fill instead
            for (int j = 1; j <= target; j++) { // prev result
                for (int kk = 1; kk <= k && (kk + j) <= target; ++kk) {
                    new_dp[kk + j] = (new_dp[kk + j] + dp[j]) % MOD;
                }
            }
            dp = std::move(new_dp);
        }
        return dp[target];
    }
};


class Solution {
public:
    int numSquares(int n) {
        std::vector<int> dp(n+1, n+1);
        dp[0] = 0;
        for (int i = 1; i * i <= n; ++i) {
            dp[i*i] = 1;
        }
        for (int i = 2; i <= n; ++i) {
            for (int j = 1; j*j <= i; ++j) { // sqrt(N)
                dp[i] = std::min(dp[i], dp[j*j] + dp[i - j*j]);
            }
        }
        return dp[n];

    }
};