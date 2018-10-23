//////////////////////////////////////////////////////////////////////////////
//
// 	Filename:	Common.c
//	Version:		
//
//	Author:		Liu, Zemin  (jyetek@gmail.com)
//	Company:	JYE Tech
//	Web: 		www.jyetech.com
//
//-----------------------------------------------------------------------------
//
// 	Target: 		ATmega64
// 	Tool chain: 	WinAvr (GCC-AVR)
//
//
//-----------------------------------------------------------------------------
//	Required files:
//
//-----------------------------------------------------------------------------
//	Notes:
//
//
//-----------------------------------------------------------------------------
//	Revision History:
//
///////////////////////////////////////////////////////////////////////////////
//
//-----------------------------------------------------------------------------
// 	Includes
//-----------------------------------------------------------------------------

#include	"Common.h"

// ===========================================================
//	Function Definitions
// ===========================================================
//
//-----------------------------------------------------------------------------
// Return the smaller
//-----------------------------------------------------------------------------
//
U8	Min(U8 a, U8 b)
{
 if(a < b) {
 	return a;
 	}
 else {
 	return b;
 	}
}

//-----------------------------------------------------------------------------
// Return the greater
//-----------------------------------------------------------------------------
//
U8	Max(U8 a, U8 b)
{
 if(a > b) {
 	return a;
 	}
 else {
 	return b;
 	}
}

U8 	*BinToDec32(U32 binary, U8 *Str)
{
 U8	 tmp0;

 tmp0 = binary/1000000000L;
 *(Str + 0) = tmp0;
 binary = binary - tmp0 * 1000000000L;
 
 tmp0 = binary/100000000L;
 *(Str + 1) = tmp0;
 binary = binary - tmp0 * 100000000L;

 tmp0 = binary/10000000L;
 *(Str + 2) = tmp0;
 binary = binary - tmp0 * 10000000L;

 tmp0 = binary/1000000L;
 *(Str + 3) = tmp0;
 binary = binary - tmp0 * 1000000L;
 
 tmp0 = binary/100000L;
 *(Str + 4) = tmp0;
 binary = binary - tmp0 * 100000L;

 tmp0 = binary/10000L;
 *(Str + 5) = tmp0;
 binary = binary - tmp0 * 10000L;
 
 tmp0 = binary/1000L;
 *(Str + 6) = tmp0;
 binary = binary - tmp0 * 1000L;

 tmp0 = binary/100L;
 *(Str + 7) =  tmp0;
 binary = binary - tmp0 * 100L;

 tmp0 = binary/10L;
 *(Str + 8) =  tmp0;
 binary = binary - tmp0 * 10L;
 
 *(Str + 9) =  binary;

 *(Str + 10) = 0;
 *(Str + 11) = 0;

 return Str;

}


void	Delay(volatile U16 count)
{
 while(count) {
 	delay1us();
 	count--;
 	}
}

