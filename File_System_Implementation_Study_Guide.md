# 💾 File System Implementation Study Guide - CSC1107 Operating Systems

## Table of Contents
1. [Block-Structured Storage](#block-structured-storage)
2. [Physical Storage Devices](#physical-storage-devices)
3. [OS Disk Access Architecture](#os-disk-access-architecture)
4. [File System Data Structures](#file-system-data-structures)
5. [Directory Implementation](#directory-implementation)
6. [Block Allocation Methods](#block-allocation-methods)
7. [Unix inode Case Study](#unix-inode-case-study)
8. [Free Space Management](#free-space-management)
9. [FAT File System Case Study](#fat-file-system-case-study)
10. [Practice Questions](#practice-questions)

---

## Block-Structured Storage

### Core Characteristics:
1. **Persistent** 💾
   - Data preserved across power cycles
   - Non-volatile storage (HDD, SSD, Flash)
   - Survives system reboots and shutdowns

2. **Block-Structured** 📦
   - Storage divided into fixed-size units (blocks)
   - Each block has a Logical Block Address (LBA)
   - Standard block size: **4KB** (matches memory page size)
   - Smallest granular unit of storage

3. **Random Access** 🎯
   - Blocks can be accessed in any order
   - No need to read sequentially
   - Contrast with **sequential devices** (magnetic tape)

### Block Size Impact:
```bash
# Check block size on Linux
stat -f .

# Compare file size vs disk usage
ls -lh file.txt    # Shows actual file content size
du -h file.txt     # Shows disk blocks allocated
```

### Internal Fragmentation:
- **Problem**: Small files still occupy entire block
- **Example**: 100-byte file uses 4KB block → 3996 bytes wasted
- **Formula**: `Wasted space = Block size - File size`

---

## Physical Storage Devices

### Traditional Hard Disk Drives (HDDs):
```
Disk Structure:
┌─────────────────┐
│   Magnetic      │ ← Multiple platters
│   Platters      │   (double-sided)
│                 │
│   Read/Write    │ ← Two heads per platter
│   Heads         │   (top/bottom surfaces)
└─────────────────┘

Block Address = (Cylinder, Head, Sector)
```

**Key Components:**
- **Cylinders**: All tracks at same radius across platters
- **Tracks**: Concentric circles on each platter surface
- **Sectors**: Smallest addressable units on tracks
- **Heads**: Read/write mechanisms (one per platter surface)

### Modern Flash Memory (NAND/NOR):

| Type | NOR Flash | NAND Flash |
|------|-----------|------------|
| **Access Speed** | Faster read | Slower read |
| **Cost** | Expensive | Cheaper |
| **Durability** | Lasts longer | Shorter lifespan |
| **Access Method** | Single-byte random | Block-level |
| **Use Case** | Bootloaders, code execution | Mass storage |
| **Error Correction** | Not required | Required |

### NAND Flash Challenges:
1. **Wear Leveling** 🔄
   - Distributes writes across memory cells
   - Prevents premature cell death
   - Extends device lifespan

2. **Error Correction** ⚠️
   - Required due to bit errors over time
   - Error Correction Code (ECC) algorithms
   - Bad block management

---

## OS Disk Access Architecture

### Layered Architecture:
```
┌─────────────────┐
│   System Calls  │ ← Application Interface
├─────────────────┤
│ Logical File    │ ← File names, metadata
│ System (VFS)    │   inodes, directories
├─────────────────┤
│ Basic File      │ ← "retrieve block 123"
│ System          │
├─────────────────┤
│ I/O Control     │ ← Device drivers
│ (Device Driver) │   hardware commands
├─────────────────┤
│ Hardware        │ ← Physical storage
└─────────────────┘
```

### Key Components:

1. **Device Drivers** 🚗
   - Translate high-level commands to hardware-specific operations
   - Example: "read drive 1, cylinder 72, track 2, sector 10"
   - Manage hardware controller communication

2. **Buffer Management** 📋
   - **Buffers**: Hold data in transit (temporary)
   - **Caches**: Store frequently used data (performance)
   - OS manages allocation, freeing, and replacement policies

3. **Logical File System** 🗂️
   - File name → file number translation
   - Maintains File Control Blocks (FCBs/inodes)
   - Directory management and protection

### Virtual File System (VFS):
- **Purpose**: Uniform API for different file systems
- **Benefits**: Same system calls work with ext4, NTFS, FAT, network filesystems
- **Implementation**: Object-oriented approach using vnodes
- **Flexibility**: Dispatches operations to appropriate filesystem implementation

---

## File System Data Structures

### On-Disk Structures:

1. **Boot Control Block** 🚀
   - Contains information to boot OS from volume
   - Located at first block if volume contains OS
   - Essential for system startup

2. **Volume Control Block (Superblock)** 📊
   - Volume metadata and configuration
   - Total number of blocks
   - Number of free blocks
   - Block size
   - Free block pointers/arrays

3. **Directory Structure** 📁
   - Organizes files hierarchically
   - Maps file names to inode numbers
   - May use Master File Table (NTFS)

4. **File Control Block (FCB/inode)** 📄
   - Per-file metadata storage
   - Inode number, permissions, size
   - Creation/modification timestamps
   - Pointers to data blocks

### In-Memory Structures:

#### Opening a File:
```
1. System call: open("file.txt", O_RDONLY)
2. Search directory structure
3. Load FCB into memory
4. Create file handle in process table
5. Return file descriptor to process
```

#### Accessing a File:
```
1. System call: read(fd, buffer, size)
2. Use file descriptor to locate FCB
3. Check permissions and current position
4. Read data blocks via block allocation method
5. Update file position pointer
```

---

## Directory Implementation

### Method 1: Linear List 📝
```
Directory Entry:
┌─────────────┬─────────────┐
│ filename    │ data_ptr    │
├─────────────┼─────────────┤
│ "file1.txt" │ inode_123   │
│ "file2.doc" │ inode_456   │
│ "file3.pdf" │ inode_789   │
└─────────────┴─────────────┘
```
- **Advantages**: Simple to implement
- **Disadvantages**: O(n) search time, slow for large directories
- **Optimization**: Keep alphabetically sorted (linked list or B+ tree)

### Method 2: Hash Table 🔍
```
Hash Function: hash(filename) → bucket_index

Bucket 0: [file1.txt → inode_123]
Bucket 1: [file2.doc → inode_456] → [collision.txt → inode_999]
Bucket 2: [file3.pdf → inode_789]
```
- **Advantages**: O(1) average search time
- **Disadvantages**: Collisions, fixed size entries
- **Collision Handling**: Chained overflow method

---

## Block Allocation Methods

### 1. Contiguous Allocation 📏

**Concept**: Each file occupies consecutive blocks

```
File A: Blocks 0-2
File B: Blocks 3-7
File C: Blocks 8-9

┌───┬───┬───┬───┬───┬───┬───┬───┬───┬───┐
│ A │ A │ A │ B │ B │ B │ B │ B │ C │ C │
└───┴───┴───┴───┴───┴───┴───┴───┴───┴───┘
  0   1   2   3   4   5   6   7   8   9
```

**Advantages:**
- Best performance (sequential access)
- Simple metadata (start block + length)
- Minimal seek time for large files

**Disadvantages:**
- External fragmentation
- Must know file size in advance
- Difficult to grow files
- Requires periodic compaction

### 2. Linked Allocation 🔗

**Concept**: Each block points to next block in file

```
File A: 0 → 3 → 7 → null
File B: 1 → 4 → 5 → 8 → null

┌───┬───┬───┬───┬───┬───┬───┬───┬───┐
│A→3│B→4│   │A→7│B→5│B→8│   │A→∅│B→∅│
└───┴───┴───┴───┴───┴───┴───┴───┴───┘
  0   1   2   3   4   5   6   7   8
```

**Advantages:**
- No external fragmentation
- Files can grow dynamically
- Simple free space management

**Disadvantages:**
- No random access (must traverse links)
- Reliability issues (broken link = lost data)
- Extra storage overhead for pointers
- Poor performance for non-sequential access

### 3. Indexed Allocation 📇

**Concept**: Each file has index block(s) pointing to data blocks

```
File A Index Block:
┌─────┐    ┌───┬───┬───┬───┐
│  0  │ →  │ 2 │ 5 │ 8 │ ∅ │
└─────┘    └───┴───┴───┴───┘
           Data blocks: 2, 5, 8

File B Index Block:
┌─────┐    ┌───┬───┬───┬───┐
│  1  │ →  │ 3 │ 6 │ 7 │ 9 │
└─────┘    └───┴───┴───┴───┘
           Data blocks: 3, 6, 7, 9
```

**Advantages:**
- Random access support
- No external fragmentation
- Dynamic file growth
- Easy to determine file size

**Disadvantages:**
- Index block overhead
- Indirect access (extra I/O for index)
- Index block can become bottleneck

---

## Unix inode Case Study

### inode Structure:
```
inode (128 bytes):
┌─────────────────┐
│ File Type       │ ← Regular, directory, link
│ Permissions     │ ← rwx for user/group/other
│ Link Count      │ ← Number of hard links
│ Owner/Group     │ ← User ID, Group ID
│ File Size       │ ← Size in bytes
│ Timestamps      │ ← atime, mtime, ctime
├─────────────────┤
│ Direct Blocks   │ ← 12 pointers to data blocks
│ (12 pointers)   │   Direct access to first 48KB
├─────────────────┤
│ Indirect Block  │ ← Pointer to block of pointers
│ (1 pointer)     │   Can address 4MB more
├─────────────────┤
│ Double Indirect │ ← Pointer to block of indirect blocks
│ (1 pointer)     │   Can address 4GB more
├─────────────────┤
│ Triple Indirect │ ← Three levels of indirection
│ (1 pointer)     │   Can address 4TB more
└─────────────────┘
```

### inode Block Addressing:
- **Direct blocks**: First 12 blocks (12 × 4KB = 48KB)
- **Indirect**: Points to block containing 1024 pointers (4MB)
- **Double indirect**: Points to 1024 indirect blocks (4GB)
- **Triple indirect**: Points to 1024 double indirect blocks (4TB)

### File Size Calculation:
```
Maximum file size = Direct + Indirect + Double + Triple
= 12×4KB + 1024×4KB + 1024²×4KB + 1024³×4KB
= 48KB + 4MB + 4GB + 4TB
≈ 4TB (for 4KB blocks, 32-bit pointers)
```

---

## Free Space Management

### Method 1: Linked List (Freelist) 🔗
```
Free Block List:
Head → Block 15 → Block 23 → Block 31 → null

Each free block contains pointer to next free block
```
**Advantages:**
- Simple implementation
- No extra space overhead
- Dynamic size

**Disadvantages:**
- Sequential traversal required
- Cannot easily determine contiguous free space
- Fragmentation over time

### Method 2: Bit Vector (Bitmap) 🗺️
```
Block bitmap (1 bit per block):
Blocks: 0 1 2 3 4 5 6 7 8 9
Bitmap: 1 1 0 1 1 0 0 1 0 1
        ↑ ↑   ↑ ↑     ↑   ↑
      Used   Free   Used  Free
```
**Advantages:**
- O(1) to check if block is free
- Easy to find contiguous blocks
- Compact representation

**Disadvantages:**
- Fixed size overhead
- Scanning for free blocks can be slow
- Must keep bitmap consistent

---

## FAT File System Case Study

### File Allocation Table (FAT):
```
FAT Table (simplified):
┌─────┬─────┬─────┬─────┬─────┬─────┬─────┬─────┐
│  0  │  1  │  2  │  3  │  4  │  5  │  6  │  7  │ ← Cluster numbers
├─────┼─────┼─────┼─────┼─────┼─────┼─────┼─────┤
│ EOF │  3  │ EOF │ EOF │  6  │ EOF │ EOF │  0  │ ← Next cluster or marker
└─────┴─────┴─────┴─────┴─────┴─────┴─────┴─────┘

File A: Start cluster 1 → 3 → EOF (clusters 1, 3)
File B: Start cluster 4 → 6 → EOF (clusters 4, 6)
```

### FAT Characteristics:
- **Static allocation**: Fixed-size table at disk beginning
- **Chain tracking**: Each entry points to next cluster or end marker
- **Simple implementation**: Easy to understand and implement
- **Reliability**: Table can be duplicated for redundancy

### FAT Entry Values:
- **0**: Free cluster
- **2-MAX**: Next cluster in chain
- **EOF**: End of file marker
- **BAD**: Bad cluster marker

### Directory Structure:
```
Directory Entry (32 bytes):
┌─────────────────┐
│ Filename (8.3)  │ ← 8 chars + 3 extension
│ Attributes      │ ← Read-only, hidden, directory
│ Creation time   │ ← Timestamp
│ Access date     │ ← Last access
│ Start cluster   │ ← First cluster number
│ File size       │ ← Size in bytes
└─────────────────┘
```

---

## Memory Technology Comparison

| Technology | Access Speed | Volatility | Cost | Capacity | Use Case |
|------------|-------------|------------|------|----------|----------|
| **CPU Registers** | ~1 cycle | Volatile | Highest | Bytes | Active computation |
| **L1 Cache** | ~3 cycles | Volatile | Very High | ~32KB | Frequently used data |
| **RAM** | ~100 cycles | Volatile | High | GBs | Working memory |
| **SSD** | ~25,000 cycles | Non-volatile | Medium | 100s GB | Fast storage |
| **HDD** | ~500,000 cycles | Non-volatile | Low | TBs | Mass storage |
| **Tape** | Very slow | Non-volatile | Lowest | TBs | Archival storage |

---

## Practice Questions

### Short Answer:
1. **What are the three key characteristics of block-structured storage?**
   - Persistent, Block-structured, Random access

2. **Why is block size typically 4KB?**
   - Matches memory page size for efficient I/O and caching

3. **What causes internal fragmentation in block allocation?**
   - Small files occupying entire blocks, leaving unused space

4. **Name the three block allocation methods and their trade-offs.**
   - Contiguous: Fast but fragmentation
   - Linked: No fragmentation but no random access
   - Indexed: Random access but overhead

### Calculation Problems:

1. **Calculate maximum file size for Unix inode with 4KB blocks:**
   ```
   Direct: 12 × 4KB = 48KB
   Indirect: 1024 × 4KB = 4MB
   Double: 1024² × 4KB = 4GB
   Triple: 1024³ × 4KB = 4TB
   Total ≈ 4TB
   ```

2. **Internal fragmentation for 100-byte file with 4KB blocks:**
   ```
   Wasted space = 4096 - 100 = 3996 bytes
   Efficiency = 100/4096 = 2.4%
   ```

### Design Questions:

1. **Compare directory implementation methods:**
   - Linear list: Simple but O(n) search
   - Hash table: O(1) average but collision handling needed

2. **When would you choose each allocation method?**
   - Contiguous: Known size, sequential access (multimedia)
   - Linked: Unknown size, sequential access (logs)
   - Indexed: Random access needed (databases)

---

## Key Commands & Tools

```bash
# Check filesystem information
df -T                    # Show filesystem types
lsblk                    # List block devices
stat -f .                # Show filesystem statistics

# File and block analysis
ls -lh file.txt          # File size in bytes
du -h file.txt           # Disk space used
stat file.txt            # Detailed file information

# inode information
ls -i file.txt           # Show inode number
debugfs /dev/sda1        # Debug filesystem (ext2/3/4)
```

---

## Study Tips for Exam Success

1. **Understand trade-offs** between allocation methods
2. **Practice calculations** for file sizes and fragmentation
3. **Know inode structure** and addressing schemes
4. **Memorize FAT concepts** for the lab session
5. **Compare implementations** (Unix vs FAT vs modern filesystems)

---

*This study guide covers File System Implementation concepts from CSC1107. Review alongside the lab exercises on FAT filesystem for comprehensive understanding!*
