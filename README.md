# Testing memory pools, dynamic arrays, etc. * *work in-progress* *
I have written a library for working with my own implementations of linked lists, variable (dynamic) arrays, and memory pools.

In its current state, the main program tests my memory pools, of which there are 3. Each pool has 8 blocks. The block size for each pool is 8 bytes, 16 bytes, and 32 bytes.

To test each memory pool, the program runs a loop 1,000,000 times. Inside the loop, each of the 8 blocks for the pool are allocated and given a value. Then they are freed back to the pool. Then it loops. The time is taken in milliseconds - from the start of each pool test, to the end of the 1,000,000 loops on that pool.

This identical loop is run for each of the 3 different pools.

For more detailed information about how this is being implemented, see the comments for each function in "dstruct.h" and "pool.h".

In the end, all allocated memory is freed (tested with Valgrind).

The ultimate goal of this project will be to create a library for working with multiple data structures/techniques (linked lists, dynamic arrays, pools) and comparing their usage, storage handling, and execution times.

"dstructs.txt" shows the output.