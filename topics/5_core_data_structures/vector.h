#pragma once

#include <vector>
/***
 * Contiguous dynamic array
 * Random access O(1)
 * push_back, amortized O(1)
 * insert middle/erase middle O(n)
 *
 * Good: continuous memory, cache friendly
 * Trap: Reallocation invalidates: iterators, references, pointers --> preallocate large size
 ***/
void use_vector () {
    std::vector<int> vec;
    vec.resize(10);
    vec.reserve(10);
    vec.emplace_back(10);
    vec.pop_back();
    vec.begin();
    vec.end();
}

