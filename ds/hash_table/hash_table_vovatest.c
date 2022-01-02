#include <stdio.h> /* puts */
#include <stdlib.h> /* malloc, free */
#include <assert.h> /* assert */
#include <string.h> /* strcmp */

#include "hash_table.h" /* hash_table API, size_t */

/* Removes warnings of specified unused variable */
#define UNUSED(x) ((void)x)

/********************************************* Functions to use in Hash Table */
static size_t HashFunc(void *key)
{
	return (size_t)(*(unsigned char *)key);
}

static int HashCmp(const void *key1, const void *key2)
{
	return strcmp(key1, key2);
}

static int PrintForEach(void *table_data, void *param)
{
	UNUSED(param);
	puts((char *)table_data);
	return 0;
}

static int FreeForEach(void *table_data, void *param)
{
	UNUSED(param);
	free(table_data);
	return 0;
}

/************************************************************* Test Functions */
static void CreateDestroy_test()
{
	hash_table_t *hash_table = HashTableCreate(255, HashFunc, HashCmp);
	
	assert(NULL != hash_table);
	assert(1 == HashTableIsEmpty(hash_table));
	
	HashTableDestroy(hash_table);
}

static void Insert_test()
{
	char *str = NULL;
	hash_table_t *hash_table = HashTableCreate(255, HashFunc, HashCmp);
	
	assert(NULL != hash_table);
	assert(1 == HashTableIsEmpty(hash_table));

	str = "xyz";
	assert(0 == HashTableInsert(hash_table, str));
	assert(1 == HashTableSize(hash_table));

	str = "xzy";
	assert(0 == HashTableInsert(hash_table, str));
	assert(2 == HashTableSize(hash_table));
	
	str = "yxz";
	assert(0 == HashTableInsert(hash_table, str));
	assert(3 == HashTableSize(hash_table));

	str = "yzx";
	assert(0 == HashTableInsert(hash_table, str));
	assert(4 == HashTableSize(hash_table));

	str = "zxy";
	assert(0 == HashTableInsert(hash_table, str));
	assert(5 == HashTableSize(hash_table));

	str = "zyx";
	assert(0 == HashTableInsert(hash_table, str));
	assert(6 == HashTableSize(hash_table));
	
	assert(0 == HashTableIsEmpty(hash_table));

	UNUSED(str);

	HashTableDestroy(hash_table);	
}

static void Remove_test()
{
	char *str[] = {"Hello", "Privet", "Shalom", "Szia"};
	hash_table_t *hash_table = HashTableCreate(255, HashFunc, HashCmp);
	
	assert(NULL != hash_table);
	assert(1 == HashTableIsEmpty(hash_table));

	assert(0 == HashTableInsert(hash_table, str[0]));
	assert(1 == HashTableSize(hash_table));

	assert(0 == HashTableInsert(hash_table, str[1]));
	assert(2 == HashTableSize(hash_table));
	
	assert(0 == HashTableInsert(hash_table, str[2]));
	assert(3 == HashTableSize(hash_table));

	assert(0 == HashTableInsert(hash_table, str[3]));
	assert(4 == HashTableSize(hash_table));

	assert(0 == HashTableIsEmpty(hash_table));

	HashTableRemove(hash_table, str[2]);
	assert(3 == HashTableSize(hash_table));

	HashTableRemove(hash_table, str[0]);
	assert(2 == HashTableSize(hash_table));

	HashTableRemove(hash_table, str[1]);
	assert(1 == HashTableSize(hash_table));

	HashTableRemove(hash_table, str[3]);
	assert(0 == HashTableSize(hash_table));

	assert(1 == HashTableIsEmpty(hash_table));

	HashTableDestroy(hash_table);	
}

static void Find_test()
{
	char *str = NULL;
	char *found = NULL;
	hash_table_t *hash_table = HashTableCreate(255, HashFunc, HashCmp);
	
	assert(NULL != hash_table);
	assert(1 == HashTableIsEmpty(hash_table));

	str = "Hello, World!";
	HashTableInsert(hash_table, str);
	
	assert(0 == HashTableIsEmpty(hash_table));

	found = HashTableFind(hash_table, str);
	assert(NULL != found);

	UNUSED(found);

	HashTableDestroy(hash_table);	
}

static void ForEach_test()
{
	char *str[] = {"Hello", "Privet", "Shalom", "Szia"};
	hash_table_t *hash_table = HashTableCreate(255, HashFunc, HashCmp);
	
	assert(NULL != hash_table);
	assert(1 == HashTableIsEmpty(hash_table));

	HashTableInsert(hash_table, str[0]);
	assert(1 == HashTableSize(hash_table));

	HashTableInsert(hash_table, str[1]);
	assert(2 == HashTableSize(hash_table));
	
	HashTableInsert(hash_table, str[2]);
	assert(3 == HashTableSize(hash_table));

	HashTableInsert(hash_table, str[3]);
	assert(4 == HashTableSize(hash_table));

	assert(0 == HashTableIsEmpty(hash_table));

	HashTableForEach(hash_table, PrintForEach, str);

	HashTableDestroy(hash_table);	
}

static void Dictionary_test()
{
	static char buff[100] = "";
	void *str_to_insert = NULL;
	char *to_find = NULL;
	char *found = NULL;
	char *to_remove = NULL;
	hash_table_t *hash_table = HashTableCreate(255, HashFunc, HashCmp);

	FILE *file = fopen("/usr/share/dict/words", "r");
	if (NULL == file)
	{
		return;
	}

	while (1 == fscanf(file, "%100s", buff))
	{
		size_t len = (strlen(buff) + 1) * sizeof(char);
		str_to_insert = (char *)malloc(len);
		memcpy(str_to_insert, buff, len);
		
		HashTableInsert(hash_table, str_to_insert);
	}

	/* Find TEST */
/*	to_find = "AMD"; FIXME*/
/*	found = HashTableFind(hash_table, to_find);*/
/*	assert(0 == strcmp(to_find, found));*/

	to_find = "zoo";
	found = HashTableFind(hash_table, to_find);
	assert(0 == strcmp(to_find, found));

	to_find = "talon";
	found = HashTableFind(hash_table, to_find);
	assert(0 == strcmp(to_find, found));

	/* Remove TEST, if done than -3 frees */
/* 	to_remove = "AMD"; FIXME */
/*	to_remove = HashTableFind(hash_table, to_remove);*/
/*	HashTableRemove(hash_table, to_remove);*/
/*	found = HashTableFind(hash_table, to_remove);*/
/*	assert(NULL == found);*/
/*	free(to_remove);*/

	to_remove = "zoo";
	to_remove = HashTableFind(hash_table, to_remove);
	HashTableRemove(hash_table, to_remove);
	found = HashTableFind(hash_table, to_remove);
	assert(NULL == found);
	free(to_remove);

	to_remove = "talon";
	to_remove = HashTableFind(hash_table, to_remove);
	HashTableRemove(hash_table, to_remove);
	found = HashTableFind(hash_table, to_remove);
	assert(NULL == found);
	free(to_remove);

	to_remove = "Something that not exists";
	HashTableRemove(hash_table, to_remove);

	UNUSED(found);

	fclose(file);
	HashTableForEach(hash_table, FreeForEach, NULL);
	HashTableDestroy(hash_table);
}

/*********************************************************************** Main */
int main()
{
/*	CreateDestroy_test();*/
/*	Insert_test();*/
/*	Remove_test();*/
/*	Find_test();*/
/*	ForEach_test();*/
	Dictionary_test();
	return 0;
}
