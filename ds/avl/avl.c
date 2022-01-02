/******************************************
* Author:   Maya Petter                   *
* Reviewer: Shddad Gomid                  *
* Status:   Approved                      *
* Purpose:  Balanced binary tree          *
*******************************************/
#include <stdlib.h> /* malloc, free */
#include <assert.h> /* assert       */

#include "avl.h"    /* API functions */

#define MAX_CHILD 2

enum children {LEFT, RIGHT};

typedef struct avl_node avl_node_t;

struct avl_node
{
    void *data;
    avl_node_t *child[MAX_CHILD];
    size_t height;
};

struct avl
{
    avl_node_t *root;
    int (*cmp_func)(const void *tree_data, const void *new_data, void *param);
    void *param;
};

/*************** Static functions *********************************************/
static avl_node_t *CreateNode(void *new_data);
static void *FreeFunction(void *to_free);
static void DestroyNode(avl_node_t *to_free);
static int GetDirection(avl_t *avl, avl_node_t *current_node, void *data);
static int IsAnotherRotateNeeded(avl_node_t *node, int direction);

static int HasTwoChildren(avl_node_t *node_to_check);
static int HasChildInDirection(avl_node_t *node_to_check, int direction);

/*************** Recursive functions ******************************************/
static void DestroyRecursive(avl_node_t *node);

static avl_node_t *InsertRecur(avl_t *avl, avl_node_t *curr_node, 
                                                        avl_node_t *new_leaf);

static avl_node_t *RemoveRecursive(avl_t *avl, avl_node_t *curr_node, 
                                                                    void *data);
static avl_node_t *RemoveNode(avl_t *avl, avl_node_t *curr_node);

static avl_node_t *FindRecursive(avl_t *avl, avl_node_t *curr_node, void *data);
static avl_node_t *FindNext(avl_node_t *current_node);

static size_t CountRecur(avl_node_t *node);

static int ForEachRecursive(void *param, avl_node_t *node, 
                             int (*action_func)(void *tree_data, void *param));

/*************** Balance and Height functions *********************************/
static void UpdateHeight(avl_node_t *curr_node);
static size_t GetNodeHeight(avl_node_t *curr_node);
static avl_node_t *Balance(avl_node_t *curr_node);
static avl_node_t *Rotate(avl_node_t *node, int direction);
static int BalanceFactor(avl_node_t *node);

/*************** API functions and recursive helpers **************************/

/********** Create new AVL tree with user comparison function *****************/
avl_t *AVLCreate(void *param, int (*cmp_func)(const void *tree_data,
								 			  const void *new_data,
								 			  void *param))
{
    avl_t *new_tree = NULL;
    
    assert(NULL != cmp_func);
    
    /* create management list */
    new_tree = (avl_t *)malloc(sizeof(struct avl));
    if (NULL == new_tree)
    {
        return NULL;
    }
    
    /* Initiate management list members */
    new_tree->root = NULL;
    new_tree->cmp_func = cmp_func;
    new_tree->param = param;
    
    return new_tree;    
}

/********** Create node and initiate node members *****************************/
static avl_node_t *CreateNode(void *new_data)
{
    avl_node_t *new_leaf = (avl_node_t *)malloc(sizeof(avl_node_t));
    if (NULL == new_leaf)
    {
        return NULL;
    }        
    
    new_leaf->data = new_data;
    new_leaf->height = 1;  
    new_leaf->child[LEFT] = NULL;
    new_leaf->child[RIGHT] = NULL;
    
    return new_leaf;        
}

/*************** Destroy AVL tree *********************************************/
void AVLDestroy(avl_t *avl)
{
    assert(NULL != avl);
    
    DestroyRecursive(avl->root);
    
    avl = FreeFunction((void *)avl);
}

/*************** Destroy recursively - POST ORDER *****************************/
static void DestroyRecursive(avl_node_t *node)
{
    if (NULL == node)
    {
        return;
    }
    
    DestroyRecursive(node->child[LEFT]);
    
    DestroyRecursive(node->child[RIGHT]);
    
    DestroyNode(node);
}

/*************** Insert new element to tree returns 0 on success **************/
int AVLInsert(avl_t *avl, const void *data_to_insert)
{
    avl_node_t *new_leaf = NULL;
    
    assert(NULL != avl);
    
    new_leaf = CreateNode((void *)data_to_insert);
    if (NULL == new_leaf)
    {
        return 1;
    }
    
    avl->root = InsertRecur(avl, avl->root, new_leaf);   
    
    return 0;
} 

