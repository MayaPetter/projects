/******************************************
* Author:   Maya Petter                   *
* Reviewer: Ran Shiber                    *
* Purpose:  binary search tree            *
*******************************************/
#include <assert.h> /* assert */
#include <stdlib.h> /* malloc, free */

#include "bst.h"

#define UNUSED(x) ((void)(x)) /* for counting nodes function */

struct bst
{
    bst_iter_t stub_root;
    int (*cmp_func)(void *iter_data, void *new_data, void *param);
    void *param;
};

struct bst_node
{
    bst_iter_t parent;
    void *data;
    bst_iter_t left_child;
    bst_iter_t right_child;   
}; 

typedef struct bst_node bst_node_t; 

/********************Static functions******************************************/
static bst_iter_t CreateBSTNode(bst_iter_t parent, void *new_data);
static void *FreeFunction(void *to_free);
static int CountNodes(void *iter_data, void *param);
static bst_iter_t FindPosition(bst_t *bst, void *data, int *cmp);

static int IsLeaf(bst_iter_t iter_to_check);
static int IsLeftChild(bst_iter_t iter_to_check);
static int IsRightChild(bst_iter_t iter_to_check);
static int HasTwoChildren(bst_iter_t iter_to_check);
int HasLeftChild(bst_iter_t iter_to_check);
int HasRightChild(bst_iter_t iter_to_check);
void SetChildInDirection(bst_iter_t to_set, bst_iter_t new_child, int direction);

static bst_iter_t GetParent(bst_iter_t iter_to_check);
static bst_iter_t GoToLeaf(bst_iter_t iter_to_advance);
bst_iter_t GetRoot(const bst_t *bst);
bst_iter_t GoLeft(bst_iter_t iter_to_advance);
bst_iter_t GoRight(bst_iter_t iter_to_advance);

static void UpdateParentChild(bst_iter_t child, bst_iter_t parent, 
                                                           bst_iter_t position);
static void RemoveWithTwoChildren(bst_iter_t iter_to_remove);
static void RemoveLeftChild(bst_iter_t iter_to_remove);
static void RemoveRightChild(bst_iter_t iter_to_remove);

/********************API functions*********************************************/

bst_t *BSTCreate(int (*cmp_func)(void *iter_data, void *new_data, void *param), 
				 void *param)
{
    bst_t *new_bst = NULL;
    
    assert(NULL != cmp_func);
    
    new_bst = (bst_t *)malloc(sizeof(bst_t));
    if(NULL == new_bst)
    {
        return NULL;
    }
    
        
    new_bst->stub_root = CreateBSTNode(NULL, NULL);
    if (NULL == new_bst->stub_root)
    {
        new_bst = FreeFunction((void *)new_bst);
        return NULL;
    }
    
    new_bst->cmp_func = cmp_func;
    new_bst->param = param;
    
    /* set stub_root to point to itself when empty */
    new_bst->stub_root->left_child = new_bst->stub_root;
    
    return new_bst;
}


/* Destroy all nodes and bst **************************************************/
void BSTDestroy(bst_t *bst)
{
    bst_iter_t runner = NULL;
    bst_iter_t iter_to_remove = NULL;
    
    assert(NULL != bst);
    
    runner = GetRoot(bst);
    
    while (!BSTIsSameIter(runner, BSTEnd(bst)))
    {
        if (!IsLeaf(runner))
        {
            runner = GoToLeaf(runner);
        }
        else
        {
            iter_to_remove = runner;
            runner = GetParent(runner);
            
         /* update parent child to NULL and remove child */            
            UpdateParentChild(iter_to_remove, runner, NULL);
            
            iter_to_remove->data = NULL;
            iter_to_remove = FreeFunction((void *)iter_to_remove);
        }
    }
    
    bst->stub_root = FreeFunction((void *)bst->stub_root);
    bst = FreeFunction((void *)bst);
}

