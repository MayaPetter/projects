#include "stack.h"
#include <assert.h> /* assert */
#include <stddef.h> /* size_t */
#include <string.h> /* memcpy */
#include <stdlib.h> /* malloc, free */

struct stack
{
	void *base;
	void *current;
	void *end;
	size_t size_of_element;
};


stack_t *StackCreate(size_t max_num_of_elements, size_t size_of_element)
{
	/* allocating memory for the stack mangment*/
	stack_t *stack = (stack_t *)malloc(sizeof(stack_t));
	
	if (NULL != stack)
	{	
		/* allocating memory for the stack*/
		stack->base = malloc(max_num_of_elements * size_of_element);

		if (NULL != stack->base)
		{
			/*initializing stack members */
			stack->current = stack->base;
			stack->end = (char *)(stack->base) + (max_num_of_elements 
														* size_of_element);
			stack->size_of_element = size_of_element;
		}
		else
		{
			/* if failed to allocate- free memory */
			free(stack); stack = NULL;
		}	
	}
	return stack;
}

void StackDestroy(stack_t *stack)
{
	assert(NULL != stack);
	free(stack->base); stack->base = NULL;
	free(stack); stack = NULL;
}


int StackPush(stack_t *stack, const void *element)
{
	assert(NULL != stack);
	assert(NULL != element);
	
	if (stack->current == stack->end)
	{
		return STACK_OVERFLOW_ERROR;
	}
		
	memcpy(stack->current, element, stack->size_of_element); 	
	
	stack->current = (char *)stack->current + stack->size_of_element;
	
	return STACK_SUCCESS;
}

int StackPop(stack_t *stack)
{
	assert(NULL != stack);
	
	  
	if (stack->current == stack->base)
	{
		return STACK_UNDERFLOW_ERROR;
	}

	stack->current = (char *)stack->current - stack->size_of_element;

	return  STACK_SUCCESS;
}


void *StackPeek(const stack_t *stack)
{
	assert(NULL != stack);
	
	return ((char *)(stack->current) - stack->size_of_element);
}

int StackIsEmpty(const stack_t *stack)
{
	assert(NULL != stack);
	
	return (stack->current == stack->base);
}

size_t StackSize(const stack_t *stack)
{
	assert(NULL != stack);
	
	return (((char *)stack->current - (char *)stack->base) 
								 / stack->size_of_element);
}

	

