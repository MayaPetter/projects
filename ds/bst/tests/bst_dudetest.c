#include <stdio.h> /* printf */
#include <assert.h> /* assert */

#include "bst.h"

#define UNUSED(x) (void)(x) 


int Cmp(void *iter_data, void *new_data, void *param)
{
	UNUSED(param);
	
	return (*(int *)iter_data - *(int *)new_data);
}


void CreateDestroy_test()
{
	bst_t *new = BSTCreate(Cmp, NULL);
}

void Insert_test()
{
	int n1 = 1, n2 = 2, n3 = 3;

	bst_t *test = BSTCreate(Cmp, NULL);
	
	bst_iter_t first = NULL;
	bst_iter_t second = NULL;
	bst_iter_t third = NULL;
	
	assert(NULL != test);
	
	first = BSTInsert(test, &n2);   
	second = BSTInsert(test, &n1);     
	third = BSTInsert(test, &n3);
	
	assert(2 == *(int *)BSTGetData(first));
	assert(1 == *(int *)(BSTGetData(BSTBegin(test))));
}

void BeginEnd_test()
{
	int n0 = -2, n1 = 1, n2 = 2, n3 = 3;  /*FIXME add End Test */

	bst_t *test = BSTCreate(Cmp, NULL);
	
	bst_iter_t first = NULL;
	bst_iter_t second = NULL;
	bst_iter_t third = NULL;
	bst_iter_t fourth = NULL;

	assert(NULL != test);
	
	assert(NULL == (BSTGetData(BSTEnd(test))));
	
	first = BSTInsert(test, &n2);	   
	assert(first == BSTBegin(test));
	
	second = BSTInsert(test, &n1); 	  
	third = BSTInsert(test, &n3);
	assert(second == BSTBegin(test));
	
	fourth = BSTInsert(test, &n0);
	assert(fourth == BSTBegin(test));
	
	assert(NULL == (BSTGetData(BSTEnd(test))));
}

void NextPrev_test()
{
	int n1 = 1, n2 = 2, n3 = 3;  

	bst_t *test = BSTCreate(Cmp, NULL);
	
	bst_iter_t first = NULL;
	bst_iter_t second = NULL;
	bst_iter_t third = NULL;
	
	assert(NULL != test);
	
	first = BSTInsert(test, &n2);	  
	second = BSTInsert(test, &n1);     
	third = BSTInsert(test, &n3);
	
	assert(first == BSTNext(second));
	assert(first == BSTPrev(third));
	
	/* check if next after max is dummy */
	assert(BSTIsSameIter(BSTNext(third),(BSTEnd(test))));
	
	/* check if prev to dummy its max */
	assert(BSTIsSameIter(third, BSTPrev(BSTEnd(test))));
}

void Size_test()
{
	int n1 = 1, n2 = 2, n3 = 3, n4 = 4;  

	bst_t *test = BSTCreate(Cmp, NULL);
	
	bst_iter_t first = NULL;
	bst_iter_t second = NULL;
	bst_iter_t third = NULL;
	bst_iter_t fourth = NULL;
	
	assert(NULL != test);
	
	first = BSTInsert(test, &n2);   
	assert(1 == BSTSize(test)); 
	
	second = BSTInsert(test, &n1);     
	assert(2 == BSTSize(test));  
	
	third = BSTInsert(test, &n3);
	assert(3 == BSTSize(test));
	
	fourth = BSTInsert(test, &n4);
	assert(4 == BSTSize(test));	   
}



int main()
{
	Insert_test();
	BeginEnd_test();
	NextPrev_test();
	Size_test();
	
	printf("VOVA THE QUEEN\n");
	
	return 0;
}	

