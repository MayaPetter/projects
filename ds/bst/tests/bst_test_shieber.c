#include <stdio.h>
#include <assert.h>

#include "bst.h"

#define UNUSED(x) (void)(x)
#define START_RED printf("\033[1;31m");
#define END_RED printf("\033[0m");
#define TRUE 1
#define FALSE 0

#define PRINT_ERROR START_RED printf("ERROR in %s.\n", func_name); END_RED

#define PREVENT_WARNINGS_FROM_UNUSED_FUNCS_FROM_TESTS_TEMPLATE \
{\
	int a = 1;\
	expect_int(1,1,"hi");\
	expect_size_t(1lu,1lu,"hi");\
	expect_NULL(NULL,"hi");\
	expect_Not_NULL(&a,"hi");\
}

#define PRINT_ERROR START_RED printf("ERROR in %s.\n", func_name); END_RED

/*************************************************************************
																		 *
				      GENERAL TEST FUNCTIONS							 *
																		 *
*************************************************************************/
void expect_int(int result, int expected_result, char *func_name)
{
	if (result != expected_result)
	{
		PRINT_ERROR
		printf("Expected: \t%d\nActual: \t%d\n", expected_result, result);
	}	
}

void expect_size_t(size_t result, size_t expected_result, char *func_name)
{
	if (result != expected_result)
	{
		PRINT_ERROR
		printf("Expected: \t%lu\nActual: \t%lu\n", expected_result, result);
	}	
}

void expect_NULL(void *pointer, char *func_name)
{
	if (pointer != NULL)
	{
		PRINT_ERROR
		printf("pointer isn't NULL.\n");
	}	
}

void expect_Not_NULL(void *pointer, char *func_name)
{
	if (pointer == NULL)
	{
		PRINT_ERROR
		printf("pointer isn't NULL.\n");
	}	
}
/*************************************************************************
																		 *
				      FORWARD DECLARATIONS								 *
																		 *
*************************************************************************/
int Int_Node_Compare(void *iter_data, void *new_data, void *param);

void BSTCreate_test();

void BSTInsertMiddleSmallerBigger_test();
void BSTInsertSmallSmallSmall_test();
void BSTInsertBigBigBig_test();

void BSTFind_element_is_in_tree_test();
void BSTFind_element_is_NOT_in_tree_test();

void BSTRemove_RemoveLeaf_test();
void BSTRemove_RemoveNodeWithOneLeftChild_test();
void BSTRemove_RemoveNodeWithOneRightChild_test();
void BSTRemove_RemoveNodeWithTwoChildren_test();
void BSTRemove_RemoveRootOnBigTree_test();
void BSTRemove_RemoveSeveralNodes_test();

int main()
{
	PREVENT_WARNINGS_FROM_UNUSED_FUNCS_FROM_TESTS_TEMPLATE
	
	BSTCreate_test();
	
	BSTInsertMiddleSmallerBigger_test();		
	
	BSTInsertSmallSmallSmall_test();	
	
	BSTInsertBigBigBig_test();
	
	BSTFind_element_is_in_tree_test();
	
	BSTFind_element_is_NOT_in_tree_test();
	
	BSTRemove_RemoveLeaf_test();
	
	BSTRemove_RemoveNodeWithOneLeftChild_test();
	
	BSTRemove_RemoveNodeWithOneRightChild_test();

	BSTRemove_RemoveNodeWithTwoChildren_test();
	
	BSTRemove_RemoveRootOnBigTree_test();
	
	BSTRemove_RemoveSeveralNodes_test();
	
	return 0;
}

