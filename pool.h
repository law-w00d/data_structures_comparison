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

#define POOL_SIZE 8 		// Num of blocks, not bytes (# of bytes will be BLOCK_SIZE * POOL_SIZE)
#define BLOCK_SIZE_8 8 		// 8 byte block
#define BLOCK_SIZE_16 16 	// 16 byte block
#define BLOCK_SIZE_32 32 	// 32 byte block

struct pool_block
{
	struct pool_block *next;
};

struct mem_pool
{
	void *start;
	size_t block_size;
	uint32_t block_num;
	struct pool_block *free_list;
};

/* Allocate big block of memory at once to be dynamically divided by the program */
struct mem_pool *pool_create (size_t block_size, uint32_t block_num)
{
	// If number of bytes is not perfectly divisible by block size
	// Or block size is not at least the size of pointer
	if (((block_size * block_num) % block_size) != 0 || block_size < sizeof(void *))
	{
		perror(POOL_SIZE_ERR);
		return NULL;
	}

	struct mem_pool *p;
	if ((p = (struct mem_pool *) malloc(sizeof(struct mem_pool))) == NULL)
	{
		perror(POOL_MEM_ERR);
		return NULL;
	}

	if ((p->start = malloc(block_size * block_num)) == NULL)
	{
		perror(POOL_MEM_ERR);
		return NULL;
	}

	p->block_size = block_size;
	p->block_num = block_num;
	p->free_list = NULL;

	return p;
}

/* Free the entire block of memory, not individual objects */
void pool_release (struct mem_pool *p)
{
	if (p == NULL || p->start == NULL)
	{
		perror(POOL_NULL_ERR);
		return;
	}
	free(p->start);
	free(p);
	return;
}

/* Create linked list of all blocks in pool, return the head */
void pool_init (struct mem_pool *p)
{
	struct pool_block *head = (struct pool_block *) p->start;
	struct pool_block *block = head;
	for (int i = 0; i < p->block_num - 1; i++)
	{
		block->next = (struct pool_block *) (((uint8_t *) block) + p->block_size);
		block = block->next;
	}
	block->next = NULL;
	p->free_list = head;
	return;
}

/* Allocate a block from pool by taking first free block, converting it to a ptr to unsigned long (8 bytes)
	and setting new head of free block list. */
void *pool_alloc (struct mem_pool *p)
{
	if (p == NULL || p->start == NULL || p->free_list == NULL)
	{
		perror(POOL_NULL_ERR);
		return NULL;
	}
	struct pool_block *next = p->free_list->next;
	void *new = p->free_list;
	p->free_list = next;
	return new;
}

/* Free a block by converting it to a pool_block ptr and making it the new head of free blocks. */
void pool_free (void *block, struct mem_pool *p)
{
	//printf("Attempting to free block at %p with value %ld\n", block, *((uint64_t *) block));
	if (block == NULL)
	{
		perror(BLOCK_NULL_ERR);
		return;
	}
	if (p == NULL || p->start == NULL)
	{
		perror(POOL_NULL_ERR);
		return;
	}

	struct pool_block *new = (struct pool_block *) block;
	new->next = NULL;
	if (p->free_list == NULL)
	{
		p->free_list = new;
		return;
	}
	new->next = p->free_list;
	p->free_list = new;
	return;
}

void pool_print (struct mem_pool *p)
{
	printf("pool_print():\n");
	if (p == NULL)
	{
		printf("No pool\n");
		return;
	}

	printf("\tPool at: %p\n\tBlock size: %ld\n\tNum of blocks: %d\n", p->start, p->block_size, p->block_num);
	return;
}

/* Iterate through linked list of free blocks, printing info about each one. */
void pool_print_free (struct mem_pool *p)
{
	printf("pool_print_free():\n");
	if (p->free_list == NULL)
	{
		printf("\tNo free blocks\n");
		return;
	}

	struct pool_block *cur = p->free_list;
	while (cur != NULL)
	{
		printf("\tFree block at: %p\n\tNext block at: %p\n\n", cur, cur->next);
		cur = cur->next;
	}
	return;
}
