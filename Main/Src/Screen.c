//////////////////////////////////////////////////////////////////////////////
//
// 	Filename:	Screen.c
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
#include	"Board.h"
#include 	"Screen.h"
#include	"DSO.h"
#include	"Command.h"

extern	U8	Title[];
extern	U8	Mfg[];
extern	U8	Website[];
extern	U8	Firmware[];

// ==========================================================
//	File Scope Variables
// ==========================================================
//
PROGMEM	U8	Bitmap1[8] = {
	0xFF,
	0xFE,
	0xFC,
	0xF8,
	0xF0,
	0xE0,
	0xC0,
	0x80
	};

PROGMEM	U8	Bitmap2[8] = {
	0x01,
	0x03,
	0x07,
	0x0F,
	0x1F,
	0x3F,
	0x7F,
	0xFF,
	};

PROGMEM	U8	RightTriangle[2] = {
	0x07,
	0x02
	};

PROGMEM	U8	LeftTriangle[2] = {
	0x02,
	0x07
	};

PROGMEM	U8	LeftRight[9] = {
	0x08, 0x1C, 0x3E, 0x1C, 0x1C, 0x1C, 0x3E, 0x1C, 0x08
	};

PROGMEM	U8	UpDown[9] = {
	0x00, 0x00, 0x14, 0x3E, 0x7F, 0x3E, 0x14, 0x00, 0x00
	};

PROGMEM	U8	RisingEdge[9] = {
	0x00, 0x00, 0x50, 0x48, 0x7F, 0x09, 0x11, 0x00, 0x00
	};

PROGMEM	U8	FallingEdge[9] = {
	0x00, 0x00, 0x05, 0x09, 0x7F, 0x48, 0x44, 0x00, 0x00
	};

PROGMEM	U8	CPL_Font[6 * 5] = {		
	0x1C, 0x0A, 0x09, 0x0A, 0x1C,	// Letter 'A' for couple display
	0x0E, 0x11, 0x11, 0x11, 0x0A,	// Letter 'C'
	0x1F, 0x11, 0x11, 0x11, 0x0E,	// Letter 'D'
	0x0E, 0x11, 0x11, 0x19, 0x1A,	// Letter 'G'
	0x1F, 0x02, 0x04, 0x08, 0x1F,	// Letter 'N'
	0x00, 0x00, 0x00, 0x00, 0x00	// Letter ' '
	};