void BSTRemove_RemoveSeveralNodes_test()
{
	bst_t *bst = BSTCreate(Int_Node_Compare, NULL);	
	
	int a_root = 15;
	int b_two_kids_not_root = 97;
	int c_leaf_right_child = 301;
	int d_one_left_child = 14;
	int e_left_leaf = 11;
	int f_left_leaf = 50;
	int g = 67, h= 70, i =19 , j = 22, k = 33, l = 44, m = 55, n = 66;
	bst_iter_t iter_to_remove1 = NULL, iter_to_remove2 = NULL
	,iter_to_remove3 = NULL, iter_to_remove4 = NULL, iter_to_remove5 = NULL;
	
	BSTInsert(bst, &a_root);
	iter_to_remove1 = BSTInsert(bst, &b_two_kids_not_root); 
	iter_to_remove2 = BSTInsert(bst, &c_leaf_right_child); 
	iter_to_remove3 = BSTInsert(bst, &d_one_left_child);  
	iter_to_remove4 = BSTInsert(bst, &e_left_leaf);
	iter_to_remove5 = BSTInsert(bst, &f_left_leaf);									
	BSTInsert(bst, &g);
	BSTInsert(bst, &h);
	BSTInsert(bst, &i);
	BSTInsert(bst, &j);
	BSTInsert(bst, &k);
	BSTInsert(bst, &l);
	BSTInsert(bst, &m);
	BSTInsert(bst, &n);
		
	
	expect_size_t(BSTSize(bst), 14, "BSTRemove_RemoveRootOnBigTree_test1");
	
	BSTRemove(iter_to_remove1);
	
	expect_size_t(BSTSize(bst), 13, "BSTRemove_RemoveRootOnBigTree_test2");	
	
	BSTRemove(iter_to_remove2);
	
	expect_size_t(BSTSize(bst), 12, "BSTRemove_RemoveRootOnBigTree_test3");	
	
	BSTRemove(iter_to_remove3);
	
	expect_size_t(BSTSize(bst), 11, "BSTRemove_RemoveRootOnBigTree_test4");	
	
	BSTRemove(iter_to_remove4);
	
	expect_size_t(BSTSize(bst), 10, "BSTRemove_RemoveRootOnBigTree_test5");	
	
	BSTRemove(iter_to_remove5);
	
	expect_size_t(BSTSize(bst), 9, "BSTRemove_RemoveRootOnBigTree_test6");	
	
	BSTDestroy(bst);
}

void BSTRemove_RemoveRootOnBigTree_test()
{
	bst_t *bst = BSTCreate(Int_Node_Compare, NULL);	
	
	int a_root = 15;
	int b_two_kids_not_root = 97;
	int c_leaf_right_child = 301;
	int d_one_left_child = 14;
	int e_left_leaf = 11;
	int f_left_leaf = 50;
	int g = 67, h= 70, i =19 , j = 22, k = 33, l = 44, m = 55, n = 66;
	bst_iter_t iter_to_remove = NULL;
	
	iter_to_remove = BSTInsert(bst, &a_root);
	BSTInsert(bst, &b_two_kids_not_root); 
	BSTInsert(bst, &c_leaf_right_child); 
	BSTInsert(bst, &d_one_left_child);  
	BSTInsert(bst, &e_left_leaf);
	BSTInsert(bst, &f_left_leaf);									
	BSTInsert(bst, &g);
	BSTInsert(bst, &h);
	BSTInsert(bst, &i);
	BSTInsert(bst, &j);
	BSTInsert(bst, &k);
	BSTInsert(bst, &l);
	BSTInsert(bst, &m);
	BSTInsert(bst, &n);
		
	
	expect_size_t(BSTSize(bst), 14, "BSTRemove_RemoveRootOnBigTree_test1");
	
	BSTRemove(iter_to_remove);
	
	expect_size_t(BSTSize(bst), 13, "BSTRemove_RemoveRootOnBigTree_test2");	
	
	BSTDestroy(bst);			
}

