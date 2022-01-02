#include "stack.h"
#include <stdio.h> /*printf*/
#include <stddef.h> /* size_t */

struct stack
{
	void *base;
	void *curr;
	void *end;
	size_t size_of_element;	
};

int main()
{
	int element = 55;
	size_t i = 0;
	
	stack_t *stack = StackCreate(20, sizeof(int));
	puts("\n===== before PUSH #1 (20 ints)\n");
	printf("base: %p\n", stack->base);
	printf("curr: %p\n", stack->curr);
	printf("end : %p\n", stack->end);
	printf("Size: %lu |  ", StackSize(stack));
	printf("IsEmpty: %d\n", StackIsEmpty(stack));
	
	puts("\n===== loop 20 times push and peek\n");
	for (i = 0; i < 20; ++i)
	{
		StackPush(stack, &element);
		printf("base: %p\n", stack->base);
		printf("curr: %p\n", stack->curr);
		printf("end : %p\n", stack->end);
		printf("PEEK: %d | ", *(int *)StackPeek(stack));
		printf("Size: %lu |  ", StackSize(stack));
		printf("IsEmpty: %d\n", StackIsEmpty(stack));
	}
	
	element = 69;
	
	printf("\nSTACK_OVERFLOW_ERROR: %d\n", StackPush(stack, &element));
	
	puts("\n===== loop 20 times push and peek\n");
	for (i = 0; i < 20; ++i)
	{
		StackPop(stack);
		printf("base: %p\n", stack->base);
		printf("curr: %p\n", stack->curr);
		printf("end : %p\n", stack->end);
		printf("PEEK: %d | ", *(int *)StackPeek(stack));
		printf("Size: %lu |  ", StackSize(stack));
		printf("IsEmpty: %d\n", StackIsEmpty(stack));
	}
	printf("\nSTACK_UNDERFLOW_ERROR: %d\n", StackPop(stack));
	
	puts("\n===== Stack Destroy\n");
	StackDestroy(stack);
	
	printf("Size: %lu \n  ", StackSize(stack));
	printf("PEEK: %d \n", *(int *)StackPeek(stack));
	printf("push after destroy: %d\n", StackPush(stack, &element));
	printf("pull after destroy: %d\n", StackPop(stack));
	
	StackDestroy(stack);
	
    return 0;
}



