/******************************************
* Author:   Maya Petter                   *
* Reviewer: Vladimir Kraykin              *
* Purpose:  Bit array                     *
*******************************************/
#include <assert.h> /* assert */
#include <stdio.h>  /* input output */
#include <stddef.h> /* size_t __CHAR_BIT__ */
#include "bit_array.h"

#define BITS_IN_BITARRAY_T (sizeof(bitarray_t)*__CHAR_BIT__)
#define BINARY_BASE 2
#define BIT_ARRAY_MASK 0x01lu
#define UNUSED(x) (void)(x) /* to avoid warnings */ 

/* Bit mirror LUT*/
static unsigned char bit_mirror_LUT[256] = 
{ 
	0x0  , 0x80 , 0x40 , 0xc0 , 0x20 , 0xa0 , 0x60 , 0xe0 , 0x10 , 0x90 , 
	0x50 , 0xd0 , 0x30 , 0xb0 , 0x70 , 0xf0 , 0x8  , 0x88 , 0x48 , 0xc8 , 
	0x28 , 0xa8 , 0x68 , 0xe8 , 0x18 , 0x98 , 0x58 , 0xd8 , 0x38 , 0xb8 , 
	0x78 , 0xf8 , 0x4  , 0x84 , 0x44 , 0xc4 , 0x24 , 0xa4 , 0x64 , 0xe4 , 
	0x14 , 0x94 , 0x54 , 0xd4 , 0x34 , 0xb4 , 0x74 , 0xf4 , 0xc  , 0x8c , 
	0x4c , 0xcc , 0x2c , 0xac , 0x6c , 0xec , 0x1c , 0x9c , 0x5c , 0xdc , 
	0x3c , 0xbc , 0x7c , 0xfc , 0x2  , 0x82 , 0x42 , 0xc2 , 0x22 , 0xa2 , 
	0x62 , 0xe2 , 0x12 , 0x92 , 0x52 , 0xd2 , 0x32 , 0xb2 , 0x72 , 0xf2 , 
	0xa  , 0x8a , 0x4a , 0xca , 0x2a , 0xaa , 0x6a , 0xea , 0x1a , 0x9a , 
	0x5a , 0xda , 0x3a , 0xba , 0x7a , 0xfa , 0x6  , 0x86 , 0x46 , 0xc6 , 
	0x26 , 0xa6 , 0x66 , 0xe6 , 0x16 , 0x96 , 0x56 , 0xd6 , 0x36 , 0xb6 , 
	0x76 , 0xf6 , 0xe  , 0x8e , 0x4e , 0xce , 0x2e , 0xae , 0x6e , 0xee , 
	0x1e , 0x9e , 0x5e , 0xde , 0x3e , 0xbe , 0x7e , 0xfe , 0x1  , 0x81 , 
	0x41 , 0xc1 , 0x21 , 0xa1 , 0x61 , 0xe1 , 0x11 , 0x91 , 0x51 , 0xd1 , 
	0x31 , 0xb1 , 0x71 , 0xf1 , 0x9  , 0x89 , 0x49 , 0xc9 , 0x29 , 0xa9 , 
	0x69 , 0xe9 , 0x19 , 0x99 , 0x59 , 0xd9 , 0x39 , 0xb9 , 0x79 , 0xf9 , 
	0x5  , 0x85 , 0x45 , 0xc5 , 0x25 , 0xa5 , 0x65 , 0xe5 , 0x15 , 0x95 , 
	0x55 , 0xd5 , 0x35 , 0xb5 , 0x75 , 0xf5 , 0xd  , 0x8d , 0x4d , 0xcd , 
	0x2d , 0xad , 0x6d , 0xed , 0x1d , 0x9d , 0x5d , 0xdd , 0x3d , 0xbd , 
	0x7d , 0xfd , 0x3  , 0x83 , 0x43 , 0xc3 , 0x23 , 0xa3 , 0x63 , 0xe3 , 
	0x13 , 0x93 , 0x53 , 0xd3 , 0x33 , 0xb3 , 0x73 , 0xf3 , 0xb  , 0x8b , 
	0x4b , 0xcb , 0x2b , 0xab , 0x6b , 0xeb , 0x1b , 0x9b , 0x5b , 0xdb , 
	0x3b , 0xbb , 0x7b , 0xfb , 0x7  , 0x87 , 0x47 , 0xc7 , 0x27 , 0xa7 , 
	0x67 , 0xe7 , 0x17 , 0x97 , 0x57 , 0xd7 , 0x37 , 0xb7 , 0x77 , 0xf7 , 
	0xf  , 0x8f , 0x4f , 0xcf , 0x2f , 0xaf , 0x6f , 0xef , 0x1f , 0x9f , 
	0x5f , 0xdf , 0x3f , 0xbf , 0x7f , 0xff
};  

