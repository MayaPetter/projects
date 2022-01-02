/******************************************
* Author:   Maya Petter                   *
* Reviewer: Vladimir Kraykin              *
* Purpose:  variable size allocation      *
*******************************************/
#include <assert.h> /* assert */
#include <stdlib.h> /* labs */
 
#include "vsa.h"

struct vsa
{
	long size_and_flag;	
	
	#ifndef NDEBUG
	long megic_num;
	#endif /* NDEBUG */
};

/* management list*/
typedef struct head
{
	void *end;
}management_t;

/*********** constant variables *********/
const size_t WORD = sizeof(size_t);
const size_t HEADER_SIZE = sizeof(vsa_t);
const size_t MENEGE_SIZE = sizeof(management_t);

/*********** static functions *********/
static void *AdvaceRunner(vsa_t *runner);
static long VSADefreg(vsa_t *block_header, void *end_of_memory);
static vsa_t *AllocInit(vsa_t *vsa_pool, size_t bytes_to_alloc);
static void *AdvaceRunner(vsa_t *runner);

vsa_t *VSAInit(void *memory, size_t memory_size) 
{
	vsa_t *new_pool = NULL;
	
	assert(NULL != memory);
	assert(0 != memory_size);
	
	/* if memory is not WORD aligned return NULL */
	if (0 != (size_t )memory % WORD)
	{
		return NULL;
	}

	memory_size -= (memory_size % WORD);
	
	/* assign management list to point to last address of aligned memory */
	((management_t *)memory)->end = (char *)memory + memory_size;
	
	new_pool = (vsa_t *)((char *)memory + MENEGE_SIZE);
	
	/* assign data to struct vsa members in release and debug versions*/ 
	new_pool->size_and_flag = memory_size - HEADER_SIZE - MENEGE_SIZE;
	
	#ifndef NDEBUG
	new_pool->megic_num = (long)0xDEADBEEF;
	#endif /* NDEBUG */
	
	return memory;
}

static long VSADefreg(vsa_t *block_header, void *end_of_memory) 
{
	void *runner = NULL;
	size_t counter = 0;
	long size = 0;
	
	assert(NULL != block_header);
	
	for (runner = block_header
  		;runner < end_of_memory && 0 <= ((vsa_t *)runner)->size_and_flag 
		;runner = AdvaceRunner((vsa_t *)runner))
	{
	    /* sum all free blocks in a row*/
		size += ((vsa_t *)runner)->size_and_flag;
		++counter;
	}	
	
	/* adding header size to the block size while de-fragmenting*/
	if (0 < counter)
	{
		block_header->size_and_flag = size + ((counter - 1) * HEADER_SIZE);
	}

	return block_header->size_and_flag;
}

size_t VSALargestChunk(vsa_t *vsa_pool)
{
	long curr_largest = 0, largest = 0;
	void *runner = NULL;
	void *end_of_memory = NULL;

	assert(NULL != vsa_pool);
	
	end_of_memory = ((management_t *)vsa_pool)->end;
	
	/* for every de-fragmented block- check if bigger then the previous block */
	for (runner = (void *)((char *)vsa_pool + MENEGE_SIZE)
	    ;runner < end_of_memory 
	    ;runner = AdvaceRunner((vsa_t *)runner))
	{
		curr_largest = VSADefreg((vsa_t *)runner, end_of_memory);

		if (curr_largest > largest)
		{
			largest = curr_largest;
		}
	}
	return largest;	
}

static vsa_t *AllocInit(vsa_t *vsa_pool, size_t bytes_to_alloc)
{
	vsa_t *runner = NULL;
	long free_space = 0;
	size_t remaining_space = 0;
	assert(NULL != vsa_pool);
	
	free_space = vsa_pool->size_and_flag;
	remaining_space = free_space - bytes_to_alloc;
	
   	/* change flag to indicate allocated block*/
   	vsa_pool->size_and_flag = -(long)bytes_to_alloc;
	
	/* if there is enough space for header after the allocated block */ 
	if (HEADER_SIZE < remaining_space)
	{   
	    /* advance runner to next header and assign header fields */
		runner = (vsa_t *)((char *)vsa_pool + bytes_to_alloc + HEADER_SIZE);
		runner->size_and_flag = free_space - bytes_to_alloc - HEADER_SIZE;
		
		#ifndef NDEBUG
		runner->megic_num = (long)0xDEADBEEF;
		#endif  /* NDEBUG */
	}
	else
	{
		vsa_pool->size_and_flag -= remaining_space;
	}
	
    return vsa_pool;
}

void *VSAAlloc(vsa_t *vsa_pool, size_t bytes_to_alloc)
{
    vsa_t *allocated_block = NULL;
    long size = 0;
    void *end_of_memory = NULL;
    
    assert(NULL != vsa_pool);
	
	/* padding to WORD alignment */
	if (0 != (bytes_to_alloc % WORD))
	{
		bytes_to_alloc += WORD - (bytes_to_alloc % WORD);
	}
	
	end_of_memory = ((management_t *)vsa_pool)->end;
	vsa_pool = (vsa_t *)((char *)vsa_pool + MENEGE_SIZE);
	
    /* find big enough block to allocate the requested size */
    while ((void *)vsa_pool < end_of_memory)
    {   
    	size = VSADefreg(vsa_pool, end_of_memory);
		if (size >= (long)bytes_to_alloc)
		{
			allocated_block = AllocInit(vsa_pool, bytes_to_alloc);
			break;
		}
    	vsa_pool = (vsa_t *)AdvaceRunner(vsa_pool);
    }
    
    if ((void *)vsa_pool >= end_of_memory)
    {
    	return NULL;
    }
    
    return (void *)((char *)allocated_block + HEADER_SIZE);
}

void VSAFree(void *address_to_free)
{
	vsa_t *block_to_free = (vsa_t *)address_to_free;
	
	assert(NULL != address_to_free);
	
	block_to_free = (void *)((char *)block_to_free - HEADER_SIZE);

	#ifndef NDEBUG
    assert((long)0xDEADBEEF == block_to_free->megic_num);
    #endif  /* NDEBUG */	
    
	if (0 > block_to_free->size_and_flag)
	{
		block_to_free->size_and_flag *= -1;
	}
}

static void *AdvaceRunner(vsa_t *runner)
{
	size_t advance_size = 0;
	
	assert(NULL != runner);
	
	advance_size =  HEADER_SIZE + labs(runner->size_and_flag);
	
	return (void *)((char *)runner + advance_size);
}

