#include <assert.h> /* assert */
#include <limits.h> /* CHAR_BIT */
#include <time.h>   /* clock */
#include <stdio.h>  /*printf */

#include "sorting.h"
#include "tests.h"

#define BITS_IN_INT (CHAR_BIT * sizeof(int))
#define ARR_SIZE(x) (sizeof(x) / sizeof(x[0]))

static void InsertionTest();
static void SelectionTest();
static void BubbleTest();
static void CountingSortTest();
static void RadixTest();
static void MergeTest();
static void QSortTest();
static int InsertionBigArrTest();
static int SelectionBigArrTest();
static int BubbleBigArrTest();
static int CountingBigArrTest();
static int RadixBigArrTest();
static int MergeBigArrTest();
static int QSortBigArrTest();

int main()
{
	InsertionTest();
	SelectionTest();
	BubbleTest();
    CountingSortTest();
    RadixTest();
    MergeTest();    
    QSortTest();
    
    BubbleBigArrTest();
    InsertionBigArrTest();
    SelectionBigArrTest();
    CountingBigArrTest();
    RadixBigArrTest();
    MergeBigArrTest();
    QSortBigArrTest();
	return 0;
}

static void InsertionTest()
{
	int not_sorted_arr[] = {6, 9, 20, 16, 100, 2, 3, 7, 8};
	int sorted_arr[] = {2, 3, 6, 7, 8, 9, 16, 20, 100};
	int orig_sorted_arr[] = {2, 3, 6, 7, 8, 9, 16, 20, 100};
	size_t index = 0;
	size_t size_not_sorted = sizeof(not_sorted_arr)/sizeof(not_sorted_arr[0]);
     
/*    double tic = 0;*/
/*    double toc = 0;	*/
/*    double sortting_efficiency = 0;*/
/*	double sorted_efficiency = 0;*/
	
/*	tic = clock();*/
	Insertion(not_sorted_arr, size_not_sorted);
/*	toc = clock();*/
/*	sortting_efficiency = (double)(toc - tic)/CLOCKS_PER_SEC;*/
	
	for(index = 0; index < size_not_sorted; ++index)
	{
		assert(not_sorted_arr[index] == sorted_arr[index]);
	}
	
/*	tic = clock();*/
	Insertion(sorted_arr, size_not_sorted);
/*	toc = clock();*/
/*	sorted_efficiency = (double)(toc - tic)/CLOCKS_PER_SEC;*/
	
	for(index = 0; index < size_not_sorted; ++index)
	{
		assert(sorted_arr[index] == orig_sorted_arr[index]);

	}
	
/*	printf("Insertion: \n sortting_efficiency %f, sorted_efficiency %f\n", */
/*	        sortting_efficiency, sorted_efficiency);*/
}

static int InsertionBigArrTest()
{
    int sorting_test[1000] = {0};
	int qsorted[1000] = {0};
	size_t size = ARR_SIZE(sorting_test);
	
	double tic = 0.0, toc = 0.0;
    double efficiency = 0.0;
	
	IntArrayFiller(sorting_test, size);
	
	CopyAndSortIntArr(qsorted, sorting_test, size);
	
	tic = clock();
	Insertion(sorting_test, size);
	toc = clock();
	efficiency = (double)(toc - tic)/CLOCKS_PER_SEC;
	
	Expect(0 == CheckIntSort(sorting_test, qsorted, size))
	
	puts("\nInsertion:\n");
	printf("sortting efficiency %f\n", efficiency);
	
	tic = clock();
	Insertion(qsorted, size);
	toc = clock();
	efficiency = (double)(toc - tic)/CLOCKS_PER_SEC;
	
	Expect(0 == CheckIntSort(qsorted, qsorted, size))
	
	printf("already sorted %f\n", efficiency);
	
	return 1;
}

static void SelectionTest()
{
	int not_sorted_arr[] = {6, 9, 20, 16, 100, 2, 3, 7, 8};
	int sorted_arr[] = {2, 3, 6, 7, 8, 9, 16, 20, 100};
	int orig_sorted_arr[] = {2, 3, 6, 7, 8, 9, 16, 20, 100};
	size_t index = 0;
	size_t size = sizeof(not_sorted_arr)/sizeof(not_sorted_arr[0]);
	
	Selection(not_sorted_arr, size);
	
	for(index = 0; index < size; ++index)
	{
		assert(not_sorted_arr[index] == sorted_arr[index]);
	}
	
	Selection(sorted_arr, size);
	
	for(index = 0; index < size; ++index)
	{
		assert(sorted_arr[index] == orig_sorted_arr[index]);
	}
}

