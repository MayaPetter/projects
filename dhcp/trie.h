#ifndef __ILRD_TRIE_H
#define __ILRD_TRIE_H

#include <stddef.h> /* size_t */

typedef struct trie trie_t;

/*
 * Create new trie .
 * Param: @height - users binary trie height.
 * Return: pointer to new trie.
 * Errors: if memory allocation failed, return NULL.
 */
trie_t *TrieCreate(size_t height);

/* 
 * Destroy trie tree.
 * Param: @trie - pointer to trie tree.
 */
void TrieDestroy(trie_t *trie);

/*
 * Insert new path to tree.
 * Param: @trie - pointer to trie tree. 
 * Param: @path - pointer to key to insert.
 * Return: 0 on success.
 * Errors: if memory allocation failed, return 1.
 */
int TrieInsert(trie_t *trie,  unsigned char *path); 

/*
 * Removes path from trie.
 * Param: @trie - pointer to trie.
	      @path: path to remove.
 * Errors: if path not found, do nothing.
 */
void TrieRemove(trie_t *trie,  unsigned char *path);

/* 
 * Count how many paths in given trie tree.
 * Param:  @trie - trie tree.
 * Return: number of existing paths in trie.
 */
size_t TrieCount(trie_t *trie); 

/* 
 * Calculate the maximum capacity of trie tree.
 * Param:  @trie - trie tree.
 * Return: number of paths of trie.
 * Error: height can be 31 at most for this calculation
 */
size_t TrieCapacity(trie_t *trie); 

/*
 * Check if tree is empty.
 * Param:  @trie - trie tree.
 * Return: 1 if trie is empty, 0 otherwise.
 */
int TrieIsEmpty( trie_t *trie); 

/*
 * Find if path is in trie tree.
 * Param:  @trie - pointer to trie tree.
 * Param:  @path - path to find.
 * Return: pointer to found end of path.
 * Errors: if path was not found, return 0.
 */
int TrieFind( trie_t *trie, unsigned char *path);

#endif /* __ILRD_TRIE_H */

