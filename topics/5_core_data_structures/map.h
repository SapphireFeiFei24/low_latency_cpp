#pragma once

#include <map>
#include <unordered_map>
/***
 * Ordered map Red-Black Tree (self-balancing BST)
 * insert, find, erase -> O(logN)
 * Properties:
 *  Median: Root and/or one of its children
 * Why prefer ordered map over unordered map?
 *  Ordered
 *  Predictable worse case complexity: O(logN) vs O(N)
 ***/

void use_map () {
    std::map<int, int> values;
    values.top();  // median, might need to include one of its child
    values.rbegin(); // highest val
    values.begin(); // lowest val

    // binary search
    auto it = values.lower_bound(10); // first ele >= 10
    auto it2 = values.upper_bound(10); // first ele > 10
}