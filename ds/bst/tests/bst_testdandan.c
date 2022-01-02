/****************************
 * Author: Dandan Ogni		*
 * Reviewer: 				*
 * Revision: 				*
 * Status: 					*
 ****************************/

/******************************************************************************/

/****************
 * HEADER FILES	*
 ****************/

#include <stdio.h> /* printf */
#include <assert.h> /* assert */

#include "bst.h" /* bst API */

/******************************************************************************/

/****************
 * TOGGLE ZONE	*
 ****************/

#define CHECK_IS_EMPTY
#define CHECK_SIZE
#define CHECK_INSERT
#define CHECK_REMOVE
#define CHECK_NEXT
#define CHECK_PREV
#define CHECK_IS_SAME
#define CHECK_FIND
#define CHECK_FOR_EACH

/************************
 * END OF TOGGLE ZONE	*
 ************************/

/******************************************************************************/

/************
 * MACRO	*
 ************/

#define Expect(x) \
{\
	if (!(x))\
	{\
		printf("Test failed at line %d\n", __LINE__);\
		return 0;\
	}\
} /* end of Expect */

#define ExpectMain(x, str) \
{\
	if (!(x))\
	{\
		printf("Test failed for %s\n", str);\
	}\
} /* end of ExpectMain */

#define UNUSED(x) ((void)(x))

/******************************************************************************/

/************************
 * FUNCTION DECLARATION	*
 ************************/

static int CheckIsEmpty();
static int CheckSize();
static int CheckInsertNext();
static int CheckInsertPrev();
static int CheckIsSameIter();
static int CheckRemoveNext();
static int CheckFind();
static int CheckForEach();

static int CmpInt(void *iter_data, void *new_data, void *param);
static int CmpByParam(void *iter_data, void *new_data, void *param);

static int AddParamToData(void *data, void *param);
static int ReturnDataIfParamIsZeroElseSubParam(void *data, void *param);

/******************************************************************************/

/************************
 * FUNCTION DEFINITION	*
 ************************/

int main()
{
	ExpectMain(CheckIsEmpty(), "IsEmpty");
	ExpectMain(CheckSize(), "Size");
	ExpectMain(CheckInsertNext(), "Insert Next");
	ExpectMain(CheckInsertPrev(), "Insert Prev");
	ExpectMain(CheckIsSameIter(), "IsSameIter");
	ExpectMain(CheckRemoveNext(), "Remove Iteration");
	ExpectMain(CheckFind(), "Find");
	ExpectMain(CheckForEach(), "ForEach");

	return 0;
}

static int CmpInt(void *iter_data, void *new_data, void *param)
{
	UNUSED(param);
	return (*(int *)iter_data - *(int *)new_data);
}

static int CmpByParam(void *iter_data, void *new_data, void *param)
{
	int *arr = param;
	int *data = iter_data;
	int *compared = new_data;

	return (arr[*data] - arr[*compared]);
}

static int AddParamToData(void *data, void *param)
{
	*(int *)data += *(int *)param;

	return 0;
}

static int ReturnDataIfParamIsZeroElseSubParam(void *data, void *param)
{
	if (0 == *(int *)param)
	{
		return *(int *)data;
	}
	else
	{
		--*(int *)param;

		return 0;
	}
}

static int CheckIsEmpty()
{
	#ifdef CHECK_IS_EMPTY
	int arr1[] = {1, 5, 3, 2, 10, 8, 7, 4, 9, 6};
	bst_t *new_tree = BSTCreate(&CmpInt, NULL);
	size_t i = 0;

	Expect(NULL != new_tree);

	Expect(1 == BSTIsEmpty(new_tree));

	# ifdef CHECK_INSERT
	for (i = 0; i < 10; ++i)
	{
		Expect(NULL != BSTInsert(new_tree, &(arr1[i])));
		Expect(0 == BSTIsEmpty(new_tree));
	}

	#  ifdef CHECK_REMOVE
	for (i = 0; i < 10; ++i)
	{
		Expect(0 == BSTIsEmpty(new_tree));
		BSTRemove(BSTBegin(new_tree));
		
	}

	Expect(1 == BSTIsEmpty(new_tree));
	#  endif /* CHECK_REMOVE */
	# endif /* CHECK_INSERT */

	BSTDestroy(new_tree);
	#endif /* CHECK_IS_EMPTY */

	return 1;
}

