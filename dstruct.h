#include <ctype.h>
#include <stdlib.h>

/* Linked list */

#define NODE_CREATE_ERR "Error: link_init() returned NULL\n"
#define NODE_NULL_ERR "Error: node is NULL\n"
#define NODE_NOT_FOUND_ERR "Error: node searched for does not exist\n"
#define INVALID_INDX_ERR "Error: invalid index\n"

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

void link_node_print (struct link_node *node)
{
	if (node == NULL)
	{
		printf(NODE_NULL_ERR);
		return;
	}
	printf("node at %p: %i\n", node, node->val);
	printf("next node at %p\n\n", node->next);
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
	//printf("iterations: %i\n", ++(*iter));
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

/* Iterate through linked list from head, find node at "index" n (zero indexed like arrays) */
struct link_node *link_find_n (struct link_node *head, int n)
{
	if (n < 0)
	{
		perror(INVALID_INDX_ERR);
		return NULL;
	}
	if (head == NULL)
	{
		perror(NODE_NULL_ERR);
		return NULL;
	}

	for (int i = 0; i < n; i++)
	{
		head = head->next;
		if (head == NULL)
		{
			perror(NODE_NOT_FOUND_ERR);
			return NULL;
		}
	}
	return head;
}

/* Variable length array */

#define ARRAY_SIZE_STEP 5

#define VARRAY_MEM_ERR "Error: Problem when requesting memory for varray\n"
#define VARRAY_NULL_ERR "Error: varray is NULL\n"
#define VARRAY_SIZE_ERR "Error: varray has incompatible size\n"

struct varray
{
	int size;
	int allocated;
	int *array;
};

/* Initialize varray struct with size n. Array size will be the closest multiple of STEP greater than n */
/* When 'n' is zero, the array will allocate ARRAY_SIZE_STEP blocks of memory, but size member will be zero. */
struct varray *varray_init (int n)
{
	if (n < 0)
	{
		perror("Invalid varray size arg\n");
		return NULL;
	}
	struct varray *v;
	if ((v = (struct varray *) malloc(sizeof(struct varray))) == NULL)
	{
		perror(VARRAY_MEM_ERR);
		return NULL;
	}
	v->size = n;
	size_t mem = sizeof(int) * ((ARRAY_SIZE_STEP * (n / ARRAY_SIZE_STEP)) + ARRAY_SIZE_STEP);
	//int mem = (n / ARRAY_SIZE_STEP) + ARRAY_SIZE_STEP;
	if ((v->array = ((int *) malloc(mem))) == NULL)
	{
		perror(VARRAY_MEM_ERR);
		return NULL;
	}
	v->allocated = mem;
	for (int i = 0; i < v->allocated / sizeof(int); i++)
	{
		(v->array)[i] = 0;
	}
	return v;
}

/* Free the array itself, then the struct */
void varray_free (struct varray *v)
{
	if (v == NULL)
	{
		perror(VARRAY_NULL_ERR);
		return;
	}
	free(v->array);
	free(v);
	return;
}

/* Print properties of varray, optionally the contents of the actual array. */
void varray_print (struct varray *v, char prcont)
{
	if (v == NULL)
	{
		perror(VARRAY_NULL_ERR);
		return;
	}
	printf("varray at: %p\n", v);
	printf("\tsize: %i\n", v->size);
	printf("\tallocated: %d bytes of memory\n", v->allocated);
	printf("\tarray at: %p\n", v->array);

	if (prcont)
	{
		printf("\tcontents: ");
		for (int i = 0; i < v->size; i++)
		{
			printf("%i, ", (v->array)[i]);
		}
	}
	printf("\n\n");
	return;
}

/* Change all elements of v->array to zero (up to index 'size' - 1). */
/* The size property is left unchanged, and no reallocation is done. */
struct varray *varray_zero (struct varray *v)
{
	if (v == NULL)
	{
		printf(VARRAY_NULL_ERR);
		return NULL;
	}

	for (int i = 0; i < v->size; i++)
	{
		(v->array)[i] = 0;
	}

	return v;
}

/* Copy the array "from" into "to". */
/* If size of "to" is less than size of "from", excess data is truncated. */
struct varray *varray_copy (struct varray *to, int from[], int size_from)
{
	if (to == NULL || from == NULL)
	{
		perror(VARRAY_NULL_ERR);
		return NULL;
	}

	int max = size_from;
	if (to->size < size_from)
		max = to->size;

	varray_zero(to);

	for (int i = 0; i < max; i++)
	{
		(to->array)[i] = from[i];
	}
	return to;
}

/* Resize varray by requesting a new one, copying contents into it, then freeing old one. */
/* Keeps values of elements. If new varray is larger, zeroes are added. If new varray is smaller, data is truncated. */
struct varray *varray_resize (struct varray *v, int size)
{
	if (v == NULL)
	{
		perror(VARRAY_NULL_ERR);
		return NULL;
	}
	if (size < 0)
	{
		perror(VARRAY_SIZE_ERR);
		return NULL;
	}

	struct varray *new;

	/* if (size <= v->allocated / sizeof(int))
	{
		v->size = size;
		return v;
	} */
	
	if ((new = varray_init(size)) == NULL)
	{
		return NULL;
	}

	/* if (size <= v->allocated / sizeof(int))
		new = varray_copy(new, v->array, new->size);
	else
		new = varray_copy(new, v->array, v->size); */
	new = varray_copy(new, v->array, v->size);

	if (new == NULL)
	{
		return NULL;
	}
	varray_free(v);
	return new;
}