#include <math.h>   /*  pow    */
#include <stdio.h>  /* _LINE_ */
#include <limits.h> /* CHAR_BIT */

#include "trie.h"

#define Expect(x) \
;{\
	if (!(x))\
	{\
		printf("test failed in line %d\n", __LINE__);\
		return 0;\
	}\
} /* end of Expect */

/*************** Test Trie Functions ******************************************/
static int TrieCreateDestroyTest();
static int TrieInsertFindRemoveTest();

int main()
{
    TrieCreateDestroyTest();
    TrieInsertFindRemoveTest();
    
    return 0;
}

static int TrieCreateDestroyTest()
{
    size_t height = 24;
    trie_t *new_trie = TrieCreate(height);
    
    Expect(NULL != new_trie)
    Expect(TrieIsEmpty(new_trie))
    
    Expect(pow(2, height) == TrieCapacity(new_trie))
    
    Expect(0 == TrieCount(new_trie))     
    
    TrieDestroy(new_trie);
    
    return 1;
}

static int TrieInsertFindRemoveTest()
{
    unsigned char ip[] = {10, 0, 0}, other_ip[] = {15, 24, 255};
    unsigned char not[] ={69, 50, 59};
    
    size_t height = sizeof(ip) * CHAR_BIT;
    
    trie_t *trie = TrieCreate(height);
    
    Expect(NULL != trie)
    Expect(TrieIsEmpty(trie))
    
    Expect(0 == TrieInsert(trie, ip))
    Expect(!TrieIsEmpty(trie))
    Expect(1 == TrieCount(trie))
    
    TrieRemove(trie, ip);
    Expect(TrieIsEmpty(trie))
    Expect(0 == TrieCount(trie))
    
    Expect(0 == TrieInsert(trie, ip))
    Expect(!TrieIsEmpty(trie))
    Expect(1 == TrieCount(trie))
    
    Expect(0 == TrieInsert(trie, other_ip))
    Expect(2 == TrieCount(trie))
    
    Expect(TrieFind(trie, ip))
    Expect(TrieFind(trie, other_ip))
    Expect(!TrieFind(trie, not))
    
    TrieRemove(trie, ip);
    Expect(!TrieFind(trie, ip))
    Expect(1 == TrieCount(trie))
    Expect(0 == TrieFind(trie, ip))
    Expect(1 == TrieFind(trie, other_ip))
    Expect(0 == TrieFind(trie, not))
    
    TrieRemove(trie, not);
    Expect(1 == TrieCount(trie))
    Expect(0 == TrieFind(trie, ip))
    Expect(1 == TrieFind(trie, other_ip))
    Expect(0 == TrieFind(trie, not))
    
    TrieRemove(trie, other_ip);
    Expect(0 == TrieCount(trie))
    Expect(TrieIsEmpty(trie))
    Expect(0 == TrieFind(trie, ip))
    Expect(0 == TrieFind(trie, other_ip))
    Expect(0 == TrieFind(trie, not))
    
    TrieDestroy(trie);
    
    return 1;
}

