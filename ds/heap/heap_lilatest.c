#include <stdio.h>
#include <assert.h> /*assert*/
#include <string.h>

#include "heap.h"

#define UNUSED(x) (void)(x)

#define Expect(x) {                                                \
    if (!(x))                                                    \
    {                                                            \
        fprintf(stderr, "Test failed in line %d\n", __LINE__);    \
        return 0;                                                \
    } } /* end of Expects(x) */
    
    
void CreateDestroy();
int PushSizeSize();
int Pop();
int Peek();
int Remove();

int priority_func(const void *num1, const void *num2, void *param)
{
	UNUSED(param);

	return (*(int *)num2 - *(int *)num1);
}

int is_match(const void *heap_data, const void *user_data)
{
	return (*(int *)heap_data == *(int *)user_data);
}

int main()
{	
	CreateDestroy();
	PushSizeSize();
	Pop();
	Peek();
	Remove();
	
	return 0;

}

void CreateDestroy()
{
	heap_t *heap = HeapCreate(priority_func, NULL);
	
	HeapDestroy(heap);
}

int PushSizeSize()
{
	heap_t *heap = HeapCreate(priority_func, NULL);
	int num1 = 1;
	int num2 = 2;
	int num3 = 3;
	int num4 = 4;
	int num5 = 5;
	int num6 = 6;
	int num7 = 7;
	int num8 = 8;
	int num9 = 9;
	int num10 = 10;
	
	Expect(0 == HeapSize(heap))

	Expect (1 == HeapIsEmpty(heap))

	HeapPush(heap, &num1);
	HeapPush(heap, &num2);
	HeapPush(heap, &num3);
	HeapPush(heap, &num4);
	HeapPush(heap, &num5);
	HeapPush(heap, &num6);
	HeapPush(heap, &num7);
	HeapPush(heap, &num8);
	HeapPush(heap, &num9);
	
	Expect (0 == HeapPush(heap, &num10))
	
	Expect (10 == HeapSize(heap))
	
	Expect (0 == HeapIsEmpty(heap))

	HeapDestroy(heap);
	
	return 0;
}

int Pop()
{
	heap_t *heap = HeapCreate(priority_func, NULL);
	int num1 = 1;
	int num2 = 2;
	int num3 = 3;
	int num4 = 4;
	int num5 = 5;
	int num6 = 6;
	int num7 = 7;
	int num8 = 8;
	int num9 = 9;
	int num10 = 10;
	
	Expect(0 == HeapSize(heap));
	
	HeapPush(heap, &num1);
	HeapPush(heap, &num2);
	HeapPush(heap, &num3);
	HeapPush(heap, &num4);
	HeapPush(heap, &num5);
	HeapPush(heap, &num6);
	HeapPush(heap, &num7);
	HeapPush(heap, &num8);
	HeapPush(heap, &num9);
	HeapPush(heap, &num10);
	
	Expect(10 == HeapSize(heap));

	HeapPop(heap);
	HeapPop(heap);
	HeapPop(heap);
	HeapPop(heap);
	HeapPop(heap);
	HeapPop(heap);
	
	Expect(4 == HeapSize(heap))

	HeapDestroy(heap);
	
	return 0;
}

