# Operating System Checklist
## Process vs Thread
* Process:
  * Own address space
  * Heavyweight
  * IPC required to communicate
* Thread
  * Shared address space, heap
  * Lightweight
  * Faster context switch

## Process Memory Layout
```aiignore
High Address
+------------------+
| Stack            |  ← function calls, local vars, grows down
+------------------+
| Heap             |  ← dynamic allocation, grows up
+------------------+
| BSS              |  ← (Block started Symbol) uninitialized globals
+------------------+
| Data             |  ← initialized globals
+------------------+
| Text / Code      |
+------------------+
Low Address
```

## Virtual Memory
> * Illusion of Large Memory
> * Isolation between process
> * Lazy allocation

### Page
> a fixed-length, contiguous block of virtual address space used by the operating system to manage memory efficiently.
#### Page Faults
> If the CPU attempts to access a page not currently in **RAM**, a "page fault" occurs, triggering the OS to load the data from the disk.
> This causes high latency, try to avoid this in trading system.

### Virtual Memory Address vs Page Table vs TLB
* Virtual Memory Address
  *  This is the overarching system enabling virtual addressing. It divides memory into "pages".
* Page Table (in RAM)
  * Data structure mapping virtual pages to physical frames.
  * When the CPU needs a data address, it uses the page table, which is a table in memory, to find the corresponding physical frame. This is a "page walk".
  * Slow
* TLB (Translation Lookaside Buffer, in CPU)
  * Hardware cache storing recent virtual-to-physical translations.
  * Fast
  * If TLB miss, check Page Table

## Heap vs Stack
> * Stack: Functions, function params, local variables, **fixed sized arrays(array)**, Object with automatic storage durations
>   * Speed: Fast allocation and deallocation
>   * Size Limits: small
> * Heap: **Dynamic Data Structures(vector)**, new, longer lifetime, large block of data
>   * Manual, might cause memory leaks if not taken care of
>   * Speed: Slower than stack allocation; involves searching for available memory blocks.
>   * Size Limits: larger than stack
### Heap Allocation is Slower than Stack
* Complex Management and Bookkeeping: Allocating memory on the stack is a simple, single instruction operation that involves just moving the stack pointer. Heap allocation is a much more complex process: the memory manager must find a suitably sized free block of memory, mark it as used, and update its internal data structures (e.g., free lists or trees) for tracking allocations and deallocations.
* Memory Fragmentation: As blocks of varying sizes are allocated and freed over time, the heap memory can become fragmented into many small, non-contiguous chunks. This makes finding a large enough single block for a new request more difficult and time-consuming, potentially requiring more extensive searches or even failing the allocation entirely.
* Cache Inefficiency: Stack memory is a single, contiguous block, and access patterns tend to be localized to the top of the stack, making it very cache-friendly. Heap memory allocations can be scattered across different memory locations, leading to more CPU cache misses when accessing related data, which significantly slows down execution.
* System Call Overhead: When the memory manager runs out of available space in its current pool, it must request more memory from the operating system via a system call (e.g., mmap). These calls involve a slow context switch from user mode to kernel mode and back, incurring substantial overhead.

