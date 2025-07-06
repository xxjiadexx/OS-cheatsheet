# Page Replacement & Memory Management Practice Questions
## Exam Preparation - CSC1107 Operating Systems - Lecture 3

### 🔥 Quick Review Questions

#### 1. Core Concepts
1. **What is demand paging?**
   - Load pages into memory only when needed (lazy loading)

2. **What causes a page fault?**
   - Process tries to access a page not currently in physical memory

3. **What's the difference between major and minor page faults?**
   - Major: Data must be read from backing storage
   - Minor: Data is resident but mapped to different process

4. **What is the working set size (WSS)?**
   - Number of pages that must be resident for a process to function efficiently

#### 2. Performance Calculations

**Q1:** Page fault performance calculation:
- Page fault rate: p = 0.1% (1 in 1000 accesses)
- Memory access time: 100ns
- Page fault time: 10ms

**Calculate Effective Access Time (EAT):**
```
EAT = (1-p) × mem_access_time + p × page_fault_time
EAT = 0.999 × 100ns + 0.001 × 10,000,000ns
EAT = 99.9ns + 10,000ns = 10,099.9ns ≈ 10,100ns
```
**Impact:** 100x slower than normal memory access!

**Q2:** Working Set vs Resident Set:
- Virtual address space: 1000 pages
- Working Set Size (WSS): 50 pages
- Resident Set Size (RSS): 30 pages

**Analysis:** RSS < WSS → Process will experience frequent page faults and poor performance

#### 3. Memory Management Concepts

**Q3:** Copy-on-Write (COW) benefits:
- Parent and child processes share physical pages initially
- Only allocate new physical frame when write occurs
- Saves memory and improves fork() performance

---

### 📝 Short Answer Questions

#### 1. Page Replacement Necessity
**Q:** Why is page replacement needed in modern operating systems?
**A:** Physical memory is limited but virtual address spaces can be much larger. When physical memory is full and a new page needs to be loaded, the OS must select a "victim page" to swap out to backing storage, making room for the new page.

#### 2. Page Fault Handling Process
**Q:** Describe the steps in handling a page fault.
**A:** 
1. Check if page is within process address space
2. Verify access permissions (read/write/execute)
3. Locate page data (swap file, file system, or zero-fill)
4. Find free physical frame or select victim page
5. Load page data into physical memory
6. Update page table mapping
7. Resume process execution

#### 3. Cache Types in Memory Management
**Q:** Explain the three types of in-memory caches in Linux.
**A:**
- **Page Cache**: Stores file data in page-sized chunks for faster subsequent access
- **Buffer Cache**: Optimizes block device access by caching frequently used blocks
- **Swap Cache**: Tracks pages written to swap to avoid unnecessary writebacks

#### 4. ASLR Security Benefits
**Q:** How does Address Space Layout Randomization (ASLR) improve security?
**A:** ASLR randomizes positions of stack, heap, and libraries in virtual address space, making it difficult for attackers to predict memory locations for buffer overflow exploits.

---

### 🎯 Problem-Solving Questions

#### Problem 1: Page Replacement Algorithm Simulation
Given pages in memory: [1, 2, 3] (capacity = 3)
Page reference string: 4, 1, 2, 5, 1, 2, 3, 4, 5

**FIFO (First In, First Out):**
```
Step  | Reference | Memory State | Page Fault?
------|-----------|--------------|------------
1     | 4         | [4, 2, 3]    | Yes (replace 1)
2     | 1         | [1, 2, 3]    | Yes (replace 4)  
3     | 2         | [1, 2, 3]    | No
4     | 5         | [1, 5, 3]    | Yes (replace 2)
5     | 1         | [1, 5, 3]    | No
6     | 2         | [1, 5, 2]    | Yes (replace 3)
7     | 3         | [1, 3, 2]    | Yes (replace 5)
8     | 4         | [4, 3, 2]    | Yes (replace 1)
9     | 5         | [4, 5, 2]    | Yes (replace 3)

Total Page Faults: 7
```

**LRU (Least Recently Used):**
```
Step  | Reference | Memory State | Page Fault?
------|-----------|--------------|------------
1     | 4         | [2, 3, 4]    | Yes (replace 1)
2     | 1         | [2, 3, 1]    | Yes (replace 4)
3     | 2         | [3, 1, 2]    | No (move 2 to recent)
4     | 5         | [1, 2, 5]    | Yes (replace 3)
5     | 1         | [2, 5, 1]    | No
6     | 2         | [5, 1, 2]    | No
7     | 3         | [1, 2, 3]    | Yes (replace 5)
8     | 4         | [2, 3, 4]    | Yes (replace 1)
9     | 5         | [3, 4, 5]    | Yes (replace 2)

Total Page Faults: 6
```

