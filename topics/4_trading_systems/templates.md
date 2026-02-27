# Templates

## 1. Sort
> Return bool, true means it's placed in front of the other.
```c++
vector<vector<int>> tasks;
...
std::sort(tasks.begin(), tasks.end(), [&](const vector<int>& a, const vector<int>& b) {
    return a[1] < b[1]; // ascending
});
```
## 2. PriorityQueue
> Return bool, true means it's placed at the bottom. (Less priority ones get popped out earlier)
```c++
using MaxHeap = std::priority_queue<int>;
using MinHeap = std::priority_queue<int, std::vector<int>, std::greater>;

// customized cmp func
auto cmp = [&]()(const vector<int>& a, const vector<int>& b) {
    return a[1] < b[1]; // ascending
});
using MaxHeap = std::priority_queue<int, std::vector<int>, decltype<cmp>()>;

// customized callable struct
struct Comparator {
bool operator()(const vector<int>& a, const vector<int>& b) const {
    return a[1] < b[1]; // ascending
}
using MaxHeap = std::priority_queue<int, std::vector<int>, decltype<cmp>()>;
};
```
## 3. Hash Key
> * Define a hash key for user defined type.
> * `map` `unordered_map` `set` `unordered_set`

```c++
using Key = std::array<int, 26>;
struct HashKey {
    size_t operator()(const Key& k) const { // has to be const
        // return size_t instead of int to avoid undefined overflow
        size_t h = 0;
        // 31 is a prime number for better distribution of hashing result. less conflict possibility
                // there'll be overflow, however unsigned overflow is well defined, like modular(signed is not defined)
        for (const auto& v: k) {
            h = h * 31 + std::hash<int>{}(v);
        }
    };
};
```

## 4. List Operation
> Using splice to avoid extra object creation
```c++
std::list<int> l1, l2;
std::list<int>::iterator it;
...
it = l2.begin();
// get it element from l2 and insert it into the beginning of l1
l1.splice(l1.begin(), l2, it);
```
## 5. Binary Search
> * Use idx to check the validity of search result
> * Assumption: `data` is sorted in ascending order.
```c++
// Find the last element that's less than target
auto iter = std::lower_bound(data.begin(), data.end(), target, [&](const std::vector<int> data, const int target) {
    return data[1] < target;
});
int idx = iter - data.begin() - 1;
if (idx >= 0) {
    // valid
}

// Find the last element that's less and equal than target
auto iter = std::upper_bound(data.begin(), data.end(), target, [&](const std::vector<int> data, const int target) {
    return data[1] < target;
});
int idx = iter - data.begin() - 1;
if (idx >= 0) {
    // valid
}
```
## 6. Union Find
> Use vector.
> * Use rank: grow the lower rank, lower rank ++
> * Use size: grow the bigger size, bigger size += smaller size
```c++
int N; // number of nodes

std::vector<int> parent(N, -1);
std::vector<int> sizes(N, 0);
auto find = [&](int a) {
    auto root = parent[a];
    while (parent[root] != root) {
        parent[root] = parent[parent[root]];
        root = parent[root];
    }
    return root;
};
auto unite = [&](int a, int b) {
    int ra = find(a), rb = find(b);
    if (sizes(ra) < sizes(rb)) {
        // always keep a > b
        std::swap(ra, rb);
    }
    root[rb] = ra; // merge into the bigger one
    sizes[ra] += sizes[rb]; // grow the bigger one -> less updates in the future
}
```
## 7. DFS/BFS

## 8. Random
```c++
class Solution {
std::mt19937 gen_; // initialize once
public: 
Solution() {
    gen_ = std::mt19937(std::random_device{}());
}

int random(N) {
    std::uniform_int_distribution<int> dist(1, N) // range from [1, N]
    int val = dist(gen_); // distribution with seeds
    return val;
}
};
```
## 9. Memory Allocation
```c++
std::allocator<int> alloc;
int capacity;
int *data;
alloc.allocate(data, capacity);
alloc.deallocate(data, capacity);

// data copy
int *old_data, *new_data;
int old_size;
alloc.allocate(new_data, old_size * 2);
std::copy(old_data, old_data + old_size, new_data);
```