void BSTRemove_RemoveNodeWithTwoChildren_test()
{
	bst_t *bst = BSTCreate(Int_Node_Compare, NULL);	
	
	int a_root = 15;
	int b_two_kids_not_root = 97;
	int c_leaf_right_child = 301;
	int d_one_left_child = 14;
	int e_left_leaf = 11;
	int f_left_leaf = 50;
	bst_iter_t iter_to_remove = NULL;
	
	BSTInsert(bst, &a_root);
	iter_to_remove = BSTInsert(bst, &b_two_kids_not_root); 
	BSTInsert(bst, &c_leaf_right_child); 
	BSTInsert(bst, &d_one_left_child);  
	BSTInsert(bst, &e_left_leaf);
	BSTInsert(bst, &f_left_leaf);									
	
	expect_size_t(BSTSize(bst), 6, "BSTRemove_test1");
	
	BSTRemove(iter_to_remove);
	
	expect_size_t(BSTSize(bst), 5, "BSTRemove_test2");	
	
	BSTDestroy(bst);			
}

void BSTRemove_RemoveNodeWithOneRightChild_test()
{
	bst_t *bst = BSTCreate(Int_Node_Compare, NULL);	
	
	int a_root = 15;
	int b_one_right_child = 97;
	int c_leaf_right_child = 301;
	int d_one_left_child = 14;
	int e_left_leaf = 11;
	bst_iter_t iter_to_remove = NULL;
	
	BSTInsert(bst, &a_root);
	
	iter_to_remove = BSTInsert(bst, &b_one_right_child); 
	BSTInsert(bst, &c_leaf_right_child); 
	BSTInsert(bst, &d_one_left_child);  
	BSTInsert(bst, &e_left_leaf);				
	
	expect_size_t(BSTSize(bst), 5, "BSTRemove_test1");
	
	BSTRemove(iter_to_remove);
	
	expect_size_t(BSTSize(bst), 4, "BSTRemove_test2");	
	
	BSTDestroy(bst);			
}

void BSTRemove_RemoveNodeWithOneLeftChild_test()
{
	bst_t *bst = BSTCreate(Int_Node_Compare, NULL);	
	
	int a_root = 15;
	int b_one_right_child = 97;
	int c_leaf_right_child = 301;
	int d_one_left_child = 14;
	int e_left_leaf = 11;
	bst_iter_t iter_to_remove = NULL;
	
	BSTInsert(bst, &a_root);
	
	BSTInsert(bst, &b_one_right_child); 
	BSTInsert(bst, &c_leaf_right_child); 
	iter_to_remove = BSTInsert(bst, &d_one_left_child);  
	BSTInsert(bst, &e_left_leaf);				
	
	expect_size_t(BSTSize(bst), 5, "BSTRemove_test1");
	
	BSTRemove(iter_to_remove);
	
	expect_size_t(BSTSize(bst), 4, "BSTRemove_test2");	
	
	BSTDestroy(bst);			
}

void BSTRemove_RemoveLeaf_test()
{
	bst_t *bst = BSTCreate(Int_Node_Compare, NULL);	
	
	int a_root = 15;
	int b_one_right_child = 97;
	int c_leaf_right_child = 301;
	int d_one_left_child = 14;
	int e_left_leaf = 11;
	bst_iter_t iter_to_remove = NULL;
	
	BSTInsert(bst, &a_root);
	
	BSTInsert(bst, &b_one_right_child);
	BSTInsert(bst, &c_leaf_right_child);
	BSTInsert(bst, &d_one_left_child);		
	iter_to_remove = BSTInsert(bst, &e_left_leaf);				
	
	expect_size_t(BSTSize(bst), 5, "BSTRemove_test1");
	
	BSTRemove(iter_to_remove);
	
	expect_size_t(BSTSize(bst), 4, "BSTRemove_test2");	
	
	BSTDestroy(bst);			
}

void BSTFind_element_is_in_tree_test()
{
	bst_t *bst = BSTCreate(Int_Node_Compare, NULL);	
	
	int a = 15;
	int b = 97;
	int c = 301;
	int unadded = 60;
	
	BSTInsert(bst, &a);
	
	BSTInsert(bst, &b);
	
	BSTInsert(bst, &c);

	expect_NULL(BSTFind(bst,&unadded), "BSTFind_element_is_in_tree_test1");
	
	BSTDestroy(bst);		
}

