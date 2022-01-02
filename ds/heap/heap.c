/******************************************
* Author:   Maya Petter                   *
* Reviewer:                               *
* Purpose:                                *
*******************************************/
#include <stdlib.h>  /* free, malloc */
#include <assert.h>  /* assert */

#include "heap.h"
#include "vector.h"

#define HEAP_RESERVE 50
#define HEAP_INIT_SIZE 1
#define TOP 0

struct heap
{
    int (*cmp_priority)(const void *data1, const void *data2, void *param);
    void *param;
    vector_t *vector;    
};

enum direction {UP, LEFT, RIGHT, NOT};

/***************** Static Functions *******************************************/
static void *FreeFunction(void *to_free);

/* Data functions */
static void *GetData(vector_t *vector, size_t index);
static int CmpData(heap_t *heap, size_t index1, size_t index2);
static void SwapData(heap_t *heap, size_t index1, size_t index2);
static size_t GetDataIndex(heap_t *heap, int (*is_match)(const void *heap_data,
			   	           const void *user_data), void *data);

/* Index functions */
static size_t GetChildIndex(size_t index, int direction);
static size_t GetParentIndex(size_t index);
static size_t GetLastIndex(heap_t *heap);
static int GetMaxChild(heap_t *heap, size_t left_child_i, size_t right_child_i);
static int HasChildInDirection(heap_t *heap, size_t index, int direction);

/* Sift functions */
static int GetDirection(heap_t *heap, size_t index);
static void SiftInDirection(heap_t *heap, size_t index, int direction);
static void Sifting(heap_t *heap, size_t index_to_remove);

/***************** API Functions **********************************************/

/***************** Create new heap ********************************************/
heap_t *HeapCreate(int (*cmp_priority)(const void *data1,
	   		            const void *data2, void *param), void *param)
{
    heap_t *new_heap = NULL;

    assert(NULL != cmp_priority);

    new_heap = (heap_t *)malloc(sizeof(struct heap));
    if (NULL == new_heap)
    {
        return NULL;
    }
    
    new_heap->vector = VectorCreate(HEAP_INIT_SIZE, sizeof(void *));
    if (NULL == new_heap->vector)
    {
        new_heap = FreeFunction(new_heap);
        
        return NULL;        
    }
    
    new_heap->cmp_priority = cmp_priority;
    new_heap->param = param;
    
    return new_heap; 
}

/***************** Destroy heap ***********************************************/
void HeapDestroy(heap_t *heap)
{
    assert(NULL != heap);
    
    VectorDestroy(heap->vector);
    
    heap = FreeFunction(heap);
}

/***************** Add new member to heap according to priority ***************/
int HeapPush(heap_t *heap, const void *data)
{
    int status = HEAP_ALLOC_FAIL;
    size_t last_index = 0;
    
    assert(NULL != heap);

    status = VectorPushBack(heap->vector, &data);
    if (VECTOR_ALLOCATION_FAILED == status)
    {
        return HEAP_ALLOC_FAIL; 
    } 
    
    last_index = GetLastIndex(heap);
    
    Sifting(heap, last_index); 
    
    return HEAP_SUCCESS;
}

/***************** Remove member from top priority ****************************/
void HeapPop(heap_t *heap)
{
    assert(NULL != heap);
    
    SwapData(heap, TOP,GetLastIndex(heap));
    
    VectorPopBack(heap->vector);

    if (!HeapIsEmpty(heap))
    {
        Sifting(heap, TOP); 
    }
}

/***************** Return pointer to data from top priority *******************/
void *HeapPeek(const heap_t *heap)
{
    assert(NULL != heap);
    
    return GetData(heap->vector, TOP);
}

/***************** Return amount of elements in heap **************************/
size_t HeapSize(const heap_t *heap)
{
    assert(NULL != heap);
    
    return VectorSize(heap->vector);
}

/***************** Check if heap is empty *************************************/
int HeapIsEmpty(const heap_t *heap)
{
    assert(NULL != heap);
    
    return VectorIsEmpty(heap->vector);
}

/***************** Remove data from heap *************************************/
void *HeapRemove(heap_t *heap, int (*is_match)(const void *heap_data,
			   	 const void *user_data), void *data)
{
    size_t found_index = 0;
    size_t size = 0; 
    void *data_to_return = NULL;
    
    assert(NULL != heap);
    size = HeapSize(heap);
    
    found_index = GetDataIndex(heap, is_match, data);
    if (size == found_index  ) /* data wasn't found */
    {
        return NULL;
    }
    
    data_to_return = GetData(heap->vector, found_index); 

    if (GetLastIndex(heap) == found_index) /* last index don't need to sift*/
	{
	    VectorPopBack(heap->vector);
	}
    else
    {
        SwapData(heap, found_index,GetLastIndex(heap));    

        VectorPopBack(heap->vector);
        
        Sifting(heap, found_index);
    }
    return data_to_return;
}

