#pragma once
#include <deque>

/***
 * Implementation: Chunked array, not contiguous
 * block1 -> block2 -> block3
 * push_front, push_back O(1)
 * Random access O(1) but slower than vector
 * Usage: sliding window, queue-like structures
 * Disadvantage: slow.
 ***/