PROGMEM	U8	Font5X7_asc[(128 - 32)*5]={
	0x00, 0x00, 0x00, 0x00, 0x00,// (space)
	0x00, 0x00, 0x5F, 0x00, 0x00,// !
	0x00, 0x07, 0x00, 0x07, 0x00,// "
	0x14, 0x7F, 0x14, 0x7F, 0x14,// #
	0x24, 0x2A, 0x7F, 0x2A, 0x12,// $
	0x23, 0x13, 0x08, 0x64, 0x62,// %
	0x36, 0x49, 0x55, 0x22, 0x50,// &
	0x00, 0x05, 0x03, 0x00, 0x00,// '
	0x00, 0x1C, 0x22, 0x41, 0x00,// (
	0x00, 0x41, 0x22, 0x1C, 0x00,// )
	0x08, 0x2A, 0x1C, 0x2A, 0x08,// *
	0x08, 0x08, 0x3E, 0x08, 0x08,// +
	0x00, 0x50, 0x30, 0x00, 0x00,// ,
	0x08, 0x08, 0x08, 0x08, 0x08,// -
	0x00, 0x60, 0x60, 0x00, 0x00,// .
	0x20, 0x10, 0x08, 0x04, 0x02,// /
	0x3E, 0x51, 0x49, 0x45, 0x3E,// 0
	0x00, 0x42, 0x7F, 0x40, 0x00,// 1
	0x42, 0x61, 0x51, 0x49, 0x46,// 2
	0x21, 0x41, 0x45, 0x4B, 0x31,// 3
	0x18, 0x14, 0x12, 0x7F, 0x10,// 4
	0x27, 0x45, 0x45, 0x45, 0x39,// 5
	0x3C, 0x4A, 0x49, 0x49, 0x30,// 6
	0x01, 0x71, 0x09, 0x05, 0x03,// 7
	0x36, 0x49, 0x49, 0x49, 0x36,// 8
	0x06, 0x49, 0x49, 0x29, 0x1E,// 9
	0x00, 0x36, 0x36, 0x00, 0x00,// :
	0x00, 0x56, 0x36, 0x00, 0x00,// ;
	0x00, 0x08, 0x14, 0x22, 0x41,// <
	0x14, 0x14, 0x14, 0x14, 0x14,// =
	0x41, 0x22, 0x14, 0x08, 0x00,// >
	0x02, 0x01, 0x51, 0x09, 0x06,// ?
	0x32, 0x49, 0x79, 0x41, 0x3E,// @
	0x7E, 0x11, 0x11, 0x11, 0x7E,// A
	0x7F, 0x49, 0x49, 0x49, 0x36,// B
	0x3E, 0x41, 0x41, 0x41, 0x22,// C
	0x7F, 0x41, 0x41, 0x22, 0x1C,// D
	0x7F, 0x49, 0x49, 0x49, 0x41,// E
	0x7F, 0x09, 0x09, 0x01, 0x01,// F
	0x3E, 0x41, 0x41, 0x51, 0x32,// G
	0x7F, 0x08, 0x08, 0x08, 0x7F,// H
	0x00, 0x41, 0x7F, 0x41, 0x00,// I
	0x20, 0x40, 0x41, 0x3F, 0x01,// J
	0x7F, 0x08, 0x14, 0x22, 0x41,// K
	0x7F, 0x40, 0x40, 0x40, 0x40,// L
	0x7F, 0x02, 0x04, 0x02, 0x7F,// M
	0x7F, 0x04, 0x08, 0x10, 0x7F,// N
	0x3E, 0x41, 0x41, 0x41, 0x3E,// O
	0x7F, 0x09, 0x09, 0x09, 0x06,// P
	0x3E, 0x41, 0x51, 0x21, 0x5E,// Q
	0x7F, 0x09, 0x19, 0x29, 0x46,// R
	0x46, 0x49, 0x49, 0x49, 0x31,// S
	0x01, 0x01, 0x7F, 0x01, 0x01,// T
	0x3F, 0x40, 0x40, 0x40, 0x3F,// U
	0x1F, 0x20, 0x40, 0x20, 0x1F,// V
	0x7F, 0x20, 0x18, 0x20, 0x7F,// W
	0x63, 0x14, 0x08, 0x14, 0x63,// X
	0x03, 0x04, 0x78, 0x04, 0x03,// Y
	0x61, 0x51, 0x49, 0x45, 0x43,// Z
	0x00, 0x00, 0x7F, 0x41, 0x41,// [
	0x02, 0x04, 0x08, 0x10, 0x20,// "\"
	0x41, 0x41, 0x7F, 0x00, 0x00,// ]
	0x04, 0x02, 0x01, 0x02, 0x04,// ^
	0x40, 0x40, 0x40, 0x40, 0x40,// _
	0x00, 0x01, 0x02, 0x04, 0x00,// `
	0x20, 0x54, 0x54, 0x54, 0x78,// a
	0x7F, 0x48, 0x44, 0x44, 0x38,// b
	0x38, 0x44, 0x44, 0x44, 0x20,// c
	0x38, 0x44, 0x44, 0x48, 0x7F,// d
	0x38, 0x54, 0x54, 0x54, 0x18,// e
	0x08, 0x7E, 0x09, 0x01, 0x02,// f
	0x08, 0x14, 0x54, 0x54, 0x3C,// g
	0x7F, 0x08, 0x04, 0x04, 0x78,// h
	0x00, 0x44, 0x7D, 0x40, 0x00,// i
	0x20, 0x40, 0x44, 0x3D, 0x00,// j
	0x00, 0x7F, 0x10, 0x28, 0x44,// k
	0x00, 0x41, 0x7F, 0x40, 0x00,// l
	0x7C, 0x04, 0x18, 0x04, 0x78,// m
	0x7C, 0x08, 0x04, 0x04, 0x78,// n
	0x38, 0x44, 0x44, 0x44, 0x38,// o
	0x7C, 0x14, 0x14, 0x14, 0x08,// p
	0x08, 0x14, 0x14, 0x18, 0x7C,// q
	0x7C, 0x08, 0x04, 0x04, 0x08,// r
	0x48, 0x54, 0x54, 0x54, 0x20,// s
	0x04, 0x3F, 0x44, 0x40, 0x20,// t
	0x3C, 0x40, 0x40, 0x20, 0x7C,// u
	0x1C, 0x20, 0x40, 0x20, 0x1C,// v
	0x3C, 0x40, 0x30, 0x40, 0x3C,// w
	0x44, 0x28, 0x10, 0x28, 0x44,// x
	0x0C, 0x50, 0x50, 0x50, 0x3C,// y
	0x44, 0x64, 0x54, 0x4C, 0x44,// z
	0x00, 0x08, 0x36, 0x41, 0x00,// {
	0x00, 0x00, 0x7F, 0x00, 0x00,// |
	0x00, 0x41, 0x36, 0x08, 0x00,// }
	0x08, 0x08, 0x2A, 0x1C, 0x08,// ->
	0x08, 0x1C, 0x2A, 0x08, 0x08 // <-	};
};


