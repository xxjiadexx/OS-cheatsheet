// adapted from Hans Boehm's GC benchmark
// by Jeremy.Singer@glasgow.ac.uk
//   -> See https://www.hboehm.info/gc/gc_bench.html
// This code is equivalent to "binarytrees" from
// the Programming Language Shootout benchmark game
//   -> See http://benchmarksgame.alioth.debian.org/u64q/performance.php?test=binarytrees


#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>

#include "my_memory_allocator_v1.h"

#define MALLOC   my_malloc
#define FREE     my_free
#define INIT_MEM init_mem_pool()

// allocate a single tree node of @param size
// and zero out all data bytes
// like calloc, or Java allocation
void *allocate_node(int size) {
  char *ptr = (char *)MALLOC(size);
  int i;
  for (i=0; i<size; i++) {
    ptr[i] = 0;
  }
  return ptr;
}

#ifdef PROFIL
  extern void init_profiling();
  extern dump_profile();
#endif

#define currentTime() stats_rtclock()
#define elapsedTime(x) (x)

/* Get the current time in milliseconds */

unsigned
stats_rtclock( void )
{
  struct timeval t;
  struct timezone tz;

  if (gettimeofday( &t, &tz ) == -1)
    return 0;
  return (t.tv_sec * 1000 + t.tv_usec / 1000);
}

static const int kStretchTreeDepth    = 18;      // about 16Mb
static const int kLongLivedTreeDepth  = 16;  // about 4Mb
static const int kArraySize  = 500000;  // about 4Mb
static const int kMinTreeDepth = 4;
static const int kMaxTreeDepth = 16;

// on x86_64, this structure has size 24 bytes
typedef struct Node0_struct {
        struct Node0_struct * left;
        struct Node0_struct * right;
        int i, j;
} Node0;


typedef Node0 *Node;

void init_Node(Node me, Node l, Node r) {
    me -> left = l;
    me -> right = r;
}

void destroy_Node(Node me) {
  if (me -> left) {
    destroy_Node(me -> left);
  }
  if (me -> right) {
    destroy_Node(me -> right);
  }
  FREE(me);
}


// Nodes used by a tree of a given size
static int TreeSize(int i) {
        return ((1 << (i + 1)) - 1);
}

// Number of iterations to use for a given tree depth
static int NumIters(int i) {
        return 2 * TreeSize(kStretchTreeDepth) / TreeSize(i);
}

// Build tree top down, assigning to older objects.
static void Populate(int iDepth, Node thisNode) {
        if (iDepth<=0) {
                return;
        } else {
	        iDepth--;
		thisNode->left  = allocate_node(sizeof(Node0));
		thisNode->right = allocate_node(sizeof(Node0));
                Populate (iDepth, thisNode->left);
                Populate (iDepth, thisNode->right);
        }
}

// Build tree bottom-up
static Node MakeTree(int iDepth) {
	Node result;
        if (iDepth<=0) {
	  result = allocate_node(sizeof(Node0));
	  /* result is implicitly initialized in both cases. */
	  return result;
        } else {
	    Node left = MakeTree(iDepth-1);
	    Node right = MakeTree(iDepth-1);
	    result = allocate_node(sizeof(Node0));
	    init_Node(result, left, right);
	    return result;
        }
}

static void PrintDiagnostics() {
#if 0
        long lFreeMemory = Runtime.getRuntime().freeMemory();
        long lTotalMemory = Runtime.getRuntime().totalMemory();

        System.out.print(" Total memory available="
                         + lTotalMemory + " bytes");
        System.out.println("  Free memory=" + lFreeMemory + " bytes");
#endif
}

static void TimeConstruction(int depth) {
        long    tStart, tFinish;
        int     iNumIters = NumIters(depth);
        Node    tempTree;
	int 	i;

	printf("Creating %d trees of depth %d\n", iNumIters, depth);
        
        tStart = currentTime();
        for (i = 0; i < iNumIters; ++i) {
	  tempTree = allocate_node(sizeof(Node0));
	  Populate(depth, tempTree);
	  destroy_Node(tempTree);
	  tempTree = 0;
        }
        tFinish = currentTime();
        printf("\tTop down construction took %ld msec\n",
               elapsedTime(tFinish - tStart));
             
        tStart = currentTime();
        for (i = 0; i < iNumIters; ++i) {
                tempTree = MakeTree(depth);
		destroy_Node(tempTree);
                tempTree = 0;
        }
        tFinish = currentTime();
        printf("\tBottom up construction took %ld msec\n",
               elapsedTime(tFinish - tStart));

}

int main() {
        Node    root;
        Node    longLivedTree;
        Node    tempTree;
        long    tStart, tFinish;
        long    tElapsed;
  	int	i, d;
	double 	*array;

	INIT_MEM;
	
	printf("Memory Allocator Test\n");
 	printf(" Live storage will peak at %lu bytes.\n\n",
               2 * sizeof(Node0) * TreeSize(kLongLivedTreeDepth) +
               sizeof(double) * kArraySize);
        printf(" Stretching memory with a binary tree of depth %d\n",
               kStretchTreeDepth);
        PrintDiagnostics();
#	ifdef PROFIL
	    init_profiling();
#	endif
       
        tStart = currentTime();
        
        // Stretch the memory space quickly
        tempTree = MakeTree(kStretchTreeDepth);
	destroy_Node(tempTree);
        tempTree = 0;

        // Create a long lived object
        printf(" Creating a long-lived binary tree of depth %d\n",
               kLongLivedTreeDepth);
	longLivedTree = allocate_node(sizeof(Node0));
        Populate(kLongLivedTreeDepth, longLivedTree);

        PrintDiagnostics();

        for (d = kMinTreeDepth; d <= kMaxTreeDepth; d += 2) {
                TimeConstruction(d);
        }

        if (longLivedTree == 0)
		fprintf(stderr, "Failed\n");
                                // fake reference to LongLivedTree
	                        // to keep them from being optimized away

        tFinish = currentTime();
        tElapsed = elapsedTime(tFinish-tStart);
        PrintDiagnostics();
        printf("Completed in %ld msec\n", tElapsed);
#	ifdef PROFIL
	  dump_profile();
#	endif
}

