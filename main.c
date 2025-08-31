#include <stdio.h>
#include <time.h>
#include "dstruct.h"
#include "pool.h"

#define VALS_LEN 10000

void test_linked_list (void);
void test_varray (void);
void test_pool (void);

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
	test_linked_list();

	// Varray
	test_varray();

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
	printf("\n\nTESTING MEMORY POOL\n\n");

	// Allocate the pool
	printf("pool byte size: %d\n", (POOL_SIZE * BLOCK_SIZE));
	void *p1 = pool_create(POOL_SIZE * BLOCK_SIZE);
	printf("pool created at %p\n\n", p1);

	// Get linked list of free blocks
	struct pool_block *freeHead = pool_init(p1, BLOCK_SIZE);
	pool_print_free(freeHead);

	// Create array of blocks to allocate from pool
	printf("Creating new array and allocating blocks from pool\n");
	uint64_t *alloced[POOL_SIZE];
	for (int i = 0; i < POOL_SIZE; i++)
	{
		uint64_t *n = pool_alloc_long(p1, &freeHead);
		if (n)
		{
			alloced[i] = n;
		}
		*(alloced[i]) = (uint64_t) i;
	}
	
	printf("Contents of new array:\n");
	for (int i = 0; i < POOL_SIZE; i++)
	{
		printf("\talloced[%i]: %p with value: %ld\n", i, alloced[i], *(alloced[i]));
	}

	// Free last block of alloced array
	pool_free_long(alloced[POOL_SIZE - 1], &freeHead);
	pool_print_free(freeHead);

	// Free second to last block of alloced array
	pool_free_long(alloced[POOL_SIZE - 2], &freeHead);
	pool_print_free(freeHead);

	pool_free(p1);
	printf("entire pool freed\n");

	printf("Finished testing memory pool\n");
	return;
}