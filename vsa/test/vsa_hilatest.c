#include <stdlib.h> /*malloc, free*/
#include <stdio.h> /*printf*/
#include <assert.h> /*assert*/

#include "vsa.h"

#define UNUSED(x) (void)x

void TestAllocAndFree_Debug();
void TestAllocAndFree_Release();

int main ()
{
	vsa_t *vsa_pool = NULL; 
	size_t memory_size = 80;
	void *memory = malloc(memory_size);
	size_t allocated_chunk = 0;
	vsa_pool = VSAInit(memory, memory_size);
	
	allocated_chunk = VSALargestChunk(vsa_pool);
	
	if (allocated_chunk == 56)
	{
		TestAllocAndFree_Debug();
	}
	else
	{
		TestAllocAndFree_Release();
	}
	
	free(memory); memory = NULL; 
	
	return 0;
}

void TestAllocAndFree_Debug()
{
	int *num = NULL;
	char *str = NULL;
	size_t *sizet = NULL; 
	size_t *longlong = NULL;
	void *to_fail = NULL;
	
	vsa_t *vsa_pool = NULL; 
	size_t memory_size = 128;
	void *memory = malloc(memory_size);
	
	vsa_pool = VSAInit(memory, memory_size);
	assert(104 == VSALargestChunk(vsa_pool));
	

	num = VSAAlloc(vsa_pool, sizeof(int)); 
	assert(NULL != num);
	assert(80 == VSALargestChunk(vsa_pool));
	
	str = VSAAlloc(vsa_pool, sizeof(char)*10);
	assert(NULL != str);
	assert(48 == VSALargestChunk(vsa_pool));
	
	sizet = VSAAlloc(vsa_pool, sizeof(size_t)); 
	assert(NULL != sizet);
	assert(24 == VSALargestChunk(vsa_pool));
	
	longlong = VSAAlloc(vsa_pool, sizeof(size_t)*2); 
	assert(NULL != longlong);
	assert(0 == VSALargestChunk(vsa_pool));
	
	to_fail = VSAAlloc(vsa_pool, sizeof(char));
	assert(NULL == to_fail);
	assert(0 == VSALargestChunk(vsa_pool));
	
	VSAFree(num);
	VSAFree(str);
	VSAFree(sizet);
	VSAFree(longlong);
	assert(104 == VSALargestChunk(vsa_pool));
	assert(104 == VSALargestChunk(vsa_pool));
	
	free(memory); memory = NULL; 	
	/*VSAFree(&memory_size);*/
}

void TestAllocAndFree_Release()
{
	int *num = NULL;
	char *str = NULL;
	size_t *sizet = NULL; 
	size_t *longlong = NULL;
	size_t *longlonglong = NULL;
	void *to_fail = NULL;
	
	vsa_t *vsa_pool = NULL; 
	size_t memory_size = 128;
	void *memory = malloc(memory_size);
	
	vsa_pool = VSAInit(memory, memory_size);
	assert(112 == VSALargestChunk(vsa_pool));
	

	num = VSAAlloc(vsa_pool, sizeof(int)); 
	assert(NULL != num);
	assert(96 == VSALargestChunk(vsa_pool));
	
	str = VSAAlloc(vsa_pool, sizeof(char)*10);
	assert(NULL != str);
	assert(72 == VSALargestChunk(vsa_pool));
	
	sizet = VSAAlloc(vsa_pool, sizeof(size_t)); 
	assert(NULL != sizet);
	assert(56 == VSALargestChunk(vsa_pool));
	
	longlong = VSAAlloc(vsa_pool, sizeof(size_t)*2); 
	assert(NULL != longlong);
	assert(32 == VSALargestChunk(vsa_pool));
	
	longlonglong = VSAAlloc(vsa_pool, sizeof(size_t)*3); 
	assert(NULL != longlonglong);
	assert(0 == VSALargestChunk(vsa_pool));
	
	to_fail = VSAAlloc(vsa_pool, sizeof(char));
	assert(NULL == to_fail);
	assert(0 == VSALargestChunk(vsa_pool));
	
	VSAFree(num);
	VSAFree(str);
	VSAFree(sizet);
	VSAFree(longlong);
	VSAFree(longlonglong);
	assert(112 == VSALargestChunk(vsa_pool));
	assert(112 == VSALargestChunk(vsa_pool));
	
	free(memory); memory = NULL; 
}
