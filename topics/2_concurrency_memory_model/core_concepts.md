# Concurrency
## Lock: Mutex
> Protect shared data from being simultaneously accessed by multiple threads \
> Preventing data races and ensuring data consistency
```c++
#include <mutex>
std::mutex mtx; // exclusive
std::shared_mutex shared_mtx; // shared

std::unique_lock(mtx);
std::shared_lock(shared_mtx); // can also use unique
```
### Generic Logical Implementation
* A counter: number of active readers
* Stat Flag: if a writer is currently active or wating
* Condition Variables

## Lock-free: atomic
> Implemented using a combination of special CPU instructions and compiler-level memory barriers

### Hardware
* exclusive access on memory
### Complex operator: Compare and Swap Loops
## Lock-free Programming
> Achieved using atomic operation and techniques like Compare-and-Swap(CAS) loops

### `compare_exchange_weak` `compare_exchange_strong`
> * `compare_exchange_weak`: may fail even if value is the same, used in a loop
> * `compare_exchange_strong`: only fail when value are different, handles any internal retries to ensure success, only do one shot try
```c++
void increment() {
    int expected = counter.load();
    // Loop until we successfully swap 'expected' with 'expected + 1'
    while (!counter.compare_exchange_weak(expected, expected + 1)) {
        // 'expected' is automatically updated with the current value on failure
    }
    
    
     std::atomic<int> shared_val(10);
    int expected = 10;
    int desired = 20;

    // Strong version: only fails if shared_val != expected.
    // If it fails, 'expected' is updated with the current value of shared_val.
    if (shared_val.compare_exchange_strong(expected, desired)) {
        std::cout << "Success! Value updated to 20.\n";
    } else {
        std::cout << "Failed. Current value was: " << expected << "\n";
    }
}
```
## Performance: Mutex vs Atomic
> In general, atomics significantly outperform mutexes for simple operations. \
> Because they operate at the hardware level, avoiding the expensive operating system "kernel calls" and "context switches" required by locks.
![img.png](img.png)

## Race condition
> Multiple threads or process access shared data concurrently, and the final outcome depends on the unpredictable timing and sequence of their execution.

## Dead lock
> occurs in a multithreaded environment when two or more threads are blocked indefinitely, each waiting for a resource that the other is holding. This causes the program to freeze or become unresponsive.

### Some Symptoms
* CPU: drops to zero, enter waiting state
* Memory: stable or increase
* Pending Task: accumulate rapidly
* Recovery: manual restart