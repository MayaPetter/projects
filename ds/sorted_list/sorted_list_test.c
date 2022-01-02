#include <assert.h>       /* assert */

#include "sorted_list.h"  /* sorted_list functions */

#define UNUSED(x) (void)(x)

void SortedListCreatSizeDestroyTest();
int IsBeforeFunc(void *data1, void *data2, const void *param);
void SortedListInsertTest();
void SortedListFindAndRemoveTest();
void SortedListPopTest();
void SortedListForEachTest();
void SortedListFindAndFindIfTest();
void SortedListMergeTest();
int ActionFuncSuccess(void *data, void *param);
int ActionFuncFail(void *data, void *param);
int IsMatch(void *data1, const void *data2, void *param);

int main()
{
	SortedListCreatSizeDestroyTest();
	SortedListInsertTest();
	SortedListFindAndRemoveTest();	
	SortedListPopTest();
	SortedListForEachTest();
	SortedListFindAndFindIfTest();
	SortedListMergeTest();
	
	return 0; 
}

int IsBeforeFunc(void *data1, void *data2, const void *param)
{
	UNUSED(param);
	
	return (*(int *)data1 < *(int *)data2); 
}

void SortedListCreatSizeDestroyTest()
{
	int i = 0;
	sorted_list_t *list = SortedListCreate(IsBeforeFunc, &i);
	assert(1 == SortedListIsEmpty(list));
	assert(0 == SortedListSize(list)); 
	SortedListDestroy(list);
}

void SortedListInsertTest()
{	
	int array[] = {1, 4, 6, 5, 2, 3};
	size_t i = 0;
	
	sorted_list_t *list = SortedListCreate(IsBeforeFunc, NULL);
	
	assert(NULL != list);
	
	assert(1 == SortedListIsEmpty(list));
	assert(0 == SortedListSize(list)); 
	assert(1 == SortedListIsSameIter(SortedListBegin(list),SortedListEnd(list)));
	
	for(  i = 0; i < 6; ++ i)
	{
		SortedListInsert(list, &array[i]);
	}	

	assert(0 == SortedListIsEmpty(list));
	assert(6 == SortedListSize(list)); 
	assert(0 == SortedListIsSameIter(SortedListBegin(list),SortedListEnd(list)));
	
	{
		sorted_list_iter_t iter = SortedListBegin(list);
		
		assert(array[0] == *(int *)SortedListGetData(iter));
		iter = SortedListNext(iter);
		assert(array[4] == *(int *)SortedListGetData(iter));
		iter = SortedListNext(iter);
		assert(array[5] == *(int *)SortedListGetData(iter));
		iter = SortedListNext(iter);
		assert(array[1] == *(int *)SortedListGetData(iter));
		iter = SortedListNext(iter);
		assert(array[3] == *(int *)SortedListGetData(iter));
		iter = SortedListNext(iter);
		assert(array[2] == *(int *)SortedListGetData(iter));
	}
	SortedListDestroy(list);
}

void SortedListFindAndRemoveTest()
{
	int array[] = {1, 4, 6, 5, 2, 3};
	size_t i = 0;
	
	sorted_list_iter_t iter = {NULL};
	sorted_list_t *list = SortedListCreate(IsBeforeFunc, NULL);
	
	assert(NULL != list);
	
	for (i = 0; i < 6; ++ i)
	{
		SortedListInsert(list, &array[i]);
	}	
	
	assert(6 == SortedListSize(list));
	
	iter = SortedListFind(list, SortedListBegin(list), SortedListEnd(list), 
	                                                                &array[1]);
	assert(array[1] == *(int *)SortedListGetData(iter));
	SortedListRemove(iter);

    iter = SortedListFind(list, SortedListBegin(list), SortedListEnd(list), 
                                                                    &array[2]);
	assert(array[2] == *(int *)SortedListGetData(iter));
	SortedListRemove(iter);
    
    assert(4 == SortedListSize(list));
    
    iter = SortedListFind(list, SortedListBegin(list), SortedListEnd(list), 
                                                                    &array[0]);
	assert(array[0] == *(int *)SortedListGetData(iter));
	SortedListRemove(iter);
	
	iter = SortedListFind(list, SortedListBegin(list), SortedListEnd(list), 
	                                                                &array[3]);
	assert(array[3] == *(int *)SortedListGetData(iter));
	SortedListRemove(iter);
	
	iter = SortedListFind(list, SortedListBegin(list), SortedListEnd(list), 
	                                                                &array[0]);
	assert(1 == SortedListIsSameIter(SortedListEnd(list), iter));
	
	iter = SortedListFind(list, SortedListBegin(list), SortedListEnd(list), 
	                                                                &array[4]);
	assert(array[4] == *(int *)SortedListGetData(iter));
	SortedListRemove(iter);
	
	iter = SortedListFind(list, SortedListBegin(list), SortedListEnd(list), 
	                                                                &array[5]);
	assert(array[5] == *(int *)SortedListGetData(iter));
	SortedListRemove(iter);

    assert(0 == SortedListSize(list));
	
	SortedListDestroy(list);
}

