# Virtual Memory Study Guide
## CSC1107 Operating Systems - Exam Preparation

### 🎯 Key Learning Outcomes
1. **Contrast** the speed and size of data storage locations across physical memory technologies
2. **Justify** the reasons for using a virtual addressing scheme
3. **Navigate** Linux page table data structures to decode a virtual address
4. **Recognize** how page metadata can support memory protection

---

## 📋 Core Concepts Summary

### 1. Physical Memory Fundamentals
- **Key Tradeoff**: Size vs Speed
  - Small memory = Fast access (CPU registers)
  - Large memory = High latency (HDD)
- **Memory Operations**:
  - Read: CPU sends address → receives data
  - Write: CPU sends address + data → memory updates
- **Evolution**: Vacuum tubes (1940s) → Semiconductors (1970s)

### 2. Virtual Memory Benefits
| Benefit | Description |
|---------|-------------|
| **Process Isolation** | Processes can't modify each other's memory |
| **Code Relocation** | Binaries load at same virtual address |
| **Hardware Abstraction** | Uniform view regardless of physical memory changes |

### 3. Virtual Address Space Layout (32-bit)
```
FFFFFFFF  ┌─────────────────┐
          │  Kernel Memory  │
C0000000  ├─────────────────┤
          │     Stack       │ ← %esp (stack pointer)
          │       ↓         │
          │                 │
          │       ↑         │
          │     Heap        │ ← brk (program break)
          ├─────────────────┤
          │  Static Data    │
          ├─────────────────┤
0         │ Program Code    │
          └─────────────────┘
```

### 4. Paging Mechanism
- **Frame**: Fixed-size physical memory blocks
- **Page**: Fixed-size virtual memory blocks
- **Page Size**: Typically 4KB in Linux
- **Benefits**: 
  - Eliminates external fragmentation
  - Non-contiguous allocation
  - Uniform chunk sizes

### 5. Page Table Structure
- **Single-level**: Simple but wastes space
- **Multi-level**: Hierarchical (Linux supports up to 5 levels)
- **32-bit Two-level**:
  - 10-bit first-level index
  - 10-bit second-level index
  - 12-bit page offset

### 6. Translation Lookaside Buffer (TLB)
- **Purpose**: Cache frequently used address translations
- **Performance**: ~10x faster than page table walk
- **TLB Miss**: Expensive, requires page table walk
- **Effective Access Time Formula**:
  ```
  r*(t+m) + (1-r) * (t + (n+1)*m)
  where:
  r = TLB hit ratio
  t = TLB access time
  m = Memory access time
  n = Page table levels
  ```

---

## 🔑 Important Formulas & Calculations

### Virtual Address Breakdown
- **Address size**: m bits
- **Page size**: 2^n bytes
- **Page number**: Upper (m-n) bits
- **Offset**: Lower n bits

### Memory Calculations
- **32-bit address space**: 2^32 = 4GB
- **Page size 4KB**: 2^12 = 4096 bytes
- **Pages in 32-bit space**: 2^32 / 2^12 = 2^20 = 1M pages

---

## 📊 Page Metadata Bits (ARM Linux)

| Bit | Macro | Description |
|-----|-------|-------------|
| 0 | L_PTE_VALID | Page resident in memory? |
| 1 | L_PTE_YOUNG | Recently accessed? |
| 2-5 | - | Cache residency |
| 6 | L_PTE_DIRTY | Needs flush to disk? |
| 7 | L_PTE_RDONLY | Read-only data? |
| 8 | L_PTE_USER | User-mode accessible? |
| 9 | L_PTE_XN | Not executable? |
| 10 | L_PTE_SHARED | Shared between processes? |
| 11 | L_PTE_NONE | Protected from access? |

---

## ⚡ Memory Allocation Methods

1. **Stack Frame**: Automatic allocation for function calls
2. **Heap**: `malloc()` or `new` operators
3. **Grow Heap**: `brk()`/`sbrk()` system calls
4. **Map Region**: `mmap()` system call

---

## 🛠️ ARM Architecture Details

### System Control Coprocessor (CP15)
- **TTBR0**: Process-specific page tables
- **TTBR1**: OS kernel addresses
- **TTBCR**: Controls which base register is used
- **Context Switch**: Updates PGD, requires cache flush

---

## 🎯 Key Commands & Functions

- `getconf PAGESIZE` - Check page size
- `mmap()` - Request memory with permissions
- `mprotect()` - Update page permissions
- `/proc/PID/maps` - View memory segments
- `/proc/PID/pagemap` - View page mappings

---

## ⚠️ Common Issues & Solutions

### Segmentation Faults (SIGSEGV)
- **Cause**: Accessing invalid pages
- **Prevention**: Proper memory management
- **Protection**: Page metadata enforcement

### Internal vs External Fragmentation
- **Internal**: Unused space within allocated pages
- **External**: Unusable gaps between allocated regions
- **Solution**: Paging eliminates external fragmentation

---

## 📚 Further Reading References
- Linux kernel memory management docs
- Gorman's Linux memory management guide
- Operating Systems in Three Easy Pieces (Chapters 12-21)
