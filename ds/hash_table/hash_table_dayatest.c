#include <assert.h> /*assert*/
#include <stdio.h>  /*printf*/
#include <string.h> /*strcmp*/
#include <stdlib.h> /*malloc, free*/
#include <time.h>   /*time*/

#include "hash_table.h"

#define UNUSED(x) (void)(x)

/******************************************************************************/
/**************************** forward declaration *****************************/
/******************************************************************************/
/*tests*/
static void Create_Test();
static void Basic_Test();
static void CreateDictionary_Test();

/*service*/
static int HashCmp(const void *key1, const void *key2);
static size_t HashFunc(void *key);
static int ActPrint(void *table_data, void *param);
static int ActCount(void *table_data, void *param);

/******************************************************************************/
/*********************************** main *************************************/
/******************************************************************************/
int main()
{	
/*	Create_Test();*/
/*	Basic_Test();*/
	CreateDictionary_Test();
	
	return 0;
}

/******************************************************************************/
/**************************** test functions **********************************/
/******************************************************************************/
char buffer[10000000] = "";

static void CreateDictionary_Test()
{
	
	size_t num_buckets = 5 * 'z';
	hash_table_t *hash_table = HashTableCreate(num_buckets, HashFunc, HashCmp);
	int status = 0;
	FILE *dictionary_file = fopen("/usr/share/dict/words", "r");
	size_t i = 1, j = 0;
	size_t buffer_len = 0;
	size_t count = 0;
	clock_t start_time = 0;
	clock_t end_time = 0;
	char *find1 = "";
	char *find2 = "";

	puts("CreateDictionary_Test");
	/*create dictionary*/
	start_time = clock();	
	for (  i = 1, status = 0
		 ; (NULL != fgets(&buffer[j], 40, dictionary_file)) &&
		   (0 == status) && (99171 >= i)
		 ; ++i, j += buffer_len + 1)
	{
		status = HashTableInsert(hash_table, &buffer[j]);
		buffer_len = strlen(&buffer[j]);
	} 
	end_time = clock();
	printf("time create dictionary : %f\n\n", 
	((double)end_time - (double)start_time) / CLOCKS_PER_SEC);

/*	PrintSizeOfBuckets(hash_table, num_buckets);*/
/*	puts("\n");*/

/*	start_time = clock();	*/
/*	assert(0 == HashTableForEach(hash_table, ActPrint, NULL));*/
/*	end_time = clock();*/
/*	printf("time print all : %lf\n", ((double)end_time - (double)start_time) / CLOCKS_PER_SEC);*/

	/* time wc words */
	start_time = clock();	
	assert(0 == HashTableForEach(hash_table, ActCount, &count));
	printf("%lu\n", count);
	end_time = clock();
	printf("time count : %f\n\n", ((double)end_time - (double)start_time) / CLOCKS_PER_SEC);

	
	/* time grep -w A words */
	start_time = clock();
	find1 = HashTableFind(hash_table, "A\n");
	find2 = HashTableFind(hash_table, "A's\n");
	printf("%s", find1);
	printf("%s", find2);
	end_time = clock();
	printf("time find : %f\n", ((double)end_time - (double)start_time) / CLOCKS_PER_SEC);
	
	assert(0 == HashTableIsEmpty(hash_table));
	assert(99171 == HashTableSize(hash_table));

	count = 0;
	assert(0 == HashTableForEach(hash_table, ActCount, &count));
	assert(99171 == count);

	#ifdef NOT
	fclose(dictionary_file);
	dictionary_file = fopen("/usr/share/dict/words", "r");

	for (  i = 1, status = 0
		 ; (NULL != fgets(&buffer[j], 40, dictionary_file)) &&
		   (0 == status) && (99171 >= i)
		 ; ++i, j += buffer_len + 1)
	{
		assert(buffer[j] == *(char *)HashTableFind(hash_table, &buffer[j]));
		buffer_len = strlen(&buffer[j]);
	} 
	#endif

	fclose(dictionary_file);
	dictionary_file = fopen("/usr/share/dict/words", "r");

	for (  i = 1, status = 0
		 ; (NULL != fgets(&buffer[j], 40, dictionary_file)) &&
		   (0 == status) && (99171 >= i)
		 ; ++i, j += buffer_len + 1)
	{
		HashTableRemove(hash_table, &buffer[j]);
		buffer_len = strlen(&buffer[j]);
	} 

	assert(1 == HashTableIsEmpty(hash_table));
	assert(0 == HashTableSize(hash_table));
	
	fclose(dictionary_file);
	HashTableDestroy(hash_table);		
}

static void Basic_Test()
{
	size_t num_buckets =  11 * 'z';
	int status = 0;
	char *str_inser1 = "A\n";
	char *str_inser2 = "zzz\n";
	char *str_not_found = "aa\n";
	
	hash_table_t *hash_table = HashTableCreate(num_buckets, HashFunc, HashCmp);
	assert(NULL != hash_table);

	assert(1 == HashTableIsEmpty(hash_table));
	assert(0 == HashTableSize(hash_table));

	status = HashTableInsert(hash_table, str_inser1);
	status = HashTableInsert(hash_table, str_inser2);
	assert(0 == status);

	assert(0 == HashTableIsEmpty(hash_table));
	assert(2 == HashTableSize(hash_table));
	
	assert(str_inser1 == HashTableFind(hash_table, str_inser1));
	assert(NULL == HashTableFind(hash_table, str_not_found));

	assert(0 == HashTableForEach(hash_table, ActPrint, NULL));
	puts("");

	HashTableRemove(hash_table, str_not_found);
	assert(0 == HashTableIsEmpty(hash_table));
	assert(2 == HashTableSize(hash_table));

	HashTableRemove(hash_table, str_inser1);
	assert(0 == HashTableIsEmpty(hash_table));
	assert(1 == HashTableSize(hash_table));

	HashTableRemove(hash_table, str_inser2);
	assert(1 == HashTableIsEmpty(hash_table));
	assert(0 == HashTableSize(hash_table));

	assert(0 == HashTableForEach(hash_table, ActPrint, NULL));
	puts("");
	
	HashTableDestroy(hash_table);		
}

static void Create_Test()
{
	size_t num_buckets = 10;
	hash_table_t *hash_table = HashTableCreate(num_buckets, HashFunc, HashCmp);
	assert(NULL != hash_table);
	
	assert(1 == HashTableIsEmpty(hash_table));
	assert(0 == HashTableSize(hash_table));
	
	HashTableDestroy(hash_table);
}


/******************************************************************************/
/**************************** service functions *******************************/
/******************************************************************************/
/*hash_cmp*/
static int HashCmp(const void *key1, const void *key2)
{
	
	int cmp_result = strcmp(key1, key2);
	
	return (0 == cmp_result) ? 0 : 1;
}

static size_t HashFunc(void *key) 
{
	size_t num_buckets = 5 * 'z'; 
	char *str = key;
	size_t ascii_val_of_string = 0;
	size_t hash_table_index = 0;

	for (; ('\0' != *str) || ('\0' != *str) ; ++str)
	{
		ascii_val_of_string += (unsigned int)*str;
		++str;
	}

	hash_table_index = ((ascii_val_of_string - 'A') % num_buckets);
	
	return hash_table_index;
}

/*act for foreach*/
static int ActPrint(void *table_data, void *param) 
{
	UNUSED(param);
	puts(table_data);
	
	return 0;
}

/*act for foreach*/
static int ActCount(void *table_data, void *param) 
{
	UNUSED(table_data);
	++*(size_t *)param;
	
	return 0;
}
