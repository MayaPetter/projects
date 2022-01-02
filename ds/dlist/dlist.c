/******************************************
* Author:   Maya Petter                   *
* Reviewer: Shaddad Gomid                 *
* Status:   Sent                          *
* Purpose:  Dlist                         *
*******************************************/

#include <stdlib.h> /* malloc, free */
#include <assert.h> /* assert */

#include "dlist.h"

typedef struct dlist_node dlist_node_t;
 
struct dlist_node 
{
	dlist_node_t *next;
	dlist_node_t *prev;
	void *data;
};

struct dlist
{
	dlist_node_t *head;
	dlist_node_t *end;
};

static dlist_iter_t CreateNode(void *data, dlist_iter_t next, dlist_iter_t prev)
{
	dlist_iter_t node = (dlist_iter_t)malloc(sizeof(struct dlist_node));
	if (NULL == node)
	{
		return NULL;
	}
	
	node->data = data;
	node->next = next;
	node->prev = prev;
	
	return node;
}

dlist_t *DListCreate(void)
{
	dlist_t *dlist = (dlist_t *)malloc(sizeof(dlist_t));
	if (NULL == dlist)
	{
		return NULL;
	}
	
	dlist->head = CreateNode(NULL, NULL, NULL);
	dlist->end = CreateNode(NULL, NULL, dlist->head);
	if (NULL == dlist->head || NULL == dlist->end)
	{
		free(dlist); dlist = NULL;
		return NULL;
	}
	
	dlist->head->next = dlist->end;
	
	return dlist;
}

void DListDestroy(dlist_t *list)
{
	assert(NULL != list);
	
	while(NULL != list->head)
	{
		dlist_iter_t next = list->head->next;
		free(list->head); list->head = NULL;
		list->head = next;
	}
	
	free(list); list = NULL;
}

int DListIsSameIter(const dlist_iter_t iter1, const dlist_iter_t iter2)
{
	assert(NULL != iter1);
	assert(NULL != iter2);
	
	return (iter1 == iter2);
}

size_t DListSize(const dlist_t *list)
{
	size_t counter = 0;
	
	dlist_iter_t next_node = NULL;
	
	assert(NULL != list);
	
	next_node = (dlist_iter_t)list->head;
	
	while (1 != DListIsSameIter(next_node->next, list->end))
	{
		next_node = next_node->next;
		++counter;
	}
	
	return counter;
}

int DListIsEmpty(const dlist_t *list)
{
	assert(NULL != list);
	
	return DListIsSameIter(list->head->next, list->end);	
}

dlist_iter_t DListInsert(dlist_t *list, dlist_iter_t iter, void *data)
{
	dlist_iter_t new_node = NULL;
	
	assert(NULL != list);
	 
	new_node = CreateNode(data, iter, iter->prev);
	if (NULL == new_node)
	{
		return list->end;
	}

	iter->prev->next = new_node;
	iter->prev = new_node;
	
	return new_node;
}

dlist_iter_t DListRemove(dlist_iter_t iter_to_remove)
{
	dlist_iter_t next_node = NULL;
	
	assert(NULL != iter_to_remove);
	
	next_node = iter_to_remove->next;
	
	iter_to_remove->prev->next = next_node;
	iter_to_remove->next->prev = iter_to_remove->prev;
	
	free(iter_to_remove); iter_to_remove = NULL;
	
	return next_node;
}

int DListForEach(dlist_iter_t from, dlist_iter_t to, action_func func, 
				 void *param)
{
    int func_status = 0;
    
    assert(NULL != from);
    assert(NULL != to);

    while ((1 != DListIsSameIter(from, to)) && (0 == func_status))
    {
    	func_status = func(from->data, param);
        from = from->next;
    }       
    return func_status;
}				 

dlist_iter_t DListFind(dlist_iter_t from, dlist_iter_t to, cmp_func cmp,
					   const void *data)
{
    assert(NULL != from);
    assert(NULL != to);

    while ((1 != DListIsSameIter(from, to)) && (0 != cmp(from->data, data)))
    {
        from = from->next;
    }
    
    return from;
}

dlist_iter_t DListPushFront(dlist_t *list, const void *data)
{
    assert(NULL != list);
    
    return DListInsert(list, DListBegin(list), (void *)data);
}

void *DListPopFront(dlist_t *list)
{
	void *data = NULL;
	
	assert(NULL != list);
	
	data = DListIterGetData(DListBegin(list));

	DListRemove(DListBegin(list));
	
	return data;
}

dlist_iter_t DListPushBack(dlist_t *list, const void *data)
{
    assert(NULL != list);
	
    return DListInsert(list, DListEnd(list), (void *)data);
}

void *DListPopBack(dlist_t *list)
{
	void *data = NULL;
	
	assert(NULL != list);
	
	data = DListIterGetData(DListBack(DListEnd(list)));

	DListRemove(DListBack(DListEnd(list)));
	
	return data;
}

dlist_iter_t DListNext(dlist_iter_t iter)
{
	assert(NULL != iter);
	
	return iter->next;
}

dlist_iter_t DListBack(dlist_iter_t iter)
{
	assert(NULL != iter);
	
	return iter->prev;
}

void *DListIterGetData(dlist_iter_t iter)
{
	assert(NULL != iter);
	 
    return iter->data;
}

dlist_iter_t DListBegin(const dlist_t *dlist)
{
	assert(NULL != dlist);
	
	return dlist->head->next;
}

dlist_iter_t DListEnd(const dlist_t *dlist)
{
	assert(NULL != dlist);
	
	return dlist->end;
}


dlist_iter_t DListSplice(dlist_iter_t iter_dest, 
						 dlist_iter_t iter_src_start, 
						 dlist_iter_t iter_src_end)
{
	dlist_iter_t dest_prev = NULL;
	
	assert(NULL != iter_dest);
	assert(NULL != iter_src_start);
	assert(NULL != iter_src_end);
	assert(1 != DListIsSameIter(iter_src_start, iter_src_end));
	
	dest_prev = iter_dest->prev;
	
	/*connect the src_end of range to dest*/ 
	iter_dest->prev = iter_src_end->prev;
	iter_src_end->prev->next = iter_dest;
	
	/*connect the src_start of range to dest*/
	iter_src_end->prev = iter_src_start->prev;
	iter_src_start->prev->next = iter_src_end;
	
	/*connect the remaining elements of src to each other*/
	dest_prev->next = iter_src_start;	 
	iter_src_start->prev = dest_prev;
	
	return iter_dest;
}
