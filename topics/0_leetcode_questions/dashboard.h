#pragma once
#include "utility"
/***
 * Use set and define pair hash, remove {old_score, id} directly
 */
class Leaderboard {
    using Pair = std::pair<int, int>;
    struct PairHash {
        bool operator()(const Pair& a, const Pair& b) const {
            if (a.first == b.first) {
                return a.second > b.second;
            }
            return a.first > b.first;


        }
    };
    std::unordered_map<int, int> data;
    std::set<Pair, PairHash> lead;

public:
    Leaderboard() {

    }

    void addScore(int playerId, int score) {
        // printf("add:%d %d\n", playerId, score);
        int old_score = 0;
        auto iter = data.find(playerId);
        if (iter != data.end()) {
            old_score = iter->second;
            lead.erase({old_score, playerId});
        }
        int new_score = old_score + score;
        data[playerId] = new_score;
        lead.insert({new_score, playerId});

    }

    int top(int K) {
        int total = 0;
        int top = 0;
        // printf("get top:%d lead(%zu) data(%zu)\n", K, lead.size(), data.size());
        for (auto iter = lead.begin(); top < K && iter != lead.end(); ++top, ++iter) {
            total += iter->first;
        }
        return total;
    }

    void reset(int playerId) {

        int old_score = data.at(playerId);
        lead.erase({old_score, playerId});
        data.erase(playerId);
    }
};

/**
 * Your Leaderboard object will be instantiated and called as such:
 * Leaderboard* obj = new Leaderboard();
 * obj->addScore(playerId,score);
 * int param_2 = obj->top(K);
 * obj->reset(playerId);
 */