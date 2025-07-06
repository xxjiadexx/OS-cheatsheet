/*******************
 * CSC1107 Operating Systems
 * Lab 2: Memory
 *
 * my_memory_allocator.h
 * 
 * version 0: simply call C library malloc/free
 */

#include <stdio.h>
#include <unistd.h>
#include <sys/mman.h>


/////////////////////////
// VERSION 0
/////////////////////////

void init_mem_pool() {
  // do nothing
  return;
}

// v0: my_malloc simply wraps malloc
void *my_malloc(size_t num_bytes) {
  return malloc(num_bytes);;
}

// v0: free routine simply wraps free
void my_free(void *ptr) {
  return free(ptr);;
}


