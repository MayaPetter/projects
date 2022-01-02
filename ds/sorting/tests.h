#ifndef __TESTS_H
#define __TESTS_H

#include <stddef.h> /* size_t */
#include <stdio.h> /* printf */
#include <stdlib.h> /* rand, qsort */
#include <string.h> /* memcpy */
#include <assert.h> /* assert */
#include <limits.h> /* INT_MIN, INT_MAX */

#define ARR_SIZE(x) (sizeof(x) / sizeof(x[0]))

#define Expect(x) \
;{\
	if (!(x))\
	{\
		printf("test failed in line %d\n", __LINE__);\
		return 0;\
	}\
} /* end of Expect */

#define UNUSED(x) (void) (x)

/* Swaps functions */
void SwapInt(int *left, int *right);

/* Swaps between pointers to data */
void SwapPointers(void **left, void **right);

/* Swaps data byte by byte */
void SwapData(void *data1, void *data2, size_t elem_size);

/* Free function returns NULL to assign */
void *FreeFunction(void *to_free);

/* finds min and max values in int array range */
int FindMax(int *arr, size_t size);
int FindMin(int *arr, size_t size);

/* returns random number in int array range */
int RandInRange(int max);

/* random numbers in int array range */
void IntArrayFiller(int *arr, size_t size);

/* copy int array and sorting through lib qsort */
void CopyAndSortIntArr(int *dest, int *src, size_t size);

/* checks if int array is sorted cmp to same array through lib qsort */ 
int CheckIntSort(int *my_sort, int *qsort, size_t size);

/* prints all ints in array */
void PrintIntArray(int *arr, size_t size);

#endif /* __TESTS_H */