U8	DispBuf[DispSize];

// ==========================================================
//	Function Definitions
// ==========================================================
//
void	ClearDBuf(void)
{
 U8 	*ptmp;
 U16	count;

 ptmp = DispBuf;
 
 // Clear Display buffer
 count = 0;
 while(count < 1024) {
 	*ptmp++ = 0x00;
	count++;
 	}
 
}

// Copy display data from 'buf' to LCD
void	Display(U8 *buf)
{
 U8	*ptmp;
 U8	tmp1, tmp2;

 ptmp = buf;

 // Set display start line to 0
 while(!(LCD_Write(LCD_I_type, LCD_I_StartLn, LCD_Block_Left))) {
 	}
 while(!(LCD_Write(LCD_I_type, LCD_I_StartLn, LCD_Block_Right))) {
 	}
 
 // Set block left X & Yaddress to 0
 while(!(LCD_Write(LCD_I_type, LCD_I_Page0, LCD_Block_Left))) {
 	}
 while(!(LCD_Write(LCD_I_type, LCD_I_Y0, LCD_Block_Left))) {
 	}

 // Set block right X & Y address to 0
 while(!(LCD_Write(LCD_I_type, LCD_I_Page0, LCD_Block_Right))) {
 	}
 while(!(LCD_Write(LCD_I_type, LCD_I_Y0, LCD_Block_Right))) {
 	}


 // Transfer Block left & right data
 tmp1 = 0;
 while(tmp1 < 8) {
 	tmp2 = 0;
	while(tmp2 < 64) {
		while(!(LCD_Write(LCD_D_type, *ptmp, LCD_Block_Left))) {
			}
		ptmp++;
		tmp2++;
		}
 	tmp2 = 0;
	while(tmp2 < 64) {
		while(!(LCD_Write(LCD_D_type, *ptmp, LCD_Block_Right))) {
			}
		ptmp++;
		tmp2++;
		}
	 // Increase X address
	tmp1++;
 	while(!(LCD_Write(LCD_I_type, LCD_I_Page0|tmp1, LCD_Block_Left))) {
		}
 	while(!(LCD_Write(LCD_I_type, LCD_I_Page0|tmp1, LCD_Block_Right))) {
		}
	
 	}
}

// Draw a vertical line at (x, y) of length "len"
void	VLine(U8 x, U8 y, U8 len)
{
 U8 *ptmp; 
 U8 tmp1, tmp2;
 
 if(len == 0) return;

 ptmp = DispBuf + (U16)(((y & 0x3F) >> 3) * Hmax)+ x;

 // -- Find the end point of the line
 len = y + len - 1; 
 if(len >= Vmax) {
 	len = Vmax - 1; 
	}

 // -- Find the number of bytes that includes the line
 tmp2 = (len >> 3) - (y >> 3);
 switch(tmp2)
 	{ case 0: // Line start and end in the same byte
 		tmp1 = pgm_read_byte((U8 *)Bitmap1 + (y & 0x07));
		tmp2 = pgm_read_byte((U8 *)Bitmap2 + (len & 0x07));
		// -- Combine the two bitmap and write to display buffer
		tmp1 &= tmp2;
  		*ptmp |= tmp1;
		break;

   	  case 1: // Line in two bytes
 		// -- Create bitmap with 1s starts at the line's starting point   
 		tmp1 = pgm_read_byte((U8 *)Bitmap1 + (y & 0x07));
  		*ptmp |= tmp1;
		ptmp += Hmax;
		tmp1 = pgm_read_byte((U8 *)Bitmap2 + (len & 0x07));
  		*ptmp |= tmp1;
		break;

	  default: // Line in multiple bytes
 		tmp1 = pgm_read_byte((U8 *)Bitmap1 + (y & 0x07));
  		*ptmp |= tmp1;
		ptmp += Hmax;
	  
		// -- Write all middle bytes
	  	while(tmp2 > 1) {
  			*ptmp |= 0xFF;
			ptmp += Hmax;
	  		tmp2--;
			}

 		// -- Create bitmap with 1s starts at the line's ending point
		tmp1 = pgm_read_byte((U8 *)Bitmap2 + (len & 0x07));
  		*ptmp |= tmp1;
		break;

	}
 
}

