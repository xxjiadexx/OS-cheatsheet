# Virtual Memory Quick Reference Card
## Last-Minute Review for Exam

### 🚀 Must-Know Definitions

| Term | Definition |
|------|------------|
| **Virtual Memory** | Creates illusion of large, flat main memory using hardware + OS |
| **MMU** | Memory Management Unit - hardware that translates virtual → physical addresses |
| **Page** | Fixed-size virtual memory block (typically 4KB) |
| **Frame** | Fixed-size physical memory block |
| **TLB** | Translation Lookaside Buffer - caches address translations |
| **Page Fault** | Exception when accessing invalid/missing page |

---

### ⚡ Critical Formulas

```
Virtual Address = Page Number + Page Offset
Page Offset Bits = log₂(Page Size)
Pages in Address Space = 2^(Address Bits - Offset Bits)

TLB Effective Access Time:
r×(t+m) + (1-r)×(t+(n+1)×m)
```

---

### 🎯 Key Numbers to Remember

- **32-bit address space**: 4GB (2³²)
- **Typical page size**: 4KB (2¹²)
- **Page offset bits**: 12 (for 4KB pages)
- **Pages in 32-bit space**: 1M (2²⁰)
- **Linux page table levels**: Up to 5
- **ARM 32-bit default**: 2-level page tables
- **Virtual space split**: 3:1 or 2:2 (user:kernel)

---

### 📊 Virtual Address Space Layout

```
High Addresses (FFFFFFFF)
┌─────────────────────────┐
│    Kernel Space         │ ← TTBR1
├─────────────────────────┤ ← C0000000
│    Stack (grows down)   │ ← %esp
│           ↓             │
│                         │
│           ↑             │
│    Heap (grows up)      │ ← brk
├─────────────────────────┤
│    Static Data          │
├─────────────────────────┤
│    Program Code         │
└─────────────────────────┘
Low Addresses (0)
```

---

### 🔧 ARM Page Table Hierarchy

```
Process → PGD → PMD → PTE → Physical Frame
         (P4D, PUD may exist in 64-bit)
```

**32-bit ARM (2-level):**
- 10 bits: First level index
- 10 bits: Second level index  
- 12 bits: Page offset

---

### 🛡️ Page Protection Bits (ARM)

| Bit | Name | Meaning |
|-----|------|---------|
| 0 | VALID | In physical memory? |
| 1 | YOUNG | Recently accessed? |
| 6 | DIRTY | Modified/needs writeback? |
| 7 | RDONLY | Read-only? |
| 8 | USER | User accessible? |
| 9 | XN | Execute Never (not executable)? |
| 10 | SHARED | Shared between processes? |

---

### ⚠️ Common Exam Scenarios

#### Address Translation Problem:
1. Given: Virtual address, page size
2. Find: Page number, offset, physical address
3. Method: Split address based on page size

#### TLB Performance Problem:
1. Given: Hit ratio, access times, page table levels
2. Find: Effective memory access time
3. Method: Use weighted average formula

#### Memory Layout Problem:
1. Given: Process segments and sizes
2. Find: Number of pages, virtual space layout
3. Method: Calculate pages needed, arrange in address space

---

### 🔥 Last-Minute Tips

#### What to Focus On:
- ✅ Virtual vs Physical address distinction
- ✅ Page table translation process
- ✅ TLB performance calculations
- ✅ Memory protection mechanisms
- ✅ Virtual address space layout

#### Common Traps:
- ❌ Confusing page size with address size
- ❌ Forgetting multi-level page table overhead
- ❌ Not considering TLB miss penalty
- ❌ Mixing internal/external fragmentation

#### Time-Saving Tricks:
- Powers of 2: 2¹⁰=1K, 2²⁰=1M, 2³⁰=1G
- Page size 4KB = 2¹² → 12 offset bits
- Quick check: Total bits = Page bits + Offset bits

---

### 📱 Quick Commands

```bash
# Check page size
getconf PAGESIZE

# View process memory layout  
cat /proc/PID/maps

# Virtual memory stats
cat /proc/meminfo
```

---

### 🎓 Exam Strategy

1. **Read carefully** - Virtual vs Physical addresses
2. **Draw diagrams** - Address space layouts help
3. **Show work** - Step-by-step for calculations
4. **Check units** - KB, MB, GB, bits vs bytes
5. **Verify answers** - Do results make sense?

**Good luck! You've got this! 🌟**
