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

#include "heap.h" /* heap API */

/******************************************************************************/

/****************
 * TOGGLE ZONE	*
 ****************/

#define CHECK_IS_EMPTY_BB
#define CHECK_SIZE_BB
#define CHECK_PUSH_BB
#define CHECK_POP_BB
#define CHECK_PEEK_BB
#define CHECK_REMOVE_BB

/******************************************************************************/

/************************
 * DEFINES AND TYPEDEFS	*
 ************************/

#define Expect(x) \
;{\
	if (!(x))\
	{\
		printf("test failed in line %d\n", __LINE__);\
		return 0;\
	}\
} /* end of Expect */

#define UNUSED(x) (void) (x)

/******************************************************************************/

/************************
 * FUNCTION DECLARATION	*
 ************************/

static int CheckIsEmptyBB();
static int CheckSizeBB();
static int CheckPushPeekPopBB();
static int CheckRemoveBB();

static int HelpingRemoveFound(size_t index);
static int HelpingRemoveNotFound();

static int CmpIntWhenParamIsBiggest(const void *data1, const void *data2, void *param);
static int IsNotEqualInt(const void *q_data, const void *user_data);

static heap_t *SetupHeap();

/******************************************************************************/

/************************
 * FUNCTION DEFINITION	*
 ************************/

int main()
{
	Expect(CheckIsEmptyBB());
	Expect(CheckSizeBB());
	Expect(CheckPushPeekPopBB());
	Expect(CheckRemoveBB());

	return 1;
}

static int CmpIntWhenParamIsBiggest(const void *data1, const void *data2, void *param)
{
	int *num1 = (int *)data1, *num2 = (int *)data2;

	assert(NULL != num1);
	assert(NULL != num2);

	if (((*num1 > *num2) && (*num2 != *(int *)param)) ||
		((NULL != param) &&(*num1 == *(int *)param) &&(*num2 != *(int *)param)))
	{
		return 23;
	}
	else if ((*num1 < *num2) || ((NULL != param) && (*num2 == *(int *)param) &&
													  (*num1 != *(int *)param)))
	{
		return -2;
	}
	else
	{
		return 0;
	}
}

static int IsNotEqualInt(const void *q_data, const void *user_data)
{
	assert(NULL != q_data);
	assert(NULL != user_data);

	return !(*(int *)q_data != *(int *)user_data);
}

static int CheckIsEmptyBB()
{
	#ifdef CHECK_IS_EMPTY_BB
	static int param = 10;
	heap_t *queue = HeapCreate(&CmpIntWhenParamIsBiggest, &param);

	assert(NULL != queue);

	Expect(HeapIsEmpty(queue));

	# ifdef CHECK_PUSH_BB
	{
		size_t i = 0;
		for (i = 0; i < 10; ++i)
		{
			Expect(HEAP_SUCCESS == HeapPush(queue, &i));
			Expect(!HeapIsEmpty(queue));
		}
	}
	#  ifdef CHECK_POP_BB
	{
		size_t i = 0;
		for (i = 0; i < 10; ++i)
		{
			Expect(!HeapIsEmpty(queue));
			HeapPop(queue);
		}
		Expect(HeapIsEmpty(queue));
	}
	#  endif /* CHECK_POP_BB */
	# endif /* CHECK_PUSH_BB */

	HeapDestroy(queue);

	#endif /* CHECK_IS_EMPTY_BB */
	return 1;
}

static int CheckSizeBB()
{
	#ifdef CHECK_SIZE_BB
	static int param = 10;
	heap_t *queue = HeapCreate(&CmpIntWhenParamIsBiggest, &param);

	assert(NULL != queue);

	Expect(0 == HeapSize(queue));

	# ifdef CHECK_PUSH_BB
	{
		size_t i = 0;
		for (i = 0; i < 10; ++i)
		{
			Expect(HEAP_SUCCESS == HeapPush(queue, &i));
			Expect((i + 1) == HeapSize(queue));
		}
	}
	#  ifdef CHECK_POP_BB
	{
		size_t i = 0;
		for (i = 0; i < 10; ++i)
		{
			Expect((10 - i) == HeapSize(queue));
			HeapPop(queue);
		}
		Expect(0 == HeapSize(queue));
	}
	#  endif /* CHECK_POP_BB */
	# endif /* CHECK_PUSH_BB */

	HeapDestroy(queue);

	#endif /* CHECK_SIZE_BB */
	return 1;
}

