#include <assert.h> /*assert*/
#include <stdio.h>  /*printf*/
#include <string.h> /*strcmp*/

#include "search.h"

#define SIZE_100 100
#define SIZE_101 101
#define SIZE_4 4
#define SIZE_5 5
#define SIZE_1 1

static int arr_1_elem[SIZE_1] = {1};

static int arr_4_elem[SIZE_4] = {1, 2, 3, 4};

static int arr_5_elem[SIZE_5] = {1, 2, 3, 4, 5};

static int arr_100_elem[SIZE_100] = 
{-50, -49, -48, -47, -46, -45, -44, -43, -42, -41, -40, -39, -38, -37, -36, -35,
 -34, -33, -32, -31, -30, -29, -28, -27, -26, -25, -24, -23, -22, -21, -20, -19, 
 -18, -17, -16, -15, -14, -13, -12, -11, -10, -9, -8, -7, -6, -5, -4, -3, -2,
 -1, 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 
 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31, 32, 33, 34, 35, 36, 37, 38, 39, 40,
 41, 42, 43, 44, 45, 46, 47, 48, 49};
 
static int arr_101_elem[SIZE_101] = 
{-50, -49, -48, -47, -46, -45, -44, -43, -42, -41, -40, -39, -38, -37, -36, -35,
 -34, -33, -32, -31, -30, -29, -28, -27, -26, -25, -24, -23, -22, -21, -20, -19, 
 -18, -17, -16, -15, -14, -13, -12, -11, -10, -9, -8, -7, -6, -5, -4, -3, -2,
 -1, 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 
 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31, 32, 33, 34, 35, 36, 37, 38, 39, 40,
 41, 42, 43, 44, 45, 46, 47, 48, 49, 50};

static char char_arr_1_elem[SIZE_1] = {'a'};

static char char_arr_4_elem[SIZE_4] = {'a', 'b', 'c', 'd'};

static char char_arr_5_elem[SIZE_5] = {'a', 'b', 'c', 'd', 'e'};

/******************************************************************************/
/**************************** service functions *******************************/
/******************************************************************************/

static int CmpIntFunc(const void *middle_data, const void *key_of_data)
{
	return *(int *)middle_data - *(int *)key_of_data;
}

static int CmpCharFunc(const void *middle_data, const void *key_of_data)
{
	return strcmp((char *)middle_data, (char *)key_of_data);
}

#define RED   "\x1B[0;31m"
#define BRED   "\x1B[1;31m"
#define GRN   "\x1B[0;32m"
#define BGRN   "\x1B[1;32m"
#define YEL   "\x1B[0;33m"
#define BYEL   "\x1B[1;33m"
#define BLU   "\x1B[0;34m"
#define BBLU   "\x1B[1;34m"
#define MAG   "\x1B[0;35m"
#define BMAG   "\x1B[1;35m"
#define CYN   "\x1B[0;36m"
#define BCYN   "\x1B[1;36m"
#define WHT   "\x1B[0;37m"
#define BWHT   "\x1B[1;37m"
#define RESET "\x1B[0m"

/*printf(RED "a" RESET);*/
/*printf(GRN "a" RESET);*/
/*printf(YEL "a" RESET);*/
/*printf(BLU "a" RESET);*/
/*printf(MAG "a" RESET);*/
/*printf(CYN "a\n" RESET);*/



