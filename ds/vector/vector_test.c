#include "vector.h"
#include <assert.h> /* assert */
#include <stdio.h> /*printf */

void VectorCreateBTest(size_t capacity, size_t size_of_element);
void VectorGetItemAddressBTest(vector_t *vector, int *item, size_t item_index);
void VectorPushBackBTest(vector_t *vector, int *item);
void VectorPopBackBTest(vector_t *vector);
void VectorSizeBTest(vector_t *vector, size_t size);
void VectorIsEmptyBTest(vector_t *vector, int is_empty);
void VectorCapacityBTest(vector_t *vector, size_t capacity);
void VectorReserveBTest(vector_t *vector, size_t capacity);

int main()
{
    enum boolen {false, true};
    
    int items[] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11};
    size_t capacity = 10;
    size_t size_of_element = 4;
    int item_more = 555;
    size_t i = 0;
    
    vector_t *vector = VectorCreate(capacity, size_of_element);
    VectorIsEmptyBTest(vector, true);
    
    for (i = 0; i < 10; ++i)
	{
		VectorSizeBTest(vector, i);
		VectorPushBackBTest(vector, &items[i]);
		VectorGetItemAddressBTest(vector, &items[i], i);
		VectorCapacityBTest(vector, capacity);
	}
	
	VectorIsEmptyBTest(vector, false);
	
	/* push 1 above capacity should double the capacity*/
	VectorPushBackBTest(vector, &item_more);
	capacity *=2;
	VectorCapacityBTest(vector, capacity);
    
    for (i = 10; i > 5; --i)
	{
        VectorPopBackBTest(vector);
		VectorCapacityBTest(vector, capacity);
	}
	
	VectorGetItemAddressBTest(vector, &items[0], 0);
	
	/* pop 1 bellow 1/4 capacity should cut by helf the capacity*/
	VectorPopBackBTest(vector);
	capacity /=2;
	VectorCapacityBTest(vector, capacity);
    
    /* reserve capacity 10 should be 20 capacity*/
    VectorReserveBTest(vector, capacity); 
    capacity *=2;
	VectorCapacityBTest(vector, capacity);
	
    VectorDestroy(vector);	
	puts("good");
	
	return 0;
}
 
void VectorCreateBTest(size_t capacity, size_t size_of_element)
{
	assert(NULL != VectorCreate(capacity, size_of_element));
} 

void VectorGetItemAddressBTest(vector_t *vector, int *item, size_t item_index)
{
	assert(NULL != VectorGetItemAddress(vector, item_index));
	assert(*item == *(int *)VectorGetItemAddress(vector, item_index));

} 

void VectorPushBackBTest(vector_t *vector, int *item)
{
	assert(VECTOR_ALLOCATION_FAILED != VectorPushBack(vector, item));
	
}

void VectorPopBackBTest(vector_t *vector)
{
    assert(VECTOR_ALLOCATION_FAILED != VectorPopBack(vector));
}

void VectorSizeBTest(vector_t *vector, size_t size)
{
	assert(size == VectorSize(vector));

}

void VectorIsEmptyBTest(vector_t *vector, int is_empty)
{
	assert(is_empty == VectorIsEmpty(vector));
}

void VectorCapacityBTest(vector_t *vector, size_t capacity)
{
	assert(capacity == VectorCapacity(vector));
}

void VectorReserveBTest(vector_t *vector, size_t capacity)
{
	
	assert(VECTOR_ALLOCATION_FAILED != VectorReserve(vector, capacity));

}	