void SortedListPopTest()
{
    int array[] = {1, 4, 6, 5, 2, 3};
	size_t i = 0;
	
	sorted_list_t *list = SortedListCreate(IsBeforeFunc, NULL);
	
	assert(NULL != list);
	
	for (i = 0; i < 6; ++ i)
	{
		SortedListInsert(list, &array[i]);
	}	
	
	assert(6 == SortedListSize(list));
    
    assert(array[0] == *(int *)SortedListPopFront(list));
    assert(array[4] == *(int *)SortedListPopFront(list));
    
    assert(4 == SortedListSize(list));
    
    assert(array[2] == *(int *)SortedListPopBack(list));
    assert(array[3] == *(int *)SortedListPopBack(list));
    
    assert(2 == SortedListSize(list));
    
    assert(array[1] == *(int *)SortedListGetData(SortedListPrev(SortedListEnd(list))));
    assert(array[1] == *(int *)SortedListPopBack(list));
    assert(array[5] == *(int *)SortedListPopFront(list));
    
    assert(1 == SortedListIsEmpty(list));

    SortedListDestroy(list);
}

int ActionFuncSuccess(void *data, void *param)
{
    UNUSED(data);
     
	++(*(size_t *)param);
	
    return 0;
}

int ActionFuncFail(void *data, void *param)
{
    UNUSED(data);
     
	++(*(size_t *)param);
	
    return 1;
}

int IsMatch(void *data1, const void *data2, void *param)
{
	UNUSED(param);
    /* returns 1 if same and 0 if not */
	return (*(int *)data1 == *(int *)data2);
}

void SortedListForEachTest()
{
    int array[] = {1, 4, 6, 5, 2, 3};
	size_t counter = 0;
	int func_status = 2;
	
	sorted_list_t *list = SortedListCreate(IsBeforeFunc, NULL);
	
	assert(NULL != list);
	
	for (counter = 0; counter < 6; ++counter)
	{
		SortedListInsert(list, &array[counter]);
	}	
	
	assert(6 == SortedListSize(list));

    counter = 0;
    func_status = SortedListForEach(SortedListBegin(list), SortedListEnd(list), 
                  ActionFuncSuccess, &counter);

    assert(0 == func_status);
    assert(counter == SortedListSize(list));                  
    
    counter = 0;
    func_status = SortedListForEach(SortedListBegin(list), SortedListEnd(list), 
                  ActionFuncFail, &counter);

    assert(1 == func_status);
    assert(1 == counter);                  

    SortedListDestroy(list);
}

void SortedListFindAndFindIfTest()
{
    int array[] = {1, 4, 5, 6, 3, 2};
    int exist = 4, doesnt_exist = 10;
	size_t counter = 0;
	
    sorted_list_iter_t find_iter = {NULL};
    sorted_list_iter_t find_if_iter = {NULL};	
	sorted_list_t *list = SortedListCreate(IsBeforeFunc, NULL);
	
	assert(NULL != list);
	
	for (counter = 0; counter < 6; ++counter)
	{
		SortedListInsert(list, &array[counter]);
	}	
	
	assert(6 == SortedListSize(list));

    find_iter = SortedListFind(list, SortedListBegin(list), SortedListEnd(list), 
	                                                                &array[1]);
	find_if_iter = SortedListFindIf(SortedListBegin(list), SortedListEnd
	                             (list), IsMatch, &array[1], NULL);                                                                
	assert(array[1] == *(int *)SortedListGetData(find_iter));
	assert(array[1] == *(int *)SortedListGetData(find_if_iter));
	assert(1 == SortedListIsSameIter(find_if_iter, find_iter));
	
	find_iter = SortedListFind(list, SortedListBegin(list), SortedListEnd(list), 
	                                                                &array[0]);
	find_if_iter = SortedListFindIf(SortedListBegin(list), SortedListEnd
	                             (list), IsMatch, &array[0], NULL);                                                                
	assert(array[0] == *(int *)SortedListGetData(find_iter));
	assert(array[0] == *(int *)SortedListGetData(find_if_iter));
	assert(1 == SortedListIsSameIter(find_if_iter, find_iter));
	
	find_iter = SortedListFind(list, SortedListBegin(list), SortedListEnd(list), 
	                                                                &array[3]);
	find_if_iter = SortedListFindIf(SortedListBegin(list), SortedListEnd
	                             (list), IsMatch, &array[3], NULL);                                                                
	assert(array[3] == *(int *)SortedListGetData(find_iter));
	assert(array[3] == *(int *)SortedListGetData(find_if_iter));
	assert(1 == SortedListIsSameIter(find_if_iter, find_iter));
	
	find_iter = SortedListFind(list, SortedListBegin(list), SortedListEnd(list), 
	                                                                &array[5]);
	find_if_iter = SortedListFindIf(SortedListBegin(list), SortedListEnd
	                             (list), IsMatch, &array[5], NULL);                                                                
	assert(array[5] == *(int *)SortedListGetData(find_iter));
	assert(array[5] == *(int *)SortedListGetData(find_if_iter));
	assert(1 == SortedListIsSameIter(find_if_iter, find_iter));

    /* find and find if data doesn't exist*/
    find_iter = SortedListFind(list, SortedListBegin(list), SortedListEnd(list), 
	                                                                &exist);
	find_if_iter = SortedListFindIf(SortedListBegin(list), SortedListEnd
	                             (list), IsMatch, &exist, NULL);                                                                
	assert(exist == *(int *)SortedListGetData(find_iter));
	assert(exist == *(int *)SortedListGetData(find_if_iter));
	assert(1 == SortedListIsSameIter(find_if_iter, find_iter));

    find_if_iter = SortedListFindIf(SortedListBegin(list), SortedListEnd(list), 
							 		IsMatch, &doesnt_exist, NULL);
	assert(1 == SortedListIsSameIter(SortedListEnd(list), find_if_iter));
	
	find_iter = SortedListFind(list, SortedListBegin(list), SortedListEnd(list), 
	                                                             &doesnt_exist);
	assert(1 == SortedListIsSameIter(SortedListEnd(list), find_iter));
	
    SortedListDestroy(list);
}

