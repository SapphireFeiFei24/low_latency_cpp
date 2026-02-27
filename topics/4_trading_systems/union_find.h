#pragma once

class Solution {
// avoid reinitialization, need inline to compile
const static inline std::vector<std::pair<int, int>> directions {
 {0, 1}, {0, -1}, {1, 0}, {-1, 0}
};
public:
    int numIslands(vector<vector<char>>& grid) {
        // Do not use unordered_map as it contains rehashing and not local friendly
        // Union find with rank, optimal balance
        const int R = grid.size();
        const int C = grid[0].size();
        int n = R * C;
        std::vector<int> root(n, -1);
        std::vector<int> rank(n, 0);
        int count = 0;
        auto index = [&](int r, int c) {
            return r * C + c;
        };

        auto find = [&](int c) {
            while (root[c] != c) {
                root[c] = root[root[c]]; // path halving
                c = root[c];
            }
            return c;
        };

        auto unite = [&](const int c1, const int c2) {
            auto r1 = find(c1);
            auto r2 = find(c2);
            if (r1 == r2) return;
            if (rank[r1] < rank[r2]) {
                // always rank1 > rank2
                std::swap(r1, r2);
            }

            root[r2] = r1; // grow the bigger sizes
            rank[r1] += rank[r2];

            // merge count
            --count;
        };

        // init all land
        for (int r = 0; r < R; ++r) {
            for (int c = 0; c < C; ++c) {
                if (grid[r][c] == '0') continue;
                root[index(r, c)] = index(r, c);
                rank[index(r, c)] = 1;
                ++count;
            }
        }

        // merge
        for (int r = 0; r < R; ++r) {
            for (int c = 0; c < C; ++c) {
                if (grid[r][c] == '0') continue;
                for (auto& d: directions) {
                    int nr = r + d.first;
                    int nc = c + d.second;
                    if (nr >= 0 and nr < R and nc >= 0 and nc < C and grid[nr][nc] == '1') {
                        unite(index(r, c), index(nr, nc));
                    }
                }
            }
        }
        return count;
    }
};