# OS Lab Exercise: Implement Custom Memory Allocation Routines

Now, you need to define your own memory management routines 
(`my_malloc` and `my_free`) that implement the functions in the 
header file `my_memory_allocator.h`.

Follow the sequence of steps below, to create three different variants of this memory allocator. 
There are comments in `my_memory_allocator_v{1,2,3}.h` files
to show you where to make modifications.
Change the `LAB_VERSION` number in `my_memory_allocator.h`
to include the appropriate version of the header file.



## v1: Simplest Approach (call `mmap` every time)

In the file `my_memory_allocator_v1.h`, just call the `mmap()` routine every time `my_malloc()` is called by the client code.
How do you translate a `malloc(num_bytes)` call into an `mmap()`
call? The `mmap()` routine takes lots more parameters.
Ask ChatGPT about these and what they mean ... you simply want
an `mmap` that returns read/writeable memory which is private and anonymous and not backed by a file.


```
void *my_malloc(int num_bytes) {
  return mmap( /* PARAMS_GO_HERE */ );
}
```

So – now your `my_malloc` calls `mmap` each time.
The corresponding `my_free` and `init_mem_pool` functions are
empty – they simply `return void`.

Now recompile your `binarytrees` benchmark with `LAB_VERSION`
set to 1 in `my_memory_allocator.h`. What happens when you run it?
Why is it inefficient to call `mmap` repeatedly, and for such small sizes of memory? Discuss this with your lab tutor.



## v2: More Complex Approach (Use a Memory Pool)

For this second approach, we will call the `mmap` routine
only once, in the `init_mem_pool()` function, to allocate a large slab of memory that we can use subsequently.

You can use the same parameters for `mmap` as in the earlier approach, only allocate a nice large block of memory this time – perhaps 100K pages, which is 400MB.

```
void *p = mmap(NULL,  // address to use (we don't care)         
                 100000 * 1<<12, // 100K * page size (in bytes) 
                 PROT_READ | PROT_WRITE, // r/w permissions       
                 MAP_ANON | MAP_PRIVATE, // request RAM, not file 
                 -1, // no file handle, since we request RAM      
                 0); // no offset required
```

Then for each call to `my_malloc`, we just need to allocate
another segment of this memory pool - this technique is known as
*bump pointer allocation*.
We will need to maintain a global `next_free_mem pointer`,
which starts at the beginning of the memory pool and is
incremented every time we call `malloc`.
For now, we don’t need to worry about disposing of this memory,
so just let `free` do nothing (i.e. `return void`).


Recompile the `binarytrees` benchmark. What is the performance like? Compared to the previous approach, what are the advantages of this technique? And what are the disadvantages? Again, discuss these questions with your tutor.

## v3: More General Approach (Fixed Size Freelist)

This is the advanced part of the lab, and the most challenging!
If you don’t complete this implementation part,
don’t worry too much.

Define a data structure like this:

```
typedef union {
    uint8_t bytes[32];
    void *ptr;
} Cell;
```

The Cell structure superimposes a pointer and a 32-byte array on top of each other. So now a memory cell can either contain an object of up to 32 bytes in length, or a single pointer.
Then make the `mmap` buffer created in `init_mem_pool` large enough to hold lots of these Cells, and cast the buffer pointer to a 
`Cell` pointer (rather than `void *`) when you need to use it.
In the `init_mem_pool` function, create a linked list
using the `Cell.ptr` fields,
that goes through the memory pool array, from first to last,
linking all elements into this list. You can use a `while` loop to do this.
Now, every time `malloc` is called, you should pop the head
`Cell` off this list and return it to the client code.
Then when you later call `free`,
push the freed `Cell` back onto the head of the list.

This is fixed-size freelist allocator, which is similar to the techniques used in real-world `malloc` libraries.
When you compile `binarytrees.c` with this version of the allocator, you should get better performance than all the other variants, including the standard C library allocator. Why is this version the best?


