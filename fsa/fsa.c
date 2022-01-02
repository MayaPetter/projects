/******************************************
* Author:   Maya Petter                   *
* Reviewer: Ran Shieber                   *
* Purpose:  Fixed Size Allocation         *
*******************************************/
#include <assert.h>

#include "fsa.h"

#define WORD sizeof(size_t)

struct fsa
{
	size_t next_free_index;
}; 

typedef struct node
{
	size_t next_offset;
}block_t;

/* calculating block size with the padding and the header */ 
static size_t BlockSize(size_t block_size)
{
    size_t padding = 0;
    size_t real_block_size = 0;
    size_t word_remainder = block_size % WORD;
	
    if (0 != word_remainder)
    {
        padding = WORD - word_remainder;
    }
	
	real_block_size = sizeof(block_t) + block_size + padding;	
	
	return real_block_size;
}

static size_t CountBlocks(size_t memory_size , size_t block_size)
{
	size_t num_of_blocks = 0;
	
	assert(0 != block_size);
	
	num_of_blocks = (memory_size - sizeof(fsa_t)) / block_size;
	
	return num_of_blocks;	
}

fsa_t *FSAInit(void *memory, size_t memory_size, size_t block_size)
{
    fsa_t *new_pool = NULL;
    block_t *new_block = NULL;
    size_t real_block_size = 0; 
    size_t num_of_blocks = 0;
    size_t offset = 0;

    assert(NULL != memory);
    assert(memory_size > block_size);
    assert(0 != block_size);
    assert(0 != memory_size);
    
    if (memory_size < FSASuggestSize(1, block_size))
    {
    	return NULL;
    }

    /* assign user memory to the new memory pool */
    new_pool = (fsa_t *)memory;
    new_pool->next_free_index = sizeof(fsa_t);
	
	/* assign the first header memory location */
    new_block = (block_t *)new_pool + 1;
    
    /*calculate block size including header and the amount of blocks available*/
    real_block_size = BlockSize(block_size); 
    num_of_blocks = CountBlocks(memory_size, real_block_size);

	/* assign offsets to all other blocks */
    for (offset = 1; offset < num_of_blocks; ++offset)
    {
        new_block->next_offset = (offset * real_block_size) + sizeof(block_t);
       	new_block = (block_t *)((char *)new_block + real_block_size);
    }
    
    /* assign 0 to the last block */
    new_block->next_offset = 0;

    return new_pool; 
}

static void SwapIndexes(fsa_t *fsa_pool, void *block)
{
	size_t temp = 0;
	
	assert(NULL != fsa_pool);
	
	temp = fsa_pool->next_free_index;
	
	fsa_pool->next_free_index = ((block_t *)block)->next_offset;

	((block_t *)block)->next_offset = temp;
}

void *FSAAlloc(fsa_t *fsa_pool)
{
	char *runner = NULL;
	
	assert(NULL != fsa_pool);
 	
 	if (0 == fsa_pool->next_free_index)
 	{
 		return NULL;
 	}   
	
	runner = (char *)fsa_pool + fsa_pool->next_free_index;
	
	SwapIndexes(fsa_pool, runner);
	
	return (runner + sizeof(block_t));
}

void FSAFree(void *block_to_free)
{
	char *runner = (char *)block_to_free;
	char *index_to_swap = NULL;

	if (NULL != block_to_free)
	{	
		/* move runner back to the begging of the block */
		runner -= sizeof(block_t);
		
		/* assign runners' next to index */
		index_to_swap = runner - ((block_t *)runner)->next_offset; 
		
		SwapIndexes((fsa_t *)index_to_swap, &((block_t *)runner)->next_offset);
	}
}

size_t FSACountFree(const fsa_t *fsa_pool)
{
	size_t free_blocks_counter = 0;
	block_t *runner = NULL;
	
	assert(NULL != fsa_pool);
	if(0 == fsa_pool->next_free_index)
	{
		return 0;
	}
	
	/* go to next free, increment counter till next == 0*/
	for (runner = (block_t *)((char *)fsa_pool + fsa_pool->next_free_index),
		 free_blocks_counter = 1
		; 0 != runner->next_offset
		; runner = (block_t *)((char *)fsa_pool + runner->next_offset), 
		  ++free_blocks_counter)
	{/* empty body */}
	
	return free_blocks_counter;
}

size_t FSASuggestSize(size_t num_of_blocks, size_t block_size)
{
	
	return ((BlockSize(block_size) * num_of_blocks) + sizeof(fsa_t));
}


