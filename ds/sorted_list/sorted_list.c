/******************************************
* Author:   Maya Petter                   *
* Reviewer: Hila Bagam                    *
* Purpose:  sorted_list                   *
*******************************************/
#include <assert.h>         /* assert */
#include <stdlib.h>         /* malloc, free */

#include "sorted_list.h"    /* sorted_list */
#include "dlist.h" /* dlist_iter_t */

struct sorted_list
{
	dlist_t *dlist;
	is_before comparator;
	void *param;
};

typedef struct patch
{
	void *user_data;
	void *user_param;
	sorted_list_is_match user_func;
} patch_t;

int IsMatchAdapt(void *data1, const void *patch);
static patch_t PatchInit(void *data, void *param, sorted_list_is_match func);

sorted_list_t *SortedListCreate(is_before func, const void *param)
{
	sorted_list_t *list = NULL;
	
	assert(NULL != func);
	
	list = (sorted_list_t *)malloc(sizeof(sorted_list_t));
	if (NULL == list)
	{
		return NULL;
	}
	
	list->dlist = DListCreate();
	if (NULL == list->dlist)
	{
		free(list); list = NULL;
		return NULL;
	}
	
	list->comparator = func;
	list->param = (void *)param;
	
	return list;
}

void SortedListDestroy(sorted_list_t *list)
{
	assert(NULL != list);
	 
	DListDestroy(list->dlist); list->dlist = NULL;
	free(list); list = NULL;
}

int SortedListIsEmpty(const sorted_list_t *list)
{
	assert(NULL != list);
	
	return DListIsEmpty(list->dlist);
}

size_t SortedListSize(const sorted_list_t *list)
{
	assert(NULL != list);
		
	return DListSize(list->dlist);
}

sorted_list_iter_t SortedListBegin(const sorted_list_t *list)
{
	sorted_list_iter_t iter = {NULL};
	
	assert(NULL != list);
	
	iter.diter = DListBegin(list->dlist);
	 
	return iter;
}

sorted_list_iter_t SortedListEnd(const sorted_list_t *list)
{
	sorted_list_iter_t iter = {NULL};
	
	assert(NULL != list);
	
	iter.diter = DListEnd(list->dlist);
	 
	return iter;
}

sorted_list_iter_t SortedListNext(sorted_list_iter_t iter)
{
	assert(NULL != iter.diter);
	
	iter.diter = DListNext(iter.diter);
	 
	return iter;
}

sorted_list_iter_t SortedListPrev(sorted_list_iter_t iter)
{
	assert(NULL != iter.diter);
	
	iter.diter = DListBack(iter.diter);
	 
	return iter;
}

void *SortedListGetData(sorted_list_iter_t iter)
{
	assert(NULL != iter.diter);
	
	return DListIterGetData(iter.diter);
}

int SortedListIsSameIter(const sorted_list_iter_t iter1,
                         const sorted_list_iter_t iter2)
{
	assert(NULL != iter1.diter);
	assert(NULL != iter2.diter);
	
	return DListIsSameIter(iter1.diter, iter2.diter);
}

void *SortedListPopFront(sorted_list_t *list)
{
	assert(NULL != list);
	
	return DListPopFront(list->dlist);
}

void *SortedListPopBack(sorted_list_t *list)
{
	assert(NULL != list);
	
	return DListPopBack(list->dlist);
}

sorted_list_iter_t SortedListRemove(sorted_list_iter_t iter_to_remove)
{
	sorted_list_iter_t iter_next = {NULL};
	
	assert(NULL != iter_to_remove.diter);
	
	iter_next.diter = DListRemove(iter_to_remove.diter);
	 
	return iter_next;
}

sorted_list_iter_t SortedListInsert(sorted_list_t *list, const void *data)
{
	sorted_list_iter_t new_node = {NULL};
	dlist_iter_t runner = NULL;
	dlist_iter_t end = NULL;
	
	assert(NULL != list);
	
	runner = DListBegin(list->dlist);
	end = DListEnd(list->dlist);
	
	while((!DListIsSameIter(end, runner)) && 
	(1 != list->comparator((void *)data, DListIterGetData(runner), 
	                                                              list->param)))
	{
		runner = DListNext(runner); 
	}
	
	new_node.diter = DListInsert(list->dlist, runner, (void *)data);
	if (1 == DListIsSameIter(DListEnd(list->dlist), new_node.diter))
	{
	    return SortedListEnd(list);
	}
	
	return new_node;
}

