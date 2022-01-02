#include <assert.h> /* assert */
#include <stdio.h>  /* printf */

#include "bst.h"

#define UNUSED(x) ((void)(x))

int cmp_func(void *iter_data, void *new_data, void *param)
{
    assert(NULL != iter_data);
    assert(NULL != new_data);
    
    UNUSED(param);
    
    return (*(int *)iter_data - *(int *)new_data);    
}

static void CreateDestroyTest()
{
    bst_t *bst = BSTCreate(cmp_func, NULL);
    assert(NULL != bst);
    
    assert(1 == BSTIsEmpty(bst));
    assert(0 == BSTSize(bst));
    
    BSTDestroy(bst);
} 

static int ARR[] = {1, 5, 3, 0, 4, 2, 6, 8};

static bst_t *CreateBst()
{
    size_t i = 0;
    bst_t *bst = BSTCreate(&cmp_func, NULL);
    assert(NULL != bst);
    
    assert(1 == BSTIsEmpty(bst));
    assert(0 == BSTSize(bst));
    
    for (i = 0; i < 8; ++i)
    {       
        bst_iter_t new = BSTInsert(bst, &ARR[i]);
        assert(NULL != new);
        assert(!BSTIsEmpty(bst));
        assert(i + 1 == BSTSize(bst));    
    }

    return bst;
} 

static void InsertTest()
{
    int new_num = 9, another_num = 7;
    bst_t *bst = CreateBst();
    bst_iter_t new = NULL;
    assert(NULL != bst);
    
    new =  BSTInsert(bst, &new_num);
    assert(NULL != new);
    assert(9 == BSTSize(bst));
    
    new =  BSTInsert(bst, &another_num);
    assert(NULL != new);
    assert(10 == BSTSize(bst));

    BSTDestroy(bst);
}

static void RemoveTest()
{
    bst_t *bst = CreateBst();
    int new_data = 7;
    size_t i = 0;
    bst_iter_t new = NULL;
    assert(NULL != bst);

    new =  BSTInsert(bst, &new_data);
    assert(NULL != new);
    assert(9 == BSTSize(bst));

    BSTRemove(new);
    assert(8 == BSTSize(bst));
    
    for (i = 0; i < 8; ++i)
    {       
        bst_iter_t new = BSTFind(bst, &ARR[i]);
        assert(NULL != new);
        BSTRemove(new);
        assert((7 - i) == BSTSize(bst));    
    }
    
    assert(BSTIsEmpty(bst));

    BSTDestroy(bst);
}

static void IterTest()
{
    bst_t *bst = CreateBst();
    assert(NULL != bst);

    assert(BSTIsSameIter(BSTBegin(bst), BSTFind(bst, &ARR[3])));
    
    assert(BSTIsSameIter(BSTNext(BSTBegin(bst)), BSTFind(bst, &ARR[0])));
    assert(BSTIsSameIter(BSTBegin(bst), BSTPrev(BSTFind(bst, &ARR[0]))));
    
    assert(!BSTIsSameIter(BSTBegin(bst), BSTEnd(bst)));
    
    assert(ARR[3] == *(int *)BSTGetData(BSTFind(bst, &ARR[3])));

    BSTDestroy(bst);
}

int PrintData(void *iter_data, void *param)
{
    UNUSED(param);
    printf("%d \n", *(int *)iter_data);
    
    return 0;
}

static void ForEachTest()
{
    bst_t *bst = CreateBst();
    int new_data = 7;
    assert(NULL != bst);
    
    BSTForEach(NULL, BSTBegin(bst), BSTEnd(bst), PrintData);
    
    BSTInsert(bst, &new_data);
    BSTForEach(NULL, BSTBegin(bst), BSTEnd(bst), PrintData);
    
    BSTDestroy(bst);
}

int main()
{
    CreateDestroyTest();
    InsertTest();
    RemoveTest();
    IterTest();
    ForEachTest();
            
    return 0;
}

