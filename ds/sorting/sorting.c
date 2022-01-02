/******************************************
* Author:   Maya Petter                   *
* Reviewer: Dor Tambour                   *
* Purpose:  Sorting algorithms            *
*******************************************/
#include <assert.h> /* assert */
#include <stdlib.h> /* free, malloc, calloc */
#include <string.h> /* memcpy */
#include <limits.h> /* CHAR_BIT */

#include "sorting.h"

#define BITS_IN_INT (CHAR_BIT * sizeof(int))

enum {SORT_SUCCESS, SORT_FAIL};

/*************static helper function *****************************************/
static void SwapInt(int *left, int *right);
static void SwapData(void *data1, void *data2, size_t elem_size);
static void *FreeFunc(void *to_free);

static void HistogramFiller(unsigned int *histogram, int arr[], size_t size, 
                                                            int relative_shift);
static void HistogramPrefixSum(unsigned int *histogram, int range);
static void HistogramSortArry(unsigned int *histogram, int *sorted_arr, 
                                    int arr[], size_t size, int relative_shift);
static void RadixSorting(int arr[], size_t size, unsigned int num_of_bits, 
                         unsigned int *histogram, int *buffer, 
                         unsigned int histogram_size, unsigned int mask, 
                         unsigned int iteration);

static int MergeArrays(int *arr, size_t begin, size_t mid, size_t end, 
                       int *begin_buff, int *end_buff, 
			           int (*is_before)(int a, int b));
static int RecMerge(int *arr, size_t begin, size_t end, int *begin_buff, 
                    int *end_buff, int (*is_before)(int a, int b));

static size_t Partition(void *base, size_t elem_size, size_t low, size_t high, 
                        int (*cmp)(const void *data1, const void *data2));
static void QSortRec(void *base, size_t elem_size, size_t low, size_t high, 
                     int (*cmp)(const void *data1, const void *data2));

/**************Swap functions*************************************************/                         
static void SwapInt(int *left, int *right)
{
	int temp = 0;
	
	assert(NULL != left);
	assert(NULL != right);
	
	temp = *left;
	*left = *right;
	*right = temp;
}

static void SwapPointers(void **left, void **right)
{
	long *temp = 0;
	
	assert(NULL != left);
	assert(NULL != right);
	
	temp = *left;
	*left = *right;
	*right = temp;
}

static void *FreeFunc(void *to_free)
{
    free(to_free); 
    to_free = NULL;
    
    return NULL;
}

/**************Selection sort *************************************************/                         
void Selection(int *arr, size_t size)
{
	size_t i = 0;
	
	assert(NULL != arr);
	
	for (i = 1; i < size ; ++i)
	{
		if (arr[i] < arr[i - 1])
		{
	        int temp = 0;
	        size_t j =0;
	        
			temp = arr[i];
			
			for (j = i; (j > 0) && (temp < arr[j - 1]); --j)
			{
				arr[j] = arr[j - 1];
			}

			arr[j] = temp;
		}	
	}
}

/**************Insertion sort *************************************************/                         
void Insertion(int *arr, size_t size)
{
	size_t index = 0;
	
	assert(NULL != arr);
	
	for (index = 0; index < size; ++index)
	{
	    size_t next_index = 0, min_index = 0;
	    
		min_index = index;
		
		for (next_index = index + 1; next_index < size; ++next_index)
		{
			if (arr[min_index] > arr[next_index])
			{
				min_index = next_index;
			}
		}
		SwapInt(&arr[index], &arr[min_index]);	
	}	
}

/**************Bubble sort *************************************************/
void Bubble(int *arr, size_t size)
{
	assert(NULL != arr);
	
	for (; 0 < size ; --size) /*modification to decrease time complexity (n-1)*/
	{
	    size_t index = 0;
        size_t counter = 0;
	    
		for (index = 0; index < size - 1; ++index)
		{
			if (arr[index] > arr[index + 1])
			{
				SwapInt(&arr[index], &arr[index + 1]);
			    ++counter;
			}
		}
	    
    /*modification to decrease time complexity in case  array is already sorted*/
	    if (0 == counter) 
	    {
	        break;
	    }
	}
}

