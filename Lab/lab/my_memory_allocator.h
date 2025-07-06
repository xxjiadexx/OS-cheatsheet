/*******************
 * CSC1107 Operating Systems
 * Lab 2: Memory
 *
 * my_memory_allocator.h
 * 
 * Students need to add code to this file
 * to implement the memory manager.
 */

#define LAB_VERSION 0

#if LAB_VERSION == 0
#include "my_memory_allocator_v0.h"
#elif LAB_VERSION == 1
#include "my_memory_allocator_v1.h"
#elif LAB_VERSION == 2
#include "my_memory_allocator_v2.h"
#elif LAB_VERSION == 3
#include "my_memory_allocator_v3.h"
#endif

/**
 * report error, when there is no memory left
 */
void out_of_mem() {
    fprintf(stderr, "out of memory!\n");
    exit(-1);
}
