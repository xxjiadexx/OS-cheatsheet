/**************
 * OS(H)
 * memory management lab 
 * Jeremy.Singer@glasgow.ac.uk
 * 17 Feb 2017
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define CELL_SIZE_IN_BYTES  16
#define NUM_CELLS           20
#define NUM_STRINGS         NUM_CELLS

#include "my_memory_allocator.h"

// #define MALLOC      my_malloc
// #define FREE        my_free
// #define INIT_MEM    init_mem_pool()

#define MALLOC      malloc
#define FREE        free
#define INIT_MEM    


// check out https://bewuethr.github.io/sorting-strings-in-c-with-qsort/
// this is a String comparison function, that takes two string pointers
int cmpstr(const void* a, const void* b)
{
  const char* aa = *(const char**)a;
  const char* bb = *(const char**)b;
  return strcmp(aa,bb);
}

int main() {
  FILE *fp;
  char *str;
  char *strings[NUM_STRINGS];
  int i;

  // init memory
  INIT_MEM;
  
  // zero out strings array
  for (i=0; i<NUM_STRINGS; i++) {
    strings[i] = NULL;
  }
  
  // open input file
  fp = fopen("./lines15.txt", "r");
  if (fp == NULL)
    exit(EXIT_FAILURE);
  

  // read in strings
  i = 0;
  str = (char *)MALLOC(sizeof(char)*CELL_SIZE_IN_BYTES);
  if (str==NULL) {
    out_of_mem();
  }
  while (str && fgets(str, CELL_SIZE_IN_BYTES, fp)) {
    // store into array
    strings[i] = str;
    
    // check if this string contains sequence "Once"
    if (strstr(str, "Once")) {
      strings[i] = NULL;
      FREE(str);
    }
    else {
      i++;
    }
    // allocate new str
    str = (char *)MALLOC(sizeof(char)*CELL_SIZE_IN_BYTES);
    if (str == NULL) {
      out_of_mem();
    }
  }
  
  // now do the sort
  qsort(strings, i, sizeof(char*), &cmpstr);
  

  // now print out strings
  for (i=0; i<NUM_STRINGS; i++) {
    if (strings[i] != NULL) {
      printf("%s\n", strings[i]);
      FREE(strings[i]);
    }
  }

  fclose(fp);
}