#### Problem 2: Memory Over-commitment Analysis
System Configuration:
- Physical RAM: 8GB
- Process A: 4GB virtual memory, WSS = 1GB
- Process B: 6GB virtual memory, WSS = 2GB  
- Process C: 3GB virtual memory, WSS = 1.5GB

**Questions:**
1. **Total virtual memory allocated?** 4GB + 6GB + 3GB = 13GB
2. **Over-commitment ratio?** 13GB / 8GB = 1.625 (62.5% over-committed)
3. **Minimum physical memory needed?** WSS total = 1GB + 2GB + 1.5GB = 4.5GB
4. **Will system thrash?** No, if RSS ≥ WSS for all processes (4.5GB < 8GB available)

#### Problem 3: Copy-on-Write Scenario
Parent process has 10 pages, child process created via fork():

**Initial state:** Both processes share same 10 physical pages
**Child writes to page 3:** 
- New physical frame allocated for child's page 3
- Parent still uses original page 3
- Total physical pages: 11 (10 original + 1 new)

**Memory savings:** Without COW, would need 20 pages immediately. With COW, only 11 pages used.

---

### 🔧 Linux-Specific Questions

#### 1. Swappiness Parameter
**Q:** What does Linux swappiness control and what are typical values?
**A:** 
- Range: 0-100
- Controls frequency of swapping vs dropping cache pages
- Low values (0-10): Prefer dropping cache over swapping
- High values (60-100): More aggressive swapping
- Default: Usually 60

#### 2. OOM Killer Behavior
**Q:** How does the Out-of-Memory killer decide which process to terminate?
**A:** 
- Calculates OOM score for each process
- Considers: memory usage, process age, importance
- Terminates highest scoring process
- Goals: Free large amount of memory, minimize processes killed, protect kernel

#### 3. NUMA Memory Policies
**Q:** Compare interleaved vs node-local NUMA allocation strategies.
**A:**
- **Interleaved**: Round-robin across nodes, uniform average access time
- **Node-local**: Allocate on same node as requesting process, minimize latency for local access

---

### 💡 Algorithm Comparison

#### Page Replacement Algorithms Summary

| Algorithm | Implementation | Pros | Cons |
|-----------|---------------|------|------|
| **Random** | Random selection | Simple, no overhead | Poor performance |
| **FIFO** | Queue of pages | Simple, fair | Belady's anomaly |
| **LRU** | Access tracking | Optimal locality | High overhead |
| **Clock/NRU** | Circular list + access bit | Good approximation | Scanning overhead |

#### Performance Ranking (Best to Worst):
1. **Optimal** (theoretical - knows future)
2. **LRU** (expensive but effective)
3. **Clock/NRU** (good practical choice)
4. **FIFO** (simple but suboptimal)
5. **Random** (baseline)

---

### 🚨 Common Exam Scenarios

#### Scenario 1: Thrashing Detection
**Signs of thrashing:**
- High page fault rate
- Low CPU utilization
- Processes spending more time swapping than computing
- System becomes unresponsive

**Solutions:**
- Increase physical memory
- Reduce process working sets
- Suspend some processes
- Tune swappiness parameter

#### Scenario 2: Security Implications
**W^X Policy:** Pages are either Writable OR Executable, never both
- Prevents code injection attacks
- Stack/heap marked non-executable
- Code pages marked non-writable

**ASLR Benefits:**
- Randomizes memory layout
- Makes ROP/JOP attacks harder
- Complements DEP/NX bit protection

---

### 📊 Key Formulas to Remember

1. **Effective Access Time:** `EAT = (1-p) × mem_time + p × fault_time`
2. **Page Fault Impact:** Even 0.1% fault rate can slow system 100x
3. **Working Set Rule:** `RSS ≥ WSS` for good performance
4. **Over-commitment Ratio:** `Total Virtual Memory / Physical Memory`

---

### 🎯 Exam Strategy Tips

#### For Page Replacement Problems:
1. **Track each step** systematically
2. **Mark recent access** for LRU
3. **Count total page faults** carefully
4. **Consider Belady's anomaly** for FIFO

#### For Performance Calculations:
1. **Convert units** carefully (ms to ns)
2. **Use probability correctly** (1-p for hits, p for misses)
3. **Show intermediate steps** for partial credit

#### Memory Management Concepts:
1. **Understand the purpose** of each algorithm
2. **Know trade-offs** between approaches
3. **Connect to real systems** (Linux examples)

Good luck with Page Replacement questions! 🚀
