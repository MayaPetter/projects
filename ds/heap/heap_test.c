#include <assert.h>  /* assert */

#include "heap.h"
#include "vector.h"

#define UNUSED(x) ((void)(x))

/******** compare functions ***************************************************/
int Cmp(const void *new_data, const void *heap_data, void *param)
{
    assert(NULL != new_data);
    assert(NULL != heap_data);
    UNUSED(param);

    return (*(int *)heap_data - *(int *)new_data); 
}

int IsMatch(const void *heap_data, const void *user_data)
{
    assert(NULL != heap_data);
    assert(NULL != user_data);

    return (*(int *)heap_data == *(int *)user_data);
}

/******** test functions ******************************************************/
static void CreateDestroyTest();
static void PushTest();
static void PopTest();
static void PeekTest();
static void RemoveTest();
static void RemoveAndPeekTest();

int main()
{
/*    CreateDestroyTest();*/
/*    PushTest();*/
/*    PopTest();*/
/*    PeekTest();*/
    RemoveTest();
/*    RemoveAndPeekTest();*/
    
    return 0;
}

/******** test functions ******************************************************/
static void CreateDestroyTest()
{
    heap_t *heap = HeapCreate(Cmp, NULL);
    
    assert(NULL != heap);
    assert(HeapIsEmpty(heap));
    assert(0 == HeapSize(heap));

    HeapDestroy(heap);
}

static void PushTest()
{
    int array[] = {1, 3, 4, 5, 2, 6, 9, 7, 8};
    size_t i = 0;
    size_t size = 9;
    
    heap_t *heap = HeapCreate(Cmp, NULL);
    assert(NULL != heap);
    assert(HeapIsEmpty(heap));
    assert(0 == HeapSize(heap));
    
    for (i = 0; i < size; ++i)
    {
        assert(0 == HeapPush(heap, &array[i]));
        assert((i + 1) == HeapSize(heap));
    }
    
    assert(!HeapIsEmpty(heap));
    HeapDestroy(heap);
}

static void PopTest()
{
    int array[] = {1, 3, 4, 5, 2, 6, 9, 7, 8};
    size_t i = 0;
    size_t size = 9;
    
    heap_t *heap = HeapCreate(Cmp, NULL);
    assert(NULL != heap);
    assert(HeapIsEmpty(heap));
    assert(0 == HeapSize(heap));
    
    for (i = 0; i < size; ++i)
    {
        assert(0 == HeapPush(heap, &array[i]));
        assert((i + 1) == HeapSize(heap));
    }
    
    assert(!HeapIsEmpty(heap));

    for (i = 1; i <= size; ++i)
    {
        HeapPop(heap);
        assert((size - i) == HeapSize(heap));
    }
    
    assert(0 == HeapSize(heap));
    
    HeapDestroy(heap);
}

static void PeekTest()
{
    int array[] = {1, 3, 4, 5, 2, 6, 9, 7, 8};
    
    heap_t *heap = HeapCreate(Cmp, NULL);
    assert(NULL != heap);
    assert(HeapIsEmpty(heap));
    assert(0 == HeapSize(heap));
    
    assert(0 == HeapPush(heap, &array[0]));
    assert(array[0] == *(int *)HeapPeek(heap));

    assert(0 == HeapPush(heap, &array[1]));
    assert(array[1] == *(int *)HeapPeek(heap));
    
    assert(0 == HeapPush(heap, &array[2]));
    assert(array[2] == *(int *)HeapPeek(heap));
    
    assert(0 == HeapPush(heap, &array[3]));
    assert(array[3] == *(int *)HeapPeek(heap));
    
    assert(0 == HeapPush(heap, &array[4]));
    assert(array[3] == *(int *)HeapPeek(heap));
    
    assert(0 == HeapPush(heap, &array[5]));
    assert(array[5] == *(int *)HeapPeek(heap));
    
    assert(0 == HeapPush(heap, &array[6]));
    assert(array[6] == *(int *)HeapPeek(heap));
    
    assert(0 == HeapPush(heap, &array[7]));
    assert(array[6] == *(int *)HeapPeek(heap));
    
    assert(0 == HeapPush(heap, &array[8]));
    assert(array[6] == *(int *)HeapPeek(heap));
    
    assert(!HeapIsEmpty(heap));
    assert(9 == HeapSize(heap));    
        
    HeapDestroy(heap);
}

static void RemoveTest()
{
    int array[] = {1, 3, 4, 5, 2, 6, 9, 7, 8};
    size_t i = 0;
    size_t size = 9;
    
    heap_t *heap = HeapCreate(Cmp, NULL);
    assert(NULL != heap);
    assert(HeapIsEmpty(heap));
    assert(0 == HeapSize(heap));
    
    for (i = 0; i < size; ++i)
    {
        assert(0 == HeapPush(heap, &array[i]));
        assert((i + 1) == HeapSize(heap));
    }
    
    for (i = 0; i < size; ++i)
    {
        assert(array[i] == *(int *)HeapRemove(heap, IsMatch, &array[i]));
        assert(((size - 1) - i ) == HeapSize(heap));
    }
    
    assert(HeapIsEmpty(heap));  
    HeapDestroy(heap);
}

static void RemoveAndPeekTest()
{
    int array[] = {1, 3, 4, 5, 2, 6, 9, 7, 8};
    size_t i = 0;
    size_t size = 9;
    
    heap_t *heap = HeapCreate(Cmp, NULL);
    assert(NULL != heap);
    assert(HeapIsEmpty(heap));
    assert(0 == HeapSize(heap));
    
    for (i = 0; i < size; ++i)
    {
        assert(0 == HeapPush(heap, &array[i]));
        assert((i + 1) == HeapSize(heap));
    }
    assert(!HeapIsEmpty(heap));
    assert(9 == HeapSize(heap));    
    
    assert(array[7] == *(int *)HeapRemove(heap, IsMatch, &array[7]));
/*    assert(array[6] == *(int *)HeapPeek(heap));*/
    
/*    assert(array[8] == *(int *)HeapRemove(heap, IsMatch, &array[8]));*/
/*    assert(array[6] == *(int *)HeapPeek(heap));*/
/*    */
/*    assert(array[6] == *(int *)HeapRemove(heap, IsMatch, &array[6]));*/
/*    assert(array[5] == *(int *)HeapPeek(heap));*/
/*    */
/*    assert(array[4] == *(int *)HeapRemove(heap, IsMatch, &array[4]));*/
/*    assert(array[5] == *(int *)HeapPeek(heap));*/

/*    assert(array[0] == *(int *)HeapRemove(heap, IsMatch, &array[0]));*/
/*    assert(array[5] == *(int *)HeapPeek(heap));*/

/*    assert(array[5] == *(int *)HeapRemove(heap, IsMatch, &array[5]));*/
/*    assert(array[3] == *(int *)HeapPeek(heap));*/

/*    assert(array[1] == *(int *)HeapRemove(heap, IsMatch, &array[1]));*/
/*    assert(array[3] == *(int *)HeapPeek(heap));*/
/*    */
/*    assert(array[2] == *(int *)HeapRemove(heap, IsMatch, &array[2]));*/
/*    assert(array[3] == *(int *)HeapPeek(heap));*/
/*    */
/*    assert(array[3] == *(int *)HeapRemove(heap, IsMatch, &array[3]));*/
/*    */
/*    assert(HeapIsEmpty(heap));*/
/*    assert(0 == HeapSize(heap));    */
        
    HeapDestroy(heap);
}

