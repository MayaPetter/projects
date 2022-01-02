#include <assert.h> /* assert*/
#include <stdio.h> /* printf */
#include "slist.h"

void SListCreateNodeTest();
void SListInsertTest();
void SListInsertAfterTest();
void SListFindTest();
static int Cmp(void *head, const void *data);
void SListRemoveAndCountTest();
void SListRemoveAfterAndCountTest();
static int FuncAction(void *data, void *param);
void SListForEachTest();
void SListFlipTest();
void SListHasLoopTest();
void SListFindIntersectionTest();

int main()
{
    SListCreateNodeTest();
    SListInsertTest();
    SListInsertAfterTest();
    SListFindTest();
    SListRemoveAndCountTest();
    SListRemoveAfterAndCountTest();        
    SListForEachTest();
    SListFlipTest();
    SListHasLoopTest();
    SListFindIntersectionTest();
    
    return 0;
}

void SListCreateNodeTest()
{
    int a = 1, b = 2, c = 3, d = 4;
    slist_node_t *new_a = NULL, *new_b = NULL, *new_c = NULL, *new_d = NULL;   
    
    /*creat list of nodes*/
    new_d = SListCreateNode(&d, NULL);
    new_c = SListCreateNode(&c, new_d);
    new_b = SListCreateNode(&b, new_c);
    new_a = SListCreateNode(&a, new_b);
     
    /*creat list of nodes and check if they are linked and contain the correct data*/    
	assert(NULL != new_a);
	assert(NULL != new_b);
	assert(NULL != new_c);
	assert(NULL != new_d);
	
	/* check if nodes are linked in the right order */
	assert(new_a->next_node == new_b);
	assert(new_b->next_node == new_c);
	assert(new_c->next_node == new_d);
	assert(new_d->next_node == NULL);
	
	/* check if nodes contain the correct data */
	assert(a == *(int *)new_a->data);
	assert(b == *(int *)new_b->data);
	assert(c == *(int *)new_c->data);
	assert(d == *(int *)new_d->data);
	
	SListFreeAll(new_a);
}	

void SListInsertTest()
{
    int new_data = 20;
    int a = 1, b = 2, c = 3, d = 4;
    slist_node_t *head = NULL, *new_b = NULL, *new_c = NULL, *new_d = NULL;   
    slist_node_t *new_node = NULL, *middle_node = NULL;
    
    /*creat list of nodes*/
    new_d = SListCreateNode(&d, NULL);
    new_c = SListCreateNode(&c, new_d);
    new_b = SListCreateNode(&b, new_c);
    head = SListCreateNode(&a, new_b);
    
    new_node = SListCreateNode(&new_data, NULL);
    assert(NULL != new_node);
    
    /* insert before head */
    SListInsert(head, new_node);
    assert(head->next_node == new_node);
    assert(*(int *)head->data == new_data);
    
    /* insert in the middle */
    middle_node = SListCreateNode(&new_data, NULL);
    SListInsert(new_c, middle_node);
    assert(new_c->next_node == middle_node);
    assert(*(int *)new_c->data == new_data);
    
    assert(head->next_node == new_node);
    assert(new_node->next_node == new_b);
	assert(new_b->next_node == new_c);
	assert(new_c->next_node == middle_node);
	assert(middle_node->next_node == new_d);
	assert(new_d->next_node == NULL);
	
	/* check if nodes contain the correct data */
	assert(new_data == *(int *)head->data);
	assert(a == *(int *)head->next_node->data);
	assert(b == *(int *)new_b->data);
	assert(new_data == *(int *)new_c->data);
	assert(c == *(int *)middle_node->data);
	assert(d == *(int *)new_d->data);

    SListFreeAll(head);
}

void SListInsertAfterTest()
{
    int new_data = 20;
    int a = 1, b = 2, c = 3, d = 4;
    slist_node_t *head = NULL, *new_b = NULL, *new_c = NULL, *new_d = NULL;   
    slist_node_t *new_node = NULL, *middle_node = NULL;
    
    /*creat list of nodes*/
    new_d = SListCreateNode(&d, NULL);
    new_c = SListCreateNode(&c, new_d);
    new_b = SListCreateNode(&b, new_c);
    head = SListCreateNode(&a, new_b);
    
    new_node = SListCreateNode(&new_data, NULL);
    assert(NULL != new_node);
    
    /* insert after head */
    head = SListInsertAfter(head, new_node);
    assert(head->next_node == new_node);
    assert(*(int *)new_node->data == new_data);
    
    /* insert in the middle */
    middle_node = SListCreateNode(&new_data, NULL);
    new_c = SListInsertAfter(new_c, middle_node);
    assert(new_c->next_node == middle_node);
    assert(*(int *)middle_node->data == new_data);
    
    assert(head->next_node == new_node);
    assert(new_node->next_node == new_b);
	assert(new_b->next_node == new_c);
	assert(new_c->next_node == middle_node);
	assert(middle_node->next_node == new_d);
	assert(new_d->next_node == NULL);
	
	/* check if nodes contain the correct data */
	assert(a == *(int *)head->data);
	assert(new_data == *(int *)new_node->data);
	assert(b == *(int *)new_b->data);
	assert(new_data == *(int *)middle_node->data);
	assert(c == *(int *)new_c->data);
	assert(d == *(int *)new_d->data);

    SListFreeAll(head);

}

