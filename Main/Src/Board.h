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
//

#ifndef	Board_h

#define	Board_h

#include	"Common.h"

// CPU frequency
//#define F_CPU      8000000L          	

// ===========================================
//	Uart parameters
//
#define	Uart0_Baud 		38400L
#define	Uart1_Baud 		38400L

#define	Uart0 			0
#define	Uart1			1

// LCD definitions
#define	LCD_Data_Port	PORTC
#define	LCD_Data_Dir	DDRC
#define	LCD_Data_In	PINC
#define	LCD_CS1_Port	PORTG
#define	LCD_CS1_Bit		PG0
#define	LCD_CS2_Port	PORTG
#define	LCD_CS2_Bit		PG1
#define	LCD_RES_Port	PORTG
#define	LCD_RES_Bit		PG2
#define	LCD_WR_Port	PORTG
#define	LCD_WR_Bit		PG3
#define	LCD_E_Port		PORTG
#define	LCD_E_Bit		PG4
#define	LCD_DI_Port		PORTD
#define	LCD_DI_Bit		PD7

#define	LCD_Delay		1

#define	LCD_I_Page0	0b10111000
#define	LCD_I_Y0		0b01000000
#define	LCD_I_On		0b00111111
#define	LCD_I_StartLn	0b11000000

#define	LCD_I_type		0
#define	LCD_D_type		1

#define	LCD_Block_Left	0
#define	LCD_Block_Right 1

#define	ADC_CLK_Port	PORTB
#define	ADC_CLK_Bit		PB7
#define	ADC_DataIn		PINA

#define	LED_Port		PORTB
#define	LED_Bit			PB1

#define 	SampleBuf1		((U8 *)(0x0C00))	// On-chip SRAM

// ====================================================
//	Macros
//

// ====================================================
//	Function Prototype Declarations
//
void	SYS_Init(void);
void	Misc_Init(void);
void Port_Init(void);
void Timer_Init (void);
void Comp_Init(void);
void ADC_Init (void);
void DelayTimer(U16 ms);
void Uart0_Init (void);
void Uart1_Init (void);

void	u0putc(U8 ch);
void	uPutc(U8 UartNum, U8 ch);
U8	u0getc(void);
void	uputs(U8 *s, U8 UartNum);
void	uputs_ROM(U8 *s, U8 UartNum);

void	LCD_Init(void);
U8	LCD_Read(U8 type, U8 block);
U8	LCD_Write(U8 type, U8 date, U8 block);

void EE_Write(U8 *Addr, U8 Data);
U8 	EE_Read(U8 *Addr);


#endif	// Board_h


