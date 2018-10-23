//////////////////////////////////////////////////////////////////////////////
//
// 	Filename:	Common.h
//	Version:		
//	Data:		
//
//	Author:		Liu, Zemin
//	Company:	JYE Tech
//	Email:		jyetek@gmail.com
//	Web:		www.jyetech.com
//
//-----------------------------------------------------------------------------
//
// 	Target: 		ATmega48
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

typedef	unsigned char		U8;
typedef	signed char		S8;
typedef	unsigned short int	U16;
typedef	signed short int	S16;
typedef	unsigned long		U32;
typedef	signed long		S32;

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
void	Zmon(void);
void	delay1us(void);
void	Delay(U16 count);

#endif // Common_h 

