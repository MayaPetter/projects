#include <stdio.h> /* printf */
#include <stdlib.h> /* rand, qsort */
#include <string.h> /* memcpy */
#include <assert.h> /* assert */
#include <limits.h> /* INT_MIN, INT_MAX */
#include "sorting.h"

#define UNUSED(x) ((void)x)
#define ARR_SIZE(x) (sizeof(x) / sizeof(x[0]))

/* Secondary functions */
static int RandInRange(int max);
static int FindMax(int *arr, size_t size);
static int FindMin(int *arr, size_t size);
static void FillArr(int *arr, size_t size);
static void CopyAndSortArr(int *dest, int *src, size_t size);
static void CheckSort(int *my_sort, int *qsort, size_t size);
static int cmpfunc(const void *a, const void *b);
/* static void PrintArr(int *arr, size_t size);
 */

/* Test functions */
static void Insertion_test();
static void Selection_test();
static void Bubble_test();
static void CountingSort_test();
static void CountingSort2_test();
static void Radix_test();
static void Merge_test();

int main()
{
	Insertion_test();
	Selection_test();
	Bubble_test();
	CountingSort_test();
	CountingSort2_test();
	Radix_test();
	Merge_test();
	return 0;
}

/* MERGE **********************************************************************/
static int IsBefore(int a, int b)
{
	return a < b;
}

static void Merge_test()
{
	int sorting_test_arr[15] = {0};
	int qsorted[15] = {0};
	size_t size = ARR_SIZE(sorting_test_arr);
	
	FillArr(sorting_test_arr, size);
	CopyAndSortArr(qsorted, sorting_test_arr, size);
	Merge(sorting_test_arr, size, IsBefore);
	
	CheckSort(sorting_test_arr, qsorted, size);
}

/******************************************************************************/
static void Radix_test()
{
	int sorting_test_arr[15] = {0};
	int qsorted[15] = {0};
	size_t size = ARR_SIZE(sorting_test_arr);
	
	FillArr(sorting_test_arr, size);
	CopyAndSortArr(qsorted, sorting_test_arr, size);
	Radix(sorting_test_arr, size, 2);
	
	CheckSort(sorting_test_arr, qsorted, size);
}

static void Insertion_test()
{
	int sorting_test_arr[15] = {0};
	int qsorted[15] = {0};
	size_t size = ARR_SIZE(sorting_test_arr);
	
	FillArr(sorting_test_arr, size);
	CopyAndSortArr(qsorted, sorting_test_arr, size);
	Insertion(sorting_test_arr, size);
	
	CheckSort(sorting_test_arr, qsorted, size);
}

static void Selection_test()
{
	int sorting_test_arr[15] = {0};
	int qsorted[15] = {0};
	size_t size = ARR_SIZE(sorting_test_arr);
	
	FillArr(sorting_test_arr, size);
	CopyAndSortArr(qsorted, sorting_test_arr, size);
	Selection(sorting_test_arr, size);
	
	CheckSort(sorting_test_arr, qsorted, size);
}

static void Bubble_test()
{
	int sorting_test_arr[15] = {0};
	int qsorted[15] = {0};
	size_t size = ARR_SIZE(sorting_test_arr);
	
	FillArr(sorting_test_arr, size);
	CopyAndSortArr(qsorted, sorting_test_arr, size);
	Bubble(sorting_test_arr, size);
	
	CheckSort(sorting_test_arr, qsorted, size);
}

static void CountingSort_test()
{
	int sorting_test_arr[12] = {0};
	int qsorted[12] = {0};
	size_t size = ARR_SIZE(sorting_test_arr);
	
	FillArr(sorting_test_arr, size);
	CopyAndSortArr(qsorted, sorting_test_arr, size);
	Counting(sorting_test_arr, size, FindMin(sorting_test_arr, size), 
									 FindMax(sorting_test_arr, size));
	
	CheckSort(sorting_test_arr, qsorted, size);
}

