#include <assert.h> /* assert */

#include "queue.h"  /* queue functions */

void QueueCreateAndDestroyTest();
void QueueEnqueueAndSizeTest();
void QueueDequeuePeekAndSizeTest();
void QueueAppendTest();

int main()
{
    /*QueueCreateAndDestroyTest();
    QueueEnqueueAndSizeTest();*/
    QueueDequeuePeekAndSizeTest();
    QueueAppendTest();
    
    return 0;
}

void QueueCreateAndDestroyTest()
{
    queue_t *queue = QueueCreate(); 
	assert(NULL != queue);
	
	QueueDestroy(queue); queue = NULL;
}

void QueueEnqueueAndSizeTest()
{
    int a = 1, b = 2, c = 3, d = 4;
	queue_t *queue = QueueCreate();
	assert(NULL != queue);
	
	assert(1 == QueueIsEmpty(queue));
	assert(0 == QueueSize(queue));
	QueueEnqueue(queue, &a);
	assert(1 == QueueSize(queue));
	QueueEnqueue(queue, &b);
	assert(2 == QueueSize(queue));
	QueueEnqueue(queue, &c);
	assert(3 == QueueSize(queue));
	QueueEnqueue(queue, &d);
	assert(4 == QueueSize(queue));
	assert(0 == QueueIsEmpty(queue));
	QueueDestroy(queue); queue = NULL;
}

void QueueDequeuePeekAndSizeTest()
{
    int a = 1, b = 2, c = 3, d = 4;
	queue_t *queue = QueueCreate();
	assert(NULL != queue);
	
	assert(1 == QueueIsEmpty(queue));
	QueueEnqueue(queue, &a);
	assert(a == *(int *)QueuePeek(queue));
	QueueEnqueue(queue, &b);
	QueueEnqueue(queue, &c);
	QueueEnqueue(queue, &d);
	assert(a == *(int *)QueuePeek(queue));
	assert(0 == QueueIsEmpty(queue));
	
    QueueDequeue(queue);
    assert(b == *(int *)QueuePeek(queue));
    QueueDequeue(queue);
    assert(c == *(int *)QueuePeek(queue));
    QueueDequeue(queue);
    assert(d == *(int *)QueuePeek(queue));
    assert(0 == QueueIsEmpty(queue));
    assert(1 == QueueSize(queue));
    QueueDequeue(queue);
    assert(1 == QueueIsEmpty(queue));
    assert(0 == QueueSize(queue));
    assert(FAIL == QueueDequeue(queue));
	
	QueueDestroy(queue); queue = NULL;
}


void QueueAppendTest()
{ 
    int a = 1, b = 2, c = 3, d = 4, e = 5, f = 6, g = 7;
	queue_t *dest = QueueCreate();
	queue_t *src = QueueCreate();
	
	QueueEnqueue(dest, &a);
	QueueEnqueue(dest, &b);
	QueueEnqueue(dest, &c);
	QueueEnqueue(dest, &d);
	assert(4 == QueueSize(dest));
	assert(a == *(int *)QueuePeek(dest));
	
	QueueEnqueue(src, &e);
	QueueEnqueue(src, &f);
	QueueEnqueue(src, &g);
	QueueEnqueue(src, &g);
    assert(4 == QueueSize(src));
    assert(e == *(int *)QueuePeek(src));
    
    assert(NULL == QueueAppend(dest, dest));
    QueueAppend(dest, src);
    assert(8 == QueueSize(dest));
	assert(a == *(int *)QueuePeek(dest));
	assert(0 == QueueSize(src));
	assert(1 == QueueIsEmpty(src));
	assert(NULL == QueuePeek(src));
	
	assert(FAIL == QueueDequeue(src));
    assert(a == *(int *)QueuePeek(dest));
    QueueDequeue(dest);
    assert(b == *(int *)QueuePeek(dest));
	QueueDequeue(dest);
    assert(c == *(int *)QueuePeek(dest));
    QueueDequeue(dest);
    assert(d == *(int *)QueuePeek(dest));
    QueueDequeue(dest);
    assert(e == *(int *)QueuePeek(dest));
    QueueDequeue(dest);
    assert(f == *(int *)QueuePeek(dest));
    QueueDequeue(dest);
    assert(g == *(int *)QueuePeek(dest));
    QueueDequeue(dest);
    assert(g == *(int *)QueuePeek(dest));
    QueueDequeue(dest);
    assert(0 == QueueSize(dest));
	assert(1 == QueueIsEmpty(dest));
	
    QueueDestroy(src); src = NULL;
    QueueDestroy(dest); dest = NULL;	
}
