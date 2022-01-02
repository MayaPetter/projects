#include "search_test1.h" /*colors*/

/******************************************************************************/
/**************************** forward declaration *****************************/
/******************************************************************************/
void BSearchRecursive_int_test();
void BSearchIterative_int_test();
void JSearch_int_test();

void BSearchRecursive_char_test();
void BSearchIterative_char_test();
void JSearch_char_test();

/******************************************************************************/
/*********************************** main *************************************/
/******************************************************************************/
int main()
{	
	BSearchRecursive_int_test();
	BSearchIterative_int_test();
	JSearch_int_test();
	
	BSearchRecursive_char_test();
	BSearchIterative_char_test();
	JSearch_char_test();
	
	return 0;
}

/******************************************************************************/
/**************************** test functions **********************************/
/******************************************************************************/
void BSearchRecursive_char_test()
{
	char data_not_found_befor = 'A', data_not_found_after = 'z';
	size_t i = 0;

	puts(BYEL "BSearch Recursive char test"RESET);

	for (i = 0; i < SIZE_1; ++i) 
	{
		assert(char_arr_1_elem[i] == 
		*(char *)BSearchRecursive(&char_arr_1_elem[i], char_arr_1_elem, 
								 SIZE_1, sizeof(char), CmpCharFunc));	
	}
	
	assert(NULL == BSearchRecursive(&data_not_found_befor, char_arr_1_elem, SIZE_1, 
									sizeof(char), CmpCharFunc));
	assert(NULL == BSearchRecursive(&data_not_found_after, char_arr_1_elem, SIZE_1, 
									sizeof(char), CmpCharFunc));
		
	for (i = 0; i < SIZE_4; ++i) 
	{
		assert(char_arr_4_elem[i] == 
		*(char *)BSearchRecursive(&char_arr_4_elem[i], char_arr_4_elem, 
								 SIZE_4, sizeof(char), CmpCharFunc));	
	}
	
	assert(NULL == BSearchRecursive(&data_not_found_befor, char_arr_4_elem, SIZE_4, 
									sizeof(char), CmpCharFunc));
	assert(NULL == BSearchRecursive(&data_not_found_after, char_arr_4_elem, SIZE_4, 
									sizeof(char), CmpCharFunc));

	for (i = 0; i < SIZE_5; ++i) 
	{
		assert(char_arr_5_elem[i] == 
		*(char *)BSearchRecursive(&char_arr_5_elem[i], char_arr_5_elem, 
								 SIZE_5, sizeof(char), CmpCharFunc));	
	}
	
	assert(NULL == BSearchRecursive(&data_not_found_befor, char_arr_5_elem, SIZE_5, 
									sizeof(char), CmpCharFunc));
	assert(NULL == BSearchRecursive(&data_not_found_after, char_arr_5_elem, SIZE_5, 
									sizeof(char), CmpCharFunc));
}

void BSearchIterative_char_test()
{
	char data_not_found_befor = 'A', data_not_found_after = 'z';
	size_t i = 0;

	puts(BYEL "BSearch Iterative char test"RESET);

	for (i = 0; i < SIZE_1; ++i) 
	{
		assert(char_arr_1_elem[i] == 
		*(char *)BSearchIterative(&char_arr_1_elem[i], char_arr_1_elem, 
								 SIZE_1, sizeof(char), CmpCharFunc));	
	}
	
	assert(NULL == BSearchIterative(&data_not_found_befor, char_arr_1_elem, SIZE_1, 
									sizeof(char), CmpCharFunc));
	assert(NULL == BSearchIterative(&data_not_found_after, char_arr_1_elem, SIZE_1, 
									sizeof(char), CmpCharFunc));
		
	for (i = 0; i < SIZE_4; ++i) 
	{
		assert(char_arr_4_elem[i] == 
		*(char *)BSearchIterative(&char_arr_4_elem[i], char_arr_4_elem, 
								 SIZE_4, sizeof(char), CmpCharFunc));	
	}
	
	assert(NULL == BSearchIterative(&data_not_found_befor, char_arr_4_elem, SIZE_4, 
									sizeof(char), CmpCharFunc));
	assert(NULL == BSearchIterative(&data_not_found_after, char_arr_4_elem, SIZE_4, 
									sizeof(char), CmpCharFunc));

	for (i = 0; i < SIZE_5; ++i) 
	{
		assert(char_arr_5_elem[i] == 
		*(char *)BSearchIterative(&char_arr_5_elem[i], char_arr_5_elem, 
								 SIZE_5, sizeof(char), CmpCharFunc));	
	}
	
	assert(NULL == BSearchIterative(&data_not_found_befor, char_arr_5_elem, SIZE_5, 
									sizeof(char), CmpCharFunc));
	assert(NULL == BSearchIterative(&data_not_found_after, char_arr_5_elem, SIZE_5, 
									sizeof(char), CmpCharFunc));
}