/*************** Insert recursively based on user cmp_func ********************/
static avl_node_t *InsertRecur(avl_t *avl, avl_node_t *curr_node, 
                                                        avl_node_t *new_leaf)
{
    assert(NULL != avl);
    assert(NULL != new_leaf);
    
    if (NULL == curr_node)
    {
        return new_leaf;
    }
    else
    {
        int direction = GetDirection(avl, curr_node, new_leaf->data);
        
        curr_node->child[direction] = InsertRecur(avl, 
                                        curr_node->child[direction], new_leaf);
    }

    UpdateHeight(curr_node);
    UpdateHeight(avl->root); 
        
    return Balance(curr_node);
}

/*************** Remove *******************************************************/
void AVLRemove(avl_t *avl, const void *data_to_remove)
{
    assert(NULL != avl);

    if (AVLIsEmpty(avl))
    {
        return;
    }
    
    avl->root = RemoveRecursive(avl, avl->root, (void *)data_to_remove); 
}

/*************** Finds recursively the node to Remove *************************/
static avl_node_t *RemoveRecursive(avl_t *avl, avl_node_t *curr_node, 
                                                                    void *data)
{
    int cmp_result = 0;
    
    assert(NULL != avl);
    
    if (NULL == curr_node)
    {
        return curr_node;
    }

    cmp_result = avl->cmp_func(curr_node->data, data, avl->param);
    
    if (0 != cmp_result)
    {
       int direction = (cmp_result < 0); /* right if true and left if not */ 

        curr_node->child[direction] = RemoveRecursive(avl, 
                                            curr_node->child[direction], data);
        
        UpdateHeight(curr_node);
        UpdateHeight(avl->root);         
            
        return Balance(curr_node);
    }

    return RemoveNode(avl, curr_node);
}

/*Removes the node if it's leaf or replace data with other node and remove it */
static avl_node_t *RemoveNode(avl_t *avl, avl_node_t *node)
{
    assert(NULL != node);
    
    if (HasTwoChildren(node))
    {
        avl_node_t *next_node = FindNext(node->child[RIGHT]);
        
        node->data = next_node->data; /* assign next data to current node */        
        
        /* remove next data from right subtree and assign back to node */
        node->child[RIGHT] = RemoveRecursive(avl, node->child[RIGHT], 
                                                  next_node->data);
                                                            
         return node;
    }
    else
    {
        /*right if true left if not*/
        int direction = (HasChildInDirection(node, RIGHT));
        
        avl_node_t *to_return = node->child[direction];
        
        DestroyNode(node);
        
        return to_return;
    }  
}

/*************** Count how many nodes in given AVL tree ***********************/
size_t AVLCount(const avl_t *avl)
{
    assert(NULL != avl);
    
    return CountRecur(avl->root); 
} 

/*************** Count recursively - PRE-ORDER ********************************/
static size_t CountRecur(avl_node_t *node)
{
    if (NULL == node)
    {
        return 0;
    }
    
    return (1 + CountRecur(node->child[LEFT]) + CountRecur(node->child[RIGHT]));
}

/*************** Check height of AVL tree *************************************/
size_t AVLHeight(const avl_t *avl)
{
    assert(NULL != avl);
    
    if (AVLIsEmpty(avl))
    {
        return 0;
    }

    return avl->root->height;
} 

/*************** Check if AVL tree is empty ***********************************/
int AVLIsEmpty(const avl_t *avl)
{
    assert(NULL != avl);
    
    return (NULL == avl->root); 
} 

/* Perform function for each element in tree stops if action returns non-zero */
int AVLForEach(void *param, avl_t *avl, int (*action_func)(void *tree_data,
														   void *param))
{
    assert(NULL != avl);
    
    return ForEachRecursive(param, avl->root, action_func);
}

/*************** Preform action recursively - IN-ORDER ************************/
static int ForEachRecursive(void *param, avl_node_t *node, 
                     int (*action_func)(void *tree_data, void *param))
{
    int status = 0;
    
    if (NULL == node)
    {
        return status;    
    }
    
    /* Perform function for each element in left subtree */
    status = ForEachRecursive(param, node->child[LEFT], action_func);

    if (0 != status)
    {
        return status;
    }

    /* Perform function on the parent */
    status = action_func(node->data, param);
 
    if (0 != status)
    {
        return status;
    }
    
    /* Perform function for each element in right subtree */    
    return ForEachRecursive(param, node->child[RIGHT], action_func);
}


/**** Find <data> in AVL tree and returns it. If not found returns NULL *******/
void *AVLFind(const avl_t *avl, const void *data_to_find)
{
    avl_node_t *found_node = NULL;
    
    assert(NULL != avl);

    if (AVLIsEmpty(avl))
	{
		return NULL; 
	}
	    
   found_node = FindRecursive((avl_t *)avl, avl->root, (void *)data_to_find);
   if (NULL == found_node)
   {
        return NULL;
   }                                                     
   
   return found_node->data;
}

