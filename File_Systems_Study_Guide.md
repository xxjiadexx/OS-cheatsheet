# 📁 File Systems Study Guide - CSC1107 Operating Systems

## Table of Contents
1. [Why File Systems?](#why-file-systems)
2. [File Attributes and Metadata](#file-attributes-and-metadata)
3. [File Operations](#file-operations)
4. [File Structure Types](#file-structure-types)
5. [File Access Methods](#file-access-methods)
6. [Directory Systems](#directory-systems)
7. [File Sharing and Access Control](#file-sharing-and-access-control)
8. [Unix File Permissions](#unix-file-permissions)
9. [Remote File Systems](#remote-file-systems)
10. [Practice Questions](#practice-questions)

---

## Why File Systems?

### Four Main Purposes of File Systems:

1. **Data Persistence** 📾
   - Unlike volatile RAM, files survive system shutdowns
   - Permanent storage of information
   - Data remains accessible across sessions

2. **Data Organization** 📂
   - Hierarchical structure for logical grouping
   - Efficient file location and management
   - Supports complex directory structures

3. **Metadata** 📋
   - Information about the data itself
   - File attributes like size, permissions, timestamps
   - Enables efficient file management

4. **Protection** 🔒
   - Access control mechanisms
   - User and group permissions
   - Security and privacy enforcement

---

## File Attributes and Metadata

### Core File Attributes:
- **Name** - Only human-readable attribute
- **Identifier** - Unique tag/number within file system
- **Type** - File format classification
- **Location** - Pointer to file location on storage device
- **Size** - Current file size in bytes
- **Protection** - Access permissions (read, write, execute)
- **Timestamps** - Creation, modification, access times
- **User Identification** - Owner and group information

### Inspecting Metadata:
```bash
stat filename.txt
# Shows: size, permissions, timestamps, inode, etc.

ls -l filename.txt
# Shows: permissions, owner, group, size, date
```

---

## File Operations

### Fundamental Operations:
| Operation | Function | System Call |
|-----------|----------|-------------|
| **Create** | Make new file | `creat()` |
| **Open** | Search directory, load to memory | `fopen()` |
| **Read** | Read from current position | `fread()` |
| **Write** | Write at current position | `fwrite()` |
| **Reposition** | Move read/write pointer | `fseek()` |
| **Close** | Save to disk, clean up | `fclose()` |
| **Delete** | Remove file | `remove()` |
| **Truncate** | Reduce file size | `ftruncate()` |

### Open File Management:
- **Open File Table**: OS tracks all opened files
- **File Handle**: Per-process pointer to file location
- **Access Rights**: Read/write permissions per process
- **Reference Count**: Number of processes using file
- Data removed when last process closes file

---

## File Structure Types

### 1. No Structure (Byte Sequence)
- **Description**: Simple sequence of bytes or words
- **Examples**: Binary files, images, executables
- **Advantage**: Maximum flexibility
- **Disadvantage**: Application must interpret structure

### 2. Simple Record Structure
#### Lines
- **Example**: Log files, text files
- **Delimiter**: Newline characters
- **Access**: Line-by-line processing

#### Fixed Length Records
- **Example**: CSV files, database records
- **Advantage**: Predictable positioning
- **Access**: Direct access to Nth record

#### Variable Length Records
- **Example**: JSON, XML files
- **Delimiter**: Special characters or headers
- **Flexibility**: Accommodates varying data sizes

### 3. Complex Structures
- **Examples**: Formatted documents (LaTeX), relocatable files (C object files)
- **Features**: Internal organization, headers, metadata
- **Complexity**: Requires specialized parsers

---

## File Access Methods

### Sequential Access
- **Method**: Read/write byte-by-byte from start to end
- **Use Cases**: Log files, streaming data
- **Advantages**: Simple implementation, good for linear processing
- **Disadvantages**: Cannot skip to specific locations

### Direct Access (Random Access)
- **Method**: Jump to specific offset N for Nth record
- **Requirements**: Fixed record structure
- **Formula**: `Position = Record_Size × Record_Number`
- **Use Cases**: Database records, binary files with known structure

### Side-Index Access
- **Method**: Use separate index structure for complex files
- **Components**: Index file + data file
- **Use Cases**: Databases, complex document systems
- **Advantages**: Very fast lookups, supports complex queries

---

## Directory Systems

### Purpose of Directories:
1. **Efficiency** - Organize and locate files quickly
2. **Grouping** - Logical collections of related files
3. **Naming** - Allow same filename in different directories

### Directory Operations:
- Search for a file
- Create a file
- Delete a file
- List directory contents
- Rename a file
- Traverse the file system

### Tree-Structured Directories
**Features:**
- Hierarchical parent-child relationships
- Working directory concept
- Relative and absolute pathnames
- Can create/delete subdirectories

**Examples:** DOS, early Windows systems

**Path Examples:**
- Absolute: `/home/user/documents/file.txt`
- Relative: `../documents/file.txt`

### Acyclic Graph Directories
**Features:**
- Same file can have multiple names (links)
- Directories contain pointers to files
- Reference counting for safe deletion
- Support for hard and soft links

**Examples:** Unix/Linux systems

**Link Commands:**
```bash
ln original.txt hardlink.txt      # Hard link
ln -s original.txt softlink.txt   # Soft link (symbolic)
```

### Hard Links vs Soft Links:
| Aspect | Hard Link | Soft Link |
|--------|-----------|-----------|
| **Points to** | inode directly | pathname |
| **File deletion** | Safe (reference count) | Creates dangling pointer |
| **Cross filesystems** | No | Yes |
| **Directories** | Usually no | Yes |
| **Storage overhead** | None | Small |

---

## File Sharing and Access Control

### Sharing Requirements:
- **User ID**: Identify individual users
- **Group ID**: Allow users to belong to groups
- **Protection Scheme**: Control access permissions

### Access Control Types:
- **Read** - View file contents
- **Write** - Modify file contents
- **Execute** - Run file as program
- **Append** - Add to end of file
- **Delete** - Remove file
- **List** - View directory contents

### Consistency Semantics:
#### Strict Consistency
- **Behavior**: Writes visible immediately to all users
- **Cost**: Very expensive to implement
- **Use**: Mission-critical systems

#### Eventual Consistency
- **Behavior**: Writes visible after file close/reopen
- **Cost**: Cheaper to implement
- **Risk**: May lead to write conflicts
- **Use**: Most distributed systems

---

## Unix File Permissions

### Permission Structure:
- **9 bits total**: 3 sets of 3 bits each
- **User types**: Owner (u), Group (g), Others (o)
- **Permission types**: Read (r), Write (w), Execute (x)

### Bit Values:
| Permission | Binary | Octal | Symbol |
|------------|--------|-------|--------|
| None | 000 | 0 | --- |
| Execute | 001 | 1 | --x |
| Write | 010 | 2 | -w- |
| Write + Execute | 011 | 3 | -wx |
| Read | 100 | 4 | r-- |
| Read + Execute | 101 | 5 | r-x |
| Read + Write | 110 | 6 | rw- |
| All permissions | 111 | 7 | rwx |

### Common Permission Patterns:
```bash
chmod 755 script.sh    # rwxr-xr-x (executable script)
chmod 644 file.txt     # rw-r--r-- (readable file)
chmod 600 private.txt  # rw------- (private file)
chmod 666 shared.txt   # rw-rw-rw- (shared writable)
```

### chmod Command Formats:
#### Method 1: Symbolic
```bash
chmod u+rw file1              # Add read/write for owner
chmod g-wx file2              # Remove write/execute for group
chmod +x file3                # Add execute for all
chmod ug+rw,o-rwx file4       # Complex combination
```

#### Method 2: Octal
```bash
chmod 755 file5               # rwxr-xr-x
chmod 644 file6               # rw-r--r--
chmod 600 file7               # rw-------
```

### Reading ls -l Output:
```bash
-rw-r--r-- 1 user group 1024 Jan 1 12:00 file.txt
│││││││││
│││││││└─ Others permissions (r--)
│││││└──── Group permissions (r--)
│││└─────── Owner permissions (rw-)
││└──────── File type (- = regular file, d = directory)
└───────── Permission bits
```

---

## Remote File Systems

### Examples:
- **FTP** - File Transfer Protocol
- **Git** - Distributed version control
- **OneDrive/Dropbox** - Cloud storage services
- **NFS** - Network File System
- **SMB/CIFS** - Windows file sharing

### Authentication Requirements:
- User login credentials
- Access tokens
- Certificate-based authentication
- Multi-factor authentication

---

## Mounting File Systems

### Mounting Process:
1. File system must be mounted before access
2. Unmounted file system attached at mount point
3. Mount points integrate different file systems

### Examples:
- **Unix/Linux**: `/mnt`, `/media`, custom mount points
- **Windows**: Drive letters (C:, D:, E:)
- **USB devices**: Automatic mounting at predefined locations

---

## Practice Questions

### Short Answer Questions:

1. **What are the four main reasons we need file systems?**
   - Data persistence, Data organization, Metadata, Protection

2. **Which file attribute is kept in human-readable form?**
   - Only the file name

3. **What does the 'stat' command show?**
   - File metadata including size, permissions, timestamps, inode

4. **What is the difference between fopen() and fclose()?**
   - fopen(): searches directory, loads file entry to memory
   - fclose(): saves file entry from memory to disk

5. **What three benefits do directories provide?**
   - Efficiency (organizing/locating), Grouping (logical collections), Naming (same names in different dirs)

### Calculation Questions:

1. **Convert chmod 755 to symbolic notation:**
   - Answer: rwxr-xr-x

2. **What octal value represents rw-r-----?**
   - Answer: 640 (6=rw-, 4=r--, 0=---)

3. **If a file has permissions -rwxrw-r--, what chmod command sets this?**
   - Answer: chmod 764 filename

### Scenario Questions:

1. **A file has 3 hard links. What happens when you delete the original file?**
   - The file remains accessible through the other 2 hard links (reference count = 2)

2. **You create a soft link to a file, then delete the original file. What happens?**
   - The soft link becomes a dangling pointer (broken link)

3. **Explain why eventual consistency might cause problems in a shared file system.**
   - Multiple users might edit a file simultaneously, and their changes only become visible after close/reopen, potentially causing conflicts or lost work

---

## Key Commands Summary

```bash
# File information
stat filename
ls -l filename
file filename

# Permissions
chmod 755 filename
chmod u+rwx,g+rx,o+rx filename

# Links
ln original.txt hardlink.txt
ln -s original.txt softlink.txt

# File operations
cp source dest
mv old new
rm filename
touch filename

# Directory operations
mkdir dirname
rmdir dirname
cd dirname
pwd
ls dirname

# File system operations
mount /dev/sdb1 /mnt/usb
umount /mnt/usb
df -h
lsof
```

---

## Study Tips for Exam Success

1. **Practice chmod calculations** - Convert between octal and symbolic notation
2. **Understand link differences** - Know when hard vs soft links are appropriate
3. **Memorize common permissions** - 755, 644, 600 patterns
4. **Review file operation sequence** - Open → Read/Write → Close cycle
5. **Study directory structures** - Tree vs acyclic graph differences
6. **Know consistency models** - Strict vs eventual consistency trade-offs

---

*This study guide covers all major file system concepts from CSC1107. Use it alongside the interactive File Systems Study App for comprehensive exam preparation!*
