/******************************************
* Author:   Maya Petter                   *
* Reviewer:                               *
* Purpose:                                *
*******************************************/
#include <stdlib.h> /* malloc, free */
#include <assert.h> /* assert */

#include "pqueue.h"
#include "sorted_list.h"

typedef struct cmp
{
	cmp_priority cmp;
	void *param;
} cmp_t;

struct pqueue
{
	sorted_list_t *list;
	cmp_t comp;
};

static int IsLowerPriority(void *data1, void *data2, const void *param);

pqueue_t *PQueueCreate(cmp_priority is_lower_priority, const void *param)
{
	pqueue_t *new_pq = {NULL};
	
	assert(NULL != is_lower_priority);
	
	new_pq = (pqueue_t *)malloc(sizeof(pqueue_t));
	if(NULL == new_pq)
	{
		return NULL;
	}
	
	new_pq->list = SortedListCreate(IsLowerPriority, &new_pq->comp);
	if (NULL == new_pq->list)
	{
		free(new_pq); new_pq =NULL;
		return NULL;
	}

	new_pq->comp.cmp = is_lower_priority;
	new_pq->comp.param = (void *)param;

	return new_pq; 
}

static int IsLowerPriority(void *data1, void *data2, const void *param)
{
	cmp_t *adaptor = NULL;
	assert(NULL != param);
	
	adaptor = (cmp_t *)param;
	
    /* return 1 if data1 is before data2 or 0 if data1 is after data2 */
	return (0 < adaptor->cmp(data1, data2, adaptor->param));
}

void PQueueDestroy(pqueue_t *pqueue)
{
	assert(NULL != pqueue);
	
	SortedListDestroy(pqueue->list); pqueue->list = NULL;
	free(pqueue); pqueue = NULL;
}

int PQueueEnqueue(pqueue_t *pqueue, const void *data)
{
	sorted_list_iter_t new_node = {NULL};
		
	assert(NULL != pqueue);
	
	new_node = SortedListInsert(pqueue->list, data);
	
	if (1 == SortedListIsSameIter(new_node,SortedListEnd(pqueue->list)))
	{
		return PQUEUE_FAILED;
	}
	
	return PQUEUE_SUCCESS;
}

size_t PQueueSize(const pqueue_t *pqueue)
{
	assert(NULL != pqueue);
	
	return SortedListSize(pqueue->list);
}

int PQueueIsEmpty(const pqueue_t *pqueue)
{
	assert(NULL != pqueue);
	
	return SortedListIsEmpty(pqueue->list);
}

void PQueueDequeue(pqueue_t *pqueue)
{
    assert(NULL != pqueue);
    
    SortedListPopFront(pqueue->list);
}

void *PQueuePeek(const pqueue_t *pqueue)
{
    assert(NULL != pqueue);
    assert(1 != PQueueIsEmpty(pqueue));
    
   return SortedListGetData(SortedListBegin(pqueue->list));
}

void *PQueueErase(pqueue_t *pqueue, is_match func, void *data, void *param)
{
    sorted_list_iter_t iter_to_remove = {NULL};
	void *removed_data = NULL;
	    
    assert(NULL != pqueue);
    assert(NULL != func);
    
    iter_to_remove = SortedListFindIf(SortedListBegin(pqueue->list), 
                              SortedListEnd(pqueue->list), func, data, param);
    if (1 == SortedListIsSameIter(iter_to_remove, SortedListEnd(pqueue->list)))
    {
        return NULL;
    }     
	
	removed_data = SortedListGetData(iter_to_remove);
	
    SortedListRemove(iter_to_remove); 
    
    return removed_data;

}

void PQueueClear(pqueue_t *pqueue)
{
    sorted_list_iter_t iter_to_remove = {NULL};
    sorted_list_iter_t list_end = {NULL};
    
    assert(NULL != pqueue);
    
    for (list_end = SortedListEnd(pqueue->list),  
    	 iter_to_remove = SortedListBegin(pqueue->list)
    	 ; !SortedListIsSameIter(iter_to_remove, list_end)
    	 ; iter_to_remove = SortedListRemove(iter_to_remove))
    { /* empty body */ }        
}



