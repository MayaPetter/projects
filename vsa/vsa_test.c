#include <assert.h> /* assert */
#include <stdlib.h> /* free, malloc */
#include <stdio.h>  /* puts */

#include "vsa.h"

static void InitTest()
{
	vsa_t *memory = (vsa_t *)malloc(60);
	vsa_t *new_pool = NULL;
	
	assert(NULL != memory);
	
	new_pool = VSAInit(memory, 60);
	assert(NULL != new_pool);
	
	free(memory); memory = NULL;
}

static void AllocFreeAndLargestTest()
{
/*	vsa_t *memory = (vsa_t *)malloc(60);*/
/*	vsa_t *new_pool = NULL;*/
/*	void *allocated = NULL;*/
/*	*/
/*	assert(NULL != memory);*/
/*	*/
/*	new_pool = VSAInit(memory, 60);*/
/*	assert(NULL != new_pool);*/
	
	
/*
 * Allocate memory block in variable size allocator
 * @ vsa_pool : variable sized allocator
 * @ bytes_to_alloc : num of bytes to allocate
 * Return: pointer to allocated memory
 * Errors: if bytes_to_alloc is larger than largest free chunk in vsa, 
 *		   returns NULL
 */
void *VSAAlloc(vsa_t *vsa_pool, size_t bytes_to_alloc);

/*
 * Free block starting in <adress_to_free> from variable size allocator
 * @address_to_free : allocated memory block that returns from Alloc
 * Return: none
 * Errors: if @address_to_free wasn't returned from Alloc, 
 * 		   behaviour is undefined
 */
void VSAFree(void *address_to_free);

/*
 * Find the largest continuous chunk of free memory
 * @vas_pool : vsa to count
 * Return: Size in bytes of the largest continuous chunk of free memory
 * Errors: none
 */
size_t VSALargestChunk(vsa_t *vsa_pool);
}

int main()
{
    InitTest();
    AllocFreeAndLargestTest();      
	return 0;
}
