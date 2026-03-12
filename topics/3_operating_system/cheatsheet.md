# Operating System
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
## Cache
### L1/L2/L3 Cache
> Specialized, high-speed SRAM memory levels embedded in or near the CPU, designed to minimize data access latency from slower DRAM.
> Storing the Data.
* L1 Cache
  * Fastest & Smallest memory.
  * Built directly into each CPU core.
  * 32KB ~ 128KB
  * instruction cache(I-Cache) and Data cache(D-Cache)
  

* L2 Cache
  * Larger but slightly slower than L1
  * Near Core
  * Secondary buffer
  * 256KB to 1MB

* L3 Cache
  * largest and slowest of the cache levels, still significantly faster than main RAm
  * shared among all CPU cores and serves to coordinate data between them
  * 4MB ~ 64Mb+

### RAM
> Stores the Data. AND Page Table.
* Slower than L1/L2/L3 Cache.

### Cache line
> Cache line size: the number of bytes loaded together in one entry in the cache \
> Usually 64 bytes

### False Sharing
> Performance-degrading phenomenon in **multi-threaded** programs where multiple threads inadvertently access different, logically independent variables that happen to reside on the same cache line in the CPU cache.

* How it occurs
  * entire cache line loaded -> one core modifies one part of it -> the entire line marked as invalid for other cores -> other cores tries to access sth on this line -> triggers reloading the whole line
* What's happening
  * Thread A (on CPU 1) reads the cache line → gets a local copy 
  * Thread B (on CPU 2) reads the same cache line → gets its own copy 
  * Thread A writes to its variable → marks the cache line as "modified" on CPU 1 
  * The cache coherence protocol (e.g. MESI) invalidates CPU 2's copy 
  * Thread B now needs to write → must fetch the updated line from CPU 1 first 
  * This ping-pong repeats on every write from either thread

## Virtual Memory
> * Illusion of Large Memory
> * Isolation between process
> * Lazy allocation
> * **Each process has its own private page tables.**
### Page
> a fixed-length, contiguous block of virtual address space used by the operating system to manage memory efficiently.
#### Page Faults
> If the CPU attempts to access a page not currently in **RAM**, a "page fault" occurs, triggering the OS to load the data from the disk.
> This causes high latency, try to avoid this in trading system.

### Virtual Memory Address vs Page Table vs TLB
> Store Virtual to Physical Address Mapping. Not the data.
* Virtual Memory Address
  *  This is the overarching system enabling virtual addressing. It divides memory into "pages".
* Page Table (in RAM)
  * Each process has its own set of page tables
  * Data structure mapping virtual pages to physical frames.
  * When the CPU needs a data address, it uses the page table, which is a table in memory, to find the corresponding physical frame. This is a "page walk".
  * Slow
* TLB (Translation Lookaside Buffer, in CPU)
  * Hardware cache storing recent virtual-to-physical translations. **Not the actual data**
  * Fast
  * If TLB miss, check Page Table
  ```text
  ┌─────────────────┬──────────────────┬─────────────────────────────┐
  │  Virtual Page   │  Physical Frame  │         Metadata            │
  │  Number (VPN)   │  Number (PFN)    │                             │
  ├─────────────────┼──────────────────┼─────────────────────────────┤
  │  0xDEAD_B000    │  0x0042_1000     │ valid, dirty, user, RW, etc │
  └─────────────────┴──────────────────┴─────────────────────────────┘
  ```

## Memory Management
### Stack vs Heap Definition

#### Stack
* temporary workspace for function calls and local variables
* memory lifetime is tied to the function/class
* Data is small (~1MB)
* Risk: stackoverflow

#### Heap
* Dynamic memory allocation
* managed by developer
* Data is larger and flexible
* Risk: memory leak
### Heap vs Stack
> * Stack: Functions, function params, local variables, **fixed sized arrays(array)**, Object with automatic storage durations
>   * Speed: Fast allocation and deallocation
>   * Size Limits: small
>   * Thread safe: isolated within single cpu
> * Heap: **Dynamic Data Structures(vector)**, new, longer lifetime, large block of data
>   * Manual, might cause memory leaks if not taken care of
>   * Speed: Slower than stack allocation; involves searching for available memory blocks.
>   * Size Limits: larger than stack
### Heap Allocation is Slower than Stack
* Complex Management and Bookkeeping: Allocating memory on the stack is a simple, single instruction operation that involves just moving the stack pointer. Heap allocation is a much more complex process: the memory manager must find a suitably sized free block of memory, mark it as used, and update its internal data structures (e.g., free lists or trees) for tracking allocations and deallocations.
* Memory Fragmentation: As blocks of varying sizes are allocated and freed over time, the heap memory can become fragmented into many small, non-contiguous chunks. This makes finding a large enough single block for a new request more difficult and time-consuming, potentially requiring more extensive searches or even failing the allocation entirely.
* Cache Inefficiency: Stack memory is a single, contiguous block, and access patterns tend to be localized to the top of the stack, making it very cache-friendly. Heap memory allocations can be scattered across different memory locations, leading to more CPU cache misses when accessing related data, which significantly slows down execution.
* System Call Overhead: When the memory manager runs out of available space in its current pool, it must request more memory from the operating system via a system call (e.g., mmap). These calls involve a slow context switch from user mode to kernel mode and back, incurring substantial overhead.