void JSearch_char_test()
{
	char data_not_found_befor = 'A', data_not_found_after = 'z';
	size_t i = 0;

	puts(BYEL "JSearch char test"RESET);

	for (i = 0; i < SIZE_1; ++i) 
	{
		assert(char_arr_1_elem[i] == 
		*(char *)JSearch(&char_arr_1_elem[i], char_arr_1_elem, 
								 SIZE_1, sizeof(char), CmpCharFunc));	
	}
	
	assert(NULL == JSearch(&data_not_found_befor, char_arr_1_elem, SIZE_1, 
									sizeof(char), CmpCharFunc));
	assert(NULL == JSearch(&data_not_found_after, char_arr_1_elem, SIZE_1, 
									sizeof(char), CmpCharFunc));
		
	for (i = 0; i < SIZE_4; ++i) 
	{
		assert(char_arr_4_elem[i] == 
		*(char *)JSearch(&char_arr_4_elem[i], char_arr_4_elem, 
								 SIZE_4, sizeof(char), CmpCharFunc));	
	}
	
	assert(NULL == JSearch(&data_not_found_befor, char_arr_4_elem, SIZE_4, 
									sizeof(char), CmpCharFunc));
	assert(NULL == JSearch(&data_not_found_after, char_arr_4_elem, SIZE_4, 
									sizeof(char), CmpCharFunc));

	for (i = 0; i < SIZE_5; ++i) 
	{
		assert(char_arr_5_elem[i] == 
		*(char *)JSearch(&char_arr_5_elem[i], char_arr_5_elem, 
								 SIZE_5, sizeof(char), CmpCharFunc));	
	}
	
	assert(NULL == JSearch(&data_not_found_befor, char_arr_5_elem, SIZE_5, 
									sizeof(char), CmpCharFunc));
	assert(NULL == JSearch(&data_not_found_after, char_arr_5_elem, SIZE_5, 
									sizeof(char), CmpCharFunc));
}

