#include <assert.h> /* assert */
#include <stdio.h>
#include "dlist.h"
#define UNUSED(x) (void)(x)


void DListCreateSizeDestroyTest();
void DListInsertTest();
void DListRemoveTest();
void DListGetDataTest();
int AddOneSucc(void *data, void *param);
int AddOneFail(void *data, void *param);
int Cmp(void *data, const void *param);
void DListForEachAndFindTest();
void DlistPushAndPopTest();
void DListSpliceTest();

int main()
{
	DListCreateSizeDestroyTest();	
	DListInsertTest();	
	DListRemoveTest();	
	DListGetDataTest();
	DListForEachAndFindTest();
	DlistPushAndPopTest();	
	DListSpliceTest();
			
	return 0;
}

void DListCreateSizeDestroyTest()
{
    dlist_t *list = DListCreate();
    assert(1 == DListIsEmpty(list));
    assert(0 == DListSize(list));    
	assert(DListEnd(list) == DListBegin(list));
	
	DListDestroy(list);
	
}

void DListInsertTest()
{
	int arr[] = {1, 2, 3, 4, 5};
	size_t i = 0;
	
	dlist_t *list = DListCreate();
	dlist_iter_t node = NULL;
	dlist_iter_t new_node = NULL;
	dlist_iter_t next_node = NULL;
	
	assert(NULL != list);
	
	assert(1 == DListIsEmpty(list));
    assert(0 == DListSize(list));    
    
	/* insert new node before end and check perv and next */
	node = DListInsert(list, DListEnd(list), &arr[0]);
	assert(DListEnd(list) == DListNext(DListBegin(list)));
	assert(node == DListBack(DListEnd(list)));
	assert(1 == DListSize(list));
	
	assert(0 == DListIsEmpty(list));
    assert(1 == DListSize(list));    
    
	/* insert new node after head and before node */
	new_node = DListInsert(list, DListBegin(list), &arr[1]);
	assert(new_node == DListBegin(list));
	assert(node == DListNext(new_node));
	
	/* insert new nodes after new_node and before node */
	for (i = 2; i < 5; ++i)
	{
		node = DListBack(node);
		next_node = DListInsert(list, node, &arr[i]);
		assert(next_node == DListBack(node));
		assert(node == DListNext(next_node));
	}
	
	assert(0 == DListIsEmpty(list));
    assert(5 == DListSize(list));    
    
	DListDestroy(list);
}

void DListRemoveTest()
{
	int arr[] = {1, 2, 3, 4, 5};
	size_t i = 0;
	
	dlist_t *list = DListCreate();
	dlist_iter_t node = NULL;
	dlist_iter_t next_node = NULL;

	assert(NULL != list);
	
	next_node = DListEnd(list); 
	
	for(i = 0; i < 5; ++i)
	{
		node = DListInsert(list, next_node, &arr[i]);	
		next_node = node;
	}
	
	assert(0 == DListIsEmpty(list));
    assert(5 == DListSize(list));    
    
    /* remove in the beginning*/
    node = DListRemove(DListBegin(list));
    assert(4 == DListSize(list));
     
    /* remove before end */
    node = DListRemove(DListBack(DListEnd(list)));
    assert(3 == DListSize(list));
    
    /* remove in the middle*/
    node = DListRemove(DListNext(DListBegin(list)));
    assert(2 == DListSize(list));

	DListDestroy(list);
}

void DListGetDataTest()
{
	int arr[] = {1, 2, 3, 4, 5};
	size_t i = 0;
	
	dlist_t *list = DListCreate();
	dlist_iter_t node = NULL;
	dlist_iter_t next_node = NULL;

	assert(NULL != list);
	
	next_node = DListBegin(list);
	
	for(i = 0; i < 5; ++i)
	{
		node = DListInsert(list, next_node, &arr[i]);	
		next_node = node;
	}
	
	for(i = 4, node = DListBegin(list); 
		node != DListEnd(list); 
		--i , node = DListNext(node))
	{
		assert(arr[i] == *(int *)DListIterGetData(node));
	}
	
	assert(5 == DListSize(list));
	
	DListDestroy(list);
} 

