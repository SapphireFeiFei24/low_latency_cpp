#pragma once

#include <queue>

#include "iterator.h"
/***
 * How to remember cmp(a,b)
 * for sort: true means a should come before b
 * for priority_queue: true means a has lower priority than b
 ***/

void sort_priority_queue() {
    using MaxHeap = std::priority_queue<int>;
    using MinHeap = std::priority_queue<int, std::vector<int>, std::greater<int>>;
}

void sort_vectors() {
    std::vector<std::vector<int>> vec{};
    std::sort(vec.begin(), vec.end(), [](const std::vector<int>& lhs, const std::vector<int>& rhs) {
        // ascending order
        if (lhs[0] == rhs[0]) {
            return lhs[1] < rhs[1];
        }
        return lhs[0] < rhs[0];
    });
}
