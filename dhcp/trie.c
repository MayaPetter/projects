#include <assert.h> /* assert */
#include <stdlib.h> /* malloc, free */
#include <limits.h> /* CHAR_BIT */

#include "trie.h"

#define MAX_DIRECTION (2)
#define BYTES_IN_IP (4)

/********** Structs and typedef ***********************************************/
typedef struct trie_node trie_node_t;

struct trie_node
{
    trie_node_t *child[MAX_DIRECTION];
    int is_path_end;    
};

struct trie
{
    trie_node_t *root;
    size_t height;
};

/********** Static Functions **************************************************/
static void *FreeFunction(void *to_free);

static unsigned int GetMask(trie_t *trie);
static int GetBitDirection(unsigned char *path, size_t bit_index, 
                           unsigned int mask);

static trie_node_t *CreateNode();
static int IsLeaf(trie_node_t *node);

static trie_node_t *InsertInDirection(trie_node_t *node, int direction);
static trie_node_t *TrieInsertRecursive(trie_t *trie, trie_node_t *node, 
                                        unsigned char *path, size_t bit_index);

static void DestroyRecursive(trie_node_t *node);

static size_t TrieRecCount(trie_node_t *node);

static trie_node_t *TrieFindRecursive(trie_t *trie, trie_node_t *node, 
                                      unsigned char *path, size_t bit_index);

static trie_node_t *RemoveRecursive(trie_t *trie, trie_node_t *node,  
                                    int direction, unsigned char *path, 
                                    size_t bit_index);

/********** Create node and initiate node members *****************************/
static trie_node_t *CreateNode()
{
    size_t index = 0;
    
    trie_node_t *new_node = (trie_node_t *)malloc(sizeof(trie_node_t));
    if (NULL == new_node)
    {
        return NULL;
    }        
    
    for (index = 0; index < MAX_DIRECTION; ++index)
    {
        new_node->child[index] = NULL;
    }
    
    return new_node;        
}

/********** Create new trie tree with user requested height *******************/
trie_t *TrieCreate(size_t height)
{
    trie_t *new_trie = NULL;
    
    /* create management list */
    new_trie = (trie_t *)malloc(sizeof(trie_t));
    if (NULL == new_trie)
    {
        return NULL;
    }
    
    /* create root */
    new_trie->root = CreateNode();
    if (NULL == new_trie->root)
    {
        new_trie = FreeFunction(new_trie);
        
        return NULL;
    }
    
    /* Initiate management list member */
    new_trie->height = height; 
    
    return new_trie;    
}

/*************** Destroy trie tree ********************************************/
void TrieDestroy(trie_t *trie)
{
    assert(NULL != trie);
    
    DestroyRecursive(trie->root);
    
    trie = FreeFunction(trie);
}

/*************** Destroy recursively ******************************************/
static void DestroyRecursive(trie_node_t *node)
{
    if (NULL == node)
    {
        return;
    }
    
    DestroyRecursive(node->child[0]);
    
    DestroyRecursive(node->child[1]);
    
    node = FreeFunction(node);
}

/*************** Insert path **************************************************/
int TrieInsert(trie_t *trie,  unsigned char *path)
{
    int direction = 3;
    
    assert(NULL != trie);
    assert(NULL != path);
    
    direction = GetBitDirection(path, 0, GetMask(trie));
    
    TrieInsertRecursive(trie, trie->root, path, 0); 

    if (NULL == trie->root->child[direction])
    {
        return 1;
    }
    
    return 0;
} 

/*************** Insert path recursively **************************************/
static trie_node_t *TrieInsertRecursive(trie_t *trie, trie_node_t *node, 
                                        unsigned char *path, size_t bit_inx)
{
    int direction = 0;
    
    assert(NULL != trie);
    assert(NULL != node);
    assert(NULL != path);
    
    direction = GetBitDirection(path, bit_inx, GetMask(trie));
    
    node->child[direction] = InsertInDirection(node, direction);
    if (NULL == node->child[direction])
    {
        return NULL;
    }
    
    if ((trie->height - 1) == bit_inx)
    {
        return node;
    }
    
    return (TrieInsertRecursive(trie, node->child[direction], path, ++bit_inx));
}

/*************** Insert node in path direction ********************************/
static trie_node_t *InsertInDirection(trie_node_t *node, int direction)
{
    if (NULL == node)
    {
        return node;
    }
    
    if (NULL == node->child[direction])
    {
        node->child[direction] = CreateNode();
        if (NULL == node->child[direction])
        {
            return NULL;
        }
    }
    
    return node->child[direction];
}

