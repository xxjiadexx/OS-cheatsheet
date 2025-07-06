/*******************
 * CSC1107 Operating Systems
 * Lab 2: Memory
 *
 * my_memory_allocator.h
 * 
 * version 2: use a dedicated memory pool
 * so the allocator becomes a simple
 * bump pointer allocation algorithm
 */

#include <stdio.h>
#include <unistd.h>
#include <sys/mman.h>

/////////////////////////
// VERSION 2
/////////////////////////


#define POOL_SIZE_IN_PAGES 100000
#define PAGE_SIZE 1<<12
void *pool = NULL;

/* initialize the memory pool
 * by calling mmap once, to allocate
 * a large buffer of memory
 */
void init_mem_pool() {
  size_t pool_size = POOL_SIZE_IN_PAGES * PAGE_SIZE;
  // void *p = NULL;
  /* FIXME call mmap here, and assign to p */
  void *p = mmap(NULL, // address to use (we don't care)
                 100000 * 1<<12, // 100K * page size (in bytes)
                 PROT_READ | PROT_WRITE, // r/w permissions
                 MAP_ANON | MAP_PRIVATE, // request RAM, not file
                 -1, // no file handle, since we request RAM
                 0); // no offset required
  if (p==MAP_FAILED) {
    // mmap request unsuccessful,
    p = NULL;
  }
  pool = p;
  return;
}

/* simple bump pointer allocation here
 */
void *my_malloc(size_t num_bytes) {
  static unsigned int index = 0;
  // the INVARIANT is that pool[index]
  // always points to the next free byte of mem
  // in the pool
  void *p;
  if (pool == NULL ||
      index+num_bytes >= POOL_SIZE_IN_PAGES * PAGE_SIZE) {
    // malloc contract specifies
    // we return NULL if we cannot
    // satisfy the allocation request
    p = NULL;
  }
  else {
    // we have enough space in pool to satisfy
    // this allocation request
    /* FIXME - FILL IN HERE */
    // (1) calculate the pointer value to return, based on
    //     current index
    // (2) update the index value
    p = (char *)pool + index;
    index += num_bytes;
  }
  return p;
}

/*
 * for simple bump pointer, free is no-op
 * (and eventually we exhaust all of memory)
 */
void my_free(void *ptr) {
  return;
}