/* Bit on (1) in a byte LUT*/
static unsigned int bit_count_LUT[256] = 
{
    0, 1, 1, 2, 1, 2, 2, 3, 1, 2, 2, 3, 2, 3, 3, 4, 1, 2, 2, 3, 2, 3, 3, 4, 2, 
    
    3, 3, 4, 3, 4, 4, 5, 1, 2, 2, 3, 2, 3, 3, 4, 2, 3, 3, 4, 3, 4, 4, 5, 2, 3, 
    
    3, 4, 3, 4, 4, 5, 3, 4, 4, 5, 4, 5, 5, 6, 1, 2, 2, 3, 2, 3, 3, 4, 2, 3, 3, 
    
    4, 3, 4, 4, 5, 2, 3, 3, 4, 3, 4, 4, 5, 3, 4, 4, 5, 4, 5, 5, 6, 2, 3, 3, 4, 
    
    3, 4, 4, 5, 3, 4, 4, 5, 4, 5, 5, 6, 3, 4, 4, 5, 4, 5, 5, 6, 4, 5, 5, 6, 5, 
    
    6, 6, 7, 1, 2, 2, 3, 2, 3, 3, 4, 2, 3, 3, 4, 3, 4, 4, 5, 2, 3, 3, 4, 3, 4, 
    
    4, 5, 3, 4, 4, 5, 4, 5, 5, 6, 2, 3, 3, 4, 3, 4, 4, 5, 3, 4, 4, 5, 4, 5, 5, 
    
    6, 3, 4, 4, 5, 4, 5, 5, 6, 4, 5, 5, 6, 5, 6, 6, 7, 2, 3, 3, 4, 3, 4, 4, 5, 
    
    3, 4, 4, 5, 4, 5, 5, 6, 3, 4, 4, 5, 4, 5, 5, 6, 4, 5, 5, 6, 5, 6, 6, 7, 3, 
    
    4, 4, 5, 4, 5, 5, 6, 4, 5, 5, 6, 5, 6, 6, 7, 4, 5, 5, 6, 5, 6, 6, 7, 5, 6, 
    
    6, 7, 6, 7, 7, 8  
};


int BitArrIsBitSet(bitarray_t bit_array, size_t bit_index)
{
	assert(BITS_IN_BITARRAY_T < bit_index);
	
    /* if the bit is on will return 1. if it's off will return 0 */
    return (BIT_ARRAY_MASK & (bit_array >> bit_index));
}

size_t BitArrSetBitOn(bitarray_t bit_array, size_t bit_index)
{
	assert(BITS_IN_BITARRAY_T < bit_index);
	
    return (bit_array | (BIT_ARRAY_MASK << bit_index));
}

size_t BitArrSetBitOff(bitarray_t bit_array, size_t bit_index)
{
	assert(BITS_IN_BITARRAY_T < bit_index);
	
    bitarray_t mask = ~BIT_ARRAY_MASK;
    
    return (bit_array & (mask << bit_index));
}

size_t BitArrSetBit(bitarray_t bit_array, size_t bit_index, int value)
{
	assert(BITS_IN_BITARRAY_T < bit_index);
	
    /* if value is 1 and the bit turned off- turn it on. Do opisit if value is 0 */
    if (1 == value && (1 != BitArrIsBitSet(bit_array, bit_index)))
    {
        bit_array = BitArrSetBitOn(bit_array, bit_index);
    }    
    else if (0 == value && 1 == BitArrIsBitSet(bit_array, bit_index))
    {
        bit_array = BitArrSetBitOff(bit_array, bit_index);
    }
    
    return bit_array;
}

size_t BitArrCountOnBits(bitarray_t bit_array)
{
	size_t counter = 0;
	bitarray_t mask = 0x01;
	size_t i = 0;
	
	/* Increase counter every time the bit is on*/	
	while (i < BITS_IN_BITARRAY_T)
	{
		if (0 != (mask & bit_array))
		{
			++counter;			
		}
		mask = mask << 1;
		++i;
	}	
	return counter;
}

size_t BitArrCountOffBits(bitarray_t bit_array)
{
    return (BITS_IN_BITARRAY_T - (BitArrCountOnBits(bit_array)));
}

bitarray_t BitArrSwapBits(bitarray_t bit_array, size_t bit_index1, size_t bit_index2)
{
    bitarray_t mask = 0x01lu;

    /* Extracting bit first position and second position*/
    bitarray_t first_bit =  (bit_array >> bit_index1) & mask; 
    bitarray_t second_bit =  (bit_array >> bit_index2) & mask;
    
    /* Turning off bits in first and second position in the original number */
    mask = 0x00lu;
    mask = BitArrSetBitOn(mask, bit_index1);
    mask = BitArrSetBitOn(mask, bit_index2);
    mask = ~mask;
    bit_array =  bit_array & mask; 
	
	/*shifting bits to the new position*/     
    first_bit = first_bit << bit_index2;
    second_bit = second_bit << bit_index1;

	/*Put bits in the original number*/
    bit_array = bit_array | first_bit; 
    bit_array = bit_array | second_bit;
    
    return bit_array;
}

