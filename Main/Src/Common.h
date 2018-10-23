//////////////////////////////////////////////////////////////////////////////
//
// 	Filename:	Common.h
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

#ifndef Common_h

#define	Common_h

#include <inttypes.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/pgmspace.h>

typedef	unsigned char		U8;
typedef	signed char		S8;
typedef	unsigned short int	U16;
typedef	signed short int	S16;
typedef	unsigned long		U32;
typedef	signed long		S32;

typedef	void	(*FuncPointer)(U8); 
typedef	void	(*CmdFuncPointer)(void); 


// ============= Macro definitions ===========================

#define	BitSet(word, bit_mask)		((word) |= (bit_mask))
#define	BitClr(word, bit_mask)		((word) &= ~(bit_mask))
#define	BitTest(word, bit_mask)		((word) & (bit_mask))
#define	BitAnd(word, bit_mask)		((word) &= (bit_mask))
#define	BitOr(word, bit_mask)		((word) |= (bit_mask))
#define	BitXor(word, bit_mask)		((word) ^= (bit_mask))

// ===========================================================
//	Function Prototype Declarations
// ===========================================================
//
U8	Min(U8 a, U8 b);
U8	Max(U8 a, U8 b);
U8 	*BinToDec32(U32 binary, U8 *Str);
void	Delay(U16 count);

void	delay1us(void);

#endif // Common_h 

