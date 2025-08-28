#include <ctype.h>
#include <stdlib.h>

#define NODE_CREATE_ERR "Error: link_init() returned NULL\n"
#define NODE_NULL_ERR "Error: node is NULL\n"

struct link_node
{
	int val;
	struct link_node *next;
};

/* Creates a list head with the specified value, then returns a pointer to it */
struct link_node *link_init (int val)
{
	struct link_node *node = (struct link_node *) malloc( sizeof(struct link_node));
	if (node == NULL)
	{
		perror(NODE_CREATE_ERR);
		return NULL;
	}
	node->val = val;
	node->next = NULL;
}

/* Starts at the node you give it, and recursively frees all nodes after it */
void link_free_all (struct link_node *head, int *iter)
{
	if (head == NULL)
	{
		printf("link_free_all(): current node is NULL, stopping\n");
		return;
	}
	struct link_node *next = head->next;
	free(head);
	printf("iterations: %i\n", ++(*iter));
	link_free_all(next, iter);
}

/* Create a new head for the linked list, return pointer to the new head */
struct link_node *link_add_front (struct link_node *head, int val)
{
	if (head == NULL)
	{
		perror(NODE_NULL_ERR);
		return NULL;
	}
	struct link_node *new;
	if ((new = link_init(val)) == NULL)
	{
		perror(NODE_CREATE_ERR);
		return NULL;
	}
	new->next = head;
	return new;
}

/* Add a node to the end of existing list, return pointer to new node */
struct link_node *link_add_back (struct link_node *last, int val)
{
	if (last == NULL)
	{
		perror(NODE_NULL_ERR);
		return NULL;
	}
	struct link_node *new;
	if ((new = link_init(val)) == NULL)
	{
		perror(NODE_CREATE_ERR);
		return NULL;
	}
	last->next = new;
	return new;
}

/* Linearly iterate through linked list, starting at head, until the last node is found, then return a pointer to it */
struct link_node *link_find_last (struct link_node *head)
{
	if (head == NULL) perror(NODE_NULL_ERR);
	while (head->next != NULL)
	{
		head = head->next;
	}
	return head;
}