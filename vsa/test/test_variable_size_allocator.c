#include <stdio.h> /* printf */
#include <assert.h> /* assert */
#include <string.h> /* string */        
#include <stdlib.h> /* malloc, free */

#include "vsa.h"

#define FLOW_GREET(name) (printf("\033[1;36mThis is a test for %s:\033[0m\n\n", name))
#define OK (printf("\033[1;32mOK.\033[0m\n"))
#define FAIL(name_of_test) (printf("\033[1;31mFAIL:\033[0m\n%s\nline: %d\n",name_of_test, __LINE__))
#define NEW_LINE (printf("\n\n"))
#define UNUSED(x) ((void)x)


static void TestFlow1();


const size_t SIZE_OF_WORD = sizeof(size_t);	
		
int main()
{
	TestFlow1();
	
    return 0;
}
        
        
static void TestFlow1()
{
	void *memory = NULL;
	vsa_t *vsa = NULL;
    size_t memory_size = 200;
	int num = 777;
	char str[] = "this is a test";
	void *addresses_to_free[10];
	void *current_address = NULL;
	size_t prev_biggest_chunk = 0;
	int j = 0;
	
	FLOW_GREET("Flow1");
	
	memory = malloc(memory_size);
	assert(NULL != memory);
	
	vsa = VSAInit(memory, memory_size);
	assert(NULL != vsa);
	
	addresses_to_free[0] = VSAAlloc(vsa, 4);
	assert(addresses_to_free[0] != NULL);
	assert(0 == (size_t)addresses_to_free[0] % SIZE_OF_WORD);
	
    assert(VSAAlloc(vsa, 200) == NULL);
	
	addresses_to_free[1] = VSAAlloc(vsa, sizeof(str));
	assert(addresses_to_free[1] != NULL);
	assert((char*)addresses_to_free[1] > (char*)addresses_to_free[0] + 4);
	assert(0 == (size_t)addresses_to_free[1] % SIZE_OF_WORD);
	
	strcpy(addresses_to_free[1], str);
	*(int*)addresses_to_free[0] = num;
	assert(0 == strcmp(addresses_to_free[1], str));
	assert(num == *(int*)addresses_to_free[0]);
	
	addresses_to_free[2] = VSAAlloc(vsa, SIZE_OF_WORD);
	assert(addresses_to_free[0] != NULL);
	assert(0 == (size_t)addresses_to_free[2] % SIZE_OF_WORD);
	assert((char*)addresses_to_free[2] >= (char*)addresses_to_free[1] + 
																SIZE_OF_WORD);
	
	addresses_to_free[3] = VSAAlloc(vsa, 5);
	assert(addresses_to_free[0] != NULL);
	assert(0 == (size_t)addresses_to_free[3] % SIZE_OF_WORD);
	assert((char*)addresses_to_free[3] >= (char*)addresses_to_free[2] + 5);

	*(int*)addresses_to_free[2] = num;
	*(int*)addresses_to_free[3] = num;
	
	assert(*(int*)addresses_to_free[0] == *(int*)addresses_to_free[2]);
	assert(*(int*)addresses_to_free[2] == *(int*)addresses_to_free[3]);
	
	/*check padding
	assert( (char*)addresses_to_free[3] - (char*)addresses_to_free[2]
									 	== 
			(char*)addresses_to_free[2] - (char*)addresses_to_free[1] );*/
			
	
	
	
	
	for (j = 3; 0 <= j; --j)
	{
		prev_biggest_chunk = VSALargestChunk(vsa);
		VSAFree(addresses_to_free[j]); 
		
		if (prev_biggest_chunk >= VSALargestChunk(vsa))
		{
			FAIL("prev_biggest_chunk >= VSALargestChunk(vsa)");
			printf("j = %d\n", j);
		}
		
		assert(prev_biggest_chunk < VSALargestChunk(vsa));
	}
	
/*	VSAFree(NULL);*/
	
	
	prev_biggest_chunk = VSALargestChunk(vsa);
	
	addresses_to_free[0] = VSAAlloc(vsa, 4);
	assert(0 == (size_t)addresses_to_free[1] % SIZE_OF_WORD);
	addresses_to_free[1] = VSAAlloc(vsa, 6);
	assert(0 == (size_t)addresses_to_free[1] % SIZE_OF_WORD);
	
	VSAFree(addresses_to_free[0]);
	VSAFree(addresses_to_free[1]);
	addresses_to_free[1] = VSAAlloc(vsa, 23);
	
	assert(addresses_to_free[1] == addresses_to_free[0]);
	
	current_address = VSAAlloc(vsa, VSALargestChunk(vsa));
	
	assert(VSAAlloc(vsa, 1) == NULL);
	
	assert(0 == VSALargestChunk(vsa));
	
	VSAFree(addresses_to_free[1]);
	VSAFree(current_address);
	
	assert(prev_biggest_chunk == VSALargestChunk(vsa));
	
	
	OK;
	
	free(memory);
	
	NEW_LINE;
}