static int SelectionBigArrTest()
{
    int sorting_test[1000] = {0};
	int qsorted[1000] = {0};
	size_t size = ARR_SIZE(sorting_test);
	
	double tic = 0, toc = 0;
    double efficiency = 0.0;
	
	IntArrayFiller(sorting_test, size);
	
	CopyAndSortIntArr(qsorted, sorting_test, size);
	
	tic = clock();
	
	Selection(sorting_test, size);
	
	toc = clock();
	efficiency = (double)(toc - tic)/CLOCKS_PER_SEC;
	
	Expect(0 == CheckIntSort(sorting_test, qsorted, size))
	
	puts("\nSelection:\n");
	printf("sortting efficiency %f\n", efficiency);
	
	tic = clock();
	
	Selection(qsorted, size);
	
	toc = clock();
	efficiency = (double)(toc - tic)/CLOCKS_PER_SEC;
	
	Expect(0 == CheckIntSort(qsorted, qsorted, size))
	
	printf("already sorted %f\n", efficiency);
	
	return 1;
}

static void BubbleTest()
{
	int not_sorted_arr[] = {6, 9, 20, 16, 100, 8, 3, 7, 2};
	int sorted_arr[] = {2, 3, 6, 7, 8, 9, 16, 20, 100};
	int orig_sorted_arr[] = {2, 3, 6, 7, 8, 9, 16, 20, 100};
	size_t index = 0;
    size_t size = sizeof(not_sorted_arr)/sizeof(not_sorted_arr[0]);
	
	Bubble(not_sorted_arr, size);
	
	for(index = 0; index < size; ++index)
	{
		assert(not_sorted_arr[index] == sorted_arr[index]);
	}
	
	Bubble(sorted_arr, size);
	
	for(index = 0; index < size; ++index)
	{
		assert(sorted_arr[index] == orig_sorted_arr[index]);
	}

}

static int BubbleBigArrTest()
{
    int sorting_test[1000] = {0};
	int qsorted[1000] = {0};
	size_t size = ARR_SIZE(sorting_test);
	
	double tic = 0, toc = 0;
    double efficiency = 0.0;
	
	IntArrayFiller(sorting_test, size);
	
	CopyAndSortIntArr(qsorted, sorting_test, size);
	
	tic = clock();
	
	Bubble(sorting_test, size);
	
	toc = clock();
	efficiency = (double)(toc - tic)/CLOCKS_PER_SEC;
	
	Expect(0 == CheckIntSort(sorting_test, qsorted, size))
	
	puts("\nBubble:\n");
	printf("sortting efficiency %f\n", efficiency);
	
	tic = clock();
	
	Bubble(qsorted, size);
	
	toc = clock();
	efficiency = (double)(toc - tic)/CLOCKS_PER_SEC;
	
	Expect(0 == CheckIntSort(qsorted, qsorted, size))
	
	printf("already sorted %f\n", efficiency);
	
	return 1;
}

static void CountingSortTest()
{
    int not_sorted_arr[] = {6, 9, 20, 16, 100, 2, 3, 7, 8};
	int sorted_arr[] = {2, 3, 6, 7, 8, 9, 16, 20, 100};
	int orig_sorted_arr[] = {2, 3, 6, 7, 8, 9, 16, 20, 100};
	
	size_t index = 0;
	size_t size = sizeof(not_sorted_arr)/sizeof(not_sorted_arr[0]);
	
    Counting(not_sorted_arr, size, 2, 100);
	
	for(index = 0; index < size; ++index)
	{
		assert(not_sorted_arr[index] == sorted_arr[index]);
	}
	
	Counting(sorted_arr, size, 2, 100);
	
	for(index = 0; index < size; ++index)
	{
		assert(sorted_arr[index] == orig_sorted_arr[index]);
	}

    {
	    int negative_arr[] = {-2, 3, -6, 7, -8, 9, -7, 2, 6};
	    int sorted_negative_arr[] = {-8, -7, -6, -2, 2, 3, 6, 7, 9};
	    
	    Counting(negative_arr, size, -8, 9);
	
	    for(index = 0; index < size; ++index)
	    {
		    assert(negative_arr[index] == sorted_negative_arr[index]);
	    }   
	}
}