// Draw a horizontal line at (x, y) of length "len"
void	HLine(U8 x, U8 y, U8 len)
{
 U8 *ptmp;
 
 if(len == 0) return;

 ptmp = DispBuf + (U16)(((y & 0x3F) >> 3) * Hmax)+ x;

 x = 0x01;
 y &= 0x07;
 x <<= y;

 while(len) {
 	*ptmp |= x;
	ptmp++;
	len--;

 	}
 
}

// Erase a horizontal line at (x, y) of length "len"
void	rHLine(U8 x, U8 y, U8 len)
{
 U8 *ptmp;
 
 if(len == 0) return;

 ptmp = DispBuf + (U16)(((y & 0x3F) >> 3) * Hmax)+ x;

 x = 0x01;
 y &= 0x07;
 x <<= y;
 x = ~x;

 while(len) {
 	*ptmp &= x;
	ptmp++;
	len--;

 	}
 
}

// Draw grid
void	Grid(void)
{
 U8	tmp1;
 U8	*ptmp1, *ptmp2, *ptmp3, *ptmp4, *ptmp5, *ptmp6, *ptmp7, *ptmp8;

 ptmp1 = DispBuf + Hmax + Xofs;
 ptmp2 = DispBuf + Hmax * 2 + Xofs;
 ptmp3 = DispBuf + Hmax * 3 + Xofs;
 ptmp4 = DispBuf + Hmax * 5 + Xofs;
 ptmp5 = DispBuf + Hmax * 6 + Xofs;
 
 tmp1 = 51;
 while(tmp1) {
	*ptmp1 |= 0x04;
	ptmp1 += 2;
	*ptmp2 |= 0x10;
	ptmp2 += 2;
	*ptmp3 |= 0x40;
	ptmp3 += 2;
	*ptmp4 |= 0x01;
	ptmp4 += 2;
	*ptmp5 |= 0x04;
	ptmp5 += 2;
	
	tmp1--;
 	}

 ptmp1 = DispBuf + 10 + Xofs;
 ptmp2 = DispBuf + Hmax + 10 + Xofs;
 ptmp3 = DispBuf + Hmax * 2 + 10 + Xofs;
 ptmp4 = DispBuf + Hmax * 3 + 10 + Xofs;
 ptmp5 = DispBuf + Hmax * 4 + 10 + Xofs;
 ptmp6 = DispBuf + Hmax * 5 + 10 + Xofs;
 ptmp7 = DispBuf + Hmax * 6 + 10 + Xofs;
 ptmp8 = DispBuf + Hmax * 7 + 10 + Xofs;
 
 tmp1 = 9;
 while(tmp1) {
	*ptmp1 |= 0x55;
	ptmp1 += 10;
	*ptmp2 |= 0x55;
	ptmp2 += 10;
	*ptmp3 |= 0x55;
	ptmp3 += 10;
	*ptmp4 |= 0x55;
	ptmp4 += 10;
	*ptmp5 |= 0x55;
	ptmp5 += 10;
	*ptmp6 |= 0x55;
	ptmp6 += 10;
	*ptmp7 |= 0x55;
	ptmp7 += 10;
	*ptmp8 |= 0x15;
	ptmp8 += 10;
	
	tmp1--;
 	}

 HLine(Xofs, 0, 100);
 HLine(Xofs, 60, 100);
 
 VLine(Xofs, 0, 61);
 VLine(100 + Xofs, 0, 61);

}

// Draw VPOS or TLevel indicator
void	Triangle(U8 loc, U8 y, U8 clear)
{
 U8 tmp0, tmp1;
 U8 *ptmp, *pfont;
 U16	tmp3, tmp4;

 // Byte address at DispBuf
 ptmp = DispBuf + (y >> 3) * Hmax + (loc ? 103 : 0);

 if(clear == 0) {
	// Get font
	pfont = loc ? LeftTriangle : RightTriangle;
	tmp3 = pgm_read_byte(pfont);
	tmp4 = pgm_read_byte(pfont + 1);
 	}
 else {
 	tmp3 = 0x0000;
	tmp4 = 0x0000;
 	}

 tmp0 = y & 0x07;
 if(tmp0 != 0) {
 	// Square located at position where x coordinate is not dividable by 8
 	// -- Create bit mask in tmp1
 	tmp1 = 0xFF;
	tmp1 <<= tmp0;

	// Shift font to the right position
	tmp3 <<= tmp0;
	tmp4 <<= tmp0;
	// Write font to DispBuf
	*ptmp++ = tmp3 & 0x00FF;
	*ptmp = tmp4 & 0x00FF;
	ptmp += Hmax -1;
	*ptmp++ = (tmp3 >> 8);
	*ptmp = (tmp4 >> 8);
 	}
 else {
 	// Square located at position where x coordinate is dividable by 8
	// Write font to DispBuf
	*ptmp++ = tmp3 & 0x00FF;
	*ptmp = tmp4 & 0x00FF;
 	}

}