int Counting(int arr[], size_t size, int min, int max)
{
    unsigned int *histogram = NULL;
    int *sorted_arr = NULL;                                                      
    
    int range = (max - min) + 1;
    int relative_shift = -min;
       
    assert(NULL != arr);
    assert(1 <= size);   
    
    /* create histogram */
    histogram = (unsigned int *)calloc(range, sizeof(unsigned int));
    if (NULL == histogram)
    {
        return SORT_FAIL;
    }
    
    /* create helper array */
    sorted_arr = (int *)malloc(size * sizeof(int));
    if (NULL == sorted_arr)
    {
        return SORT_FAIL;
    }
    
    /* fill histogram with counted appearances */  
    HistogramFiller(histogram, arr, size, relative_shift);
 
    /* calculate running sum */
    HistogramPrefixSum(histogram, range);
    
    /* sort the array */
    HistogramSortArry(histogram, sorted_arr, arr, size, relative_shift);
    
    sorted_arr = FreeFunc(sorted_arr);
    histogram = FreeFunc(histogram);
    
    return SORT_SUCCESS;
}

static void HistogramFiller(unsigned int *histogram, int arr[], size_t size, 
                                                            int relative_shift)
{
    size_t i = 0;

    assert(NULL != arr);    
    assert(NULL != histogram);
    
    for (i = 0; i < size; ++i)
    {
        histogram[arr[i] + relative_shift] += 1;
    }    
}

static void HistogramPrefixSum(unsigned int *histogram, int range)
{
    int i = 0;
    
    assert(NULL != histogram);
    
    for (i = 1; i < range; ++i)
    {
        histogram[i] += histogram[i - 1];
    }    
}

static void HistogramSortArry(unsigned int *histogram, int *sorted_arr, 
                             int arr[], size_t size, int relative_shift)
{   
    long i = 0;
    
    assert(NULL != histogram);
    assert(NULL != arr);
     
    for (i = size - 1; i >= 0; --i)
    {
        sorted_arr[histogram[arr[i] + relative_shift] -1] = arr[i];
        histogram[arr[i] + relative_shift] -= 1;
    }
    
    memcpy(arr, sorted_arr, (size * sizeof(int)));
}

int Radix(int arr[], size_t size, unsigned int num_of_bits)
{
    unsigned int *histogram = NULL;
    int *buffer = NULL;
    
    unsigned int histogram_size = 1 << num_of_bits;
    unsigned int mask = 0;
    unsigned int iteration = 0;
       
    assert(1 <= size);
    assert(NULL != arr);
    assert(0 == (BITS_IN_INT % num_of_bits));   
    
    /* create histogram */
    histogram = (unsigned int *)calloc(histogram_size, sizeof(unsigned int));
    if (NULL == histogram)
    {
        return SORT_FAIL;
    }
    
    /* create buffer */
    buffer = (int *)malloc(size * sizeof(int));
    if (NULL == buffer)
    {
        return SORT_FAIL;
    }
    
    for (mask = ~((~0) << num_of_bits); mask != 0; mask <<= num_of_bits, 
    																++iteration)
    {
        RadixSorting(arr, size, num_of_bits, histogram, buffer, histogram_size, 
                                                               mask, iteration);
                                                                     	   
        /* swap pointers between buffer and arr */
        SwapPointers((void **)&buffer, (void **)&arr);
            
        /* zero out all elements in histogram*/
		memset(histogram, 0, histogram_size * sizeof(unsigned int));
    }   
    
    buffer = FreeFunc(buffer);
    histogram = FreeFunc(histogram);
    
    return SORT_SUCCESS;

}

static void RadixSorting(int arr[], size_t size, unsigned int num_of_bits, 
                         unsigned int *histogram, int *buffer, 
                         unsigned int histogram_size, unsigned int mask, 
                         unsigned int iteration) 
{
    size_t i = 0;
    unsigned int shift = iteration * num_of_bits;
    
    /* fill histogram with counted appearances*/ 
    for (i = 0; i < size; ++i)
    {
        histogram[(arr[i] & mask) >> shift] += 1;
    }    
    
    /* calculate running sum */
    HistogramPrefixSum(histogram, histogram_size);
    
    /* sort the array to buffer */
    for (i = size; i > 0; --i)
    {   
        buffer[histogram[(arr[i - 1] & mask) >> shift] -1] = arr[i - 1];
        histogram[(arr[i - 1] & mask) >> shift] -= 1;
    }

}

