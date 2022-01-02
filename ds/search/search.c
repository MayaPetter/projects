/******************************************
* Author:   Maya Petter                   *
* Reviewer: Dor Tambour                   *
* Purpose:  Search Algorithms             *
*******************************************/
#include <assert.h> /* assert */
#include <math.h>   /* sqrt   */

#include "search.h"

/************* Static Functions ***********************************************/
void *RecBSearchRecursive(const void *key, const void *base, size_t beginning, 
                          size_t end, size_t elm_size, 
                          int (*cmp)(const void *key, const void *arr_data));
void *LinearSearch(const void *key, void *from, void *to, 
				   size_t elm_size, 
				   int (*cmp)(const void *key, const void *arr_data));
				   
/************* API Functions **************************************************/

/************* Binary search **************************************************/
void *BSearchIterative(const void *key, const void *base, size_t nitems,
					   size_t elm_size, 
					   int (*cmp)(const void *key, const void *arr_data))
{
    char *runner = NULL;
    size_t beginning = 0;
    size_t end = (0 == nitems) ? 0 : nitems - 1;
    int cmp_result = 0;
    
    assert(NULL != key);
    assert(NULL != base);
    assert(NULL != cmp);
    assert(0 != nitems);
    assert(0 != elm_size);
    
    while (beginning <= end)
    { 
        size_t middle = (beginning + end) >> 1;
        runner = (char *)base + (middle * elm_size);
        cmp_result = cmp(key, runner);
        
        if(0 == cmp_result)
        {
            return runner;
        }
        else if (0 > cmp_result && 0 != middle)
        {
            end = middle - 1;
        }
        else
        {
            beginning = middle + 1;
        }
    }
    
    return runner;
}

/************* Binary search recursive ****************************************/
void *BSearchRecursive(const void *key, const void *base, size_t nitems,
					   size_t elm_size, 
					   int (*cmp)(const void *key, const void *arr_data)) 
{
    size_t end = (0 == nitems) ? 0 : nitems - 1;
    
    assert(NULL != key);
    assert(NULL != base);
    assert(NULL != cmp);
    
    return RecBSearchRecursive(key, base, 0, end, elm_size, cmp);
    
}

void *RecBSearchRecursive(const void *key, const void *base, size_t beginning, 
                          size_t end, size_t elm_size, 
                          int (*cmp)(const void *key, const void *arr_data))
{                          
                           
    size_t middle = 0;
    char *runner = NULL;
    int cmp_result = 0;
    
    assert(NULL != key);
    assert(NULL != base);
    assert(NULL != cmp);
    
    middle = (beginning + end) >> 1;
    runner = (char *)base + (middle * elm_size);
    cmp_result = cmp(key, runner);
    
    if (0 == cmp_result)
    {
        return runner;
    }
    
    if (beginning > end)
    {
        return NULL;
    }
    
    if (0 > cmp_result && 0 != middle)
    {
        return RecBSearchRecursive(key, base, beginning, middle - 1, 
                                                                elm_size, cmp);
    }
    
    runner = (char *)base + ((middle + 1) * elm_size);
        
    return RecBSearchRecursive(key, base, middle + 1, end, elm_size, cmp); 
}

/************* Jump search ****************************************************/
void *JSearch(const void *key, const void *base, size_t nitems, size_t elm_size,
		      int (*cmp)(const void *, const void *))
{
	void *jumper = NULL;
	void *begin = NULL;
	void *end = NULL;	
	size_t jump_size = sqrt(nitems)* elm_size;
	
	assert(NULL != key);
    assert(NULL != cmp);
    
	jumper = (void *)base;
	begin = (void *)base;
	end = (char *)base + (nitems * elm_size);	
	jump_size = sqrt(nitems);
	
	
	for ( jumper = begin
		; jumper < end
		; jumper = (char *)jumper + (jump_size * elm_size))
	{
		int cmp_result = cmp(jumper, key);
		
		if (0 == cmp_result)
		{
			return jumper;
		}	
		else if (0 > cmp_result)
		{
			begin = jumper;
		}
		else
		{
			return LinearSearch(key, begin, jumper, elm_size, cmp);
		}
	}
	
	return LinearSearch(key, begin, end, elm_size, cmp);	
}

void *LinearSearch(const void *key, void *from, void *to, size_t elm_size,
				   int (*cmp)(const void *, const void *))
{
	void *runner = NULL;
	
	assert(NULL != key);
    assert(NULL != from);
    assert(NULL != to);
    assert(NULL != cmp);
    
	for ( runner = from
		; runner < to
		; runner = (char *)runner + elm_size)
	{
		int cmp_result = cmp(runner, key);
		
		if (0 == cmp_result)
		{
			return runner;
		}
		
		if (0 < cmp_result) 
		{
			return NULL;
		}
	}
	
	return NULL;
}