/*************** Find data recursively ****************************************/
static avl_node_t *FindRecursive(avl_t *avl, avl_node_t *curr_node, void *data)
{
    int direction = 0;
    int cmp_result = 0;
    
    assert(NULL != avl);
    assert(NULL != curr_node);
    
    direction = GetDirection(avl, curr_node, data);
    cmp_result = avl->cmp_func(curr_node->data, data, avl->param);
    
    if (0 == cmp_result)
    {
        return curr_node;
    }
    else if (NULL == curr_node->child[direction])
    {
        return NULL;
    }

    return FindRecursive(avl, curr_node->child[direction], data);
}

/*************** Update height and Balance *************************************/

static void UpdateHeight(avl_node_t *node)
{
    size_t height_left = GetNodeHeight(node->child[LEFT]);
    size_t height_right = GetNodeHeight(node->child[RIGHT]);
    
    /* add 1 to the highest subtree and update current node height */
    node->height = (height_left > height_right) ? height_left : height_right;
    
    node->height += 1;  
}

static size_t GetNodeHeight(avl_node_t *curr_node)
{
    return ((NULL == curr_node) ? 0 : curr_node->height);
}

static avl_node_t *Rotate(avl_node_t *node, int direction)
{
    avl_node_t *new_parent = NULL;
    
    new_parent = node->child[!direction];

    if (IsAnotherRotateNeeded(new_parent, direction))
	{
		new_parent = Rotate(new_parent, !direction);
	}
    
    node->child[!direction] = new_parent->child[direction];
    
    new_parent->child[direction] = node;

    UpdateHeight(node);
    UpdateHeight(new_parent);
    
    return new_parent;
}

static avl_node_t *Balance(avl_node_t *node)
{
    int balance_factor = 0;
    
    if (NULL == node)
    {
        return node;
    }
        
    balance_factor = BalanceFactor(node);

    if (1 < balance_factor)
    {
        node = Rotate(node, RIGHT);
    }
    else if (-1 > balance_factor)
    {
        node = Rotate(node, LEFT);
    }    
    
    UpdateHeight(node);
    
    return node; 
}

static int BalanceFactor(avl_node_t *node)
{
    if (NULL == node)
    {
        return 0;
    }
    
    return (GetNodeHeight(node->child[LEFT]) - 
            GetNodeHeight(node->child[RIGHT]));
}  

static int IsAnotherRotateNeeded(avl_node_t *node, int direction)
{
	int balance_factor = 0;
	
	assert(NULL != node);

	balance_factor = BalanceFactor(node); 
	
	if (RIGHT == direction && 0 > balance_factor)
	{
		return 1;
	}
	else if (LEFT == direction && 0 < balance_factor)
	{
		return 1;
	}
	
	return 0;
}

/*************** Helper Functions *********************************************/
static int GetDirection(avl_t *avl, avl_node_t *current_node, void *data)
{    
    assert(NULL != avl);
    assert(NULL != current_node);
    
    /* right if true left if not*/ 
    return (0 > avl->cmp_func(current_node->data, data, avl->param));
}

/*************** Find next data in AVL tree ***********************************/
static avl_node_t *FindNext(avl_node_t *current_node)
{
    assert(NULL != current_node);
    
    if (!HasChildInDirection(current_node, LEFT))
    {
        return current_node;
    }

    return FindNext(current_node->child[LEFT]);
}

/*************** Free pointer *************************************************/
static void *FreeFunction(void *to_free)
{
    assert(NULL != to_free);
    
    free(to_free);
    
    to_free = NULL;
    
    return NULL;
}

static void DestroyNode(avl_node_t *to_free)
{
    assert(NULL != to_free);
    
    to_free->child[LEFT] = NULL;
    to_free->child[RIGHT] = NULL;
    to_free->data = NULL;
    
    to_free = FreeFunction(to_free);
}

/*************** Check node's children ****************************************/
static int HasTwoChildren(avl_node_t *node_to_check)
{
    if (NULL == node_to_check)
	{
	    return 0;
	} 
    
    return (HasChildInDirection(node_to_check, RIGHT) 
            && HasChildInDirection(node_to_check, LEFT)); 
}

static int HasChildInDirection(avl_node_t *node_to_check, int direction)
{
	if (NULL == node_to_check)
	{
	    return 0;
	} 
	
	return (NULL != node_to_check->child[direction]);
}