### Best Practice
1. Use Smart Pointers
2. RAII(Resource Acquisition Is Initialization)
   * Encapsulation resources within objects
3. Prefer Stack Allocation
4. Avoid Raw Pointers
5. Use Std library (mem management handled internally).

## Process
### Definition
* An instance of a running program
* Has its own
  * virtual address space
  * file descriptor table
  * stack & heap
  * registers
  * PID
  * Signal handlers
  * Scheduling state

### Copy-On-Write
> Does not copy memory immediately. This makes fork() relatively cheap. 
* Parent & child share same physical pages
  * A private copy is only made when a write operation occurs
* Marked read-only
  * Will not be able to modify parent memory by default
* When one writes → page fault → kernel copies page
  * O(number of page tables), not full memory copy

### `fork()`
* Creates a new child process, gets a copy of parent
* After fork
  * Memory: copy-on-write
  * File descriptors: Shared
  * Heap: COW
  * Stack: COW

### `vfork()`
> Very dangerous.
> * Child must NOT modify memory
    >   * Must NOT return from function
>   * Must NOT call exit() (use _exit())
> * Used for:
    >     * Immediately calling exec()
>     * Avoid page table duplication
* Child shares same address space
* Parent is suspended until
  * child calls exec(), OR,
  * child calls _exit()
* **No copy-on-write**
* Faster:
  * No page table duplication
  * No COW setup
  * Parent blocked -> no race
* Risky

### `clone()`
> `fork()` is implemented using `clone()`
clone allows:
* Share memory
* Share file descriptors
* Share signal handlers
* Create threads

## IPC(inter-process communication)
### Shared Memory
* `mmap()`: map files or devices into a process's virtual memory space
* `shmget()`: return the identifier of the shared memory segment associated with the value of the argument key
### Pipes andSockets

## TLB flush
> clears cached virtual-to-physical address mappings in the CPU's memory management unit.
### When
Occurs during context switches between processes (if ASIDs are not used), when freeing virtual memory via mmap, during page table updates, or via TLB shootdowns (when one CPU notifies others to flush).
### Performance Impact
Expensive. Forces the CPU to perform costly page table walks to re-cache mappings

### Why
TLB flushes ensure security by preventing a process from accessing memory that has been reassigned to another process or the kernel.

## Page Table Walk
> Triggered by a TLB miss. Expensive. Tree traverse on Multilevel Table.

A page table walk is the hardware-driven (MMU) or software-driven (kernel) process of traversing multi-level page tables to translate a virtual address to a physical address. 

## System Call
> The programmatic secure method by which a user-level program requests specialized services from the operating system's kernel,
> such as assessing hardware (dist, network), creating processes, or managing memory.

### Why expensive?
* User-to-Kernel Mode Switching(Context Switching): save the current process's execution context to memory to ensure it can resume later.
* Privilege Level Changes:  Moving to kernel mode involves changing CPU rings of execution and modifying segment registers, which is much slower than a simple function call, which only changes the instruction pointer within user space.
* Cache and TLB Thrashing: Switching to the kernel means the CPU starts executing different code, often resulting in "cache misses." The CPU's hot cache lines—data and instructions currently being used by the application—are evicted to make room for kernel data, causing a slowdown until the cache is repopulated.
* I/O Blocking: when used for Input/Output

### Examples
* process control
  * `fork()`, `exec`, `wait()`
* file management
  * `open()`, `read()`, `write()`, `clone()`
* device management
  *  `ioctl()`, `read()`
* information maintainance
  * `getpid()`
* communication
  * `pipe()`, `socket()`
## Debugging Cache Misses
### Identify hot loops/functions
```shell
perf top
```
### L1/L2/L3 Level Cache Profiling
#### `perf`
>Perf doesn’t tell you the exact memory address by default, but it tells you the line/function.
```shell
perf record -e cache-misses -g ./my_program
perf report
```

## SoA(Struct of Array) vs AoS(Array of Struct)
### When to use AoS
* Always access all fields together
* Simple objects, low N

### When to use SoA
* Access a few fields
* GPU/SIMD heavy code

## Memory Pool
### Disadvantage of calling `new` `delete` repeatedly
* Memory fragmentation
  * slower allocations
  * poor cache locality
* Unpredictable latency
  * search free lists
  * lock internal structures
  * request memory from OS
* Does not guarantee
  * contiguous memory
  * cache-friendly layout
### Two Scenarios
* Arena memory: alloc and dealloc in sequence
  * Continuous block
  * record top
  * message queue, request process
* Free-List Memory Pools
  * Random allocation, random free
  * Long-lived objects