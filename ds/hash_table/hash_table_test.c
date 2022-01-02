/******************************************
* Author:   Maya Petter                   *
* Reviewer:                               *
* Purpose:                                *
*******************************************/
#include <assert.h>    /* assert */
#include <stdio.h>     /* fscanf, fopen, fclose */
#include <string.h>    /* string functions */
#include <stdlib.h>    /* malloc, free */

#include "hash_table.h" /* API    */
#include "dlist.h"      /* dlist  */

#define UNUSED(x) ((void)(x))
#define NUM_OF_WORDS 99171

const size_t ARRAY_SIZE = 1381;

/* helper functions */
size_t HashingFunc(void *data);
int HashCmp(const void *key1, const void *key2);
int ActionFunc(void *table_data, void *param);
int PrintFunc(void *table_data, void *param);
int FailFunc(void *table_data, void *param);
int FreeFunction(void *table_data, void *param);

/* Tests */
static void CreateDestroyTest();
static void InsertTest();
static void RemoveTest();
static void FindTest();
static void ForEachTest();
static void DictionaryTest();
extern double CalculateVariance(hash_table_t *hash_table);

size_t HashingFunc(void *data)
{
    char *runner = NULL;
    size_t key = 0;
    size_t sum = 0;
    
    assert(NULL != data);
    
    for (runner = (char *)data; '\0' != *runner; ++runner)
    {
        sum += *runner;
        sum <<= 2;
    }

    key = sum % ARRAY_SIZE;
    
    return key;
}

int HashCmp(const void *key1, const void *key2)
{   
    return strcmp(key1, key2);
}

int ActionFunc(void *table_data, void *param)
{
    return (*(char *)table_data + *(char *)param);
}

int PrintFunc(void *table_data, void *param)
{
    UNUSED(param);
    
    printf("%s\n", (char *)table_data);
    
    return 0;
}

int FreeFunction(void *table_data, void *param)
{
    UNUSED(param);
    
    free(table_data);
    table_data = NULL;
    
    return 0;
}

int FailFunc(void *table_data, void *param)
{
    UNUSED(table_data);
    UNUSED(param);
    
    return 1;
}

static void CreateDestroyTest()
{
    hash_table_t *new_hash = HashTableCreate(10, HashingFunc, HashCmp); 
    
    assert(NULL != new_hash);
    assert(0 == HashTableSize(new_hash));
    assert(HashTableIsEmpty(new_hash));
    
    HashTableDestroy(new_hash);
}

static void InsertTest()
{
    hash_table_t *new_hash = HashTableCreate(ARRAY_SIZE, HashingFunc, HashCmp); 
    char str1[] = "hi";
    char str2[] = "By";
    char str3[] = "Hello";
        
    assert(NULL != new_hash);
    assert(0 == HashTableSize(new_hash));
    assert(HashTableIsEmpty(new_hash));
    
    assert(0 == HashTableInsert(new_hash, str1));
    assert(1 == HashTableSize(new_hash));
    assert(!HashTableIsEmpty(new_hash));
    
    assert(0 == HashTableInsert(new_hash, str2));
    assert(2 == HashTableSize(new_hash));
    assert(!HashTableIsEmpty(new_hash));

    assert(0 == HashTableInsert(new_hash, str3));
    assert(3 == HashTableSize(new_hash));
    assert(!HashTableIsEmpty(new_hash));

    HashTableDestroy(new_hash);
}

static void RemoveTest()
{
    hash_table_t *new_hash = HashTableCreate(ARRAY_SIZE, HashingFunc, HashCmp); 
    char str1[] = "hi";
    char str2[] = "By";
    char str3[] = "Hello";
        
    assert(NULL != new_hash);
    assert(0 == HashTableSize(new_hash));
    assert(HashTableIsEmpty(new_hash));
    
    assert(0 == HashTableInsert(new_hash, str1));
    assert(0 == HashTableInsert(new_hash, str2));
    assert(0 == HashTableInsert(new_hash, str3));
    assert(3 == HashTableSize(new_hash));
    assert(!HashTableIsEmpty(new_hash));

    HashTableRemove(new_hash, str2);
    assert(2 == HashTableSize(new_hash));
    assert(!HashTableIsEmpty(new_hash));
    
    HashTableRemove(new_hash, str1);
    assert(1 == HashTableSize(new_hash));
    assert(!HashTableIsEmpty(new_hash));
    
    HashTableRemove(new_hash, str3);
    assert(0 == HashTableSize(new_hash));
    assert(HashTableIsEmpty(new_hash));
    
    HashTableDestroy(new_hash);
}

