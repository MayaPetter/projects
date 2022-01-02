#include <assert.h> /* assert */
#include <stdio.h>
#include "pqueue.h"

#define UNUSED(x) (void)(x)

static pqueue_t *CreatQueueSix(int array[]);
static void PQCreatDestroyTest();
int Comparator(void *p1, void *p2, const void *param);
static void PQueueDequeuePeekTest();
static void PQueueEnqueuePeekTest();
static void PQueueClearTest();
static void PQEraseTest();
int IsMatch(void *q_data, const void *user_data, void *param);

static int ARRAY[] = {1, 3, 4, 2, 6, 5};

int main()
{
	PQCreatDestroyTest();
	PQueueEnqueuePeekTest();
	PQueueDequeuePeekTest();
	PQueueClearTest();
	PQEraseTest();
	
	return 0;
}

static pqueue_t *CreatQueueSix(int array[])
{
    size_t i = 0;
    int enqueue_status = PQUEUE_SUCCESS;
    pqueue_t *pqueue = PQueueCreate(Comparator, NULL);
    if (NULL == pqueue)
	{
		return NULL; 
	}
	
    for (i = 0; (i < 6) && (PQUEUE_FAILED != enqueue_status); ++i)
    {
        enqueue_status = PQueueEnqueue(pqueue, &array[i]);
        assert(PQUEUE_FAILED != enqueue_status);
    }
    
    return pqueue;    
}

static void PQCreatDestroyTest()
{
	pqueue_t *pqueue = PQueueCreate(Comparator, NULL);

	assert(0 == PQueueSize(pqueue));
	assert(1 == PQueueIsEmpty(pqueue));
	
	PQueueDestroy(pqueue); pqueue = NULL;		
}

static void PQueueEnqueuePeekTest()
{
    pqueue_t *pqueue = PQueueCreate(Comparator, NULL);
    assert(NULL != pqueue);
	
	assert(PQUEUE_SUCCESS == PQueueEnqueue(pqueue, &ARRAY[1]));
	assert(ARRAY[1] == *(int *)PQueuePeek(pqueue));
	assert(PQUEUE_SUCCESS == PQueueEnqueue(pqueue, &ARRAY[3]));
	assert(ARRAY[3] == *(int *)PQueuePeek(pqueue));
	assert(PQUEUE_SUCCESS == PQueueEnqueue(pqueue, &ARRAY[5]));
	assert(ARRAY[3] == *(int *)PQueuePeek(pqueue));
	assert(PQUEUE_SUCCESS == PQueueEnqueue(pqueue, &ARRAY[0]));
	assert(ARRAY[0] == *(int *)PQueuePeek(pqueue));
	assert(PQUEUE_SUCCESS == PQueueEnqueue(pqueue, &ARRAY[2]));
	assert(ARRAY[0] == *(int *)PQueuePeek(pqueue));
	assert(PQUEUE_SUCCESS == PQueueEnqueue(pqueue, &ARRAY[4]));
	assert(ARRAY[0] == *(int *)PQueuePeek(pqueue));
	
	assert(6 == PQueueSize(pqueue));
	assert(0 == PQueueIsEmpty(pqueue));
	
	PQueueDestroy(pqueue); pqueue = NULL;
} 

static void PQueueDequeuePeekTest()
{
    pqueue_t *pqueue = CreatQueueSix(ARRAY);
    assert(NULL != pqueue);
    
    assert(6 == PQueueSize(pqueue));
	assert(0 == PQueueIsEmpty(pqueue));
	
	assert(ARRAY[0] == *(int *)PQueuePeek(pqueue));
	PQueueDequeue(pqueue);
	assert(ARRAY[3] == *(int *)PQueuePeek(pqueue));
	PQueueDequeue(pqueue);
	assert(ARRAY[1] == *(int *)PQueuePeek(pqueue));
	PQueueDequeue(pqueue);
	assert(ARRAY[2] == *(int *)PQueuePeek(pqueue));
	PQueueDequeue(pqueue);
	assert(ARRAY[5] == *(int *)PQueuePeek(pqueue));
	PQueueDequeue(pqueue);
	assert(ARRAY[4] == *(int *)PQueuePeek(pqueue));
	PQueueDequeue(pqueue);

    assert(0 == PQueueSize(pqueue));
	assert(1 == PQueueIsEmpty(pqueue));
	
    PQueueDestroy(pqueue); pqueue = NULL;
}

void PQueueClearTest()
{
    pqueue_t *pqueue = CreatQueueSix(ARRAY);
    assert(NULL != pqueue);
    
    assert(6 == PQueueSize(pqueue));
	assert(0 == PQueueIsEmpty(pqueue));
	
    PQueueClear(pqueue);	

    assert(0 == PQueueSize(pqueue));
	assert(1 == PQueueIsEmpty(pqueue));
	
	PQueueDestroy(pqueue); pqueue = NULL;
}

static void PQEraseTest()
{
    pqueue_t *pqueue = CreatQueueSix(ARRAY);
    int not_found = 10;
    assert(NULL != pqueue);
    
    assert(6 == PQueueSize(pqueue));
	assert(0 == PQueueIsEmpty(pqueue));
	
    assert(ARRAY[2] == *(int *)PQueueErase(pqueue, IsMatch, &ARRAY[2], NULL));
    assert(5 == PQueueSize(pqueue));
    
    assert(ARRAY[0] == *(int *)PQueueErase(pqueue, IsMatch, &ARRAY[0], NULL));
    assert(4 == PQueueSize(pqueue));
    
    assert(ARRAY[4] == *(int *)PQueueErase(pqueue, IsMatch, &ARRAY[4], NULL));
    assert(3 == PQueueSize(pqueue));
    
    assert(NULL == PQueueErase(pqueue, IsMatch, &not_found, NULL));

    PQueueDestroy(pqueue); pqueue = NULL;
}

int Comparator(void *p1, void *p2, const void *param)
{
	UNUSED(param);
		
    /* return 1 if data1 is before data2 or 0 if data1 is after data2 */
	return (*(int *)p2 - *(int *)p1);
}

int IsMatch(void *q_data, const void *user_data, void *param)
{
	UNUSED(param);
    /* return 1 if q_data and user_data match, 0 otherwise */
    return (*(int *)q_data == *(int *)user_data);
}

