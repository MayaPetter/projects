/******************************************
* Author:   Maya Petter                   *
* Reviewer:                               *
* Purpose:                                *
*******************************************/
#include <assert.h> /* assert */
#include <stdlib.h> /* free, malloc */

#include "fsa.h"


static void FSAInitTest()
{
    fsa_t *new_pool = (fsa_t *)malloc(50 * sizeof(int));    

    assert(NULL != new_pool);
    
    new_pool = FSAInit((fsa_t *)new_pool, 50, 4);
    
    assert(NULL != new_pool);
    
    free(new_pool); new_pool = NULL;
}

static void SuggestSizeTest()
{
	size_t suggested_size = 0;
    size_t int_size = sizeof(int);
    size_t float_size = sizeof(float);
    size_t long_double_size = sizeof(long double);
    size_t num_of_blocks = 30;
    
    suggested_size = FSASuggestSize(num_of_blocks, int_size);
    assert(488 == suggested_size); 
    
    suggested_size = FSASuggestSize(num_of_blocks, float_size);
    assert(488 == suggested_size); 
    
    suggested_size = FSASuggestSize(num_of_blocks, long_double_size);
    assert(728 == suggested_size); 
    
    suggested_size = FSASuggestSize(num_of_blocks, 1);
    assert(488 == suggested_size); 
    
    suggested_size = FSASuggestSize(num_of_blocks, 0);
    assert(248 == suggested_size); 
}

static void AllocateFreeAndCountFreeTest()
{
    int arr[] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
    int *data = NULL;
    int *first_out = NULL;
    int *seconed_out = NULL;
    int *third_out = NULL;
    
    size_t num_of_blocks = 50;
    size_t counter = 0;
    
    size_t suggested = FSASuggestSize(num_of_blocks, sizeof(int));
    fsa_t *new_pool = (fsa_t *)malloc(suggested);    
    assert(NULL != new_pool);
    
    new_pool = FSAInit((fsa_t *)new_pool, suggested, sizeof(int));
    assert(NULL != new_pool);
    assert(num_of_blocks == FSACountFree(new_pool));
    
    for (counter = 1; counter <= 10; ++counter)
    {
        data = (int *)FSAAlloc(new_pool);
        assert((num_of_blocks - counter) == FSACountFree(new_pool));
        *data = arr[counter - 1];
    }
    
    first_out = (int *)FSAAlloc(new_pool);
    seconed_out = (int *)FSAAlloc(new_pool);
    third_out = (int *)FSAAlloc(new_pool);
    assert(37 == FSACountFree(new_pool));

    FSAFree(first_out);
    assert(38 == FSACountFree(new_pool));
    FSAFree(seconed_out);
    assert(39 == FSACountFree(new_pool));
    FSAFree(third_out);
    assert(40 == FSACountFree(new_pool));
    
    FSAFree(NULL);
    
    free(new_pool); new_pool = NULL;
}

int main()
{
    FSAInitTest();
    SuggestSizeTest();
    AllocateFreeAndCountFreeTest();
          
	return 0;
}
