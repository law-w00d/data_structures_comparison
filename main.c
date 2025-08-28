#include <stdio.h>
#include <time.h>
#include "dstruct.h"

#define VALS_LEN 10000

int main (int argc, char *argv[])
{
	// test time when creating and accessing (or resizing) linked lists, variable length arrays, and pools

	// Fixed array of values to test each data structure with
	int values[VALS_LEN];
	for (int i = 0; i < VALS_LEN; i++)
	{
		values[i] = i;
	}

	// Linked list
	struct link_node *head;
	struct link_node *last;
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

	struct link_node *cur = head;
	while (cur != NULL)
	{
		printf("value at node: %i\n", cur->val);
		printf("next pointer at node: %p\n", cur->next);
		cur = cur->next;
	}

	int *iter = (int *) malloc(sizeof(int));
	*iter = 0;
	link_free_all(head, iter);
	free(iter);
	printf("Freed all nodes\n");
	exit(0);
}