# Virtual Memory Practice Questions
## Exam Preparation - CSC1107 Operating Systems

### 🔥 Quick Review Questions

#### 1. Conceptual Understanding
1. **What are the three main benefits of virtual memory?**
   - Process isolation, code relocation, hardware abstraction

2. **What is the fundamental tradeoff in memory technology?**
   - Size vs Speed (Small = Fast, Large = Slow)

3. **What does MMU stand for and what is its role?**
   - Memory Management Unit - translates virtual to physical addresses

#### 2. Address Space Calculations

**Q1:** In a 32-bit system with 4KB pages:
- How many bits for page offset? **12 bits** (2^12 = 4KB)
- How many bits for page number? **20 bits** (32-12)
- How many pages in virtual address space? **2^20 = 1,048,576 pages**

**Q2:** Virtual address `0x12345678` with 4KB pages:
- Page number: `0x12345` (upper 20 bits)
- Page offset: `0x678` (lower 12 bits)

#### 3. TLB Performance

**Q3:** Calculate effective memory access time:
- TLB hit ratio: 95%
- TLB access time: 1ns
- Memory access time: 100ns
- Page table levels: 2

**Solution:**
```
Effective time = 0.95 * (1 + 100) + 0.05 * (1 + 3 * 100)
               = 0.95 * 101 + 0.05 * 301
               = 95.95 + 15.05
               = 111ns
```

---

### 📝 Short Answer Questions

#### 1. Virtual Memory Benefits
**Q:** Explain why virtual memory provides process isolation.
**A:** Each process has its own virtual address space. The MMU prevents one process from accessing another's physical memory by controlling address translation. Invalid addresses cause segmentation faults rather than system corruption.

#### 2. Page Table Structure
**Q:** Why does Linux use multi-level page tables instead of single-level?
**A:** Single-level page tables waste space by allocating entries for unused virtual addresses. Multi-level tables only allocate memory for actually used portions of the address space, significantly reducing memory overhead.

#### 3. Page Size Trade-offs
**Q:** Compare advantages of small vs large page sizes.
**A:** 
- **Small pages (4KB)**: Less internal fragmentation, better memory utilization
- **Large pages**: Fewer TLB entries needed, reduced TLB misses, better for data-intensive applications

#### 4. TLB Importance
**Q:** Why is the TLB critical for system performance?
**A:** Without TLB, every memory access would require multiple additional memory accesses for page table walks. TLB caching makes address translation ~10x faster, dramatically improving overall system performance.

---

### 🎯 Problem-Solving Questions

#### Problem 1: Virtual Address Translation
Given a 32-bit system with 4KB pages and 2-level page tables:
- First level: 10 bits
- Second level: 10 bits  
- Offset: 12 bits

**Translate virtual address `0xABCDE123`:**

1. Extract components:
   - First level index: `0xABC` (bits 31-22)
   - Second level index: `0xDE1` (bits 21-12)  
   - Page offset: `0x123` (bits 11-0)

2. Translation process:
   - Use first level index to find second level page table
   - Use second level index to find physical frame
   - Add offset to get final physical address

#### Problem 2: Memory Layout Analysis
A process has the following memory segments:
- Code: 100KB
- Static data: 50KB  
- Heap: 200KB (grows up)
- Stack: 150KB (grows down)

**Questions:**
1. How many 4KB pages needed for each segment?
2. What's the minimum virtual address space size?
3. How would these be arranged in the virtual address space?

**Solutions:**
1. Pages needed:
   - Code: ⌈100KB/4KB⌉ = 25 pages
   - Static: ⌈50KB/4KB⌉ = 13 pages
   - Heap: ⌈200KB/4KB⌉ = 50 pages
   - Stack: ⌈150KB/4KB⌉ = 38 pages

2. Minimum space: (25+13+50+38) × 4KB = 504KB

3. Layout: Code (low) → Static → Heap (grows up) → Gap → Stack (grows down, high)

---

### 🔧 Linux-Specific Questions

#### 1. Page Metadata
**Q:** What happens when L_PTE_XN bit is set in a page table entry?
**A:** The page is marked as non-executable, preventing code execution. This is a security feature to prevent buffer overflow attacks.

#### 2. System Calls
**Q:** Which system call would you use to:
- Allocate memory with specific permissions? **`mmap()`**
- Change permissions of existing pages? **`mprotect()`**
- Grow the heap? **`brk()` or `sbrk()`**

#### 3. Process Information
**Q:** How can you view memory layout of a running process with PID 1234?
**A:** 
- Segments: `cat /proc/1234/maps`
- Page mappings: Use `/proc/1234/pagemap` (binary format)

---

### 💡 Exam Tips

#### Common Mistakes to Avoid:
1. Confusing virtual vs physical addresses
2. Forgetting that page size affects offset bits
3. Not accounting for all memory accesses in TLB calculations
4. Mixing up internal vs external fragmentation

#### Key Formulas to Remember:
- Pages in address space = 2^(address_bits - offset_bits)
- Page offset bits = log₂(page_size)
- TLB effective time = hit_ratio × (tlb_time + mem_time) + miss_ratio × (tlb_time + page_walk_time)

#### Process for Address Translation:
1. Split virtual address into page number + offset
2. Use page number to index page table
3. Get physical frame number
4. Concatenate frame number + offset = physical address

Good luck with your exam! 🍀
