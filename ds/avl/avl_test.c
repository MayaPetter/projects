#include <assert.h> /* assert */
#include <stdio.h>  /* printf */

#include "avl.h"

#define UNUSED(x) ((void)(x)) /* for cmp_func */

int ARRAY[] = {4, 5, 8, 3, 2, 9, 1, 6};

int cmp(const void *tree_data, const void *new_data, void *param)
{
    UNUSED(param);

    return (*(int *)tree_data - *(int *)new_data); 
}

static void CreateDestroyTest()
{
    avl_t *avl = AVLCreate(NULL, cmp);
    
    assert(NULL != avl);
    assert(AVLIsEmpty(avl));
    assert(0 == AVLCount(avl));

    AVLDestroy(avl);
}

static avl_t *CreateAVLTest()
{
    size_t i = 0;
    avl_t *avl = AVLCreate(NULL, cmp);
    
    assert(NULL != avl);
    
    for (i = 0; i < 8; ++i)
    {
        int status = 0;
            
        status = AVLInsert(avl, &ARRAY[i]);
        assert(1 != status);
        assert(!AVLIsEmpty(avl));
        assert((i + 1) == AVLCount(avl));        
    } 
    
    return avl;    
}

static void InsertTest()
{
    avl_t *avl = CreateAVLTest();
    int status = 0;
    int data_to_insert = 0, another_data = 7;
    
    assert(NULL != avl);
    
    status = AVLInsert(avl, &data_to_insert); 
    assert(1 != status);
    assert(!AVLIsEmpty(avl));
    assert(9 == AVLCount(avl));
    
    status = AVLInsert(avl, &another_data); 
    assert(1 != status);
    assert(!AVLIsEmpty(avl));
    assert(10 == AVLCount(avl));

    AVLDestroy(avl);
}

static void RemoveTest()
{
    avl_t *avl = CreateAVLTest();
    int data_not_in_tree = 0;
    size_t i = 0;
          
    assert(NULL != avl);
    assert(8 == AVLCount(avl));

    AVLRemove(avl, &data_not_in_tree);
    assert(8 == AVLCount(avl));        

    for (i = 0; i < 8; ++i)
    {
        AVLRemove(avl, &ARRAY[i]);
        assert((7 -i) == AVLCount(avl));        
    } 
    
    assert(AVLIsEmpty(avl));

    AVLDestroy(avl);
}

static void HightTest()
{
    avl_t *avl = AVLCreate(NULL, cmp);
    int data_to_insert = 0, another_data = 7;
    
    assert(NULL != avl);
    
    assert(0 == AVLHeight(avl));
    
    /* changes in height after insertion */
    assert(1 != AVLInsert(avl, &ARRAY[0]));
    assert(1 == AVLHeight(avl));
    assert(1 != AVLInsert(avl, &ARRAY[1]));
    assert(2 == AVLHeight(avl));
    assert(1 != AVLInsert(avl, &ARRAY[2]));
    assert(2 == AVLHeight(avl));
    assert(1 != AVLInsert(avl, &ARRAY[3]));
    assert(3 == AVLHeight(avl));
    assert(1 != AVLInsert(avl, &ARRAY[4]));
    assert(3 == AVLHeight(avl));
    assert(1 != AVLInsert(avl, &ARRAY[5]));
    assert(3 == AVLHeight(avl));
    assert(1 != AVLInsert(avl, &ARRAY[6]));
    assert(4 == AVLHeight(avl));
    assert(1 != AVLInsert(avl, &ARRAY[7]));
    assert(4 == AVLHeight(avl));
    assert(1 != AVLInsert(avl, &data_to_insert));
    assert(4 == AVLHeight(avl));
    assert(1 != AVLInsert(avl, &another_data));
    assert(4 == AVLHeight(avl));

    /* changes in height after removing */
    AVLRemove(avl, &ARRAY[1]);
    assert(4 == AVLHeight(avl));
    AVLRemove(avl, &ARRAY[7]);
    assert(4 == AVLHeight(avl));
    AVLRemove(avl, &ARRAY[6]);
    assert(4 == AVLHeight(avl));
    AVLRemove(avl, &ARRAY[2]);
    assert(4 == AVLHeight(avl));
    AVLRemove(avl, &another_data);
    assert(3 == AVLHeight(avl));
    AVLRemove(avl, &data_to_insert);
    assert(3 == AVLHeight(avl));
    AVLRemove(avl, &ARRAY[4]);
    assert(2 == AVLHeight(avl));
    AVLRemove(avl, &ARRAY[3]);
    assert(2 == AVLHeight(avl));
    AVLRemove(avl, &ARRAY[0]);
    assert(1 == AVLHeight(avl));
    AVLRemove(avl, &ARRAY[5]);
    assert(0 == AVLHeight(avl));
    
    assert(AVLIsEmpty(avl));
    
    AVLDestroy(avl);
}

int ActionFunc(void *tree_data, void *param)
{
    UNUSED(param);
    
    printf("%d \n", *(int *)tree_data);
    
    return 0;
}

static void ForEachTest()
{
    avl_t *avl = CreateAVLTest();
    
    assert(1 != AVLForEach(NULL, avl, ActionFunc));

    AVLDestroy(avl);
}

static void FindTest()
{
    avl_t *avl = CreateAVLTest();
    int data_not_in_tree = 0;
    size_t i = 0;
    int data_to_insert = 7;
    
    assert(NULL != avl);
    assert(8 == AVLCount(avl));

    assert(NULL == AVLFind(avl, &data_not_in_tree));
    assert(1 != AVLInsert(avl, &data_not_in_tree)); 
    assert(data_not_in_tree == *(int *)AVLFind(avl, &data_not_in_tree));
    
    for (i = 0; i < 8; ++i)
    {
        assert(ARRAY[i] == *(int *)AVLFind(avl, &ARRAY[i]));
    } 

    assert(1 != AVLInsert(avl, &data_to_insert));
    assert(data_to_insert == *(int *)AVLFind(avl, &data_to_insert));
    
    assert(1 != AVLForEach(NULL, avl, ActionFunc));
    
    AVLDestroy(avl);
}

int main()
{
    CreateDestroyTest();
    InsertTest();
    RemoveTest();
    HightTest();
    ForEachTest(); 
    FindTest();
          
    return 0;
}
