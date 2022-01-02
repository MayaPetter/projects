/******************************************
* Author:   Maya Petter                   *
* Reviewer: Liliya Melmed                 *
* Status:   S                             *
* Purpose:  circular buffer               *
*******************************************/
#include "circ_buff.h"
#include <assert.h> /* assert */
#include <stdio.h> /* puts */
enum boolen {false, true};

circ_buff_t *CircBuffCreateTest(size_t init_capacity);
void CircBuffWriteTest(circ_buff_t *circ_buff, const void *src, size_t count, size_t excpect);
void CircBuffReadTest(circ_buff_t *circ_buff, void *dest, size_t count, size_t excpect);
void CircBuffIsEmptyTest(const circ_buff_t *circ_buff, int is_empty);
void CircBuffFreeSpaceTest(const circ_buff_t *circ_buff, size_t size_free);
void CircBuffCapacityTest(const circ_buff_t *circ_buff, size_t capacity);


int main()
{
	size_t capacity = 8;
	size_t size_free = 8;
	circ_buff_t *circ_buff = CircBuffCreateTest(capacity);
	char src[] = "what's up with this one?";
	char dest[20] ="";
	
	CircBuffIsEmptyTest(circ_buff, true);
	CircBuffCapacityTest(circ_buff, capacity);
	CircBuffFreeSpaceTest(circ_buff, size_free);
	CircBuffReadTest(circ_buff, dest, 1, 0);
	CircBuffDestroy(circ_buff);
	
	capacity = 0;
	size_free = 0;
	
	circ_buff = CircBuffCreateTest(capacity);
	CircBuffIsEmptyTest(circ_buff, true);
	CircBuffCapacityTest(circ_buff, capacity);
	CircBuffFreeSpaceTest(circ_buff, size_free);
	CircBuffDestroy(circ_buff);
	
	capacity = 20;
	size_free = 20;
	
	circ_buff = CircBuffCreateTest(capacity);
	CircBuffIsEmptyTest(circ_buff, true);
	CircBuffCapacityTest(circ_buff, capacity);
	CircBuffFreeSpaceTest(circ_buff, size_free);
	CircBuffWriteTest(circ_buff, src, 30, 20);
	CircBuffReadTest(circ_buff, dest, 30, 20);
	CircBuffWriteTest(circ_buff, src, 4, 4);
	CircBuffReadTest(circ_buff, dest, 1, 1);
	CircBuffIsEmptyTest(circ_buff, false);
	CircBuffCapacityTest(circ_buff, capacity);
	CircBuffFreeSpaceTest(circ_buff, 17);
	CircBuffReadTest(circ_buff, dest, 3, 3);
	CircBuffReadTest(circ_buff, dest, 1, 0);
	CircBuffWriteTest(circ_buff, src, 21, 20);
	CircBuffDestroy(circ_buff);
	puts("good");
	
	return 0;
}

circ_buff_t *CircBuffCreateTest(size_t init_capacity)
{
	circ_buff_t *circ_buff = CircBuffCreate(init_capacity);
    assert(NULL != circ_buff);
    
    return circ_buff;
}

void CircBuffWriteTest(circ_buff_t *circ_buff, const void *src, size_t count, size_t excpect)
{
	assert(excpect == CircBuffWrite(circ_buff, src, count));
}

void CircBuffReadTest(circ_buff_t *circ_buff, void *dest, size_t count, size_t excpect)
{
	assert(excpect == CircBuffRead(circ_buff, dest, count));
}
	
void CircBuffIsEmptyTest(const circ_buff_t *circ_buff, int is_empty)
{
	assert(is_empty == CircBuffIsEmpty(circ_buff));
}

void CircBuffFreeSpaceTest(const circ_buff_t *circ_buff, size_t size_free)
{
	assert(size_free == CircBuffFreeSpace(circ_buff));

}

void CircBuffCapacityTest(const circ_buff_t *circ_buff, size_t capacity)
{
	assert(capacity == CircBuffCapacity(circ_buff));
}

