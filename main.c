#include <stdio.h>
#include <time.h>
#include "dstruct.h"

#define VALS_LEN 10000

void test_linked_list (void);
void test_varray (void);

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
	
	exit(0);
}

void test_linked_list (void)
{
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

	int *iter = (int *) malloc(sizeof(int));
	*iter = 0;
	link_free_all(head, iter);
	free(iter);
	printf("Freed all nodes\n");
	printf("Finished testing linked list\n\n\n");
}

void test_varray (void)
{
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
	/* v1 = varray_resize(v1, 3);
	varray_print(v1, 1); */

	v1 = varray_resize(v1, 8);
	varray_print(v1, 1);

	v1 = varray_resize(v1, 26);
	varray_print(v1, 1);

	varray_free(v1);
	printf("Finished testing varray\n");
	return;
}