void BSearchRecursive_int_test()
{
	int data_not_found_befor = -100, data_not_found_after = 1000;
	size_t i = 0;

	puts(BYEL "BSearch Recursive int test"RESET);

	for (i = 0; i < SIZE_1; ++i) 
	{
		assert(arr_1_elem[i] == *(int *)BSearchRecursive(&arr_1_elem[i], 
								arr_1_elem, SIZE_1, sizeof(int), CmpIntFunc));	
	}
	
	assert(NULL == BSearchRecursive(&data_not_found_befor, arr_1_elem, SIZE_1, 
									sizeof(int), CmpIntFunc));
	assert(NULL == BSearchRecursive(&data_not_found_after, arr_1_elem, SIZE_1, 
									sizeof(int), CmpIntFunc));

	for (i = 0; i < SIZE_4; ++i) 
	{
		assert(arr_4_elem[i] == *(int *)BSearchRecursive(&arr_4_elem[i], 
								arr_4_elem, SIZE_4, sizeof(int), CmpIntFunc));	
	}

	assert(NULL == BSearchRecursive(&data_not_found_befor, arr_4_elem, SIZE_4, 
									sizeof(int), CmpIntFunc));
	assert(NULL == BSearchRecursive(&data_not_found_after, arr_4_elem, SIZE_4, 
									sizeof(int), CmpIntFunc));

	for (i = 0; i < SIZE_5; ++i) 
	{
		assert(arr_5_elem[i] == *(int *)BSearchRecursive(&arr_5_elem[i], 
								arr_5_elem, SIZE_5, sizeof(int), CmpIntFunc));	
	}
	
	assert(NULL == BSearchRecursive(&data_not_found_befor, arr_5_elem, SIZE_5, 
									sizeof(int), CmpIntFunc));
	assert(NULL == BSearchRecursive(&data_not_found_after, arr_5_elem, SIZE_5, 
									sizeof(int), CmpIntFunc));
									
	for (i = 0; i < SIZE_100; ++i) 
	{
		assert(arr_100_elem[i] == *(int *)BSearchRecursive(&arr_100_elem[i], 
								arr_100_elem, SIZE_100, sizeof(int), CmpIntFunc));
	}
		
	assert(NULL == BSearchRecursive(&data_not_found_befor, arr_100_elem, 
									SIZE_100, sizeof(int), CmpIntFunc));
	assert(NULL == BSearchRecursive(&data_not_found_after, arr_100_elem, 
									SIZE_100, sizeof(int), CmpIntFunc));
									
	for (i = 0; i < SIZE_101; ++i) 
	{
		assert(arr_101_elem[i] == *(int *)BSearchRecursive(&arr_101_elem[i], 
								arr_101_elem, SIZE_101, sizeof(int), CmpIntFunc));
	}
	
	assert(NULL == BSearchRecursive(&data_not_found_befor, arr_101_elem, 
									SIZE_101, sizeof(int), CmpIntFunc));
	assert(NULL == BSearchRecursive(&data_not_found_after, arr_101_elem, 
									SIZE_101, sizeof(int), CmpIntFunc));
}

void BSearchIterative_int_test()
{
	int data_not_found_befor = -100, data_not_found_after = 1000;
	size_t i = 0;

	puts(BYEL "BSearch Iterative int test"RESET);

	for (i = 0; i < SIZE_1; ++i) 
	{
		assert(arr_1_elem[i] == *(int *)BSearchIterative(&arr_1_elem[i], 
								arr_1_elem, SIZE_1, sizeof(int), CmpIntFunc));	
	}
	
	assert(NULL == BSearchIterative(&data_not_found_befor, arr_1_elem, SIZE_1, 
									sizeof(int), CmpIntFunc));
	assert(NULL == BSearchIterative(&data_not_found_after, arr_1_elem, SIZE_1, 
									sizeof(int), CmpIntFunc));

	for (i = 0; i < SIZE_4; ++i) 
	{
		assert(arr_4_elem[i] == *(int *)BSearchIterative(&arr_4_elem[i], 
								arr_4_elem, SIZE_4, sizeof(int), CmpIntFunc));	
	}

	assert(NULL == BSearchIterative(&data_not_found_befor, arr_4_elem, SIZE_4, 
									sizeof(int), CmpIntFunc));
	assert(NULL == BSearchIterative(&data_not_found_after, arr_4_elem, SIZE_4, 
									sizeof(int), CmpIntFunc));

	for (i = 0; i < SIZE_5; ++i) 
	{
		assert(arr_5_elem[i] == *(int *)BSearchIterative(&arr_5_elem[i], 
								arr_5_elem, SIZE_5, sizeof(int), CmpIntFunc));	
	}
	
	assert(NULL == BSearchIterative(&data_not_found_befor, arr_5_elem, SIZE_5, 
									sizeof(int), CmpIntFunc));
	assert(NULL == BSearchIterative(&data_not_found_after, arr_5_elem, SIZE_5, 
									sizeof(int), CmpIntFunc));
									
	for (i = 0; i < SIZE_100; ++i) 
	{
		assert(arr_100_elem[i] == *(int *)BSearchIterative(&arr_100_elem[i], 
								arr_100_elem, SIZE_100, sizeof(int), CmpIntFunc));
	}
		
	assert(NULL == BSearchIterative(&data_not_found_befor, arr_100_elem, 
									SIZE_100, sizeof(int), CmpIntFunc));
	assert(NULL == BSearchIterative(&data_not_found_after, arr_100_elem, 
									SIZE_100, sizeof(int), CmpIntFunc));
									
	for (i = 0; i < SIZE_101; ++i) 
	{
		assert(arr_101_elem[i] == *(int *)BSearchIterative(&arr_101_elem[i], 
								arr_101_elem, SIZE_101, sizeof(int), CmpIntFunc));
	}
	
	assert(NULL == BSearchIterative(&data_not_found_befor, arr_101_elem, 
									SIZE_101, sizeof(int), CmpIntFunc));
	assert(NULL == BSearchIterative(&data_not_found_after, arr_101_elem, 
									SIZE_101, sizeof(int), CmpIntFunc));
}