int SortedListForEach(sorted_list_iter_t from, sorted_list_iter_t to,
                      sorted_list_action_func func, void *param)
{   
    assert(NULL != from.diter);
	assert(NULL != to.diter);
	assert(NULL != func);
	                   
    return DListForEach(from.diter, to.diter, func, param);
}

sorted_list_iter_t SortedListFindIf(sorted_list_iter_t from,
                                    sorted_list_iter_t to,
                                    sorted_list_is_match func, 
                                    const void *data, void *param)
{
	patch_t patch_struct = {NULL};
	
	assert(NULL != from.diter);
	assert(NULL != to.diter);
	assert(NULL != func);
	
	patch_struct = PatchInit((void *)data, param, func);
	
	from.diter = DListFind(from.diter, to.diter, IsMatchAdapt, &patch_struct);
	
	return from;
}

sorted_list_iter_t SortedListFind(const sorted_list_t *list,
                                  sorted_list_iter_t from,
                                  sorted_list_iter_t to, const void *param)
{
	void *data = NULL;
	int is_data_before_param = -1;
	int is_param_before_data = -2;
	
    assert(NULL != from.diter);
    assert(NULL != to.diter);
    assert(NULL != list);
 
 	data = SortedListGetData(from);
 	
    while ((!SortedListIsSameIter(from, to)) && 
    	  (0 != list->comparator(data, (void *)param, list->param)))
    {
        from = SortedListNext(from);
        data = SortedListGetData(from);
    }
    
    if (1 == SortedListIsSameIter(from, SortedListEnd(list)))
    {
        return from;    
    }
    
    is_data_before_param = list->comparator(data, (void *)param, list->param);
    is_param_before_data = list->comparator((void *)param, data, list->param);
    
   	
   	if (is_data_before_param != is_param_before_data)
    {	
    	return to;
    }
    
    return from;
}                                  

sorted_list_t *SortedListMerge(sorted_list_t *list_dest,
                               sorted_list_t *list_src)
{
	sorted_list_iter_t dest_iter = {NULL};
	sorted_list_iter_t src_from = {NULL};
	sorted_list_iter_t src_to = {NULL};		
	
	assert(NULL != list_dest);
    assert(NULL != list_src);
    
    dest_iter = SortedListBegin(list_dest);
    src_from = SortedListBegin(list_src);
    src_to = src_from;
    
    while (1 != SortedListIsSameIter(src_to, SortedListEnd(list_src)))
    {
 		/* search dest for data that param comes before */
    	for ( 
    		;(1 != SortedListIsSameIter(dest_iter, SortedListEnd(list_dest)))
    		&& (1 != list_dest->comparator(SortedListGetData(src_to),    
    		              SortedListGetData(dest_iter), list_dest->param))
    		; dest_iter = SortedListNext(dest_iter))             
    	{
    		/* empty body */
    	}
    	
    	/* if reach end of dest splice all the rest of src into dest */
    	if ((1 == SortedListIsSameIter(dest_iter, SortedListEnd(list_dest))))
    	{
    		src_to = SortedListEnd(list_src);
    	}
    	
    	/* search src for the range to splice into dest */
    	for (
    		;(1 != SortedListIsSameIter(src_to, SortedListEnd(list_src))) 
    		&& (1 == list_dest->comparator(SortedListGetData(src_to),    
    		              SortedListGetData(dest_iter), list_dest->param))
    		; src_to = SortedListNext(src_to))
    	{
    		/* empty body */
    	}             	
    	
    	DListSplice(dest_iter.diter, src_from.diter, src_to.diter);
 		
 		src_from = SortedListBegin(list_src);
    }		
	
	return 	list_dest;
}


int IsMatchAdapt(void *data1, const void *patch)
{
	patch_t *patch_casted = (patch_t *)patch;
	
	assert(NULL != patch);
	
	return !((patch_casted->user_func)(data1, patch_casted->user_data,
												patch_casted->user_param));
}

static patch_t PatchInit(void *data, void *param, sorted_list_is_match func)
{
	patch_t new_patch = {NULL};
	
	assert(NULL != func);
	
	new_patch.user_data = data;
	new_patch.user_param = param;
	new_patch.user_func = func;
	
	return new_patch;
}
