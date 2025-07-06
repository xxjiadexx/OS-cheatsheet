/*******************
 * CSC1107 Operating Systems
 * Lab 2: Memory
 *
 * my_memory_allocator.h
 * 
 * version 1: simply call mmap for each malloc
 */

#include <stdio.h>
#include <unistd.h>
#include <sys/mman.h>


/////////////////////////
// VERSION 1
/////////////////////////

void init_mem_pool() {
  // do nothing
  return;
}

// v1: malloc simply wraps mmap
void *my_malloc(size_t num_bytes) {
  // void *p = NULL;
  /* call mmap here */
  // void *addr = mmap(NULL, size, PROT_READ | PROT_WRITE,
  //                   MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);
  // return p;
  return mmap(NULL, // address to use (we don't care)
               num_bytes, // size of the mapping
               PROT_READ | PROT_WRITE, // r/w permissions
               MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);
}

// v1: free routine could be no-op,
// or could do an munmap
// (dual of mmap, to release mapping back to OS)
// The key challenge is that munmap
// requires a `size` parameter, which we can't
// get directly from the `free` context, so we
// would need to store in runtime metadata, somewhere.
void my_free(void *ptr) {
  return;
}


