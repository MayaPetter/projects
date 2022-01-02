#include <assert.h> /*assert*/
#include <time.h>   /*clock*/

#include "search.h" /* search functions */
#include "tests.h"  /* testing helper functions */

#define ARR_SIZE(x) (sizeof(x) / sizeof(x[0]))

static int BasicBSearchTest();
static int BSearchIterativeTest();
static int BasicRecBSearchTest();
static int BSearchRecursiveTest();
static int BasicJSearchTest();
static int JSearchTest();

static void InitUpArray(int *array, size_t size);
static void InitDownArray(int *array, size_t size);
static int CmpAbeforeB(const void *data_a, const void *data_b);
static int CmpBbeforeA(const void *data_a, const void *data_b);

/***** global arrays **********************************************************/
static int UP_ARR[1000] = {0};
static int DOWN_ARR[1000] = {0};
static int ALL_0_ARR[1000] = {0};

int main()
{
    InitUpArray(UP_ARR, ARR_SIZE(UP_ARR));
    InitDownArray(DOWN_ARR, ARR_SIZE(DOWN_ARR));
 
    BasicBSearchTest();
    BSearchIterativeTest();
    BasicRecBSearchTest();
    BSearchRecursiveTest();
    BasicJSearchTest();
    JSearchTest();
    
    return 0;
}

static int BasicBSearchTest()
{
    int array[] = {1, 2, 3, 4, 5, 6, 7, 8, 9};
    int down_array[] = {9, 8, 7, 6, 5, 4, 3, 2, 1};
    int all_0_arr[] = {0, 0, 0, 0, 0, 0, 0, 0, 0};
    size_t size = ARR_SIZE(array);
    size_t i = 0;
    
    for (i = 0; i < size; ++i)
    {
       Expect(array[i] == *(int *)BSearchIterative(&array[i], array, 
                                 size, sizeof(int), CmpAbeforeB))
    }
    
    for (i = 0; i < size; ++i)
    {
       Expect(down_array[i] == *(int *)BSearchIterative(&down_array[i], down_array, 
                                 size, sizeof(int), CmpBbeforeA))
    }
    
    for (i = 0; i < size; ++i)
    {
       Expect(all_0_arr[i] == *(int *)BSearchIterative(&all_0_arr[i], all_0_arr, 
                                 size, sizeof(int), CmpBbeforeA))
    }
    return 1;    
}

static int BSearchIterativeTest()
{
    int data_in_array = 0;
    int not_in_array = 0;
    
    double  start = 0.0;
    double  beginnig = 0.0;
    double  middle_up = 0.0,  middle_down = 0.0;
    double  end = 0.0;
    
    start = clock();
    Expect(data_in_array == *(int *)BSearchIterative(&data_in_array, UP_ARR, 
                             ARR_SIZE(UP_ARR), sizeof(int), CmpAbeforeB))    
    beginnig = clock();
    beginnig = (double)( beginnig -  start)/CLOCKS_PER_SEC;
    
    start = clock();
    Expect(data_in_array == *(int *)BSearchIterative(&data_in_array, DOWN_ARR, 
                             ARR_SIZE(DOWN_ARR), sizeof(int), CmpBbeforeA))        
    end = clock();
    end = (double)(end - start)/CLOCKS_PER_SEC;
    
    Expect(data_in_array == *(int *)BSearchIterative(&data_in_array, ALL_0_ARR, 
                                 ARR_SIZE(ALL_0_ARR), sizeof(int), CmpAbeforeB))                                         
    data_in_array = 666;
    
    start = clock();
    Expect(data_in_array == *(int *)BSearchIterative(&data_in_array, UP_ARR, 
                             ARR_SIZE(UP_ARR), sizeof(int), CmpAbeforeB))
    middle_up = clock();
    middle_up = (double)(middle_up - start)/CLOCKS_PER_SEC;
    
    start = clock();
    Expect(data_in_array == *(int *)BSearchIterative(&data_in_array, DOWN_ARR, 
                             ARR_SIZE(DOWN_ARR), sizeof(int), CmpBbeforeA))
    middle_down = clock();
    middle_down = (double)(middle_down - start)/CLOCKS_PER_SEC;                         
                             
    Expect(NULL == BSearchIterative(&data_in_array, ALL_0_ARR, 
                                 ARR_SIZE(ALL_0_ARR), sizeof(int), CmpAbeforeB))                
    data_in_array = 969;
    
    Expect(data_in_array == *(int *)BSearchIterative(&data_in_array, UP_ARR, 
                             ARR_SIZE(UP_ARR), sizeof(int), CmpAbeforeB))
    Expect(data_in_array == *(int *)BSearchIterative(&data_in_array, DOWN_ARR, 
                             ARR_SIZE(DOWN_ARR), sizeof(int),  CmpBbeforeA))           
    
    not_in_array = 1576;
    
    Expect(NULL == BSearchIterative(&not_in_array, UP_ARR, ARR_SIZE(UP_ARR), 
                                    sizeof(int), CmpAbeforeB)) 
    Expect(NULL == BSearchIterative(&not_in_array, DOWN_ARR, 
                                  ARR_SIZE(DOWN_ARR), sizeof(int), CmpBbeforeA))
    
    puts("\nB-Search Iterative efficiency:\n");                                
    printf("data at beginning - %f\ndata at end - %f\nmiddle up- %f\n\
    middle down- %f\n", beginnig, end, middle_up, middle_down); 
    
    return 1; 
} 

