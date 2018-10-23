//////////////////////////////////////////////////////////////////////////////
//
// 	Filename:	113-06805-010.c
//	Version:		0.10
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
//-----------------------------------------------------------------------------
//	Application: 	DSO 068 (Learning Edition)
//
//-----------------------------------------------------------------------------
//
//	Notes:
//
//-----------------------------------------------------------------------------
//	Revision History:
//
///////////////////////////////////////////////////////////////////////////////

#include "Common.h"
#include "Board.h"
#include "Screen.h"
#include	"Command.h"
#include	"DSO.h"

PROGMEM	U8	CRLF[12] = {"\n\r"};
PROGMEM	U8	Title[20] = {"DSO 068 (LE)"};
PROGMEM	U8	Mfg[20] = {"By JYE Tech Ltd. "};
PROGMEM	U8	Website[20] = {"WWW.JYETECH.COM "};
PROGMEM	U8	Firmware[20] = {"FW: 06805-010"};

int	main (void)
{
 U8	tmp;

 // -- Initialize ports
 Port_Init();

 // -- Initialize misc registers
 Misc_Init();

 Timer_Init();
 Comp_Init();
 ADC_Init();
 
  // -- Initialize Uart 0 and Uart 1
 Uart0_Init();
 Uart1_Init();
 
 uputs_ROM(CRLF, Uart0);
 uputs_ROM(Title, Uart0);
 uputs_ROM(CRLF, Uart0);
 uputs_ROM(Mfg, Uart0);
 uputs_ROM(CRLF, Uart0);
 uputs_ROM(Website, Uart0);
 uputs_ROM(CRLF, Uart0);
 uputs_ROM(Firmware, Uart0);

 LCD_Init();

 ScreenInit();
 
 // --------------------------------------------------
 // Blink LED twice at PB1
 BitSet(DDRB, (1<<DDB1));		// Set PB1 to output
 BitSet(DDRB, (1<<DDB6));		// Set PB6 to output
 BitClr(PORTB, (1<<PB1));		// LED on
 tmp = 200;					// Beep
 while(tmp) {
 	BitClr(PORTB, (1<<PB6));
	Delay(250);
 	BitSet(PORTB, (1<<PB6));
	Delay(250);
	tmp--;
 	}
 BitSet(PORTB, (1<<PB1));		// LED off
 Delay(60000);
 Delay(60000);
 Delay(60000);
 BitClr(PORTB, (1<<PB1));		// LED on
 tmp = 200;					// Beep
 while(tmp) {
 	BitClr(PORTB, (1<<PB6));
	Delay(250);
 	BitSet(PORTB, (1<<PB6));
	Delay(250);
	tmp--;
 	}
 BitSet(PORTB, (1<<PB1));		// LED off
// BitClr(DDRB, (1<<DDB1));	// Set PB1 to input
 BitClr(DDRB, (1<<DDB6));		// Set PB6 to input

 sei();

 // Application initialization
 ScopeInit();

 CmdInit();
 
 ScopePanel();
  
 BitSet(MyDso.Status, (1<<DoCapture));
 while(1) {
 	// Main loop
	if(BitTest(MyDso.Status, (1 << DoCapture))) {
		BitClr(MyDso.Status, (1<<DoCapture));
		StartCapture();
 		}
	if(BitTest(MyDso.Status, (1 << CaptureDone))) {
		BitClr(MyDso.Status, (1<<CaptureDone));
		WaveformProc();
		}
	
	// -- Key processing
	if(KeyCode) {
		CmdProc();
		}		
	
 	}

 return (0);

}

