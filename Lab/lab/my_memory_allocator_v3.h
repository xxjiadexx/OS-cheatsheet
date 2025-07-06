/*******************
 * CSC1107 Operating Systems
 * Lab 2: Memory
 *
 * my_memory_allocator.h
 * 
 * version 3: use a dedicated memory pool
 * with an integrated free-list
 * so the allocator becomes a simple
 * free-list allocation algorithm
 */

#include <assert.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/mman.h>

/////////////////////////
// VERSION 3
/////////////////////////

#define CELL_SIZE 32

/**
 * STEP 3.1: Define your type carefully.
 * I recommend a union type, with a neat
 * typedef
 */
typedef union {
  uint8_t bytes[CELL_SIZE];
  void *ptr;
} Cell;

/**
 * STEP 3.2: Initialize a large block of memory
 * You might want a global pointer, to point to 
 * the start of your memory block.
 * You should allocate the block using mmap() or
 * sbrk(). I recommend mmap() here --- search online
 * for details, or (if you are old-fashioned like me)
 * type "man mmap" on the terminal.
 * 
 * You will need to organize your free-list data
 * structure on top of this flat block of memory,
 * ideally by splitting it into a set of fixed
 * size cells and adding a 'next' pointer from each cell
 * to the next one
 * 
 */

#define POOL_SIZE_IN_PAGES 100000 /* 100 Kpages == 400 MB */
#define PAGE_SIZE 1<<12
void *pool = NULL;

Cell *freelist = NULL;

void init_mem_pool() {
  size_t pool_size = POOL_SIZE_IN_PAGES * PAGE_SIZE;
  void *p NULL;
  /* FIXME call mmap here */
  if (p==MAP_FAILED) {
    // mmap request unsuccessful,
    p = NULL;
  }
  else {
    // set up linked list data structure,
    // initially just a linear list, with
    // ordered sequence of cells

    /* FIXME initialize linked list here */

    // freelist points to start of this newly
    // constructed, ordered linked list sequence
    freelist = (Cell *)p;
  }
  pool = p;
  return;

}

/**
 * STEP 3.3: Write your malloc routine.
 * This routine allocates one of the fixed size blocks ---
 * we only have the num_bytes argument for compatibility with
 * the standard malloc routine. You might do an assertion
 * to check the size is the same as your fixed size cells.
 * Your allocation routine needs to return a pointer to
 * one of these cells, and update the metadata so the returned
 * cell is no longer on the freelist.
 * NOTE: if no memory is available then return NULL
 */
void *my_malloc(size_t num_bytes) {
  assert(num_bytes <= sizeof(Cell));
  void *p;
  if (freelist == NULL) {
    p = NULL;
  }
  else {
    // FIXME - pop the first cell from the freelist
    // and return this pointer to the user
  }
  return p;
}

/**
 * STEP 3.4: Write your free routine
 * Given a pointer to a fixed size cell, you need to
 * add this cell to the freelist, updating metadata
 * as appropriate.
 */
void my_free(void *ptr) {
  Cell *empty = (Cell *)ptr;
  // FIXME - push this newly empty
  // cell onto the front of freelist,
  // ensuring we update the empty->ptr
  // field to maintain the linked list
  // state
  return;
}