void	PutChar5x5(U8 x, U8 y, U8 ch)
{
 U8 tmp0, tmp1, tmp4;
 U8 *ptmp, *pfont;
 U16	tmp2, tmp3;

 // Byte address at DispBuf
 ptmp = DispBuf + (y >> 3) * Hmax + x;

 // Font address
 pfont = CPL_Font + ch * 5;

 tmp0 = y & 0x07;
 if(tmp0 != 0) {
 	// Square located at position where x coordinate is not dividable by 8
 	tmp2 = 0x1F;
	tmp2 <<= tmp0;
	// Get font
	tmp1 = 0;
	while(tmp1 < 5) {
		tmp3 = pgm_read_byte(pfont);
		// Shift font to the right position
		tmp3 <<= tmp0;
		// Write font to DispBuf
		tmp4 = *ptmp;
		tmp4 &= ~((U8) tmp2);
		tmp4 |= (tmp3 & 0x00FF);
		*ptmp = tmp4;
		tmp4 = *(ptmp + Hmax);
		tmp4 &= ~((U8)(tmp2>>8));
		tmp4 |=  (tmp3 >> 8);
		*(ptmp + Hmax) = tmp4;
		ptmp++;
		pfont++;
		tmp1++;
		}
 	}
 else {
 	// Square located at position where x coordinate is dividable by 8
	// Get font
	tmp1 = 0;
	while(tmp1 < 5) {
		tmp3 = pgm_read_byte(pfont);
		// Write font to DispBuf
		tmp4 = *ptmp;
		tmp4 &= 0xE0;
		tmp4 |= (tmp3 & 0x00FF);
		*ptmp = tmp4;
		ptmp++;
		pfont++;
		tmp1++;
		}
 	}

 
}

void	Put_9x7(U8 x, U8 y, U8 *pfont)
{
 U8 tmp0, tmp1, tmp2, tmp4;
 U8 *ptmp;
 U16	tmp3;

 // Byte address at DispBuf
 ptmp = DispBuf + (y >> 3) * Hmax + x;

 tmp0 = y & 0x07;
 if(tmp0 != 0) {
 	// Square located at position where x coordinate is not dividable by 8
 	tmp2 = 0xFF;
	tmp2 <<= tmp0;
	// Get font
	tmp1 = 0;
	while(tmp1 < 9) {
		tmp3 = pgm_read_byte(pfont);
		// Shift font to the right position
		tmp3 <<= tmp0;
		// Write font to DispBuf
		tmp4 = *ptmp;
		tmp4 &= ~tmp2;
		tmp4 |= (tmp3 & 0x00FF);
		*ptmp = tmp4;
		tmp4 = *(ptmp + Hmax);
		tmp4 &= tmp2;
		tmp4 |=  (tmp3 >> 8);
		*(ptmp + Hmax) = tmp4;
		ptmp++;
		pfont++;
		tmp1++;
		}
 	}
 else {
 	// Square located at position where x coordinate is dividable by 8
	// Get font
	tmp1 = 0;
	while(tmp1 < 9) {
		tmp3 = pgm_read_byte(pfont);
		// Write font to DispBuf
		tmp4 = *ptmp;
		tmp4 &= 0x80;
		tmp4 |= (tmp3 & 0x00FF);
		*ptmp = tmp4;
		ptmp++;
		pfont++;
		tmp1++;
		}
 	}
}