static int CheckPushPeekPopBB()
{
	#ifdef CHECK_PUSH_BB
	int arr[10] = {3,1,2,5,4,10,6,7,8,9};
	int expectedpush[10] = {3, 3, 3, 5, 4, 4, 4, 4, 4, 4};
	int expected[10] = {4,10,9,8,7,6,5,3,2,1};
	int param = {4};
	size_t i = 0;
	heap_t *queue = HeapCreate(&CmpIntWhenParamIsBiggest, &param);

	assert(NULL != queue);

	for (i = 0; i < (sizeof(arr) / sizeof(int)); ++i)
	{
		Expect(HEAP_SUCCESS == HeapPush(queue, &arr[i]));
		printf("%d == %d\n", expectedpush[i], *(int *)HeapPeek(queue));
		# ifdef CHECK_BB_PEEK
		Expect(expectedpush[i] == *(int *)HeapPeek(queue));
		# endif /* CHECK_PEEK_BB */
	}

	# ifdef CHECK_POP_BB
	for (i = 0; i < (sizeof(arr) / sizeof(int)); ++i)
	{
		#  ifdef CHECK_PEEK_BB
		Expect(expected[i] == *(int *)HeapPeek(queue));
		#  endif /* CHECK_PEEK_BB */
		HeapPop(queue);
	}
	# endif /* CHECK_POP_BB */

	HeapDestroy(queue);

	#endif /* CHECK_PUSH_BB */
	return 1;
}

static heap_t *SetupHeap()
{
	static int param = 4;
	static int arr[10] = {1,2,3,5,6,7,8,9,10,4};
	size_t i = 0;
	heap_t *queue = HeapCreate(&CmpIntWhenParamIsBiggest, &param);
	#ifdef CHECK_PUSH_BB

	assert(NULL != queue);

	for (i = 0; i < (sizeof(arr) / sizeof(int)); ++i)
	{
		Expect(HEAP_SUCCESS == HeapPush(queue, &arr[i]));
	}
	#endif /* CHECK_PUSH_BB */
	return queue;
}

static int CheckRemoveBB()
{
	#ifdef CHECK_REMOVE_BB
	Expect(HelpingRemoveFound(0));
	Expect(HelpingRemoveFound(9));
	Expect(HelpingRemoveFound(5));
	Expect(HelpingRemoveNotFound());

	#endif /* CHECK_CLEAR_BB */
	return 1;
}

static int HelpingRemoveFound(size_t index)
{
	#ifdef CHECK_REMOVE_BB
	heap_t *queue = SetupHeap();
	int contains[10] = {4,10,9,8,7,6,5,3,2,1};
	size_t i = 0;

	assert(NULL != queue);

	Expect(contains[index] == 
		   *(int *)HeapRemove(queue, &IsNotEqualInt, &contains[index]));

	for (i = 0; i < (sizeof(contains) / sizeof(int) - 1); ++i)
	{
		if (i < index)
		{
			Expect(contains[i] == *(int *)HeapPeek(queue));
		}
		else
		{
			Expect(contains[i + 1] == *(int *)HeapPeek(queue));
		}
		HeapPop(queue);
	}

	Expect(0 == HeapSize(queue));
	Expect(HeapIsEmpty(queue));

	HeapDestroy(queue);
	
	#endif /* CHECK_REMOVE_BB */
	return 1;
}

static int HelpingRemoveNotFound()
{
	#ifdef CHECK_REMOVE_BB
	heap_t *queue = SetupHeap();
	int contains[10] = {4,10,9,8,7,6,5,3,2,1};
	int out_of_queue = 80;
	size_t i = 0;

	assert(NULL != queue);

	Expect(NULL == HeapRemove(queue, &IsNotEqualInt, &out_of_queue));

	for (i = 0; i < (sizeof(contains) / sizeof(int)); ++i)
	{
		Expect(contains[i] == *(int *)HeapPeek(queue));
		HeapPop(queue);
	}

	Expect(0 == HeapSize(queue));
	Expect(HeapIsEmpty(queue));

	HeapDestroy(queue);
	
	#endif /* CHECK_CLEAR_BB */
	return 1;
}