static void CountingSort2_test()
{
	int sorting_test_arr[12] = {1, 5, 0, -2, -9, 0, 3, -1, 7, -9, 17, -20};
	int qsorted[12] = {0};
	size_t size = ARR_SIZE(sorting_test_arr);
	
	CopyAndSortArr(qsorted, sorting_test_arr, size);
	
	Counting(sorting_test_arr, size, -20, 17);
	
	CheckSort(sorting_test_arr, qsorted, size);
}

static int RandInRange(int max)
{
	return rand() % max;
}

static int FindMax(int *arr, size_t size)
{
	int max = INT_MIN;
	while (size--)
	{
		if (arr[size] > max)
		{
			max = arr[size];
		}
	}
	
	return max;
}

static int FindMin(int *arr, size_t size)
{
	int min = INT_MAX;
	while (size--)
	{
		if (arr[size] < min)
		{
			min = arr[size];
		}
	}
	
	return min;
}

static void FillArr(int *arr, size_t size)
{
	size_t i = 0;
	
	assert(NULL != arr);
	
	for (i = 0; i < size; ++i)
	{
		arr[i] = RandInRange(20);
	}
}

static void CopyAndSortArr(int *dest, int *src, size_t size)
{
	assert(NULL != dest);
	assert(NULL != src);
	
	memcpy(dest, src, (size * sizeof(int)));
	qsort(dest, size, sizeof(int), cmpfunc);
}

static void CheckSort(int *my_sort, int *qsort, size_t size)
{
	size_t i = 0;
	for (i = 0; i < size; ++i)
	{
		assert(my_sort[i] == qsort[i]);
	}
	UNUSED(my_sort);
	UNUSED(qsort);
}

static int cmpfunc(const void *a, const void *b)
{
	assert(NULL != a);
	assert(NULL != b);
	return (*(int *)a - *(int *)b);
}

/* static void PrintArr(int *arr, size_t size)
{
	size_t i = 0;
	for (i = 0; i < size; ++i)
	{
		printf("%d, ", arr[i]);
	}
} */

/* PRINTING TEST */

/*static void PrintArr(int *arr, size_t size)*/
/*{*/
/*	size_t i = 0;*/
/*	for (i = 0; i < size; ++i)*/
/*	{*/
/*		printf("%d, ", arr[i]);*/
/*	}*/
/*	puts("");*/
/*}*/

/*static void Insertion_test()*/
/*{*/
/*	int arr[15] = {0};*/
/*	size_t size = ARR_SIZE(arr);*/
/*	*/
/*	FillArr(arr, size);*/
/*	puts("\nInsertion Sort test.");*/
/*	puts("Before sort.");*/
/*	PrintArr(arr, size);*/
/*	*/
/*	Insertion(arr, size);*/
/*	*/
/*	puts("After sort.");*/
/*	PrintArr(arr, size);*/
/*	*/
/*	puts("==================================================\n");*/
/*}*/

/*static void Selection_test()*/
/*{*/
/*	int arr[15] = {0};*/
/*	size_t size = ARR_SIZE(arr);*/
/*	*/
/*	FillArr(arr, size);*/
/*	puts("Selection Sort test.");*/
/*	puts("Before sort.");*/
/*	PrintArr(arr, size);*/
/*	*/
/*	Selection(arr, size);*/
/*	*/
/*	puts("After sort.");*/
/*	PrintArr(arr, size);*/
/*	*/
/*	puts("==================================================\n");*/
/*}*/

/*static void Bubble_test()*/
/*{*/
/*	int arr[15] = {0};*/
/*	size_t size = ARR_SIZE(arr);*/
/*	*/
/*	FillArr(arr, size);*/
/*	puts("Bubble Sort test.");*/
/*	puts("Before sort.");*/
/*	PrintArr(arr, size);*/
/*	*/
/*	Bubble(arr, size);*/
/*	*/
/*	puts("After sort.");*/
/*	PrintArr(arr, size);*/
/*	*/
/*	puts("==================================================\n");*/
/*}*/