/* Insert new node sorted by it's data ****************************************/
bst_iter_t BSTInsert(bst_t *bst, void *data)
{
    bst_iter_t new_leaf = NULL;
    bst_iter_t found_pos = NULL;
    int cmp = 0;
    
    assert(NULL != bst);
    assert(NULL != data);

    new_leaf = CreateBSTNode(NULL, data);
    if (NULL == new_leaf)
    {
        return BSTEnd(bst);
    }

    found_pos = FindPosition(bst, data, &cmp);

    if (cmp < 0)
    {
        found_pos->right_child = new_leaf;
    }
    else
    {
        found_pos->left_child = new_leaf; 
    }
    
    new_leaf->parent = found_pos;
        
    return new_leaf;
} 

/* Removes iter from tree *****************************************************/
void BSTRemove(bst_iter_t iter_to_remove)
{
    assert(NULL != iter_to_remove);

    if (HasTwoChildren(iter_to_remove))
    {
        RemoveWithTwoChildren(iter_to_remove);            
    }
    else
    {
        if (IsLeftChild(iter_to_remove))
	    {
		    RemoveLeftChild(iter_to_remove);
	    }
	    else 
	    {
	        RemoveRightChild(iter_to_remove);
        } 
    }
    
    iter_to_remove = FreeFunction(iter_to_remove);
}

/******* Count how many nodes in given binary search tree *********************/
size_t BSTSize(const bst_t *bst)
{
    size_t counter = 0;
    
    assert(NULL != bst);
    
    BSTForEach(&counter, BSTBegin(bst), BSTEnd(bst), &CountNodes);    

    return counter;
} 

/******* Check if tree is empty ***********************************************/
int BSTIsEmpty(const bst_t *bst)
{
    assert(NULL != bst);

    return BSTIsSameIter(GetRoot(bst), BSTEnd(bst)); 
} 

/* Perform <action_func> for each element in <bst>, stops if action fails *****/
int BSTForEach(void *param, bst_iter_t from, bst_iter_t to, 
			                   int (*action_func)(void *iter_data, void *param))
{
    int func_status = 0;
    bst_iter_t runner = NULL;
    
    assert(NULL != from);
    assert(NULL != to);

    for (runner = from ;(!BSTIsSameIter(runner, to)) && (0 == func_status)
          ;runner = BSTNext(runner))
    {
    	func_status = action_func(BSTGetData(runner), param);
    }       
    
    return func_status;
}

/* find the node that holds equal data to searched data. if not found- NULL ***/
bst_iter_t BSTFind(bst_t *bst, void *data_to_find)
{
    bst_iter_t found_pos = NULL;
    
    assert(NULL != bst);
    assert(NULL != data_to_find);
    
    found_pos = GetRoot(bst);
    
    while (NULL != found_pos && !BSTIsSameIter(found_pos, BSTEnd(bst)))
    {
        void *param = bst->param;
        int cmp = bst->cmp_func(BSTGetData(found_pos), data_to_find, param);
        
        if (0 == cmp)
        {
            return found_pos;
        }
        else if(0 < cmp)
        {
            found_pos = GoLeft(found_pos); 
        }
        else if (0 > cmp)
        {
            found_pos = GoRight(found_pos);
        }
    }
    return NULL;
}   

/******* get pointer to data in the node iter points to ***********************/
void *BSTGetData(bst_iter_t iter)
{
    assert(NULL != iter);
    
    return iter->data;
}

/******* find the next node with bigger data in the bst***********************/ 
bst_iter_t BSTNext(bst_iter_t iter)
{
    if (!HasRightChild(iter))
    {
        for (;IsRightChild(iter); iter = GetParent(iter))
        {/* empty body */}

        iter = GetParent(iter);
    }
    else
    {
        for (iter = GoRight(iter); HasLeftChild(iter); iter = GoLeft(iter))
        {/* empty body */} 
    }
    return iter;
}

