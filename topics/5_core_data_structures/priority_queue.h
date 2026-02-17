#pragma once

#include <queue>

/***
 * Binary Heap, default max heap
 * O(logN) for insertion/deletion from top
 * O(1) for top
 * Usage:
 *  Top K
 *  Order book best price
 * Disadvantage:
 *  Can't delete arbitrary elements --> Use ordered map instead
 ***/

void use_priority_queue () {
    std::priority_queue<int> max_heap;
    std::priority_queue<int, std::vector<int>, std::greater<int>> min_heap;

}