void SortedListMergeTest()
{
    int dest_array[] = {1, 4, 8, 10, 2, 3};
    int src_array[] = {5, 6, 9, 7, 12, 11};
	size_t counter = 0;
	
	sorted_list_t *checker = NULL;
	sorted_list_t *dest_list = SortedListCreate(IsBeforeFunc, NULL);
	sorted_list_t *src_list = SortedListCreate(IsBeforeFunc, NULL);
	
	assert(NULL != dest_list);
	assert(NULL != src_list);
	
	/*merge 2 empty lists*/
	checker = SortedListMerge(dest_list,src_list);
	
	assert(checker == dest_list);
	assert(0 == SortedListSize(dest_list));
	assert(0 == SortedListSize(src_list));
	
	for (counter = 0; counter < 6; ++counter)
	{
		SortedListInsert(dest_list, &dest_array[counter]);
	}	
	
	assert(6 == SortedListSize(dest_list));
	
	/* merge 1 empty and 1 full*/
	checker = SortedListMerge(dest_list,src_list);
	
	assert(checker == dest_list);
	assert(6 == SortedListSize(dest_list));
	assert(0 == SortedListSize(src_list));
	
	for (counter = 0; counter < 6; ++counter)
	{
		SortedListInsert(src_list, &src_array[counter]);
	}	
	
	assert(6 == SortedListSize(src_list));
	
	/* merge 2 full*/
	checker = SortedListMerge(dest_list,src_list);
	
	assert(checker == dest_list);
	assert(12 == SortedListSize(dest_list));
	assert(0 == SortedListSize(src_list));

    {
		sorted_list_iter_t iter = SortedListBegin(dest_list);
		
		assert(dest_array[0] == *(int *)SortedListGetData(iter));
		iter = SortedListNext(iter);
		assert(dest_array[4] == *(int *)SortedListGetData(iter));
		iter = SortedListNext(iter);
		assert(dest_array[5] == *(int *)SortedListGetData(iter));
		iter = SortedListNext(iter);
		assert(dest_array[1] == *(int *)SortedListGetData(iter));
		iter = SortedListNext(iter);
		
		assert(src_array[0] == *(int *)SortedListGetData(iter));
		iter = SortedListNext(iter);
		assert(src_array[1] == *(int *)SortedListGetData(iter));
		iter = SortedListNext(iter);
		assert(src_array[3] == *(int *)SortedListGetData(iter));
		iter = SortedListNext(iter);
		
		assert(dest_array[2] == *(int *)SortedListGetData(iter));
		iter = SortedListNext(iter);
		
		assert(src_array[2] == *(int *)SortedListGetData(iter));
		iter = SortedListNext(iter);
		
		assert(dest_array[3] == *(int *)SortedListGetData(iter));
		iter = SortedListNext(iter);
		
		assert(src_array[5] == *(int *)SortedListGetData(iter));
		iter = SortedListNext(iter);
		assert(src_array[4] == *(int *)SortedListGetData(iter));
		iter = SortedListNext(iter);
	}

    SortedListDestroy(dest_list);
    SortedListDestroy(src_list);
}                               