/*******find the previous node with smaller data in the bst********************/
bst_iter_t BSTPrev(bst_iter_t iter)
{
    assert(NULL != iter);
    
    if (HasLeftChild(iter))
    {
       for (iter = GoLeft(iter); HasRightChild(iter); iter = GoRight(iter))
       {/* empty body */} 

    }
    else
    {
        for ( ; IsLeftChild(iter); iter = GetParent(iter))
        {/* empty body */}

        iter = GetParent(iter);
    }    
    return iter;
}

/******* invalid root to mark the end of the bst ******************************/
bst_iter_t BSTEnd(const bst_t *bst)
{
    assert(NULL != bst);
    
    return bst->stub_root;
}

/*******find the node that holds the smallest data in the bst******************/
bst_iter_t BSTBegin(const bst_t *bst)
{
    bst_iter_t begin = NULL;
    
    assert(NULL != bst);
    
    begin = GetRoot(bst);
    
    if (!BSTIsEmpty(bst))
    {
        for ( ; HasLeftChild(begin); begin = GoLeft(begin))
        {/* empty body */}
    } 

    return begin;
}

/*******check if two iters points to the same address**************************/
int BSTIsSameIter(bst_iter_t iter1, bst_iter_t iter2)
{
    return (iter1 == iter2);
}

/********************Static functions******************************************/
static bst_iter_t CreateBSTNode(bst_iter_t parent, void *new_data)   
{
    bst_iter_t bst_node = NULL;

    bst_node = (bst_iter_t)malloc(sizeof(struct bst_node));
    if (NULL == bst_node)
    {
        return NULL;
    }
    
    /* Initiate bst node members */
    bst_node->parent = parent;
    bst_node->data = new_data;
    bst_node->left_child = NULL;
    bst_node->right_child = NULL;  
    
    return bst_node; 
}

static void *FreeFunction(void *to_free)
{
    assert(NULL != to_free);

    free(to_free); 
    to_free = NULL;

    return NULL;
}

static int CountNodes(void *iter_data, void *param)
{
    assert(NULL != param);

    UNUSED(iter_data);
    
    ++(*(size_t *)param);
    
    return 0;
}

static bst_iter_t FindPosition(bst_t *bst, void *data, int *cmp)
{   
    bst_iter_t found_pos = NULL;
    void *param = NULL;
    
    assert(NULL != bst);
    assert(NULL != data);
    
    found_pos = GetRoot(bst);
    param = bst->param;
    
    while (!BSTIsSameIter(found_pos, BSTEnd(bst)))
    {
        *cmp = bst->cmp_func(BSTGetData(found_pos), data, param);
        
        if(0 < (*cmp) && HasLeftChild(found_pos))
        {
            found_pos = GoLeft(found_pos); 
        }
        else if (0 > (*cmp) && HasRightChild(found_pos))
        {
            found_pos = GoRight(found_pos);
        }
        else
        {
            break;
        }
    }
    return found_pos;
}

static int IsLeftChild(bst_iter_t iter_to_check)
{
    assert(NULL != iter_to_check);
    
    return BSTIsSameIter(iter_to_check, GoLeft(GetParent(iter_to_check)));
}

static int IsRightChild(bst_iter_t iter_to_check)
{
    assert(NULL != iter_to_check);
    
    return !IsLeftChild(iter_to_check);
}

int HasLeftChild(bst_iter_t iter_to_check)
{
    assert(NULL != iter_to_check);
    
    return (NULL != iter_to_check->left_child);
}

int HasRightChild(bst_iter_t iter_to_check)
{
    assert(NULL != iter_to_check);
    
    return (NULL != iter_to_check->right_child);
}

static int HasTwoChildren(bst_iter_t iter_to_check)
{
    assert(NULL != iter_to_check);
    
    return (HasRightChild(iter_to_check) && HasLeftChild(iter_to_check)); 
}

