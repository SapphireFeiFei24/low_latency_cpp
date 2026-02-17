#pragma once
#include <iostream>
#include <list>
#include <map>
#include <vector>
#include <map>
#include <ostream>
/***
 * Iterator: A generalized pointer that abstracts traversal of a container
 *
 * vector -> Raw pointer
 * list -> point to node
 * map -> pointer to tree node
 * unordered_map -> pointer to bucket node
 *
 * Categories:
 * Random access(it+n, it-n, []): vector, deque
 * Bidirectional(++it, --it, cannot it+n): list, map, set
 * Forward(++it only): unordered_map
 * Input/Ouput: streams, only once
 ***/

void use_vector_iterator() {
    // Reallocation invalidates ALL iterators, push_back, insert, resize
    vector<int>::iterator vi;
    vi + 5;
}

void use_list_iterator() {
    // insertion/erase does not invalidate other iterators
    // only erased iterator invalidated

    std::list<int>::iterator li;
    std::list<int> my_list;
    ++li;
    *li;

    li = my_list.begin();
    li = my_list.end();

    li = my_list.erase(li); // return the next iterator

    // insert to the head
    my_list.insert(my_list.begin(), *li);
    my_list.emplace_front(*li);

    auto back_iter = std::prev(my_list.end());
}

void use_map_iterator() {
    // iterators remain valid after insert, erase
    std::map<int, int> my_map;
    std::map<int, int>::iterator map_it = my_map.begin();
    std::map<int, int>::reverse_iterator map_reit = my_map.rbegin();

    my_map.top(); // median
    auto it = my_map.lower_bound(map_it->first);
}

void move_around() {
    std::list<int> data = {10, 20, 30, 40};
    auto it = data.begin(); // it points to 10

    // Get an iterator to the next element (20)
    auto next_it = std::next(it);
    std::cout << "Next element: " << *next_it << std::endl; // Output: Next element: 20

    // Get an iterator to the element two positions ahead (30)
    auto two_ahead = std::next(it, 2);
    std::cout << "Element two ahead: " << *two_ahead << std::endl; // Output: Element two ahead: 30

    // Get an iterator to the element before two_ahead (20)
    auto previous_it = std::prev(two_ahead);
    std::cout << "Previous element: " << *previous_it << std::endl; // Output: Previous element: 20

    auto previous_next_it = std::prev(two_ahead, 2);
    std::cout << "Previous next: " << *previous_next_it << std::endl;
    return 0;
}