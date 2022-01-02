/******************************************
* Author:   Maya Petter                   *
* Reviewer:                               *
* Purpose:                                *
*******************************************/
#include <assert.h> /* assert */
#include <stdlib.h> /* malloc, free */ 
#include <math.h>   /* pow */
#include <stdio.h>  /* printf */

#include "hash_table.h"
#include "dlist.h"

/*************** Static functions *********************************************/
static void *FreeFunction(void *to_free);
static dlist_t **InitHashTable(size_t num_of_buckets);
static void DestroyLists(dlist_t **hash_table, size_t size);
static size_t GetHashKey(hash_table_t *hash_table, void *data);
static dlist_iter_t FindPosition(hash_table_t *hash_table, void *data);
static void PrintVariance(hash_table_t *hash_table);
double CalculateVariance(hash_table_t *hash_table);

/*************** API functions and struct & enum definitions ******************/
enum status {HASH_SUCSS, HASH_FAIL};

typedef int (*hash_cmp_wrapper)(void *key1, const void *key2);

struct hash_table
{
    size_t num_of_buckets;
    size_t (*hash_func)(void *data);
    int (*hash_cmp_func)(const void *key1, const void *key2);
    dlist_t **hash_table; 
};

/*************** Create new hash table ****************************************/
hash_table_t *HashTableCreate(size_t num_of_buckets,
							  size_t (*hash_func)(void *data),
							  int (*hash_cmp_func)(const void *key1,
							  				  	   const void *key2))
{
    hash_table_t *new_table = NULL;
    
    assert(0 != num_of_buckets);
    assert(NULL != hash_func);
    assert(NULL != hash_cmp_func);
    
    new_table = (hash_table_t *)malloc(sizeof(struct hash_table));
    if (NULL == new_table)
    {
        return NULL;
    }
    
    /* initiate management members */
    new_table->num_of_buckets = num_of_buckets;
    new_table->hash_func = hash_func;
    new_table->hash_cmp_func = hash_cmp_func;
    
    /* initiate hash table */ 
    new_table->hash_table = InitHashTable(num_of_buckets); 
    if (NULL == new_table->hash_table)
    {
        new_table = FreeFunction(new_table);
        return NULL;
    }

    return new_table;
}

/*************** Destroy the hash table ***************************************/
void HashTableDestroy(hash_table_t *hash_table)
{
    assert(NULL != hash_table);
    
    DestroyLists(hash_table->hash_table, hash_table->num_of_buckets);
    
    hash_table->hash_table = FreeFunction(hash_table->hash_table);    
    
    hash_table = FreeFunction(hash_table);
}

/*************** Insert data to the hash table ********************************/
int HashTableInsert(hash_table_t *hash_table, void *data)
{
    size_t key = HASH_FAIL; 
    dlist_iter_t inserted = NULL;
    dlist_iter_t end = NULL;
    
    assert(NULL != hash_table);
    
    key = GetHashKey(hash_table, data);
    end = DListEnd(hash_table->hash_table[key]);
    
    inserted = DListPushFront(hash_table->hash_table[key], data);
    if (DListIsSameIter(inserted, end))
    {
        return HASH_FAIL;
    }
    
    return HASH_SUCSS;
}

/*************** Remove data from the hash table ******************************/
void HashTableRemove(hash_table_t *hash_table, void *data)
{
    dlist_iter_t to_remove = NULL;
    
    assert(NULL != hash_table);
    
    to_remove = FindPosition(hash_table, data);
    if (NULL == to_remove)
    {
        return;
    }
    
    DListRemove(to_remove);
}

/*************** Find data in the hash table **********************************/
void *HashTableFind(const hash_table_t *hash_table, void *data)
{
    dlist_iter_t found_data = NULL;
    
    assert(NULL != hash_table);
    
    found_data = FindPosition((hash_table_t *)hash_table, data);
    if (NULL == found_data)
    {
        return NULL;
    }
    
    return DListIterGetData(found_data);
}

/** Perform action for each in hash table, stops if action returns non-zero **/
int HashTableForEach(const hash_table_t *hash_table,
					 int (*act_func)(void *table_data, void *param), 
					 void *param)
{
    size_t index = 0;
    int status = 0;
    
    assert (NULL != hash_table);
    
    for (index = 0, status = 0
        ;index < hash_table->num_of_buckets && (0 == status)
        ; ++index)
    {
        dlist_iter_t beginning = DListBegin(hash_table->hash_table[index]);
        dlist_iter_t end = DListEnd(hash_table->hash_table[index]);
    
        status = DListForEach(beginning, end, act_func, param);
    }
    
    return status;    
}