int AddOneSucc(void *data, void *param)
{
	UNUSED(data);
	
	++(*(size_t *)param);
		
	return 0;
}

int AddOneFail(void *data, void *param)
{
	UNUSED(data);
	
	++(*(size_t *)param);
	
	return 1;
}

int Cmp(void *data, const void *param)
{
	return !(*(int *)data == *(int *)param);
}

void DListForEachAndFindTest()
{
	int arr[] = {1, 2, 3, 4, 5};
	int func_status = 0;
	size_t counter = 0;
	
	dlist_t *list = DListCreate();
	dlist_iter_t node = NULL;
	dlist_iter_t next_node = NULL;

	assert(NULL != list);
	
	next_node = DListBegin(list);
	
	for(counter = 0; counter < 5; ++counter)
	{
		node = DListInsert(list, next_node, &arr[counter]);	
		next_node = node;
	}
	
	counter = 0;
	
	func_status = DListForEach(DListBegin(list), 
							   DListEnd(list),
							   AddOneSucc, &counter);
	assert(0 == func_status);
	assert(counter == DListSize(list));
	
	counter = 0;
	func_status = DListForEach(DListBegin(list), 
							   DListEnd(list), 
							   AddOneFail, &counter);
	assert(1 == func_status);
	assert(1 == counter); 
	
	assert(DListBack(DListEnd(list)) == 
		   DListFind(DListBegin(list), DListEnd(list), Cmp, &arr[0]));
		   
	assert(DListBegin(list) == 
		   DListFind(DListBegin(list), DListEnd(list), Cmp, &arr[4]));
		   
	DListDestroy(list);
}

void DlistPushAndPopTest()
{
	int arr[] = {1, 2, 3, 4, 5};
		
	dlist_t *list = DListCreate();

	assert(NULL != list);
	
	DListPushFront(list, &arr[2]);
	DListPushFront(list, &arr[1]);
	DListPushFront(list, &arr[0]);
	
	DListPushBack(list, &arr[3]);
	DListPushBack(list, &arr[4]);
	
	assert(arr[0] == *(int *)DListPopFront(list));
	assert(arr[1] == *(int *)DListPopFront(list));
	assert(arr[2] == *(int *)DListPopFront(list));
	assert(arr[4] == *(int *)DListPopBack(list));
	assert(arr[3] == *(int *)DListPopBack(list));

	DListDestroy(list);
}

void DListSpliceTest()
{
	int dest[] = {1, 2, 3, 8};
	int src[] = {4, 5, 6, 7};
	int i = 0;
	dlist_iter_t dest_iter = NULL;
	
	dlist_t *dest_list = DListCreate();
	dlist_t *src_list = DListCreate();
	
	assert(NULL != dest_list);
	assert(NULL != src_list);
	
	for (i = 0; i < 4; ++i)
	{
		DListPushBack(dest_list, &dest[i]);
		DListPushBack(src_list, &src[i]);
	}
	
	assert(4 == DListSize(dest_list));
	assert(4 == DListSize(src_list));
	
	dest_iter = DListBack(DListEnd(dest_list));
	
	assert(dest[3] == *(int *)DListIterGetData(dest_iter));
	 
	DListSplice(dest_iter, DListBegin(src_list), DListEnd(src_list));

	assert(8 == DListSize(dest_list));
	assert(0 == DListSize(src_list));

	assert(dest[0] == *(int *)DListPopFront(dest_list));
	assert(dest[1] == *(int *)DListPopFront(dest_list));
	assert(dest[2] == *(int *)DListPopFront(dest_list));
	assert(src[0] == *(int *)DListPopFront(dest_list));
	assert(src[1] == *(int *)DListPopFront(dest_list));	
	assert(src[2] == *(int *)DListPopFront(dest_list));
	assert(src[3] == *(int *)DListPopFront(dest_list));
	assert(dest[3] == *(int *)DListPopFront(dest_list));

	assert(0 == DListSize(dest_list));
		
	DListDestroy(dest_list);
	DListDestroy(src_list);
	
}