static int CountingBigArrTest()
{
    int sorting_test[1000] = {0};
	int qsorted[1000] = {0};
	size_t size = ARR_SIZE(sorting_test);
	
	double tic = 0, toc = 0;
    double efficiency = 0.0;
	
	IntArrayFiller(sorting_test, size);
	
	CopyAndSortIntArr(qsorted, sorting_test, size);
	
	tic = clock();
	Counting(sorting_test, size, FindMin(sorting_test, size), 
	                             FindMax(sorting_test, size));
	toc = clock();
	efficiency = (double)(toc - tic)/CLOCKS_PER_SEC;
	
	Expect(0 == CheckIntSort(sorting_test, qsorted, size))
	
	puts("\nCounting:\n");
	printf("sortting efficiency %f\n", efficiency);
	
	tic = clock();
	Counting(qsorted, size, FindMin(sorting_test, size), 
	                             FindMax(sorting_test, size));
	toc = clock();
	efficiency = (double)(toc - tic)/CLOCKS_PER_SEC;
	
	Expect(0 == CheckIntSort(qsorted, qsorted, size))
	
	printf("already sorted %f\n", efficiency);
	
	return 1;
}


static void RadixTest()
{
    int not_sorted_arr[] = {6, 9, 20, 16, 100, 2, 3, 7, 8};
    int not_sorted_again[] = {6, 9, 20, 16, 100, 2, 3, 7, 8};
	int sorted_arr[] = {2, 3, 6, 7, 8, 9, 16, 20, 100};
	int orig_sorted_arr[] = {2, 3, 6, 7, 8, 9, 16, 20, 100};
	
	size_t index = 0;
	size_t size = sizeof(not_sorted_arr)/sizeof(not_sorted_arr[0]);
	
	assert(0 == (Radix(not_sorted_arr, size, CHAR_BIT / 2)));

	for(index = 0; index < size; ++index)
	{
		assert(not_sorted_arr[index] == sorted_arr[index]);
	}
	
	assert(0 == (Radix(not_sorted_again, size, CHAR_BIT)));
	
	for(index = 0; index < size; ++index)
	{
		assert(not_sorted_again[index] == sorted_arr[index]);
		assert(not_sorted_again[index] == not_sorted_arr[index]);
	}
	
	assert(0 == (Radix(sorted_arr, size, 4)));
	
	for(index = 0; index < size; ++index)
	{
		assert(sorted_arr[index] == orig_sorted_arr[index]);
	}
}

static int RadixBigArrTest()
{
    int sorting_test[1000] = {0};
	int qsorted[1000] = {0};
	size_t size = ARR_SIZE(sorting_test);
	
	double tic = 0, toc = 0;
    double efficiency = 0.0;
	
	IntArrayFiller(sorting_test, size);
	
	CopyAndSortIntArr(qsorted, sorting_test, size);
	
	tic = clock();
	Radix(sorting_test, size, CHAR_BIT / 2);
	toc = clock();
	efficiency = (double)(toc - tic)/CLOCKS_PER_SEC;
	
	Expect(0 == CheckIntSort(sorting_test, qsorted, size))
	
	puts("\nRadix:\n");
	printf("sortting efficiency CHAR_BIT / 2 %f\n", efficiency);
	
	tic = clock();
	Radix(sorting_test, size, CHAR_BIT);
	toc = clock();
	efficiency = (double)(toc - tic)/CLOCKS_PER_SEC;
	
	Expect(0 == CheckIntSort(sorting_test, qsorted, size))
	printf("sortting efficiency CHAR_BIT %f\n", efficiency);
	
	tic = clock();
	Radix(qsorted, size, CHAR_BIT);
	toc = clock();
	efficiency = (double)(toc - tic)/CLOCKS_PER_SEC;
	
	Expect(0 == CheckIntSort(qsorted, qsorted, size))
	
	printf("already sorted %f\n", efficiency);
	
	return 1;
}

static int IsBefore(int a, int b)
{
    return (a < b);
}