/*************** Get size of the hash table ***********************************/
size_t HashTableSize(const hash_table_t *hash_table)
{
    size_t index = 0;
    size_t size = 0;
    
    assert (NULL != hash_table);
    
    for (index = 0; index < hash_table->num_of_buckets; ++index)
    {
        size += DListSize(hash_table->hash_table[index]);
    }
    
    return size;
}

/*************** Check if hash table is empty ***********************************/
int HashTableIsEmpty(const hash_table_t *hash_table)
{
    size_t index = 0;
    int is_empty = 0;
    
    assert (NULL != hash_table);
    
    for (index = 0; index < hash_table->num_of_buckets; ++index)
    {
        is_empty = DListIsEmpty(hash_table->hash_table[index]);
        if (0 == is_empty)
        {
            return 0;
        }
    }
    return is_empty;
}

/*************** Static functions *********************************************/
static dlist_t **InitHashTable(size_t num_of_buckets)
{
    dlist_t **hash_table = NULL;
    size_t index = 0;
    
    hash_table = (dlist_t **)malloc(num_of_buckets * sizeof(dlist_t *));
    if (NULL == hash_table)
    {
        return NULL;
    }
    
    /* initiate each of hash_table indexes to store separate d-list */
    for (index = 0; index < num_of_buckets; ++index)
    {
        hash_table[index] = DListCreate();

        /* if failed to create, destroy all previous lists */
        if (NULL == hash_table[index])
        {
            DestroyLists(hash_table, index);

            hash_table = FreeFunction(hash_table); 
            
            return NULL;
        }
    }
    
    return hash_table;
}

/*************** Destroy every list in the hash table *************************/
static void DestroyLists(dlist_t **hash_table, size_t size)
{
    size_t index = 0;
    
    assert (NULL != hash_table);
    
    for (index = 0; index < size; ++index)
    {
        DListDestroy(hash_table[index]);
    }
}

/*************** Free pointer *************************************************/
static void *FreeFunction(void *to_free)
{
    assert(NULL != to_free);
    
    free(to_free);
    
    to_free = NULL;
    
    return NULL;
}

/*************** Find hash key ************************************************/
static size_t GetHashKey(hash_table_t *hash_table, void *data)
{
    assert(NULL != hash_table);
    
    return hash_table->hash_func(data);
}

/*************** Find the position of data in the hash table ******************/
static dlist_iter_t FindPosition(hash_table_t *hash_table, void *data)
{
    size_t key = 0;
    dlist_iter_t found_node = NULL;
    dlist_iter_t beginning = NULL;
    dlist_iter_t end = NULL;
    
    assert(NULL != hash_table);
    
    key = GetHashKey((hash_table_t *)hash_table, data);
    
    beginning = DListBegin(hash_table->hash_table[key]);
    end = DListEnd(hash_table->hash_table[key]);
    
    found_node = DListFind(beginning, end, 
                          (hash_cmp_wrapper)hash_table->hash_cmp_func, data);
    if (DListIsSameIter(found_node, end))
    {
        return NULL;
    }
    
    return found_node;
}

/*************** Calculate variance *******************************************/
static void PrintVariance(hash_table_t *hash_table)
{
    size_t index = 0;
    
    assert (NULL != hash_table);
    
    for (index = 0; index < hash_table->num_of_buckets; ++index)
    {
        if (0 == (index % 20))
        {
            puts("\n");
        }
 
        printf("%lu, ", DListSize(hash_table->hash_table[index]));
    }
}

double CalculateVariance(hash_table_t *hash_table)
{
    size_t index = 0;
    double sum = 0.0;
    double average = 0.0;
    
    assert (NULL != hash_table);
    assert (0 != hash_table->num_of_buckets);

    average = HashTableSize(hash_table) / hash_table->num_of_buckets;   
    
    for (index = 0; index < hash_table->num_of_buckets; ++index)
    {
        sum += pow(((double)DListSize(hash_table->hash_table[index]) - (double)average), 2);
    }
    
    sum /= (double)hash_table->num_of_buckets;
    
    PrintVariance(hash_table);
       
    return sum;
}
