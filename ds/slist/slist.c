#include "slist.h"
#include <assert.h> /* assert */
#include <stdlib.h> /* free, malloc */

slist_node_t *SListCreateNode(void *data, slist_node_t *next)
{	
	slist_node_t *node = NULL;
	
	/* allocating memory for new node */
	node = (slist_node_t *)malloc(sizeof(slist_node_t));
	
	/* initializing node members */
	node->data = data;
	node->next_node = next;
	
	return node;
}

void SListFreeAll(slist_node_t *head)
{
	assert(1 != SListHasLoop(head));
	
	while (NULL != head)
	{
		slist_node_t *next = head->next_node;
		free(head); head = NULL;
		head = next;
	}
}


slist_node_t *SListInsertAfter(slist_node_t *current_node, 
                               slist_node_t *new_node)
{
    assert(NULL != current_node);
	assert(NULL != new_node);
	
	new_node->next_node =  current_node->next_node;
	current_node->next_node = new_node;
    
    return current_node;
}

slist_node_t *SListInsert(slist_node_t *current_node, slist_node_t *new_node)
{
    void *temp_data = NULL;
    
	assert(NULL != current_node);
	assert(NULL != new_node);
	
	temp_data = current_node->data;
	
	/* insert after - new node will point to current */
	current_node = SListInsertAfter(current_node, new_node);
	
	/* swapping data */
	current_node->data = new_node->data;
	new_node->data = temp_data;
    
    return current_node;
}

slist_node_t *SListFind(slist_node_t *head, cmp_func cmp, const void *data)
{
    assert(1 != SListHasLoop(head));
    
    while ((NULL != head) && (0 != cmp(head->data, data)))
    {
        head = head->next_node;
    }       
    
    return head;

}

slist_node_t *SListRemoveAfter(slist_node_t *head)
{
	slist_node_t *removed = head->next_node;
   
    if (NULL == removed)
    {
        return NULL;
    }
    
    head->next_node = removed->next_node;		
    removed->next_node = NULL;
    
    return removed;
}

slist_node_t *SListRemove(slist_node_t *head)
{
    void *temp_data = head->data;
	slist_node_t *removed = head->next_node;
    if (NULL == removed)
    {
        return NULL;
    }

	head->data = removed->data;
	head->next_node = removed->next_node;
	
	removed->data = temp_data;
	removed->next_node = NULL; 
	
    return removed; 
}

size_t SListCount(const slist_node_t *head)
{
    size_t counter = 0;
    
    assert(1 != SListHasLoop(head));
    
    while (NULL != head)
    {
        head = head->next_node;
        ++counter;
    }       
    
    return counter;
}


int SListForEach(slist_node_t *head, func_action func, void *param)
{
    int func_status = 0;
    
    assert(NULL != head);
    assert(1 != SListHasLoop(head));
     

    while ((NULL != head)  && (0 == func_status))
    {
    	func_status = func(head->data, param);
        head = head->next_node;
    }       
    return func_status;
}

slist_node_t *SListFlip(slist_node_t *head)
{
    
    slist_node_t *temp_head = NULL;
    slist_node_t *temp_next = NULL;
    
    assert(NULL != head);
	assert(1 != SListHasLoop(head));
	
	temp_head = head;
	temp_next = head->next_node;   
    head->next_node = NULL; 
    head = temp_next;
    
    while (NULL != head->next_node)
    {
        /* assign the next of current to temp */
        temp_next = head->next_node;
        head->next_node = temp_head; 
        
		temp_head = head;
        head = temp_next;
    }
    
    if (NULL == head->next_node)
    {
    	head->next_node = temp_head;
    }
    
	return head;
}

int SListHasLoop(const slist_node_t *head)
{
	slist_node_t *slow = (slist_node_t *)head;
	slist_node_t *fast = (slist_node_t *)head;	
	
	int has_loop = 0;
	
	while (NULL != fast && NULL != fast->next_node)
	{
		slow = slow->next_node;	
		fast = fast->next_node->next_node;
		
		if (slow == fast)
		{
			has_loop = 1;
			break;
		}
	} 
	
	return has_loop;    
}

slist_node_t *SListFindIntersection(const slist_node_t *node1,
									const slist_node_t *node2)
{ 
 	size_t len_1 = SListCount(node1);
	size_t len_2 = SListCount(node2);
	
	while (len_1 < len_2)
	{
		node2 = (slist_node_t *)node2->next_node;
		--len_2;
	}
	
	while (len_1 > len_2)
	{
		node1 = (slist_node_t *)node1->next_node;
		--len_1;
	}
	
	if (node1 == node2)
	{
		return NULL;	
	}
	
	while (NULL != node1)
	{
		node1 = (slist_node_t *)node1->next_node;
		node2 = (slist_node_t *)node2->next_node;
		if(node1 == node2)
		{
			break;
		}
	}

	return (slist_node_t *)node1;
}