static int BasicRecBSearchTest()
{
    int array[] = {1, 2, 3, 4, 5, 6, 7, 8, 9};
    int down_arr[] = {9, 8, 7, 6, 5, 4, 3, 2, 1};
    int all_0_arr[] = {0, 0, 0, 0, 0, 0, 0, 0, 0};
    size_t size = ARR_SIZE(array);
    size_t i = 0;
    
    for (i = 0; i < size; ++i)
    {
       Expect(array[i] == *(int *)BSearchRecursive(&array[i], array, 
                                 size, sizeof(int), CmpAbeforeB))
    }
    
    for (i = 0; i < size; ++i)
    {
       Expect(down_arr[i] == *(int *)BSearchRecursive(&down_arr[i], down_arr, 
                                 size, sizeof(int), CmpBbeforeA))
    }
    
    for (i = 0; i < size; ++i)
    {
       Expect(all_0_arr[i] == *(int *)BSearchRecursive(&all_0_arr[i], all_0_arr, 
                                 size, sizeof(int), CmpBbeforeA))
    }
    return 1;    
}

static int BSearchRecursiveTest()
{
    int data_in_array = 0;
    int not_in_array = 0;
    
    double  start = 0.0;
    double  beginnig = 0.0;
    double  middle_up = 0.0,  middle_down = 0.0;
    double  end = 0.0;
    
    start = clock();
    Expect(data_in_array == *(int *)BSearchRecursive(&data_in_array, UP_ARR, 
                             ARR_SIZE(UP_ARR), sizeof(int), CmpAbeforeB))    
    beginnig = clock();
    beginnig = (double)(beginnig - start)/CLOCKS_PER_SEC;
    
    start = clock();
    Expect(data_in_array == *(int *)BSearchRecursive(&data_in_array, DOWN_ARR, 
                             ARR_SIZE(DOWN_ARR), sizeof(int), CmpBbeforeA))        
    end = clock();
    end = (double)(end - start)/CLOCKS_PER_SEC;
    
    Expect(data_in_array == *(int *)BSearchRecursive(&data_in_array, ALL_0_ARR, 
                                 ARR_SIZE(ALL_0_ARR), sizeof(int), CmpAbeforeB))                                         
    data_in_array = 666;
    
    start = clock();
    Expect(data_in_array == *(int *)BSearchRecursive(&data_in_array, UP_ARR, 
                             ARR_SIZE(UP_ARR), sizeof(int), CmpAbeforeB))
    middle_up = clock();
    middle_up = (double)(middle_up - start)/CLOCKS_PER_SEC;
    
    start = clock();
    Expect(data_in_array == *(int *)BSearchRecursive(&data_in_array, DOWN_ARR, 
                             ARR_SIZE(DOWN_ARR), sizeof(int),  CmpBbeforeA))
    middle_down = clock();
    middle_down = (double)(middle_down - start)/CLOCKS_PER_SEC;                         
                             
    Expect(NULL ==  BSearchRecursive(&data_in_array, ALL_0_ARR, 
                                 ARR_SIZE(ALL_0_ARR), sizeof(int), CmpAbeforeB))                
    data_in_array = 969;
    
    Expect(data_in_array == *(int *)BSearchRecursive(&data_in_array, UP_ARR, 
                             ARR_SIZE(UP_ARR), sizeof(int), CmpAbeforeB))
    Expect(data_in_array == *(int *)BSearchRecursive(&data_in_array, DOWN_ARR, 
                             ARR_SIZE(DOWN_ARR), sizeof(int),  CmpBbeforeA))           
    
    not_in_array = 1576;
    
    Expect(NULL ==  BSearchRecursive(&not_in_array, UP_ARR, ARR_SIZE(UP_ARR), 
                                    sizeof(int), CmpAbeforeB)) 
    Expect(NULL ==  BSearchRecursive(&not_in_array, DOWN_ARR, 
                                    ARR_SIZE(DOWN_ARR), sizeof(int),  CmpBbeforeA))
    
    puts("\nB-Search Recursive efficiency:\n");                                
    printf("data at beginning - %f\ndata at end - %f\nmiddle up- %f\n\
    middle down- %f\n\n", beginnig, end, middle_up, middle_down); 
    
    return 1; 
}