static int Cmp(void *head, const void *data)
{
    /* return 0 if succssed and 1 if not */
	return !(*(int *)head == *(int *)data);
}

void SListFindTest()
{
    slist_node_t *found_node = NULL;
    
    int a = 1, b = 2, c = 3, d = 4 , e = 0;
    slist_node_t *head = NULL, *new_b = NULL, *new_c = NULL, *new_d = NULL;   
    
    /*creat list of nodes*/
    new_d = SListCreateNode(&d, NULL);
    new_c = SListCreateNode(&c, new_d);
    new_b = SListCreateNode(&b, new_c);
    head = SListCreateNode(&a, new_b);
    
    /*creat if node was found and the correct value- end of list*/
    found_node = SListFind(head, Cmp, &d);
	assert(d == *(int *)found_node->data);
	assert(new_d == found_node);
	
	/*creat if node was found and the correct value- middle of list*/
	found_node = SListFind(head, Cmp, &b);
	assert(b == *(int *)found_node->data);
	assert(new_b == found_node);
	
	/*creat if node was found and the correct value- head of list*/
	found_node = SListFind(head, Cmp, &a);
	assert(a == *(int *)found_node->data);
	assert(head == found_node);

	/* Check find for not existing value */
	assert(NULL == SListFind(head, Cmp, &e));

	SListFreeAll(head);
}

void SListRemoveAndCountTest()
{
    int a = 1, b = 2, c = 3, d = 4;
    slist_node_t *head = NULL, *new_b = NULL, *new_c = NULL, *new_d = NULL;   
    
    /*creat list of nodes*/
    new_d = SListCreateNode(&d, NULL);
    new_c = SListCreateNode(&c, new_d);
    new_b = SListCreateNode(&b, new_c);
    head = SListCreateNode(&a, new_b);
    
    assert(4 == SListCount(head));
    assert(new_c == SListRemove(new_b));
    assert(head->next_node == new_b);
    assert(3 == SListCount(head));
    
    SListFreeAll(new_c);
    SListFreeAll(head);
}

void SListRemoveAfterAndCountTest()
{
    int a = 1, b = 2, c = 3, d = 4;
    slist_node_t *head = NULL, *new_b = NULL, *new_c = NULL, *new_d = NULL;   
    
    /*creat list of nodes*/
    new_d = SListCreateNode(&d, NULL);
    new_c = SListCreateNode(&c, new_d);
    new_b = SListCreateNode(&b, new_c);
    head = SListCreateNode(&a, new_b);
    
    assert(4 == SListCount(head));
    assert(new_b == SListRemoveAfter(head));
    assert(head->next_node == new_c);
    
    assert(3 == SListCount(head));
    assert(NULL == SListRemoveAfter(new_d));

    SListFreeAll(new_b);
    SListFreeAll(head);
}

static int FuncAction(void *data, void *param)
{
    printf("%d %p\n", *(int *)param, data);
    
    return 0;
}

void SListForEachTest()
{
    int a = 1, b = 22;
    slist_node_t *head = NULL, *new_b = NULL;   
    
    /*creat list of nodes*/
    new_b = SListCreateNode(&b, NULL);
    head = SListCreateNode(&a, new_b);
    
    assert(2 == SListCount(head));
    
    assert(0 == SListForEach(head, FuncAction, head->data));
    
    SListFreeAll(head);
}

void SListFlipTest()
{
    int a = 1, b = 2, c = 3, d = 4;
    slist_node_t *head = NULL, *new_b = NULL, *new_c = NULL, *new_d = NULL;   
    
    new_d = SListCreateNode(&d, NULL);
    new_c = SListCreateNode(&c, new_d);
    new_b = SListCreateNode(&b, new_c);
    head = SListCreateNode(&a, new_b);
    
    SListFlip(head);
    
    assert(new_c == new_d->next_node);
    assert(new_b == new_c->next_node);
    assert(head == new_b->next_node);
    assert(NULL == head->next_node);
    
    SListFreeAll(new_d);
}

void SListHasLoopTest()
{
    int a = 1, b = 2, c = 3, d = 4;
    slist_node_t *head = NULL, *new_b = NULL, *new_c = NULL, *new_d = NULL;   
    
    new_d = SListCreateNode(&d, NULL);
    new_c = SListCreateNode(&c, new_d);
    new_b = SListCreateNode(&b, new_c);
    head = SListCreateNode(&a, new_b);
    assert(0 == SListHasLoop((slist_node_t *)head));
    
    new_d->next_node = head;
    assert(1 == SListHasLoop((slist_node_t *)head));
    
    new_d->next_node = NULL;
    SListFreeAll(head);
}

void SListFindIntersectionTest()
{
    int a = 1, b = 2, c = 3, d = 4, f = 6;
    slist_node_t *head = NULL, *new_b = NULL, *new_c = NULL, *new_d = NULL;   
    slist_node_t *inter = NULL;
    
    new_d = SListCreateNode(&d, NULL);
    new_c = SListCreateNode(&c, new_d);
    new_b = SListCreateNode(&b, new_c);
    head = SListCreateNode(&a, new_b);
    assert(NULL == SListFindIntersection(head, inter));
    inter = SListCreateNode(&f, new_b);
    assert(new_b == SListFindIntersection(head, inter));
    
    inter->next_node = NULL;
    
    SListFreeAll(head);
    SListFreeAll(inter);
}

