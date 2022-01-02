#include <stdio.h> /*printf*/
#include <assert.h> /* assert */

#include "heap.h"

#define UNUSED(x) ((void)x)

static void TestCreateDestroy();
static void TestPushPop();
static void TestRemove();
static void TestHeapSort();

static int CmpPriority(const void *data1, const void *data2, void *param);
static int IsMatch(const void *heap_data, const void *user_data);

int main()
{
	TestCreateDestroy();
	TestPushPop();
	TestRemove();
	TestHeapSort();
	
	return 0;
}

/******************************************************************************/

static void TestCreateDestroy()
{
	heap_t *new_heap = HeapCreate(CmpPriority, NULL);
	assert(NULL != new_heap);
	
	
	HeapDestroy(new_heap);	
}


static void TestPushPop()
{
	heap_t *heap = HeapCreate(CmpPriority, NULL);
	
	int data5 = 5, data10 = 10, data3 = 3;	
	
	HeapPush(heap, &data5);
	assert(1 == HeapSize(heap));
	assert(data5 == *(int *)HeapPeek(heap));
		
	HeapPush(heap, &data10);
	assert(2 == HeapSize(heap));
	assert(data10 ==  *(int *)HeapPeek(heap));
		
	HeapPush(heap, &data3);
	assert(3 == HeapSize(heap));
	assert(data10 == *(int *)HeapPeek(heap));
	
	HeapPop(heap);
	assert(2 == HeapSize(heap));
	assert(data5 == *(int *)HeapPeek(heap));
		
	HeapPop(heap);
	assert(1 == HeapSize(heap));
	assert(data3 == *(int *)HeapPeek(heap));
	
	HeapPop(heap);
	assert(0 == HeapSize(heap));
	assert(1 == HeapIsEmpty(heap));
	
	HeapDestroy(heap);
}

static void TestRemove()
{
	heap_t *heap = HeapCreate(CmpPriority, NULL);
	
	int data15 = 15, data110 = 110, data13 = 13, not_find = 4;	
	
	HeapPush(heap, &data15);
	assert(1 == HeapSize(heap));
	assert(data15 == *(int *)HeapPeek(heap));
		
	HeapPush(heap, &data110);
	assert(2 == HeapSize(heap));
	assert(data110 == *(int *)HeapPeek(heap));
		
	HeapPush(heap, &data13);
	assert(3 == HeapSize(heap));
	assert(data110 == *(int *)HeapPeek(heap));

	assert(data110 == *(int *)HeapRemove(heap, IsMatch, &data110));
	assert(data15 == *(int *)HeapPeek(heap)); 
	assert(2 == HeapSize(heap));
		
	/* remove not exict data */
	assert(NULL == 	HeapRemove(heap, IsMatch, &not_find));
	assert(data15 == *(int *)HeapPeek(heap));
	assert(2 == HeapSize(heap));
	
	assert(data15 == *(int *)HeapRemove(heap, IsMatch, &data15)); 
	assert(data13 == *(int *)HeapPeek(heap));
	
	HeapDestroy(heap);
}

static void TestHeapSort()
{	
	heap_t *heap = HeapCreate(CmpPriority, NULL);
	int arr_unsorted[] = {98,45, 7, 91, 150, 55, 75, 3, 9, 25};
	int arr_sorted[] = {150, 98, 91, 75, 55, 45, 25, 9, 7, 3};
	int new_arr[10] = {0};
	size_t arr_length = 10; 
	size_t i = 0;
	
	for (i = 0; i < arr_length; ++i)
	{
		assert(0 == HeapPush(heap, &arr_unsorted[i]));
		assert(i + 1 == HeapSize(heap));
	}	
	
	for (i = 0; i < arr_length; ++i)
	{
		new_arr[i] = *(int *)HeapPeek(heap);
		printf("%lu\n", i);
		assert(arr_sorted[i] == new_arr[i]);
		HeapPop(heap);
	}
	
	HeapDestroy(heap);
}

/******************************************************************************/
static int CmpPriority(const void *data1, const void *data2, void *param)
{
	UNUSED(param);
	
	return (*( int*)data2 - *(int *)data1);	
}

static int IsMatch(const void *heap_data, const void *user_data)
{
	return (*(int *)heap_data == *(int *)user_data);
}

