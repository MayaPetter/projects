#include "tests.h"

/**************Swap functions*************************************************/                         
void SwapInt(int *left, int *right)
{
	int temp = 0;
	
	assert(NULL != left);
	assert(NULL != right);
	
	temp = *left;
	*left = *right;
	*right = temp;
}

void SwapPointers(void **left, void **right)
{
	long *temp = 0;
	
	assert(NULL != left);
	assert(NULL != right);
	
	temp = *left;
	*left = *right;
	*right = temp;
}

void SwapData(void *data1, void *data2, size_t elem_size)
{
    char *right = NULL;
    char *left = NULL;
    char temp = 0;
    size_t index = 0;
    
    assert(NULL != data1);
    assert(NULL != data2);
    
    right = (char *)data1;
    left = (char *)data2;
    
    for (index = 0; index < elem_size; ++index, ++right, ++left)
    {
        temp = *right;
        *right = *left;
        *left = temp;
    }
}

/**************Free functions*************************************************/
void *FreeFunction(void *to_free)
{
    free(to_free); 
    to_free = NULL;
    
    return NULL;
}

/**************Int array functions*********************************************/
int FindMax(int *arr, size_t size)
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

int FindMin(int *arr, size_t size)
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

int RandInRange(int max)
{
	return rand() % max;
}

static int cmpfunc(const void *a, const void *b)
{
	assert(NULL != a);
	assert(NULL != b);
	return (*(int *)a - *(int *)b);
}

void IntArrayFiller(int *arr, size_t size)
{
	size_t i = 0;
	
	assert(NULL != arr);
	
	for (i = 0; i < size; ++i)
	{
		arr[i] = RandInRange(20);
	}
}

void CopyAndSortIntArr(int *dest, int *src, size_t size)
{
	assert(NULL != dest);
	assert(NULL != src);
	
	memcpy(dest, src, (size * sizeof(int)));
	qsort(dest, size, sizeof(int), cmpfunc);
}

int CheckIntSort(int *my_sort, int *qsort, size_t size)
{
	size_t i = 0;
	for (i = 0; i < size; ++i)
	{
		Expect(my_sort[i] == qsort[i]);
	}
	return 0;
}

void PrintIntArray(int *arr, size_t size)
{
	size_t i = 0;
	for (i = 0; i < size; ++i)
	{
		printf("%d, ", arr[i]);
	}
} 