/******** Merge Sort **********************************************************/
int Merge(int *arr, size_t size, int (*is_before)(int a, int b))
{   
    int *begin_buff = NULL;
    int *end_buff = NULL;
    int merge_status = 0;

    size_t begin = 0;
    size_t end = size - 1;
    size_t mid = end >> 1;

    size_t size1 = mid + 1;
	size_t size2 =  end - mid; 
    
    assert(NULL != arr);
    assert(NULL != is_before);
    
    begin_buff = (int *)malloc(size1 * sizeof(int));
    if (NULL == begin_buff)
    {
        return 1;
    }
	
	end_buff = (int *)malloc(size2 * sizeof(int));
	if (NULL == begin_buff)
    {
        return 1;
    }
	
	merge_status = RecMerge(arr, begin, end, begin_buff, end_buff, is_before);
    
	free(begin_buff);
	free(end_buff);	
	
	return merge_status;
}

static int RecMerge(int *arr, size_t begin, size_t end, int *begin_buff, 
                    int *end_buff, int (*is_before)(int a, int b))
{
	int status = 0;
	size_t mid = 0;
    
    assert(NULL != arr);
    assert(NULL != begin_buff);
    assert(NULL != end_buff);
    assert(NULL != is_before);
    
	if (begin >= end)
	{
	    return status;
	}

	mid = (end + begin) >> 1;
	
	RecMerge(arr, begin, mid, begin_buff, end_buff, is_before);
	RecMerge(arr, mid + 1, end, begin_buff, end_buff, is_before);
	
	return MergeArrays(arr, begin, mid, end, begin_buff, end_buff, is_before);
}

static int MergeArrays(int *arr, size_t begin, size_t mid, size_t end, 
                       int *begin_buff, int *end_buff, 
			           int (*is_before)(int a, int b))
{
	size_t i = 0, j = 0, k = 0;
	size_t size1 = mid - begin + 1;
	size_t size2 =  end - mid; 
    
    assert(NULL != arr);
    assert(NULL != is_before);

	memcpy(begin_buff, (arr + begin), size1 * sizeof(int));
	memcpy(end_buff, (arr + mid + 1), size2 * sizeof(int));
    
	for (i = begin, j = 0, k = 0; j < size1 && k < size2; ++i)
	{
		if(is_before(begin_buff[j], end_buff[k]))
		{ 
			arr[i] = begin_buff[j];
			++j; 
		}
		else
		{ 
			arr[i] = end_buff[k];
			++k; 
		} 
	}

	for (; j < size1; ++i, ++j) 
	{ 
		arr[i] = begin_buff[j];
	} 

	for (; k < size2; ++i, ++k)
	{ 
		arr[i] = end_buff[k];
	}

	return 0;
}

/******** Quick Sort **********************************************************/
static void SwapData(void *data1, void *data2, size_t elem_size)
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

static size_t Partition(void *base, size_t elem_size, size_t low, size_t high, 
                        int (*cmp)(const void *data1, const void *data2))
{
    size_t pivot = high;
    size_t i = low;
    size_t j = low;
    char *array = NULL;
    
    assert(NULL != base);
    assert(NULL != cmp);
    
    array = (char *)base;
    
    for (; j < high; ++j)
    {
        if (0 < cmp(&array[j * elem_size], &array[pivot * elem_size]))
        {
            SwapData(&array[i * elem_size], &array[j * elem_size], elem_size);
            ++i;
        }
    }
    
    SwapData(&array[i * elem_size], &array[high * elem_size], elem_size);
    
    return i;
}

static void QSortRec(void *base, size_t elem_size, size_t low, size_t high,
                     int (*cmp)(const void *data1, const void *data2))
{
    size_t pivot = 0;
    
    assert(NULL != base);
    assert(NULL != cmp);

    if (low >= high)
    {
        return;
    }
    
    pivot = Partition(base, elem_size, low, high, cmp);

    if (0 < pivot)
    {
        QSortRec(base, elem_size, low, pivot - 1, cmp);
    }

    QSortRec(base, elem_size, pivot + 1 , high, cmp);
}

void QSort(void *base, size_t elem_size, size_t num_of_elem, 
           int (*cmp)(const void *data1, const void *data2))
{
    
    assert(NULL != base);
    assert(NULL != cmp);

    QSortRec(base, elem_size, 0, num_of_elem - 1, cmp);
}