bitarray_t BitArrBitMirror(bitarray_t bit_array)
{
	bitarray_t i = 0;
    bitarray_t mirror = 0x00;
    bitarray_t mask = 0x01;
    
    
    /* 
     * Isolate the right bit, put it in the mirror and shift to the left.
     * shift the bits in the original number to the right. 
     * repeat till reaching byte size 
     */ 
    for (i = 0; i < BITS_IN_BITARRAY_T; ++i)
    {
		mirror = mirror << 1;
		mirror += mask & bit_array;
		bit_array = bit_array >> 1;
    }
    
    return mirror;
}

char *BitArrBitToStr(bitarray_t bit_array, char *str) 
{
	char *runner = str + (BITS_IN_BITARRAY_T - 1);
	bitarray_t reminder = 0;
	
	assert(NULL != str);
	assert(NULL != runner);
	
	/*Adding null charecter to the end of the string*/
	*runner = '\0';
	--runner;
	
	/* Converting digit to char */
	while (runner != str)
	{	
	    reminder = bit_array % BINARY_BASE;
		*runner = reminder + '0'; 
		bit_array = bit_array / BINARY_BASE;
		--runner;
	}
	return str;
}

bitarray_t BitArrFlipAllBits(bitarray_t bit_array)
{
    return (~bit_array);
}

bitarray_t BitArrSetAllBitsOn(bitarray_t bit_array)
{
	UNUSED(bit_array);
    return 0xffffffffffffffff;
}

bitarray_t BitArrSetAllBitsOff(bitarray_t bit_array)
{
	UNUSED(bit_array);
    return 0;
} 

bitarray_t BitArrFlipBit(bitarray_t bit_array, size_t bit_index)
{
	assert(BITS_IN_BITARRAY_T < bit_index);
	
	/* if the bit on turns it off and vice versa*/ 
	if (0 == BitArrIsBitSet(bit_array, bit_index))
	{
		bit_array = BitArrSetBitOn(bit_array, bit_index);
	}
	else if (1 == BitArrIsBitSet(bit_array, bit_index))
	{
		bit_array = BitArrSetBitOff(bit_array, bit_index);
	}
	
	return bit_array;	

}

bitarray_t BitArrRotateBitsRight(bitarray_t bit_array, size_t rotate_by)
{
	bitarray_t mask = 0xfffffffffffffffful;
	/* in case index > 64 */
	rotate_by = rotate_by % (BITS_IN_BITARRAY_T); 
	/* 
     * Isolate the right bits according to rotate_by, put it in mask 
     * and shift to the left.
     */ 	
	mask = mask << rotate_by;
	mask = ~mask;
	mask = bit_array & mask;
	mask = mask << (BITS_IN_BITARRAY_T - rotate_by);
	
	/* shift the bits in the original number to the right according to rotate_by*/
	bit_array = bit_array >> rotate_by;
	bit_array = bit_array | mask;
	
	return  bit_array;

}

bitarray_t BitArrRotateBitsLeft(bitarray_t bit_array, size_t rotate_by)
{
	bitarray_t mask = 0xfffffffffffffffful;
	/* in case index > 64 */
	rotate_by = rotate_by % ( BITS_IN_BITARRAY_T);
	
	/* 
     * Isolate the left bits according to rotate_by, put it in mask 
     * and shift to the right.
     */ 	
	mask = mask >> rotate_by;
	mask = ~mask;
	mask = bit_array & mask;
	mask = mask >> (BITS_IN_BITARRAY_T - rotate_by);
	
	/* shift the bits in the original number to the left according to rotate_by*/
	bit_array = bit_array << rotate_by;
	bit_array = bit_array | mask;
	
	return  bit_array;
}

size_t BitArrCountOnBitsLUT(bitarray_t bit_array)
{

	bitarray_t mask = 0xfful;
	bitarray_t count_bit = 0x00ul;
	size_t i = 0;
	
	/* 
	* Masking the other 7 bytes in bit_array to find the turned on bits in one 
	* byte. shifting the mask to the next byte and bit_array to the first byte.
	* repeat untill al bit in al bytes are counted.
	*/  
	
	for(i = 0; i < sizeof(bitarray_t); ++i)
	{
	    count_bit += bit_count_LUT[(bit_array & mask) >> (__CHAR_BIT__ * i)];
	    mask <<= __CHAR_BIT__;
	}
	return count_bit;	
}

size_t BitArrCountOffBitsLUT(bitarray_t bit_array)
{
	return (BITS_IN_BITARRAY_T - BitArrCountOnBitsLUT(bit_array));
}

bitarray_t BitArrBitMirrorLUT(bitarray_t bit_array)
{
	bitarray_t mask = 0xfful;
	bitarray_t mirror = 0x00ul;
	size_t i = 0;
	
	/* 
	* Masking the other 7 bytes in bit_array, mirror each byte and assgning  
	* to mirror. repeat untill al bit in al bytes are mirrored.
	*/  
	
	for(i = 0; i < sizeof(bitarray_t); ++i)
	{
	    mirror = bit_mirror_LUT[(bit_array & mask) >> (__CHAR_BIT__ * i)];
	    mask <<= __CHAR_BIT__;
	    mirror <<= __CHAR_BIT__;
	}
	return mirror;
}


