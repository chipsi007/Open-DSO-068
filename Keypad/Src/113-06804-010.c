//////////////////////////////////////////////////////////////////////////////
//
// 	Filename:	113-06804-010.c
//	Version:		0.10
//	Data:		
//
//	Author:		Liu, Zemin
//	Company:	JYE Tech
//	Email:		jyetek@gmail.com
//	Web:		www.jyetech.com
//
//-----------------------------------------------------------------------------
//
// 	Target: 		ATMega48
// 	Tool chain: 	WinAvr (GCC-AVR)
//
//-----------------------------------------------------------------------------
//	Program Description: 
//	
//	This firmware detects activities of keypad, switches, and rotary encoder. It first converts 
//		scan codes to key codes and then sends them out through Uart. 
//
//	Communication format:
//		Baudrate:	38400 bps
//		Data bit:		8
//		Stop bit:		1
//		Parity:		None
//		
//		Key codes:
//
//-----------------------------------------------------------------------------
//	Required files:
//
//-----------------------------------------------------------------------------
//	Notes:
//
///////////////////////////////////////////////////////////////////////////////

#include <inttypes.h>
#include <avr/io.h>
#include <avr/wdt.h>
#include <avr/interrupt.h>
#include <avr/pgmspace.h>

#include "Common.h"
#include "Board.h"
#include	"Command.h"


int	main (void)
{
 Misc_Init();
 
 // -- Initialize ports
 Port_Init();

 Uart0_Init();

 Timer0_Init();
 Timer1_Init();	
// Timer2_Init();


 ADC_Init();

 Delay(100);

 sei();

 CmdInit();
 
 while(1) {
 	// Scan buttons and switches
	if(BitTest(KFlags, (1<<KScanRdy))) {
		BitClr(KFlags, (1<<KScanRdy));
		KeyScan();
		DetectSwitch();
		}

	// -- Button processing
	if(KeyCode) {
		KeyCodeProc();
		}

	// Send rotary encoder direction
	if(BitTest(KFlags, (1<<Enc_CW))) {
		BitClr(KFlags, (1<<Enc_CW));
		u0putc(KAdjCW);
		}
	if(BitTest(KFlags, (1<<Enc_CCW))) {
		BitClr(KFlags, (1<<Enc_CCW));
		u0putc(KAdjCCW);
		}

	// Process command from master
	if(MasterCmd) {
		if(MasterCmd == 0xF0) {
			// Clear slide switch status
			Prev_Cpl = 0xFF;
			Prev_VSEN1 = 0xFF;
			Prev_VSEN2 = 0xFF;
			}
		MasterCmd = 0;
		}
 	}

 return (0);
}

