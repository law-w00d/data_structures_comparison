#include <stdio.h>
#include <time.h>
#include "dstruct.h"
#include "pool.h"

#define VALS_LEN 10000
#define LOOP_NUM 1000000

void test_linked_list (void);
void test_varray (void);
void test_pool (void);
void loop_pool (struct mem_pool *p);

/* Test time when creating and accessing (or resizing) linked lists, variable length arrays, and pools */
int main (int argc, char *argv[])
{
	// Fixed array of values to test each data structure with
	int values[VALS_LEN];
	for (int i = 0; i < VALS_LEN; i++)
	{
		values[i] = i;
	}

	// Linked list
	//test_linked_list();

	// Varray
	//test_varray();

	// Memory pool
	test_pool();
	
	exit(0);
}

void test_linked_list (void)
{
	printf("\n\nTESTING LINKED LIST\n\n");

	struct link_node *head;
	struct link_node *last;

	// Initialize entire list
	for (int i = 0; i < VALS_LEN; i++)
	{
		if (i > 0)
		{
			last = link_add_back(last, i);
		}
		else
		{
			last = head = link_init(i);
		}

		if (last == NULL) exit(1);
	}

	// Find nodes in list by index
	struct link_node *n1;
	if ((n1 = link_find_n(head, 77)) == NULL)
	{
		//exit(1);
	}
	link_node_print(n1);

	struct link_node *n2;
	if ((n2 = link_find_n(head, -1)) == NULL)
	{
		//exit(1);
	}
	link_node_print(n2);

	struct link_node *n3;
	if ((n3 = link_find_n(head, 99999)) == NULL)
	{
		//exit(1);
	}
	link_node_print(n3);

	struct link_node *n4;
	if ((n4 = link_find_n(head, 9999)) == NULL)
	{
		//exit(1);
	}
	link_node_print(n4);

	struct link_node *n5;
	if ((n5 = link_find_n(head, 10000)) == NULL)
	{
		//exit(1);
	}
	link_node_print(n5);

	link_free_all(head);
	printf("Freed all nodes\n");
	printf("Finished testing linked list\n\n\n");
}

void test_varray (void)
{
	printf("\n\nTESTING VARIABLE ARRAY\n\n");

	struct varray *v1;
	if ((v1 = varray_init(7)) == NULL)
	{
		exit(1);
	}
	varray_print(v1, 1);

	v1 = varray_resize(v1, 15);
	varray_print(v1, 1);
	int t1[19] = {7, 10, 18, 14, 1, 2, 3, 6, 6, 8, 8, 9, 1, 2, 53, 7, 2, 87, 19};
	v1 = varray_copy(v1, t1, sizeof(t1) / sizeof(int));
	varray_print(v1, 1);

	v1 = varray_resize(v1, 8);
	varray_print(v1, 1);

	v1 = varray_resize(v1, 26);
	varray_print(v1, 1);

	varray_free(v1);
	printf("Finished testing varray\n");
	return;
}

void test_pool (void)
{
	printf("\nTESTING MEMORY POOLS\n\n");

	// Create 3 pools of different block sizes (8, 16, and 32 blocks)
	// All pools have the same number of blocks
	struct mem_pool *p8, *p16, *p32;

	if ((p8 = pool_create(BLOCK_SIZE_8, POOL_SIZE)) == NULL)
		exit(1);
	if ((p16 = pool_create(BLOCK_SIZE_16, POOL_SIZE)) == NULL)
		exit(1);
	if ((p32 = pool_create(BLOCK_SIZE_32, POOL_SIZE)) == NULL)
		exit(1);
	
	pool_init(p8);
	pool_init(p16);
	pool_init(p32);

	printf("p8:\t");
	loop_pool(p8);
	printf("p16:\t");
	loop_pool(p16);
	printf("p32:\t");
	loop_pool(p32);

	pool_release(p8);
	pool_release(p16);
	pool_release(p32);

	printf("\nFINISHED TESTING MEMORY POOLS\n\n");
	return;
}

/* Loop some actions for memory pool and time it */
void loop_pool (struct mem_pool *p)
{
	clock_t start, end;
	start = clock();
	for (int t = 0; t < LOOP_NUM; t++)
	{
		// Create array to hold blocks
		void *alloced[POOL_SIZE];

		// Allocate blocks from pool and set their value
		for (int i = 0; i < POOL_SIZE; i++)
		{
			void *n = pool_alloc(p);
			if (n != NULL)
			{
				alloced[i] = n;
				*(uint64_t *)(alloced[i]) = (uint64_t) i;
			}
		}

		// Free blocks back to pool
		for (int i = 0; i < POOL_SIZE; i++)
		{
			pool_free(alloced[i], p);
		}
	}
	end = clock();
	printf("Time elapsed: %ld ms\n", (end - start) / (CLOCKS_PER_SEC / 1000));
}