static int IsLeaf(bst_iter_t iter_to_check)
{
    assert(NULL != iter_to_check);
    
    return (!HasLeftChild(iter_to_check) && !HasRightChild(iter_to_check));
}

bst_iter_t GetRoot(const bst_t *bst)
{
    assert(NULL != bst);
    
    return bst->stub_root->left_child;
}

static bst_iter_t GetParent(bst_iter_t iter_to_check)
{
    assert(NULL != iter_to_check);
    
    return iter_to_check->parent;
}

bst_iter_t GoLeft(bst_iter_t iter_to_advance)
{
    assert(NULL != iter_to_advance);
    
    return iter_to_advance->left_child;
}

bst_iter_t GoRight(bst_iter_t iter_to_advance)
{
    assert(NULL != iter_to_advance);
    
    return iter_to_advance->right_child;
}

static bst_iter_t GoToLeaf(bst_iter_t iter_to_advance)
{
    assert(NULL != iter_to_advance);

    if (HasLeftChild(iter_to_advance))
    {
        iter_to_advance = GoLeft(iter_to_advance);
    }
    else if (HasRightChild(iter_to_advance))
    {
        iter_to_advance = GoRight(iter_to_advance);
    }

    return iter_to_advance;
}

static void UpdateParentChild(bst_iter_t child, bst_iter_t parent, 
                                                            bst_iter_t position)
{
    assert(NULL != child);
    assert(NULL != parent);

    if (IsLeftChild(child))
    {
        parent->left_child = position;
    }
    else
    {
        parent->right_child = position;
    }
}

static void RemoveWithTwoChildren(bst_iter_t iter_to_remove)
{
    bst_iter_t next = NULL;
    
    assert(NULL != iter_to_remove);
    
    next = BSTNext(iter_to_remove);
	
    next->left_child = GoLeft(iter_to_remove);
    
    iter_to_remove->left_child->parent = next;
    
    UpdateParentChild(iter_to_remove, GetParent(iter_to_remove),
                                                      GoRight(iter_to_remove)); 

    iter_to_remove->right_child->parent = GetParent(iter_to_remove);
}
   
static void RemoveLeftChild(bst_iter_t iter_to_remove)
{
    assert(NULL != iter_to_remove);
    
    if (HasLeftChild(iter_to_remove)) 
    {
        iter_to_remove->left_child->parent = GetParent(iter_to_remove);
        iter_to_remove->parent->left_child = GoLeft(iter_to_remove);
    }
    else if (HasRightChild(iter_to_remove)) 
    {
        iter_to_remove->right_child->parent = GetParent(iter_to_remove);
        iter_to_remove->parent->left_child = GoRight(iter_to_remove);
    }
    else /*Is Leaf*/
    {   
        /*removing the root*/
        if (NULL == GetParent(GetParent(iter_to_remove))) 
        {
	        iter_to_remove->parent->left_child = GetParent(iter_to_remove);
        }
        else
        {
	        iter_to_remove->parent->left_child = NULL;
        }
    }
}

static void RemoveRightChild(bst_iter_t iter_to_remove)
{
    if (HasLeftChild(iter_to_remove))
    {
        iter_to_remove->left_child->parent = GetParent(iter_to_remove);
        iter_to_remove->parent->right_child = GoLeft(iter_to_remove);
    }
    else if (HasRightChild(iter_to_remove))
    {
        iter_to_remove->right_child->parent = GetParent(iter_to_remove);
        iter_to_remove->parent->right_child = GoRight(iter_to_remove);
    }
    else /*Is Leaf*/
    {
        iter_to_remove->parent->right_child = NULL;
    }
}

void SetChildInDirection(bst_iter_t to_set, bst_iter_t new_child, int direction)
{
	assert(NULL != to_set);
	
	if (RIGHT == direction)
	{
		to_set->right_child = new_child;
	}
	else
	{
		to_set->left_child = new_child;
	}
	
}    