static int CheckSize()
{
	#ifdef CHECK_SIZE
	int arr1[] = {1, 5, 3, 2, 10, 8, 7, 4, 9, 6};
	bst_t *new_tree = BSTCreate(&CmpInt, NULL);
	size_t i = 0;

	Expect(NULL != new_tree);

	Expect(0 == BSTSize(new_tree));

	# ifdef CHECK_INSERT
	for (i = 0; i < 10; ++i)
	{
		Expect(i == BSTSize(new_tree));
		Expect(NULL != BSTInsert(new_tree, &(arr1[i])));
	}

	#  ifdef CHECK_REMOVE
	for (i = 0; i < 10; ++i)
	{
		Expect((10 - i) == BSTSize(new_tree));
		BSTRemove(BSTBegin(new_tree));
	}

	Expect(0 == BSTSize(new_tree));
	#  endif /* CHECK_REMOVE */
	# endif /* CHECK_INSERT */

	BSTDestroy(new_tree);
	#endif /* CHECK_SIZE */

	return 1;
}

static int CheckInsertNext()
{
	#ifdef CHECK_INSERT
	# ifdef CHECK_NEXT
	int arr1[] = {1, 5, 3, 2, 10, 8, 7, 4, 9, 6};
	bst_t *new_tree = BSTCreate(&CmpInt, NULL);
	bst_iter_t iter = NULL;
	size_t i = 0;

	Expect(NULL != new_tree);

	for (i = 0; i < 10; ++i)
	{
		Expect(NULL != BSTInsert(new_tree, &(arr1[i])));
	}

	iter = BSTBegin(new_tree);
	for (i = 0; i < 10; ++i)
	{
		Expect(((int)(i + 1)) == *(int *)BSTGetData(iter))
		iter = BSTNext(iter);
	}

	#  ifdef CHECK_IS_SAME
	Expect(BSTIsSameIter(iter, BSTEnd(new_tree)));
	#  endif /* CHECK_IS_SAME */

	BSTDestroy(new_tree);
	# endif /* CHECK_NEXT */
	#endif /* CHECK_INSERT */

	return 1;
}

static int CheckInsertPrev()
{
	#ifdef CHECK_INSERT
	# ifdef CHECK_PREV
	int arr1[] = {1, 5, 3, 2, 10, 8, 7, 4, 9, 6};
	bst_t *new_tree = BSTCreate(&CmpInt, NULL);
	bst_iter_t iter = NULL;
	size_t i = 0;

	Expect(NULL != new_tree);

	for (i = 0; i < 10; ++i)
	{
		Expect(NULL != BSTInsert(new_tree, &(arr1[i])));
	}

	iter = BSTEnd(new_tree);
	for (i = 0; i < 10; ++i)
	{
		iter = BSTPrev(iter);
		Expect(((int)(10 - i)) == *(int *)BSTGetData(iter))
	}

	BSTDestroy(new_tree);
	# endif /* CHECK_PREV */
	#endif /* CHECK_INSERT */

	return 1;
}

static int CheckIsSameIter()
{
	#ifdef CHECK_IS_SAME
	int arr1[] = {1, 5};
	bst_t *new_tree = BSTCreate(&CmpInt, NULL);
	size_t i = 0;

	Expect(NULL != new_tree);

	Expect(BSTIsSameIter(BSTEnd(new_tree), BSTEnd(new_tree)));
	Expect(BSTIsSameIter(BSTEnd(new_tree), BSTBegin(new_tree)));

	# ifdef CHECK_INSERT
	for (i = 0; i < 2; ++i)
	{
		Expect(NULL != BSTInsert(new_tree, &(arr1[i])));
	}

	Expect(BSTIsSameIter(BSTEnd(new_tree), BSTEnd(new_tree)));
	Expect(!BSTIsSameIter(BSTBegin(new_tree), BSTEnd(new_tree)));
	Expect(BSTIsSameIter(BSTBegin(new_tree), BSTBegin(new_tree)));
	#  ifdef CHECK_NEXT
	Expect(!BSTIsSameIter(BSTBegin(new_tree), BSTNext(BSTBegin(new_tree))));
	#  endif /* CHECK_NEXT */

	# endif /* CHECK_INSERT */
	BSTDestroy(new_tree);
	#endif /* CHECK_IS_SAME */

	return 1;
}

