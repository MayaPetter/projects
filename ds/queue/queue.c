#include "queue.h"
#include "../slist/slist.h"
#include "../slist/slist.c"

struct queue
{
	slist_node_t *dummy_head;
	slist_node_t *dummy_end;
	size_t size;
};

queue_t *QueueCreate(void)
{
	queue_t *queue = (queue_t *)malloc(sizeof(queue_t));
	if (NULL == queue)
	{
		return NULL;
	}
	
	queue->dummy_end = SListCreateNode(NULL, NULL); 
	queue->dummy_head = SListCreateNode(NULL, queue->dummy_end);
	if (NULL == queue->dummy_end || NULL == queue->dummy_head)
	{
		return NULL;
	} 
   
   	queue->size = 0;

	return queue;
}

void QueueDestroy(queue_t *queue)
{
	assert(NULL != queue);

	SListFreeAll(queue->dummy_head);
	free(queue); queue = NULL;
}

int QueueEnqueue(queue_t *queue, const void *data)
{
	slist_node_t *new = SListCreateNode((void *)data, NULL);
	slist_node_t *temp = SListInsert(queue->dummy_end, new);								 
	
	assert(NULL != queue);
	
	if (NULL == new)
	{
		return FAIL;
	}
    
    /* after swapping data assigning new node to end and end to new*/
	queue->dummy_end = new;
	new = temp;
	
	if (new->next_node != queue->dummy_end)
	{
		return FAIL;
	}
    
	++queue->size;

	return SUCCESS;
}

int QueueDequeue(queue_t *queue)
{
	slist_node_t *removed = NULL; 
	slist_node_t *next = queue->dummy_head->next_node->next_node;
	
	if (0 == queue->size)
	{
		return FAIL;
	}
	 
	removed = SListRemoveAfter(queue->dummy_head);	
	assert(NULL != removed);

	free(removed); removed = NULL;
	
	if (queue->dummy_head->next_node != next)
	{
	    return FAIL;
	}    
	--queue->size;
	
	return SUCCESS;
}

void *QueuePeek(const queue_t *queue)
{
	assert(NULL != queue);
	
	if (1 == QueueIsEmpty(queue))
	{
	    return NULL;
	}    
	
	return queue->dummy_head->next_node->data;
}

size_t QueueSize(const queue_t *queue)
{
	return queue->size;
}

int QueueIsEmpty(const queue_t *queue)
{
	assert(NULL != queue);
	
	return (0 == queue->size);
}

queue_t *QueueAppend(queue_t *dest, queue_t *src)
{
    slist_node_t *temp = NULL;
    slist_node_t *src_end = NULL;
        
    assert(NULL != dest);
    assert(NULL != src);
    
    dest->size = QueueSize(dest) + QueueSize(src);
    src->size = 0;
    
    temp = src->dummy_head->next_node->next_node;
    src_end = src->dummy_end;
    
    SListInsert(dest->dummy_end, src->dummy_head->next_node);
   
    /* update queue managment list */ 
    dest->dummy_end->next_node = temp;
    dest->dummy_end = src_end;
    src->dummy_end = temp;
    
    return dest;
}

