//////////////////////////////////////////////////////////////////////////////
//
// 	Filename:	Board.h
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

#ifndef Screen_h

#define	Screen_h

#include	"Common.h"
//#include	"DSO.h"

// ------------ Display parameters -----------------------
// Overall display parameters
#define		Hmax			128					// Horizontal maximun points
#define		Vmax			64					// Vertical maximun points
#define		DBUFsize		((Hmax * Vmax)/8)
#define		Xofs			2
#define		Yofs			0
#define		WLen			100					// Wave window length
#define		MarkVPos		0
#define		MarkTLvl		1

// ----------------------------------------------------
// Info screen parameters
#define		Title_x	10
#define		Title_y	8
#define		Mfg_x	10
#define		Mfg_y	20
#define		Web_x	10
#define		Web_y	32
#define		Ver_x	10
#define		Ver_y	45

#define	DispSize		1024

extern	U8	Bitmap1[];
extern	U8	Bitmap2[];
extern	U8 	RightTriangle[];
extern	U8 	LeftTriangle[];
extern	U8	LeftRight[];
extern	U8	UpDown[];
extern	U8	RisingEdge[];
extern	U8	FallingEdge[];
extern	U8	DispBuf[];
extern	U8	AscBuf[];


// ===========================================================
//	Function Prototype Declarations
// ===========================================================
//
void	ClearDBuf(void);
void	Display(U8 *buf);
void	VLine(U8 x, U8 y, U8 len);
void	HLine(U8 x, U8 y, U8 len);
void	rHLine(U8 x, U8 y, U8 len);
void	Grid(void);
void	Triangle(U8 loc, U8 y, U8 clear);
void	PutChar5x5(U8 x, U8 y, U8 ch);
void	Put_9x7(U8 x, U8 y, U8 *pfont);
void	Hightlight(U8 focus);
void	PutBox(U8 x, U8 y, U8 xSize, U8 ySize);
void	ScreenInit(void);
void	PutC5x7(U8 x, U8 y, U8 ch);
void	LcdPuts(U8 x, U8 y, U8* s); 
void	Puts_ROM(U8 x, U8 y, U8* s, U8 space);

#endif

