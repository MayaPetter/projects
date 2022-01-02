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
static trie_node_t *CreateNode();
static int GetBitDirection(unsigned char num, size_t bit_index);
static size_t GetByteIndex(size_t num_bytes);
static size_t GetNumBytes(const trie_t *trie);

static trie_node_t *InsertInDirection(trie_node_t *node, int direction);
static void DestroyRecursive(trie_node_t *node);
static size_t TrieRecCount(trie_node_t *node);
static trie_node_t *TrieFindRecursive(trie_t *trie, trie_node_t *node, 
                                      unsigned char *path, size_t bit_index,
                                      size_t path_byte_index);
static trie_node_t *TrieInsertRecursive(trie_t *trie, trie_node_t *node, 
                                        unsigned char *path, size_t bit_index,
                                        size_t path_byte_index);

static trie_node_t *RemoveNode(trie_t *trie, trie_node_t *node);
static trie_node_t *RemoveRecursive(trie_t *trie, trie_node_t *node, 
                                                            unsigned char *path);

static int IsLeaf(trie_node_t *node);
static void *FreeFunction(void *to_free);

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
int TrieInsert(trie_t *trie, const unsigned char *path)
{
    trie_node_t *runner = NULL;
    size_t index = 0;
    size_t num_bytes = 0;
    
    assert(NULL != trie);
    
    runner = trie->root;
    num_bytes = GetNumBytes(trie);
/*    index = GetByteIndex(num_bytes); */
    
    for (index = 0; index < num_bytes; ++index) /*FIXME function? recursion ?*/
    {
/*        runner = TrieInsertRecursive(trie, runner, (unsigned char *)path, 0,*/
/*                                        index); */
        size_t bit_index = 0;
        int direction = 3;
            
        for (bit_index = 0 ; bit_index < CHAR_BIT; ++bit_index)
        {
            direction = GetBitDirection(path[index], bit_index);

            runner = InsertInDirection(runner, direction);
            if (NULL == runner)
            {
                return 1;
            }

            runner = runner->child[direction]; 
        }
    }
    
    return 0;
} 

static trie_node_t *TrieInsertRecursive(trie_t *trie, trie_node_t *node, 
                                        unsigned char *path, size_t bit_index,
                                        size_t path_byte_index)
{
    int direction = 0;
    
    assert(NULL != trie);
    
    direction = GetBitDirection(path[path_byte_index], bit_index);
    
    node = InsertInDirection(node, direction);
    if (NULL == node)
    {
        return NULL;
    }

    if (CHAR_BIT == bit_index)
    {
        return node;
    }
    
    return (TrieInsertRecursive(trie, node->child[direction], path, ++bit_index, 
                                                            path_byte_index));
}

static trie_node_t *InsertInDirection(trie_node_t *node, int direction)
{
    assert(NULL != node);
    
    if (NULL == node->child[direction])
    {
        node->child[direction] = CreateNode();
        if (NULL == node->child[direction])
        {
            return NULL;
        }
    }
    return node;
}

void TrieRemove(trie_t *trie, const unsigned char *path)
{
    assert(NULL != trie);

    if (TrieIsEmpty(trie))
    {
        return;
    }

    RemoveRecursive(trie, trie->root, (unsigned char *)path); 
}