static void FindTest()
{
    hash_table_t *new_hash = HashTableCreate(ARRAY_SIZE, HashingFunc, HashCmp); 
    char str1[] = "hi";
    char str2[] = "By";
    char str3[] = "Hello";
        
    assert(NULL != new_hash);
    assert(0 == HashTableSize(new_hash));
    assert(HashTableIsEmpty(new_hash));
    
    assert(0 == HashTableInsert(new_hash, str1));
    assert(0 == HashTableInsert(new_hash, str2));
    assert(2 == HashTableSize(new_hash));
    
    assert(str1[0] == *(char *)HashTableFind(new_hash, str1));
    assert(str2[0] == *(char *)HashTableFind(new_hash, str2));
    assert(NULL == HashTableFind(new_hash, str3));
    
    HashTableDestroy(new_hash);
}

static void DictionaryTest()
{
    FILE *file_ptr = NULL;
    const char *file_name = "/usr/share/dict/words";
    char new_word[50] = "";
    char *to_remove = NULL;
    
    size_t i = 0;
    char zoology[] = "zoology";
    char eclats[] = "éclat's";
    
    hash_table_t *new_hash = HashTableCreate(ARRAY_SIZE, HashingFunc, HashCmp);
    assert(NULL != new_hash);

    file_ptr = fopen(file_name, "r");
    if (NULL == file_ptr)
    {
        return;
    }

    for (i = 0
        ; i < (NUM_OF_WORDS + 1) && 1 == fscanf(file_ptr, "%50s", new_word)
        ; ++ i)
    {
        size_t length = (strlen(new_word) + 1) * sizeof(char);
        
        char *to_insert = (char *)malloc(length);
        
        memcpy(to_insert, new_word, length);
        
        assert(0 == HashTableInsert(new_hash, to_insert));
    }

    assert(NUM_OF_WORDS == HashTableSize(new_hash));

/*    assert(0 == HashTableForEach(new_hash, PrintFunc, &i));*/
    assert(1 == HashTableForEach(new_hash, FailFunc, &i));
    
    assert(NULL != HashTableFind(new_hash, zoology));
    assert(zoology[0] == *(char *)HashTableFind(new_hash, zoology));
    
    assert(NULL != HashTableFind(new_hash, eclats));
    assert(eclats[0] == *(char *)HashTableFind(new_hash, eclats));
    
    printf("\n\nvariance %f \n\n", CalculateVariance(new_hash));
    to_remove = HashTableFind(new_hash, zoology);
    HashTableRemove(new_hash, zoology);

    free(to_remove); to_remove = NULL; 

    assert(0 == HashTableForEach(new_hash, FreeFunction, &i));
    fclose(file_ptr); 
    HashTableDestroy(new_hash);
}

static void ForEachTest()
{
    hash_table_t *new_hash = HashTableCreate(ARRAY_SIZE, HashingFunc, HashCmp); 
    char str1[] = "hi";
    char str2[] = "By";
    char str3[] = "Hello";
    int i = 1;
        
    assert(NULL != new_hash);
    assert(0 == HashTableSize(new_hash));
    assert(HashTableIsEmpty(new_hash));
    
    assert(0 == HashTableInsert(new_hash, str1));
    assert(0 == HashTableInsert(new_hash, str2));
    assert(0 == HashTableInsert(new_hash, str3));
    assert(3 == HashTableSize(new_hash));
    assert(!HashTableIsEmpty(new_hash));
    
    assert(0 == HashTableForEach(new_hash, PrintFunc, &i));
    assert(1 == HashTableForEach(new_hash, FailFunc, &i)); 

    HashTableDestroy(new_hash);
}

int main()
{
/*    CreateDestroyTest();*/
/*    InsertTest();*/
/*    RemoveTest();*/
/*    FindTest();*/
/*    ForEachTest();      */
    DictionaryTest();   
    
    return 0;
}