/*************** Remove path **************************************************/
void TrieRemove(trie_t *trie,  unsigned char *path)
{
    int direction = 3;
        
    assert(NULL != trie);
    assert(NULL != path);

    if (TrieIsEmpty(trie))
    {
        return;
    }
    
    if (!TrieFind(trie, path))
    {
        return;
    }
    
    direction = GetBitDirection(path, 0, GetMask(trie));

    trie->root->child[direction] = RemoveRecursive(trie, 
                                                   trie->root->child[direction], 
                                                   direction, path, 1); 
}

/*************** Removes recursively all leafs in the path ********************/
static trie_node_t *RemoveRecursive(trie_t *trie, trie_node_t *node, 
                                    int direction, unsigned char *path, 
                                    size_t bit_index)
{
    assert(NULL != trie);
    assert(NULL != path);
    
    if (NULL == node)
    {
        return NULL;
    }
    
    direction = GetBitDirection(path, bit_index, GetMask(trie));

    node->child[direction] = RemoveRecursive(trie, node->child[direction], 
                                             direction, path, ++bit_index);
                                             
    if (IsLeaf(node))
    {
        node = FreeFunction(node);
        return NULL;
    }

    return node;
}

/*************** Count how many paths in given trie ***************************/
size_t TrieCount(trie_t *trie)
{
    assert(NULL != trie);
    
    if (TrieIsEmpty(trie))
    {
        return 0;
    }   
       
    return TrieRecCount(trie->root);
} 

/*************** Count paths recursively **************************************/
static size_t TrieRecCount(trie_node_t *node)
{
 
    if (NULL == node)
    {
        return 0;
    }
    
    if (IsLeaf(node))
    {
        return 1;
    }
    
    return (TrieRecCount(node->child[0]) + TrieRecCount(node->child[1]));
} 

/*************** Calculate path capacity- max height 31 ***********************/
size_t TrieCapacity(trie_t *trie)
{
    assert(NULL != trie);

   /*num of paths in complete binary tree */    
    return (1 << trie->height);
} 

/*************** Check if trie is empty ***************************************/
int TrieIsEmpty( trie_t *trie)
{
    assert(NULL != trie);
    assert(NULL != trie->root);
    
    return (NULL == trie->root->child[0] && NULL == trie->root->child[1]);
} 

/**** Find if path is in trie tree. If path wasn't found returns 0 ************/
int TrieFind( trie_t *trie, unsigned char *path)
{
    assert(NULL != trie);
    assert(NULL != path);
    
    if (TrieIsEmpty(trie))
	{
		return 0; 
	}

    return (NULL != TrieFindRecursive(trie, trie->root, path, 0));
}

/*************** Find path recursively ****************************************/
static trie_node_t *TrieFindRecursive(trie_t *trie, trie_node_t *node, 
                                      unsigned char *path, size_t bit_index)
{
    int direction = 0;
    
    assert(NULL != trie);
    assert(NULL != path);
    
    if (NULL == node)
    {
        return NULL;
    }    
    
    direction = GetBitDirection(path, bit_index, GetMask(trie));
    
    if (NULL == node->child[direction])
    {
        return NULL;
    }
    
    if ((trie->height - 1) == bit_index)
    {
        return node;
    }
    
    return (TrieFindRecursive(trie, node->child[direction], path, ++bit_index));
}

/*************** Adjust mask to trie height ***********************************/
static unsigned int GetMask(trie_t *trie)
{
    unsigned int mask = 0x01;
    unsigned int bit_index = 0;
    
    assert(NULL != trie);
    
    bit_index = (unsigned int)trie->height - 1;
    
    return (mask << bit_index);
}

/*************** Get path direction *******************************************/
static int GetBitDirection(unsigned char *path, size_t bit_index, 
                                                unsigned int mask)
{
	assert(NULL != path);
	
    /* return 1 if it's on and 0 otherwise */
    return (0 != ((mask >> bit_index) & (*(unsigned int *)path)));
}

/*************** Static helper functions **************************************/
static int IsLeaf(trie_node_t *node)
{
    if (NULL == node)
    {
        return 0;
    }
    
    return (NULL == node->child[0] && NULL == node->child[1]);
} 

static void *FreeFunction(void *to_free)
{
    free(to_free);
    to_free = NULL;
    
    return NULL;
}