/*************** Removes recursively the leaf in the path *********************/
static trie_node_t *RemoveRecursive(trie_t *trie, trie_node_t *node, 
                                                            unsigned char *path)
{
    int direction = 3;
    static size_t byte_index = 0;
    static size_t bit_index = 0;
    
    assert(NULL != trie);
    
    if (NULL == node)
    {
        return NULL;
    }
    
    if (CHAR_BIT == bit_index)
    {
        bit_index = 0;
        byte_index = (byte_index == GetNumBytes(trie)) ? 0 : ++byte_index;
    }
    
    direction = GetBitDirection(path[byte_index], bit_index++);

    node->child[direction] = RemoveRecursive(trie, 
                                             node->child[direction], path);
    if (node == trie->root)
    {
        return node;
    }
    if ( IsLeaf(node)) /*FIXME*/
    {
        node = FreeFunction(node);
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
int TrieIsEmpty(const trie_t *trie)
{
    assert(NULL != trie);
    
    return (NULL == trie->root->child[0] && NULL == trie->root->child[1]);
} 

/****FIXME Find if path is in trie tree. If path wasn't found returns 0 ************/
/*int TrieFind(const trie_t *trie, unsigned char *path)*/
/*{*/
/*    trie_node_t *runner = NULL;*/
/*    size_t index = 0;*/
/*    size_t num_bytes = 0;*/
/*    */
/*    assert(NULL != trie);*/
/*    */
/*    if (TrieIsEmpty(trie))*/
/*	{*/
/*		return 0; */
/*	}*/

/*    runner = trie->root;*/
/*    */
/*    num_bytes = GetNumBytes(trie);*/
/*    index = GetByteIndex(num_bytes); */
/*    */
/*    for (; index <= num_bytes; ++index) */
/*    {*/
/*        if(NULL != TrieFindRecursive((trie_t *)trie, runner, path, 0, index))*/
/*        {*/
/*            return 1;*/
/*        }*/
/*    }*/
/*    */
/*    return (NULL != runner && runner->is_path_end);*/
/*}*/

/*************** Find path recursively ****************************************/
static trie_node_t *TrieFindRecursive(trie_t *trie, trie_node_t *node, 
                                      unsigned char *path, size_t bit_index,
                                      size_t path_byte_index)
{
    int direction = 0;
    
    assert(NULL != trie);
    
    direction = GetBitDirection(path[path_byte_index], bit_index);
    
    if (NULL == node->child[direction])
    {
        return NULL;
    }

    if (CHAR_BIT == bit_index)
    {
        bit_index = 0;
        return node;
    }
    
    return (TrieFindRecursive(trie, node->child[direction], path, ++bit_index, 
                                                            path_byte_index));
}

static int GetBitDirection(unsigned char num, size_t bit_index)
{
	unsigned char mask = 0x80;
	
    /* return 1 if it's on and 0 otherwise */
    return (0 != ((mask >> bit_index) & num));
}

static size_t GetNumBytes(const trie_t *trie)
{
    assert(NULL != trie);
    
    return (trie->height / CHAR_BIT);
}

static size_t GetByteIndex(size_t num_bytes)
{
    return (BYTES_IN_IP - num_bytes);
}

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

int TrieFind(const trie_t *trie, unsigned char *path)
{
    trie_node_t *runner = NULL;
    size_t index = 0;
    size_t num_bytes = 0;
    
    assert(NULL != trie);
    
    if (TrieIsEmpty(trie))
	{
		return 0; 
	}
    
    runner = trie->root;
    num_bytes = trie->height / CHAR_BIT;
    
/*    index = BYTES_IN_IP - num_bytes; */
    
    for (index = 0 ; index < num_bytes; ++index) 
    {
        size_t bit_index = 0;
        int direction = 3;
            
        for (bit_index = 0 ; bit_index < CHAR_BIT; ++bit_index)
        {
            direction = GetBitDirection(path[index], bit_index);

            if (NULL == runner->child[direction])
            {
                return 0;
            }

            runner = runner->child[direction]; 
        }
    }
    return (NULL != runner);
}

/*int TrieFind( trie_t *trie, unsigned char *path)*/
/*{*/
/*    trie_node_t *runner = NULL;*/
/*    size_t bit_index = 0;*/
/*    int direction = 3;*/
/*            */
/*    assert(NULL != trie);*/
/*    */
/*    if (TrieIsEmpty(trie))*/
/*	{*/
/*		return 0; */
/*	}*/
/*    */
/*    runner = trie->root;*/
/*            */
/*    for (bit_index = 0 ; bit_index < trie->height; ++bit_index)*/
/*    {*/
/*        direction = GetBitDirection(path, bit_index, GetMask(trie));*/

/*        if (NULL == runner->child[direction])*/
/*        {*/
/*            return 0;*/
/*        }*/

/*        runner = runner->child[direction]; */
/*    }*/
/*    */
/*    return (NULL != runner);*/
/*}*/
