# Concurrency
## Mutex
> Protect shared data from being simultaneously accessed by multiple threads \
> Preventing data races and ensuring data consistency
```c++
#include <mutex>
std::mutex mtx; // exclusive
std::shared_mutex shared_mtx; // shared

std::unique_lock(mtx);
std::shared_lock(shared_mtx); // can also use unique
```
## atomic
> Implemented using a combination of special CPU instructions and compiler-level memory barriers

### Hardware
* exclusive access on memory
### Complex operator: Compare and Swap Loops
## Lock-free Programming
> Achieved using atomic operation and techniques like Compare-and-Swap(CAS) loops

## Performance: Mutex vs Atomic
> In general, atomics significantly outperform mutexes for simple operations. \
> Bacause they operate at the hardware level, avoiding the expensive operating system "kernel calls" and "context switches" required by locks.
![img.png](img.png)

## Race condition
> Multiple threads or process access shared data concurrently, and the final outcome dependes on the unpredictable timing and sequence of their execution.

## Dead lock
> occurs in a multithreaded environment when two or more threads are blocked indefinitely, each waiting for a resource that the other is holding. This causes the program to freeze or become unresponsive.

### Some Symptoms
* CPU: drops to zero, enter waiting state
* Memory: stable or increase
* Pending Task: accumulate rapidly
* Recovery: manual restart