static void MergeTest()
{
    int not_sorted_arr[] = {6, 9, 20, 16, 100, 2, 3, 7, 8};
    int not_sorted_again[] = {6, 9, 20, 16, 100, 2, 3, 7, 8};
	int sorted_arr[] = {2, 3, 6, 7, 8, 9, 16, 20, 100};
	int orig_sorted_arr[] = {2, 3, 6, 7, 8, 9, 16, 20, 100};
	
	size_t index = 0;
	size_t size = sizeof(not_sorted_arr)/sizeof(not_sorted_arr[0]);

	assert(0 == (Merge(not_sorted_arr, size, IsBefore)));

	for(index = 0; index < size; ++index)
	{
		assert(not_sorted_arr[index] == sorted_arr[index]);
	}

	assert(0 == (Merge(not_sorted_again, size, IsBefore)));
	
	for(index = 0; index < size; ++index)
	{
		assert(not_sorted_again[index] == sorted_arr[index]);
		assert(not_sorted_again[index] == not_sorted_arr[index]);
	}

	assert(0 == (Merge(sorted_arr, size, IsBefore)));
	
	for(index = 0; index < size; ++index)
	{
		assert(sorted_arr[index] == orig_sorted_arr[index]);
	}
}

static int MergeBigArrTest()
{
    int sorting_test[1000] = {0};
	int qsorted[1000] = {0};
	size_t size = ARR_SIZE(sorting_test);
	
	double tic = 0, toc = 0;
    double efficiency = 0.0;
	
	IntArrayFiller(sorting_test, size);
	
	CopyAndSortIntArr(qsorted, sorting_test, size);
	
	tic = clock();
	Merge(sorting_test, size, IsBefore);
	toc = clock();
	efficiency = (double)(toc - tic)/CLOCKS_PER_SEC;
	
	Expect(0 == CheckIntSort(sorting_test, qsorted, size))
	
	puts("\nMerge:\n");
	printf("sortting efficiency %f\n", efficiency);
	
	tic = clock();
	Merge(qsorted, size, IsBefore);
	toc = clock();
	efficiency = (double)(toc - tic)/CLOCKS_PER_SEC;
	
	Expect(0 == CheckIntSort(qsorted, qsorted, size))
	
	printf("already sorted %f\n", efficiency);
	
	return 1;
}

static int CmpQuick(const void *data1, const void *data2)
{
    return (*(int *)data2 - *(int *)data1);
}

static void QSortTest()
{
    int not_sorted_arr[] = {6, 9, 20, 16, 100, 2, 3, 7, 8};
    int not_sorted_again[] = {6, 9, 20, 16, 100, 2, 3, 7, 8};
	int sorted_arr[] = {2, 3, 6, 7, 8, 9, 16, 20, 100};
	int orig_sorted_arr[] = {2, 3, 6, 7, 8, 9, 16, 20, 100};
	
	size_t index = 0;
	size_t size = sizeof(not_sorted_arr)/sizeof(not_sorted_arr[0]);
    
	QSort(not_sorted_arr, sizeof(int), size, CmpQuick);

	for(index = 0; index < size; ++index)
	{
		assert(not_sorted_arr[index] == sorted_arr[index]);
	}
    
	QSort(not_sorted_again, sizeof(int), size, CmpQuick);
	
	for(index = 0; index < size; ++index)
	{
		assert(not_sorted_again[index] == sorted_arr[index]);
		assert(not_sorted_again[index] == not_sorted_arr[index]);
	}

	QSort(sorted_arr, sizeof(int), size, CmpQuick);
	
	for(index = 0; index < size; ++index)
	{
		assert(sorted_arr[index] == orig_sorted_arr[index]);
	}
}

static int QSortBigArrTest()
{
    int sorting_test[1000] = {0};
	int qsorted[1000] = {0};
	size_t size = ARR_SIZE(sorting_test);
	
	double tic = 0, toc = 0;
    double efficiency = 0.0;
	
	IntArrayFiller(sorting_test, size);
	
	CopyAndSortIntArr(qsorted, sorting_test, size);
	
	tic = clock();
	QSort(sorting_test, sizeof(int), size, CmpQuick);
	toc = clock();
	efficiency = (double)(toc - tic)/CLOCKS_PER_SEC;
	
	Expect(0 == CheckIntSort(sorting_test, qsorted, size))
	
	puts("\nQSort:\n");
	printf("sortting efficiency %f\n", efficiency);
	
	tic = clock();
	QSort(qsorted, sizeof(int), size, CmpQuick);
	toc = clock();
	efficiency = (double)(toc - tic)/CLOCKS_PER_SEC;
	
	Expect(0 == CheckIntSort(qsorted, qsorted, size))
	
	printf("already sorted %f\n", efficiency);
	
	return 1;
}