/***************** Static Functions *******************************************/
static void *FreeFunction(void *to_free)
{
    assert(NULL != to_free);
    
    free(to_free);
    
    to_free = NULL;
    
    return NULL;    
}

/***************** Data Functions *********************************************/
static void SwapData(heap_t *heap, size_t index1, size_t index2)
{
    void **data1 = VectorGetItemAddress(heap->vector, index1); 
    void **data2 = VectorGetItemAddress(heap->vector, index2);
    void *tmp = NULL;
    
    assert(NULL != data1);
    assert(NULL != data2);
    
    tmp = *data1;
    *data1 = *data2;
    *data2 = tmp;
}

static void *GetData(vector_t *vector, size_t index)
{
    assert(NULL != vector);
    
    return *(void **)VectorGetItemAddress(vector, index);
}

static int CmpData(heap_t *heap, size_t index1, size_t index2)
{
    void *data1 = NULL;
    void *data2 = NULL;
    
    assert(NULL != heap);

    data1 = GetData(heap->vector, index1);
    data2 = GetData(heap->vector, index2);
    
    return heap->cmp_priority(data1, data2, heap->param);
}

/***************** Sift Functions *********************************************/
static int GetDirection(heap_t *heap, size_t index)
{
    void *index_data = NULL;

    assert(NULL != heap);

    index_data = GetData(heap->vector, index);

    if (0 != index && 0 < CmpData(heap, GetParentIndex(index), index))
    {
	    return UP;
    }
    else
    {
        int cmp = 0;
        int direc = NOT;
        
	    if (HasChildInDirection(heap, index, LEFT)) 
	    {
		    if (HasChildInDirection(heap, index, RIGHT)) 
		    {
			    void *max_data =  NULL;

			    direc = GetMaxChild(heap, 
			                        GetChildIndex(index, LEFT), 
			                        GetChildIndex(index, RIGHT));
	            		
			    max_data = GetData(heap->vector, GetChildIndex(index, direc));
			
			    cmp = heap->cmp_priority(index_data, max_data, heap->param);
		    }
		    else 
		    {
		        direc = LEFT;
                cmp = CmpData(heap, index, GetChildIndex(index, LEFT));
			    
		    }	
		    return ((0 < cmp) ? direc : NOT);										   		
	    }
    }
    return NOT;
}

static void SiftInDirection(heap_t *heap, size_t index, int direction)
{
	int status = 0;

	for (status = GetDirection(heap, index)
		; NOT != status
		; status = GetDirection(heap, index))
	{
		int sift_direction = status;
		
		if (UP == direction) 
		{
            size_t parent_index = GetParentIndex(index);
                    
            SwapData(heap, parent_index, index);
                    
            index = parent_index;
        
		}		
		else  
		{
			size_t child_index = GetChildIndex(index, sift_direction); 
        
            SwapData(heap, child_index, index);      
        
            index = child_index;        
		}
	}
}

static void Sifting(heap_t *heap, size_t index)
{
	int direction = 0;

	assert(NULL != heap);

	direction = GetDirection(heap, index);
	
    if (UP == direction)
	{
		SiftInDirection(heap, index, UP);
	}
	else
	{
		SiftInDirection(heap, index, direction);
	}
}		               	                    

/***************** Index Functions ********************************************/
static int GetMaxChild(heap_t *heap, size_t left_child_i, size_t right_child_i)
{
    int cmp = NOT;
    size_t size = 0; 
    
    assert(NULL != heap);
    
    size = HeapSize(heap);
    
    if (size > left_child_i && size > right_child_i)
    {
        cmp = CmpData(heap, left_child_i, right_child_i);
        cmp = (0 < cmp) ? RIGHT : LEFT;
    }
    return cmp; 
}

static size_t GetDataIndex(heap_t *heap, int (*is_match)(const void *heap_data,
			   	           const void *user_data), void *data)
{
    size_t index = 0;
    size_t size = 0;
    int match = NOT;
    
    assert(NULL != heap);
    assert(NULL != is_match);
    
    size = HeapSize(heap);
    
    for (index = 0 ;index < size && 1 != match; ++index)
    {
        match = is_match(GetData(heap->vector, index), data);
        
    }
    
    /* last loop will increase by 1 */    
    if (1 == match)
	{
		index -= 1; 
	}
    
    return index;
}
			 
static size_t GetChildIndex(size_t index, int direction)
{
    /*2n+1 for left child or 2n+2 for right child */
    return (2 * index + direction); 
}

static size_t GetParentIndex(size_t index)
{   
    if (0 == index)
    {
        return 0;
    }
    
    return ((index - 1) / 2); /* (n-1)/2  */
}

static size_t GetLastIndex(heap_t *heap)
{
    assert(NULL != heap);

    return (HeapSize(heap) - 1);
}

static int HasChildInDirection(heap_t *heap, size_t index, int direction)
{
    assert(NULL != heap);
     
    return (HeapSize(heap) > GetChildIndex(index, direction));
}
