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