int Peek()
{
	heap_t *heap = HeapCreate(priority_func, NULL);
	int num1 = 1;
	int num2 = 2;
	int num3 = 3;
	int num4 = 4;
	int num5 = 5;
	int num6 = 6;
	int num7 = 7;
	int num8 = 8;
	int num9 = 9;
	int num10 = 10;
	int *data = NULL;
	
	Expect(0 == HeapSize(heap));
	
	HeapPush(heap, &num1);
	data = HeapPeek(heap);
	Expect(1 == *(int *)data)
	
	HeapPush(heap, &num2);
	data = HeapPeek(heap);
	Expect(2 == *(int *)data)
	
	HeapPush(heap, &num3);
	data = HeapPeek(heap);
	Expect(3 == *(int *)data)
	
	HeapPush(heap, &num4);
	data = HeapPeek(heap);
	Expect(4 == *(int *)data)
	
	HeapPush(heap, &num5);
	data = HeapPeek(heap);
	Expect(5 == *(int *)data)
	
	HeapPush(heap, &num6);
	data = HeapPeek(heap);
	Expect(6 == *(int *)data)
	
	HeapPush(heap, &num7);
	data = HeapPeek(heap);
	Expect(7 == *(int *)data)
	
	HeapPush(heap, &num8);
	data = HeapPeek(heap);
	Expect(8 == *(int *)data)
	
	HeapPush(heap, &num9);
	data = HeapPeek(heap);
	Expect(9 == *(int *)data)
	
	HeapPush(heap, &num10);
	data = HeapPeek(heap);
	Expect(10 == *(int *)data)
	
	
	Expect(10 == HeapSize(heap));

	HeapPop(heap);
	data = HeapPeek(heap);
	Expect(9 == *(int *)data)
	
	HeapPop(heap);
	data = HeapPeek(heap);
	Expect(8 == *(int *)data)
	
	HeapPop(heap);
	data = HeapPeek(heap);
	Expect(7 == *(int *)data)
	
	HeapPop(heap);
	data = HeapPeek(heap);
	Expect(6 == *(int *)data)
	
	HeapPop(heap);
	data = HeapPeek(heap);
	Expect(5 == *(int *)data)
	
	HeapPop(heap);
	data = HeapPeek(heap);
	Expect(4 == *(int *)data)
	
	HeapPop(heap);
	data = HeapPeek(heap);
	Expect(3 == *(int *)data)
	
	HeapPop(heap);
	data = HeapPeek(heap);
	Expect(2 == *(int *)data)
	
	HeapPop(heap);
	data = HeapPeek(heap);
	Expect(1 == *(int *)data)
	
	HeapPop(heap);
	
	Expect(0 == HeapSize(heap))

	HeapDestroy(heap);
	
	return 0;
}

int Remove()
{
	heap_t *heap = HeapCreate(priority_func, NULL);
	int num1 = 1;
	int num2 = 2;
	int num3 = 3;
	int num4 = 4;
	int num5 = 5;
	int num6 = 6;
	int num7 = 7;
	int num8 = 8;
	int num9 = 9;
	int num10 = 10;
	int to_remove = 9;
	int *data = NULL;
	
	Expect(0 == HeapSize(heap));
	
	HeapPush(heap, &num1);
	HeapPush(heap, &num2);
	HeapPush(heap, &num3);
	HeapPush(heap, &num4);
	HeapPush(heap, &num5);
	HeapPush(heap, &num6);
	HeapPush(heap, &num7);
	HeapPush(heap, &num8);
	HeapPush(heap, &num9);
	HeapPush(heap, &num10);
	
	Expect(10 == HeapSize(heap));

	data = HeapRemove(heap, is_match, &to_remove);
	Expect(9 == *(int *)data)
	Expect(9 == HeapSize(heap))
	
	to_remove = 7;
	data = HeapRemove(heap, is_match, &to_remove);
	Expect(7 == *(int *)data)
	Expect(8 == HeapSize(heap))
	
	to_remove = 10;
	data = HeapRemove(heap, is_match, &to_remove);
	Expect(10 == *(int *)data)
	Expect(7 == HeapSize(heap))
	
	to_remove = 2;
	data = HeapRemove(heap, is_match, &to_remove);
	Expect(2 == *(int *)data)
	Expect(6 == HeapSize(heap))
	
	to_remove = 23;
	data = HeapRemove(heap, is_match, &to_remove);
	Expect(NULL == data)
	Expect(6 == HeapSize(heap))
	
	to_remove = 6;
	data = HeapRemove(heap, is_match, &to_remove);
	Expect(6 == *(int *)data)
	Expect(5 == HeapSize(heap))
	
	to_remove = 1;
	data = HeapRemove(heap, is_match, &to_remove);
	Expect(1 == *(int *)data)
	Expect(4 == HeapSize(heap))
	
	to_remove = 3;
	data = HeapRemove(heap, is_match, &to_remove);
	Expect(3 == *(int *)data)
	Expect(3 == HeapSize(heap))
	
	to_remove = 4;
	data = HeapRemove(heap, is_match, &to_remove);
	Expect(4 == *(int *)data)
	Expect(2 == HeapSize(heap))
	
	to_remove = 5;
	data = HeapRemove(heap, is_match, &to_remove);
	Expect(5 == *(int *)data)
	Expect(1 == HeapSize(heap))
	
	to_remove = 8;
	data = HeapRemove(heap, is_match, &to_remove);
	Expect(8 == *(int *)data)
	Expect(0 == HeapSize(heap))
	
	HeapDestroy(heap);
	
	return 0;
}
