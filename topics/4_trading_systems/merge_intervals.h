#pragma once

/***
 * Solution 1: two pointer on two sorted vectors, cache friendly
 * Solution 2: priority queue, heavy on branch
 * Verbally better: predefine minheap
 * using MinHeap = priority_queue<int, std::vector<int>, std::greater<int>>; // template

 ***/
class Solution {
public:
    int minMeetingRooms(vector<vector<int>>& intervals) {
        // Solution Two, without heap, cache friendly
        std::vector<int> starts, ends;
        starts.reserve(intervals.size());
        ends.reserve(intervals.size());
        for (const auto& data: intervals) {
            starts.push_back(data[0]);
            ends.push_back(data[1]);
        }
        std::sort(starts.begin(), starts.end());
        std::sort(ends.begin(), ends.end());
        size_t s = 0, e = 0;
        int max_rooms = 0;
        int rooms = 0;
        while (s < starts.size()) {
            if (starts[s] < ends[e]) {
                rooms += 1;
                s += 1;
            } else {
                rooms -= 1;
                e += 1;
            }
            max_rooms = max(max_rooms, rooms);
        }
        return max_rooms;
    }

};

int minMeetingRooms2(vector<vector<int>>& intervals) {
    // Solution one, using heap
    using MinHeap = priority_queue<int, std::vector<int>, std::greater<int>>; // template
    std::sort(intervals.begin(), intervals.end(), [](const vector<int>& a, const vector<int>& b) {
        if (a[0] == b[0]) {
            return a[1] < b[1];
        }
        return a[0] < b[0];
    });

    size_t max_room_cnt = 0;
    MinHeap queue;

    for (const auto& inter: intervals) {
        while (!queue.empty() &&  queue.top() <= inter[0]) {
            queue.pop();
        }

        queue.push(inter[1]);
        max_room_cnt = std::max(max_room_cnt, queue.size());

    }
    return max_room_cnt;
}