void BSTFind_element_is_NOT_in_tree_test()
{
bst_t *bst = BSTCreate(Int_Node_Compare, NULL);
	
	int a = 15;
	int b = 97;
	int c = 301;
	int g = 13;
	
	BSTInsert(bst, &a);
	
	BSTInsert(bst, &b);
	
	BSTInsert(bst, &c);

	expect_Not_NULL(BSTFind(bst,&b), "BSTFind_element_is_NOT_in_tree_test1");

	expect_Not_NULL(BSTFind(bst,&a), "BSTFind_element_is_NOT_in_tree_test2");
	
	expect_Not_NULL(BSTFind(bst,&c), "BSTFind_element_is_NOT_in_tree_test3");
	
	g = *(int *)BSTGetData(BSTFind(bst,&b));
	
	expect_int(g, b, "BSTFind_element_is_NOT_in_tree_test4");
	
	BSTDestroy(bst);	
}

void BSTInsertBigBigBig_test()
{
	bst_t *bst = BSTCreate(Int_Node_Compare, NULL);
	
	int a = 15;
	int b = 97;
	int c = 301;
	
	expect_int(BSTIsEmpty(bst), TRUE, "BSTInsert_test1");
	
	BSTInsert(bst, &a);
	
	expect_int(BSTIsEmpty(bst), FALSE, "BSTInsert_test2");	
	
	BSTInsert(bst, &b);
	
	expect_size_t(BSTSize(bst), 2, "BSTInsert_test3");
	
	BSTInsert(bst, &c);	
	
	expect_size_t(BSTSize(bst), 3, "BSTInsert_test3");
	
	BSTDestroy(bst);		
}

void BSTInsertSmallSmallSmall_test()
{
	bst_t *bst = BSTCreate(Int_Node_Compare, NULL);
	
	int a = 3;
	int b = 2;
	int c = 1;
	
	expect_int(BSTIsEmpty(bst), TRUE, "BSTInsert_test1");
	
	BSTInsert(bst, &a);
	
	expect_int(BSTIsEmpty(bst), FALSE, "BSTInsert_test2");	
	
	BSTInsert(bst, &b);
	
	expect_size_t(BSTSize(bst), 2, "BSTInsert_test3");
	
	BSTInsert(bst, &c);	
	
	expect_size_t(BSTSize(bst), 3, "BSTInsert_test3");	
	
	BSTDestroy(bst);		
}

void BSTInsertMiddleSmallerBigger_test()
{
	bst_t *bst = BSTCreate(Int_Node_Compare, NULL);
	
	int a = 5;
	int b = 4;
	int c = 7;
	
	expect_int(BSTIsEmpty(bst), TRUE, "BSTInsert_test1");
	
	BSTInsert(bst, &a);
	
	expect_int(BSTIsEmpty(bst), FALSE, "BSTInsert_test2");	
	
	BSTInsert(bst, &b);
	
	expect_size_t(BSTSize(bst), 2, "BSTInsert_test3");
	
	BSTInsert(bst, &c);	
	
	expect_size_t(BSTSize(bst), 3, "BSTInsert_test3");	
	
	BSTDestroy(bst);	

}

void BSTCreate_test()
{
	bst_t *new_bst = BSTCreate(Int_Node_Compare, NULL);
	
	BSTDestroy(new_bst);
}

int Int_Node_Compare(void *iter_data, void *new_data, void *param)
{
	int result = 0;
	int iter_int_data = *(int *)iter_data;
	int new_int_data = *(int *)new_data;	
	UNUSED(param);

	assert(NULL != iter_data);
	assert(NULL != new_data);
	
	if (iter_int_data > new_int_data)
	{
		result = 1;
	}

	else if (iter_int_data < new_int_data)
	{
		result = -1;
	}
	else /* iter_int_data == new_int_data */
	{
		result = 0;
	}
	
	return result;
}
