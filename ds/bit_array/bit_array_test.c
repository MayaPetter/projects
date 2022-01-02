/******************************************
* Author:   Maya Petter                   *
* Reviewer: Vladimir Kraykin              *
* Purpose:  Bit array                     *
*******************************************/
#include <stdio.h> /* puts, printf*/
#include "bit_array.h"

static void BitArrIsBitSetTest();
static void BitArrCountOnBitsTest();
static void BitArrCountOffBitsTest();
static void BitArrBitToStrTest();
static void BitArrSetBitOnTest();
static void BitArrSetBitOffTest();
static void BitArrSetBitTest();
static void BitArrFlipBitTest();
static void BitArrFlipAllBitsTest();
static void BitArrSetAllBitsOnTest();
static void BitArrSetAllBitsOffTest();
static void BitArrSwapBitsTest();
static void BitArrRotateBitsRightTest();
static void BitArrRotateBitsLeftTest();
static void BitArrBitMirrorTest();

int main()
{
	/*BitArrIsBitSetTest();*/
	BitArrCountOnBitsTest(); 
	BitArrCountOffBitsTest();
	/*BitArrSetBitOnTest(); 
	BitArrSetBitOffTest();*/
	/*BitArrBitToStrTest();*/
	
	/*BitArrSetBitTest();*/
	
	/*BitArrSetAllBitsOnTest();
	BitArrSetAllBitsOffTest();
	BitArrSwapBitsTest();*/
	BitArrBitMirrorTest();
	
	/*BitArrRotateBitsRightTest();
	BitArrRotateBitsLeftTest();*/
	
	/*BitArrFlipBitTest();*/
	
	/*BitArrFlipAllBitsTest();*/
	
	return 0;
}
	

static void BitArrIsBitSetTest()
{
	if (0 != BitArrIsBitSet(0, 5))
	{
		puts("IsBitSet 0 failed");
	}	
	else if (1 != BitArrIsBitSet(0x8000000000000000lu, 63))
	{
		puts("IsBitSet 0x8000000000000000 index 63 failed");
	}
	else if (0 != BitArrIsBitSet(0x8000000000000000lu, 0))
	{
		puts("IsBitSet 0x8000000000000000 index 0 failed");
	}
	else if (1 != BitArrIsBitSet(0x04lu, 2))
	{
		puts("IsBitSet 0x04 index 2 failed");
	}
	else
	{
		puts("IsBitSet succeed");
	}				
}

static void BitArrCountOnBitsTest()
{
	if (0 != BitArrCountOnBits(0))
	{
		puts("CountOnBits 0 failed");
	}	
	else if (64 != BitArrCountOnBits(0xfffffffffffffffflu))
	{
		puts("CountOnBits 0xffffffffffffffff failed");
	}
	else if (8 != BitArrCountOnBits(0x3003000000300003lu))
	{
		puts("CountOnBits 0x3003000000300003 failed");
	}
	else
	{
		puts("CountOnBits succeed");
	}
	
	if (BitArrCountOnBits(0xfffffffffffffffflu) ==	
		BitArrCountOnBitsLUT(0xfffffffffffffffflu))
	{
		puts("CountOnBitsLUT succeed");
	}	
		
}

static void BitArrCountOffBitsTest()
{
	if (64 != BitArrCountOffBits(0))
	{
		puts("CountOffBits 0 failed");
	}	
	else if (0 != BitArrCountOffBits(0xfffffffffffffffflu))
	{
		puts("CountOffBits 0xffffffffffffffff failed");
	}
	else if (56 != BitArrCountOffBits(0x3003000000300003lu))
	{
		puts("CountOffBits 0x3003000000300003 failed");
	}
	else
	{
		puts("CountOffBits succeed");
	}
	
	if (BitArrCountOffBits(0xfffffffffffffffflu) ==	
		BitArrCountOffBitsLUT(0xfffffffffffffffflu))
	{
		puts("CountOffBitsLUT succeed");
	}					
}

static void BitArrBitToStrTest()
{
	char buffer[70];
	
	printf("string for 0x00 is %s\n", BitArrBitToStr(0x00lu, buffer));
	
	printf("string for 0xffffffffffffffff is %s\n", 
								BitArrBitToStr(0xfffffffffffffffflu, buffer));
	
	printf("string for 0x01 is %s\n", BitArrBitToStr(0x01lu, buffer));
	
	printf("string for 0x1000000000000000 is %s\n", 
								BitArrBitToStr(0x1000000000000000lu, buffer));
}

static void BitArrSetBitOnTest()
{
	if (1 != BitArrSetBitOn(0x00lu, 0))
	{
		puts("SetBitOn 0 index 0 failed");
	}	
	else if (0x8000000000000000lu != BitArrSetBitOn(0x00lu, 63))
	{
		puts("SetBitOn 0 index 63 failed");
	}
	else if (0x0100lu != BitArrSetBitOn(0x00lu, 8))
	{
		puts("SetBitOn 0x0100 index 8 failed");
	}
	else
	{
		puts("SetBitOn succeed");
	}				
}

static void BitArrSetBitOffTest() 
{
	if (0 != BitArrSetBitOff(0x01lu, 0))
	{
		puts("SetBitOff 0x01lu index 0 failed");
	}	
	else if (0 != BitArrSetBitOff(0x8000000000000000lu, 63))
	{
		puts("SetBitOff 0x8000000000000000lu index 63 failed");
	}
	else if (0 != BitArrSetBitOff(0x0100lu, 8))
	{
		puts("SetBitOff 0x0100 index 8 failed");
	}
	else
	{
		puts("SetBitOff succeed");
	}				
}

