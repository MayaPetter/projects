#include "vector.h"
#include <assert.h> /* assert */
#include <stdlib.h> /* mallo, realloc, free */
#include <string.h> /* memcpy */

struct vector 
{	
	void *base;
	void *current;
	size_t size;
	size_t capacity;
	size_t size_of_element;

};

vector_t *VectorCreate(size_t init_capacity, size_t size_of_element)
{
	/* allocating memory for the vector mangment*/
	vector_t *vector = (vector_t *)malloc(sizeof(vector_t));
	
	assert(0 != size_of_element);
	assert(0 != init_capacity);
	
	if (NULL != vector)
	{	
		/* allocating memory for the vector*/
		vector->base =(void *)malloc(init_capacity * size_of_element);

		if (NULL != vector->base)
		{
			/*initializing vector members */
			vector->current = vector->base;
			vector->size = 0; 
			vector->capacity = init_capacity;
			vector->size_of_element = size_of_element;
		}
		else
		{
			/* if failed to allocate- free memory */
			free(vector); vector = NULL;
		}	
	}
	return vector;
}

void VectorDestroy(vector_t *vector)
{
	assert(NULL != vector);
	
	free(vector->base); vector->base = NULL;
	free(vector); vector = NULL;

}

void *VectorGetItemAddress(const vector_t *vector, size_t element_index)
{
	assert(NULL != vector);
		
    return ((char *)(vector->base) + (element_index * vector->size_of_element));
}

int VectorPushBack(vector_t *vector, const void *element_value)
{
	assert(NULL != vector);
	assert(NULL != element_value);
	
	if (vector->capacity == vector->size)
	{
        if (VECTOR_ALLOCATION_FAILED == VectorReserve(vector, vector->capacity))
        {
			return VECTOR_ALLOCATION_FAILED;
		}
	}
	
	memcpy(vector->current, element_value, vector->size_of_element);
	
	vector->current =(char *)vector->current + vector->size_of_element;
	++vector->size;
	
	return VECTOR_SUCCESS; 		
}


int VectorPopBack(vector_t *vector)
{
	assert(NULL != vector);
	assert(vector->current != vector->base);
	
	vector->current =(char *)vector->current - vector->size_of_element;
	--vector->size;
	
	/*if new size is quarter of capacity decrease capacity by half*/
	if ((vector->capacity / 4) >= vector->size)
	{
	    vector->capacity /= 2;
	    
	    /*failed to allocate new capacity + 0*/
	    if (VECTOR_ALLOCATION_FAILED == VectorReserve(vector, 0)) 
        {
			return VECTOR_ALLOCATION_FAILED;
		}
	}

	return  VECTOR_SUCCESS;
}

size_t VectorSize(const vector_t *vector)
{
	assert(NULL != vector);
	
	return vector->size;
}

int VectorIsEmpty(const vector_t *vector)
{
	assert(NULL != vector);
	
	/*Returns 1 if vector is empty. 0 otherwise */
	return (0 == vector->size);
}

size_t VectorCapacity(const vector_t *vector)
{
	assert(NULL != vector);
	
	return vector->capacity;
}

int VectorReserve(vector_t *vector, size_t size)
{
	void *temp_ptr = NULL;
	size_t reserve = (size + vector->capacity) * vector->size_of_element;
	
	assert(NULL != vector);
	assert(0 != reserve);
	
	temp_ptr = (void *)realloc(vector->base, reserve);  

	if(NULL == temp_ptr)
	{
		return VECTOR_ALLOCATION_FAILED; 	
	}		
	
	vector->base = temp_ptr;
	vector->current = (char *)vector->base + 
                                       (vector->size * vector->size_of_element);	
	vector->capacity = reserve / vector->size_of_element;
	
	return VECTOR_SUCCESS;
    
}