static int BasicJSearchTest()
{
    int array[] = {1, 2, 3, 4, 5, 6, 7, 8, 9};
    int down_arr[] = {9, 8, 7, 6, 5, 4, 3, 2, 1};
    int all_0_arr[] = {0, 0, 0, 0, 0, 0, 0, 0, 0};
    size_t size = ARR_SIZE(array);
    size_t i = 0;
    
    for (i = 0; i < size; ++i)
    {
       Expect(array[i] == *(int *)JSearch(&array[i], array, 
                                 size, sizeof(int), CmpAbeforeB))
    }
    
    for (i = 0; i < size; ++i)
    {
       Expect(down_arr[i] == *(int *)JSearch(&down_arr[i], down_arr, 
                                 size, sizeof(int), CmpBbeforeA))
    }
    
    for (i = 0; i < size; ++i)
    {
       Expect(all_0_arr[i] == *(int *)JSearch(&all_0_arr[i], all_0_arr, 
                                 size, sizeof(int), CmpBbeforeA))
    }
    return 1;    
}

static int JSearchTest()
{
    int data_in_array = 0;
    int not_in_array = 0;
    
    double  start = 0.0;
    double  beginnig = 0.0;
    double  middle_up = 0.0,  middle_down = 0.0;
    double  end = 0.0;
    
    start = clock();
    Expect(data_in_array == *(int *)JSearch(&data_in_array, UP_ARR, 
                             ARR_SIZE(UP_ARR), sizeof(int), CmpAbeforeB))    
    beginnig = clock();
    beginnig = (double)(beginnig - start)/CLOCKS_PER_SEC;
    
    start = clock();
    Expect(data_in_array == *(int *)JSearch(&data_in_array, DOWN_ARR, 
                             ARR_SIZE(DOWN_ARR), sizeof(int),  CmpBbeforeA))        
    end = clock();
    end = (double)(end - start)/CLOCKS_PER_SEC;
    
    Expect(data_in_array == *(int *)JSearch(&data_in_array, ALL_0_ARR, 
                             ARR_SIZE(ALL_0_ARR), sizeof(int), CmpAbeforeB))                                         
    data_in_array = 666;
    
    start = clock();
    Expect(data_in_array == *(int *)JSearch(&data_in_array, UP_ARR, 
                             ARR_SIZE(UP_ARR), sizeof(int), CmpAbeforeB))
    middle_up = clock();
    middle_up = (double)(middle_up - start)/CLOCKS_PER_SEC;
    
    start = clock();
    Expect(data_in_array == *(int *)JSearch(&data_in_array, DOWN_ARR, 
                             ARR_SIZE(DOWN_ARR), sizeof(int),  CmpBbeforeA))
    middle_down = clock();
    middle_down = (double)(middle_down - start)/CLOCKS_PER_SEC;                         
                             
    Expect(NULL == JSearch(&data_in_array, ALL_0_ARR, 
                            ARR_SIZE(ALL_0_ARR), sizeof(int), CmpAbeforeB))                
    data_in_array = 969;
    
    Expect(data_in_array == *(int *)JSearch(&data_in_array, UP_ARR, 
                             ARR_SIZE(UP_ARR), sizeof(int), CmpAbeforeB))
    Expect(data_in_array == *(int *)JSearch(&data_in_array, DOWN_ARR, 
                             ARR_SIZE(DOWN_ARR), sizeof(int),  CmpBbeforeA))           
    
    not_in_array = 1576;
    
    Expect(NULL == JSearch(&not_in_array, UP_ARR, ARR_SIZE(UP_ARR), 
                           sizeof(int), CmpAbeforeB)) 
    Expect(NULL == JSearch(&not_in_array, DOWN_ARR, 
                           ARR_SIZE(DOWN_ARR), sizeof(int),  CmpBbeforeA))
    
    puts("\nJ-Search efficiency:\n");                                
    printf("data at beginning - %f\ndata at end - %f\nmiddle up- %f\n\
    middle down- %f\n", beginnig, end, middle_up, middle_down); 
    
    return 1; 
}

/****** Initiate global arrays ************************************************/
static void InitUpArray(int *array, size_t size)
{
    size_t i = 0;
    
    assert(NULL != array);
    
    for (i = 0; i < size; ++i)
    {
        array[i] = i;
    }
}

static void InitDownArray(int *array, size_t size)
{
    size_t i = 0, j = 0;
    
    assert(NULL != array);
    
    for (i = 0, j = size - 1; j >= 0 && i < size; ++i, --j)
    {
        array[i] = j;
    }
}

static int CmpAbeforeB(const void *data_a, const void *data_b)
{
    assert(NULL != data_a);
    assert(NULL != data_b);
    
    return (*(int *)data_a - *(int *)data_b);
}

static int CmpBbeforeA(const void *data_a, const void *data_b)
{
    assert(NULL != data_a);
    assert(NULL != data_b);
    
    return (*(int *)data_b - *(int *)data_a);
}
