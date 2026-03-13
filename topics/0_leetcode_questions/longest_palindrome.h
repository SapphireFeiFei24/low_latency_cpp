#pragma once

string longestPalindrome(string s) {
    int n = s.size();
    if (n <= 1) return s;

    int start = 0, max_len = 1;

    auto expand = [&](int l, int r) { // tidiness
        while (l >= 0 && r < n && s[l] == s[r]) {
            --l;
            ++r;
        }
        return pair<int,int>{l + 1, r - l - 1};
    };

    for (int i = 0; i < n; ++i) {
        for (auto [l, len] : {expand(i, i), expand(i, i+1)}) {
            if (len > max_len) {
                start = l;
                max_len = len;
            }
        }
    }

    return s.substr(start, max_len); // allocate only once
}
