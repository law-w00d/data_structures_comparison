# Comparing memory pools to dynamic arrays and linked lists * *work in-progress* *
*This side project is only a learning experience. Much of the code I have written is messy, intended for debugging. In no way does it reflect the quality of a final product. There is room for re-factoring and additional error-checking. Currently, my functions (especially for memory pools) are not very flexible and are restricted to certain types/sizes, with the goal of improving this in the future.*

Currently I have written a simple library for working with my own implementations of linked lists, variable (dynamic) arrays, and memory pools.

The main program successfully tests using the functions I wrote. Some test cases are intentinally erroneous, and errors are thrown when expected.

For more detailed information about how this is being implemented, see the comments for each function in "dstruct.h" and "pool.h".

In the end, all allocated memory is freed (tested with Valgrind).

The ultimate goal of this project will be to create a library for working with multiple data structures/techniques (linked lists, dynamic arrays, pools) and comparing their usage, storage handling, and execution times.

"dstructs.txt" shows the output.