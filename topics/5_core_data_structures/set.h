#pragma once

#include <set>
#include <vector>
/***
 * Multiset
 * Red-Black tree, support duplicates
 * Use Case:
 *  Maintain sorted values with duplicates
 *  Top K sliding window
 ***/

void use_set () {
    // init with vector
    std::vector<int> vec;
    std::multiset<int> set(vec.begin(), vec.begin() + 3);
}