void JSearch_int_test()
{
	int data_not_found_befor = -100, data_not_found_after = 1000;
	size_t i = 0;

	puts(BYEL "JSearch int test"RESET);

	for (i = 0; i < SIZE_1; ++i) 
	{
		assert(arr_1_elem[i] == *(int *)JSearch(&arr_1_elem[i], 
								arr_1_elem, SIZE_1, sizeof(int), CmpIntFunc));	
	}
	
	assert(NULL == JSearch(&data_not_found_befor, arr_1_elem, SIZE_1, 
									sizeof(int), CmpIntFunc));
	assert(NULL == JSearch(&data_not_found_after, arr_1_elem, SIZE_1, 
									sizeof(int), CmpIntFunc));

	for (i = 0; i < SIZE_4; ++i) 
	{
		assert(arr_4_elem[i] == *(int *)JSearch(&arr_4_elem[i], 
								arr_4_elem, SIZE_4, sizeof(int), CmpIntFunc));	
	}

	assert(NULL == JSearch(&data_not_found_befor, arr_4_elem, SIZE_4, 
									sizeof(int), CmpIntFunc));
	assert(NULL == JSearch(&data_not_found_after, arr_4_elem, SIZE_4, 
									sizeof(int), CmpIntFunc));

	for (i = 0; i < SIZE_5; ++i) 
	{
		assert(arr_5_elem[i] == *(int *)JSearch(&arr_5_elem[i], 
								arr_5_elem, SIZE_5, sizeof(int), CmpIntFunc));	
	}
	
	assert(NULL == JSearch(&data_not_found_befor, arr_5_elem, SIZE_5, 
									sizeof(int), CmpIntFunc));
	assert(NULL == JSearch(&data_not_found_after, arr_5_elem, SIZE_5, 
									sizeof(int), CmpIntFunc));
									
	for (i = 0; i < SIZE_100; ++i) 
	{
		assert(arr_100_elem[i] == *(int *)JSearch(&arr_100_elem[i], 
								arr_100_elem, SIZE_100, sizeof(int), CmpIntFunc));
	}
		
	assert(NULL == JSearch(&data_not_found_befor, arr_100_elem, 
									SIZE_100, sizeof(int), CmpIntFunc));
	assert(NULL == JSearch(&data_not_found_after, arr_100_elem, 
									SIZE_100, sizeof(int), CmpIntFunc));
									
	for (i = 0; i < SIZE_101; ++i) 
	{
		assert(arr_101_elem[i] == *(int *)JSearch(&arr_101_elem[i], 
								arr_101_elem, SIZE_101, sizeof(int), CmpIntFunc));
	}
	
	assert(NULL == JSearch(&data_not_found_befor, arr_101_elem, 
									SIZE_101, sizeof(int), CmpIntFunc));
	assert(NULL == JSearch(&data_not_found_after, arr_101_elem, 
									SIZE_101, sizeof(int), CmpIntFunc));
}
