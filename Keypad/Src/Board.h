//////////////////////////////////////////////////////////////////////////////
//
// 	Filename:	Board.h
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
//

#ifndef	Board_h

#define	Board_h

#include	"Common.h"


// CPU frequency is defined in makefile

// ===========================================
//	Uart parameters
//
#define	Uart0_Baud 	38400L
#define	Uart1_Baud 	38400L

#define	Uart0 	0
#define	Uart1	1

// -- Port bit definitions
#define	Backlight_Port	PORTB
#define	Backlight_Bit	PB5
#define	Power_Port		PORTB
#define	Power_Bit		PB0

#define	KeyPortIn1		PINC
#define	KeyPortIn1_Bits	((1 << PINC3)|(1 << PINC2)|(1 << PINC1)|(1 << PINC0))
#define	KeyPortIn2		PINB
#define	KeyPortIn2_Bits	((1 << PINB7) | (1 << PINB6))
#define	KeyPortOut		PORTD
#define	KeyPortOut_Bits	((1 << PD7)|(1 << PD6))
#define	PushButton_Port	PIND
#define	PushButton_Bit	PIND5

// ADC channels
#define	ADC_VSENSEL1	0x04
#define	ADC_VSENSEL2	0x05
#define	ADC_CPLSEL		0x06

// ADC reference voltage: AVcc (5.0V)
// Slide switch detection threshhold
#define	LevelHigh		192	// 
#define	LevelLow		64	// 

// ====================================================
//	Macros
//

// ====================================================
//	Function Prototype Declarations
//
void	Misc_Init(void);
void Port_Init(void);
void	ExtIntr_Init(void);
void ADC_Init (void);
U8	ADC_Poll(U8 channel);
void	VREF_Init(void);
void Timer0_Init(void);
void Timer1_Init (void);
void Timer2_Init (void);
void Comp_Init(void);
void Uart0_Init (void);

void	u0putc(U8 ch);
U8	u0getc(void);
U8	u0rcheck(void);
void	uputs(U8 *s);

U8 EEPROM_read(U16 uiAddress);
void EEPROM_write(U16 uiAddress, U8 ucData);

#endif	// Board_h