static void BitArrSetBitTest()
{
	if (0 != BitArrSetBit(0x01lu, 0, 0))
	{
		puts("SetBitTest 0x01lu index 0 to off failed");
	}	
	else if (0 != BitArrSetBit(0x8000000000000000lu, 63, 0))
	{
		puts("SetBitTest 0x8000000000000000lu index 63 to off failed");
	}
	else if (0x8000000000000000lu != BitArrSetBit(0x00lu, 63, 1))
	{
		puts("SetBitTest 0x00lu index 63 to on failed");
	}
	else
	{
		puts("SetBitTest succeed");
	}					
}

static void BitArrFlipBitTest()
{
    if (0 != BitArrFlipBit(0x01lu, 0))
	{
		puts("FlipBit 0x01lu index 0 to off failed");
	}	
	else if (0 != BitArrFlipBit(0x8000000000000000lu, 63))
	{
		puts("FlipBit 0x8000000000000000lu index 63 to off failed");
	}
	else if (0x8000000000000000lu != BitArrFlipBit(0x00lu, 63))
	{
		puts("FlipBit 0x00lu index 63 to on failed");
	}
	else
	{
		puts("FlipBit succeed");
	}					
}

static void BitArrFlipAllBitsTest()
{
	if (0xffffffffffffffff != BitArrFlipAllBits(0x00lu))
	{
		puts("FlipAllBits 0x00lu failed");
	}
	else if (0x00lu != BitArrFlipAllBits(0xffffffffffffffff))
	{
		puts("FlipAllBits 0xffffffffffffffff failed");
	}
	else
	{
		puts("FlipAllBits succeed");
	}					

}

static void BitArrSetAllBitsOnTest()
{
	if (0xffffffffffffffff != BitArrSetAllBitsOn(0x00lu))
	{
		puts("SetAllBitsOn 0x00lu failed");
	}
	else if (0xffffffffffffffff != BitArrSetAllBitsOn(0x10lu))
	{
		puts("SetAllBitsOn 0x10lu failed");
	}
	else
	{
		puts("SetAllBitsOn succeed");
	}					
}

static void BitArrSetAllBitsOffTest()
{
	if (0 != BitArrSetAllBitsOff(0xffffffffffffffff))
	{
		puts("SetAllBitsOff 0xffffffffffffffff failed");
	}
	else if (0 != BitArrSetAllBitsOff(0x10lu))
	{
		puts("SetAllBitsOff 0x10lu failed");
	}
	else
	{
		puts("SetAllBitsOff succeed");
	}					
}

static void BitArrSwapBitsTest()
{
	bitarray_t bit1 = BitArrSetBitOn(0x00lu, 3);
	bitarray_t bit2 = BitArrSetBitOn(0x00lu, 1);
	
	if (bit2 != BitArrSwapBits(bit1 , 1, 3))
    {
        puts("SwapBits 1, 3 failed");
    }
    
    bit1 = BitArrSetBitOn(0x00lu, 63);
	bit2 = BitArrSetBitOn(0x00lu, 1);
	
    if (bit2 != BitArrSwapBits(bit1 , 1, 63))
    {
        puts("SwapBits 1, 63 failed");
    }
    
    bit1 = BitArrSetBitOn(0x00lu, 30);
	bit2 = BitArrSetBitOn(0x00lu, 15);
	
    if (bit2 != BitArrSwapBits(bit1 , 15, 30))
    {
        puts("SwapBits 30, 15 failed");
    }
    else
    {
        puts("SwapBits success");
    }
}

static void BitArrRotateBitsRightTest()
{
	if (0x8000000000000000lu != BitArrRotateBitsRight(0x01lu, 1))
    {
        puts("RotateBitsRight 0x01lu failed");
    }
    else if (0x01lu != BitArrRotateBitsRight(0x08lu, 3))
    {
        puts("RotateBitsRight 0x08lu failed");
    }
    else if (0x01lu != BitArrRotateBitsRight(0x01lu, 64))
    {
        puts("RotateBitsRight 0x01lu 64 failed");
    }
    else
    {
        puts("RotateBitsRight success");
    }

}

static void BitArrRotateBitsLeftTest()
{
	if (0x01lu != BitArrRotateBitsLeft(0x8000000000000000lu, 1))
    {
        puts("RotateBitsLeft 0x8000000000000000lu failed");
    }
    else if (0x08lu != BitArrRotateBitsLeft(0x01lu, 3))
    {
        puts("RotateBitsRight 0x08lu failed");
    }
    else
    {
        puts("RotateBitsLeft success");
    }
}

static void BitArrBitMirrorTest()
{
	if (0x8000000000000000lu != BitArrBitMirror(0x01lu))
    {
        puts("BitMirror 0x01lu failed");
    }
    else if (0x01lu != BitArrBitMirror(0x8000000000000000lu))
    {
        puts("BitMirror 0x8000000000000000lu failed");
    }
    else if (0x4000000000000000lu != BitArrBitMirror(0x02lu))
    {
        puts("BitMirror 0x02lu failed");
    }
    else
    {
        puts("BitMirror success");
    }
    
    if (BitArrBitMirror(0x01lu) == BitArrBitMirrorLUT(0x01lu))
    {
    	puts("BitMirrorLUT success");
    }
   
}