static int CheckRemoveNext()
{
	#ifdef CHECK_INSERT
	# ifdef CHECK_REMOVE
	#  ifdef CHECK_NEXT
	int arr1[] = {1, 5, 3, 2, 10, 8, 7, 4, 9, 6};
	bst_t *new_tree = BSTCreate(&CmpInt, NULL);
	size_t i = 0;

	Expect(NULL != new_tree);

	for (i = 0; i < 10; ++i)
	{
		Expect(NULL != BSTInsert(new_tree, &(arr1[i])));
	}

	BSTRemove((BSTNext(BSTBegin(new_tree))));
	Expect(3 == *(int *)BSTGetData(BSTNext(BSTBegin(new_tree))));

	BSTRemove((BSTNext(BSTBegin(new_tree))));
	Expect(4 == *(int *)BSTGetData(BSTNext(BSTBegin(new_tree))));

	{
		bst_iter_t iter = BSTBegin(new_tree);
		for (i = 0; i < 4; ++i)
		{
			iter = BSTNext(iter);
		}

		Expect(8 == *(int *)BSTGetData(BSTNext(iter)));
		BSTRemove(BSTNext(iter));
		Expect(9 == *(int *)BSTGetData(BSTNext(iter)));
	}

	BSTRemove(BSTBegin(new_tree));
	Expect(4 == *(int *)BSTGetData(BSTBegin(new_tree)));

	BSTDestroy(new_tree);
	#  endif /* CHECK_NEXT */
	# endif /* CHECK_REMOVE */
	#endif /* CHECK_INSERT */

	return 1;
}

static int CheckFind()
{
	#ifdef CHECK_INSERT
	# ifdef CHECK_FIND
	int arr1[] = {1, 5, 3, 2, 10, 8, 7, 4, 9, 6};
	int param[] = {1, 2, 4, 5, 3, 9, 10, 8, 7, 6, 0, 11, 12, 13, 14, 15, 16};
	bst_t *new_tree = BSTCreate(&CmpByParam, param);
	int i = 0;

	Expect(NULL != new_tree);

	for (i = 0; i < 10; ++i)
	{
		Expect(NULL != BSTInsert(new_tree, &(arr1[i])));
	}

	for (i = 1; i < 11; ++i)
	{
		Expect((int)i == *(int *)BSTGetData(BSTFind(new_tree, &i)));
	}

	Expect(NULL == BSTFind(new_tree, &i));

	#  ifdef CHECK_REMOVE
	{
		int to_remove = 8;
		BSTRemove(BSTFind(new_tree, &to_remove));
		Expect(NULL == BSTFind(new_tree, &to_remove));
	
		for (i = 1; i < 11; ++i)
		{
			if (8 == i)
			{
				++i;
			}
			Expect((int)i == *(int *)BSTGetData(BSTFind(new_tree, &i)));
		}
	}
	#  endif /* CHECK_REMOVE */

	BSTDestroy(new_tree);
	# endif /* CHECK_FIND */
	#endif /* CHECK_INSERT */

	return 1;
}

static int CheckForEach()
{
	#ifdef CHECK_FOR_EACH
	# ifdef CHECK_INSERT
	#  ifdef CHECK_NEXT
	int arr1[] = {1, 5, 3, 2, 10, 8, 7, 4, 9, 6};
	bst_t *new_tree = BSTCreate(&CmpInt, NULL);
	size_t i = 0;
	int param = 0;

	Expect(NULL != new_tree);

	for (i = 0; i < 10; ++i)
	{
		Expect(NULL != BSTInsert(new_tree, &(arr1[i])));
	}

	{
		param = 3;
		Expect(5 == BSTForEach( &param,
								BSTNext(BSTBegin(new_tree)),
								BSTEnd(new_tree),
								&ReturnDataIfParamIsZeroElseSubParam));
		Expect(0 == param);
	}
	{
		param = 8;
		Expect(0 == BSTForEach( &param,
								BSTNext(BSTBegin(new_tree)),
								BSTPrev(BSTEnd(new_tree)),
								&ReturnDataIfParamIsZeroElseSubParam));
		Expect(0 == param);
	}

	{
		bst_iter_t iter = BSTBegin(new_tree);
		param = 100;
		Expect(0 == BSTForEach( &param,
								BSTBegin(new_tree),
								BSTEnd(new_tree),
								&AddParamToData));

		for (i = 1; i < 11; ++i)
		{
			Expect((int)i + param == *(int *)BSTGetData(iter));
			iter = BSTNext(iter);
		}
	}

	BSTDestroy(new_tree);
	#  endif /* CHECK_NEXT */
	# endif /* CHECK_INSERT */
	#endif /* CHECK_FOR_EACH */

	return 1;
}
