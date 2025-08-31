/* Memory Pool */
/* Test using a memory pool with various ways of structuring data (eg. arrays, linked lists) */

#include <ctype.h>
#include <stdlib.h>
#include <stdint.h>

#define POOL_SIZE_ERR "Error: pool size must be multiple of block size\n"
#define POOL_MEM_ERR "Error: problem requesting heap memory for pool\n"
#define POOL_NULL_ERR "Error: pool ptr is NULL\n"
#define BLOCK_SIZE_ERR "Error: block size not big enough to hold pointer\n"
#define BLOCK_NULL_ERR "Error: block ptr is NULL\n"

#define POOL_SIZE 8 //	# of blocks, not bytes (# of bytes will be data_size * pool_size)
#define BLOCK_SIZE 8 // Num of bytes per block (must be greater than size of block pointer)

struct pool_block
{
	struct pool_block *next;
};

/* Allocate big block of memory at once to be dynamically divided by the program */
void *pool_create (size_t size)
{
	if ((size % BLOCK_SIZE) != 0)
	{
		perror(POOL_SIZE_ERR);
		return NULL;
	}
	void *p = NULL;
	if ((p = malloc(size)) == NULL)
	{
		perror(POOL_MEM_ERR);
		return NULL;
	}
	return p;
}

/* Free the entire block of memory, not individual objects */
void pool_free (void *p)
{
	if (p == NULL)
	{
		perror(POOL_NULL_ERR);
		return;
	}
	free(p);
	return;
}

/* Create linked list of all blocks in pool, return the head */
struct pool_block *pool_init (void *pool, size_t blockSize)
{
	if (blockSize < sizeof(struct pool_block *))
	{
		perror(BLOCK_SIZE_ERR);
		return NULL;
	}
	struct pool_block *head = (struct pool_block *) pool;
	struct pool_block *block = head;
	for (int i = 0; i < POOL_SIZE - 1; i++)
	{
		block->next = (struct pool_block *) (((uint8_t *) block) + blockSize);
		block = block->next;
	}
	block->next = NULL;
	return head;
}

/* Allocate a block from pool by taking first free block, converting it to a ptr to unsigned long (8 bytes)
	and setting new head of free block list. */
uint64_t *pool_alloc_long (void *pool, struct pool_block **free)
{
	if (pool == NULL || (*free) == NULL)
	{
		perror(POOL_NULL_ERR);
		return NULL;
	}
	struct pool_block *next = (*free)->next;
	uint64_t *new = (uint64_t *) (*free);
	*new = 0UL;
	(*free) = next;
	return new;
}

/* Free a block by converting it to a pool_block ptr and making it the new head of free blocks. */
void pool_free_long (uint64_t *block, struct pool_block **free)
{
	printf("Attempting to free block at %p with value %ld\n", block, *block);
	if (block == NULL)
	{
		perror(BLOCK_NULL_ERR);
		return;
	}
	struct pool_block *new = (struct pool_block *) block;
	new->next = NULL;
	if ((*free) == NULL)
	{
		(*free) = new;
		return;
	}
	new->next = (*free);
	(*free) = new;
	return;
}

/* Iterate through linked list of free blocks, printing info about each one. */
void pool_print_free (struct pool_block *free)
{
	printf("pool_print_free():\n");
	if (free == NULL)
	{
		printf("\tNo free blocks\n");
		return;
	}

	while (free != NULL)
	{
		printf("\tFree block at: %p\n\tNext block at: %p\n\n", free, free->next);
		free = free->next;
	}
	return;
}