void	Hightlight(U8 focus)		// y size fixed to 9
{
 U8 tmp0, tmp1, tmp2;
 U8 *ptmp;
 U16	tmp3;

 // x location
 tmp0 = pgm_read_byte((U8 *)FBox + focus*3 + 0);
 
 // y location
 tmp1 = pgm_read_byte((U8 *)FBox + focus*3 + 1);

 // x size
 tmp2 = pgm_read_byte((U8 *)FBox + focus*3 + 2);
 
 // Byte address at DispBuf
 ptmp = DispBuf + (tmp1 >> 3) * Hmax + tmp0;

 // Create mask in tmp3
 tmp0 = tmp1 & 0x07;
 tmp3 = 0x01FF;
 tmp3 <<= tmp0;
 
 // Write font to DispBuf
 while(tmp2) {
	*ptmp ^= (tmp3 & 0x00FF);
	*(ptmp + Hmax) ^=  (tmp3 >> 8);
	ptmp++;
	tmp2--;
	}
}

// Copy a rantangle area at (x, y) from display buffer to LCD
//
void	PutBox(U8 x, U8 y, U8 xSize, U8 ySize)
{
 U8 tmp0, tmp1, tmp2, tmp3;
 U8	*ptmp;

 // Byte address in Dispbuf
 ptmp = DispBuf + (U16)(((y & 0x3F) >> 3) * Hmax)+ x;

 // Determine end Page address
 ySize = y + ySize - 1;
 ySize >>= 3;

 // Begin Page address
 y >>= 3;
 
 // Determine end X address
 tmp3 = x + xSize - 1;

 while(y <= ySize) {
 	tmp1 = x;
	while(tmp1 <= tmp3) {
		// Determine Block
		if(tmp1 < 64) {
			tmp0 = 0;
			}
		else {
			tmp0 = 1;
			}
		// Set Page address
		LCD_Write(LCD_I_type, LCD_I_Page0|y, tmp0);
		// Set byte address within a page
		tmp2 = tmp1 - tmp0 * 64;
		LCD_Write(LCD_I_type, LCD_I_Y0|tmp2, tmp0);
		
		LCD_Write(LCD_D_type, *ptmp, tmp0);
		tmp1++;
		ptmp++;
		}
	y++;
	ptmp += Hmax - xSize;
 	}
 
}

void	ScreenInit(void)
{
 U8	tmp;
 
 ClearDBuf();

 Puts_ROM(Title_x, Title_y, Title, 6);

 Puts_ROM(Mfg_x, Mfg_y, Mfg, 6);

 Puts_ROM(Web_x, Web_y, Website, 6);

 Puts_ROM(Ver_x, Ver_y, Firmware, 6);

 Display(DispBuf);

 tmp = 10;
 while(tmp) {
	Delay(60000);
	tmp--;
 	}

}

// Write charactor to location (x, y) of 'buf'
//
void	PutC5x7(U8 x, U8 y, U8 ch)
{
 U8 tmp0, tmp1, tmp2, tmp4;
 U8 *ptmp, *pfont;
 U16	tmp3;

 // Byte address at Buf
 ptmp = DispBuf + (y >> 3) * Hmax + x;

 // Font address
 pfont = Font5X7_asc + (ch - 32) * 5;

 tmp0 = y & 0x07;
 if(tmp0 != 0) {
 	// Square located at position where x coordinate is not dividable by 8
 	tmp2 = 0xFF;
	tmp2 <<= tmp0;
	// Get font
	tmp1 = 0;
	while(tmp1 < 5) {
		tmp3 = pgm_read_byte(pfont);
		// Shift font to the right position
		tmp3 <<= tmp0;
		// Write font to DispBuf
		tmp4 = *ptmp;
		tmp4 &= ~tmp2;
		tmp4 |= (tmp3 & 0x00FF);
		*ptmp = tmp4;
		tmp4 = *(ptmp + Hmax);
		tmp4 &= tmp2;
		tmp4 |=  (tmp3 >> 8);
		*(ptmp + Hmax) = tmp4;
		ptmp++;
		pfont++;
		tmp1++;
		}
 	}
 else {
 	// Square located at position where x coordinate is dividable by 8
	// Get font
	tmp1 = 0;
	while(tmp1 < 5) {
		tmp3 = pgm_read_byte(pfont);
		// Write font to DispBuf
		tmp4 = *ptmp;
		tmp4 &= 0x80;
		tmp4 |= (tmp3 & 0x00FF);
		*ptmp = tmp4;
		ptmp++;
		pfont++;
		tmp1++;
		}
 	}

 
}

// Write ROM string to location (x, y) of 'buf'
//
void	Puts_ROM(U8 x, U8 y, U8* s, U8 space) 
{
 U8	tmp;

 tmp = pgm_read_byte(s);
 while(tmp != 0) {
	PutC5x7(x, y, tmp);
	s++;
	x += space;
	tmp = pgm_read_byte(s);
 	}

}




