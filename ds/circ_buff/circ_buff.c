/******************************************
* Author:   Maya Petter                   *
* Reviewer: Liliya Melmed                 *
* Status:   S                             *
* Purpose:  circular buffer               *
*******************************************/

#include "circ_buff.h"
#include <stdlib.h> /* malloc, free */
#include <assert.h> /* assert */

#define PEDDING_SIZE 8

struct buffer
{
	size_t write;
	size_t read;
	size_t capacity;
	char buffer[1];
};

circ_buff_t *CircBuffCreate(size_t init_capacity)
{
	circ_buff_t *circ_buff = NULL;
	size_t capacity = init_capacity;
	
	/* if init_capacity is less then 8 the pedding will be enough */
	if( capacity <= PEDDING_SIZE)
	{
		capacity = 0;
	}
	else
	{
		capacity -= PEDDING_SIZE;
	}
	
	/* allocate circ_buff_t by init_capacity */
	circ_buff = (circ_buff_t *)malloc(sizeof(circ_buff_t) + capacity); 
	
	/* if allocation succssed initialze members */
	if(NULL != circ_buff)
	{
		circ_buff->write = 0;
		circ_buff->read = 0;
		circ_buff->capacity = init_capacity;
	}
	
	return  circ_buff;
}

void CircBuffDestroy(circ_buff_t *circ_buff)
{
	assert(NULL != circ_buff);
	
	free(circ_buff); circ_buff = NULL;
}

size_t CircBuffWrite(circ_buff_t *circ_buff, const void *src, size_t count)
{
	size_t i = 0;

	assert(NULL != circ_buff);
	assert(NULL != src);

	/* write until reaching count and there is free space */
	for(i = 0; i < count && (0 != CircBuffFreeSpace(circ_buff)); ++i)
	{
		size_t tmp = circ_buff->write % circ_buff->capacity;
		
		circ_buff->buffer[tmp] = *((char *)src + i);

		++circ_buff->write;
	}

	return i;
}

size_t CircBuffRead(circ_buff_t *circ_buff, void *dest, size_t count)
{
	size_t i = 0;

	assert(NULL != circ_buff);
	assert(NULL != dest);

	/* read until reaching count or until read reaches write */
	for(i = 0; (i < count) && (circ_buff->read < circ_buff->write); ++i)
	{
		size_t tmp = circ_buff->read % circ_buff->capacity;

		*((char *)dest + i) = circ_buff->buffer[tmp];

		++circ_buff->read;
	}

	return i;
}

int CircBuffIsEmpty(const circ_buff_t *circ_buff)
{
	assert(NULL != circ_buff);
	
	/*if buffer is empty will return  1, else 0 */
	return circ_buff->write == circ_buff->read;
}

size_t CircBuffCapacity(const circ_buff_t *circ_buff)
{
	assert(NULL != circ_buff);

	return circ_buff->capacity;
}

size_t CircBuffFreeSpace(const circ_buff_t *circ_buff)
{
	assert(NULL != circ_buff);
	
	return circ_buff->capacity - (circ_buff->write - circ_buff->read);
}

