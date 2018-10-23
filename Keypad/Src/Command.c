//////////////////////////////////////////////////////////////////////////////
//
// 	Filename:	Command.c
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
#include	<avr/Pgmspace.h>

#include "Common.h"
#include "Board.h"
#include "Command.h"

// ===========================================================
// 	File Scope Global variables
// ===========================================================
//
		
PROGMEM	U8	enc_table[16] = {
	0,		// 0000
	1,		// 0001
	-1,		// 0010
	0,		// 0011
	0,		// 0100
	0,		// 0101
	0,		// 0110
	0,		// 0111
	0,		// 1000
	0,		// 1001
	0,		// 1010
	0,		// 1011
	0,		// 1100
	-1,		// 1101
	1,		// 1110
	0		// 1111
	};

// Key position
// Bit[9:8] = Col[1:0].  Bit[7:0] = {HOLD,  SEC/DIV, HPOS, VPOS, AUX, LEVEL, SLOPE, MODE}
PROGMEM 	KeyMap KMap[24] = {
	{0x7DEF, KVPos		},	// SW2
	{0x7EFE, KTrigMode	},	// SW3
	{0x7DDF, KHPos		},	// SW4
	{0x7EFD, KTrigEdge	},	// SW5
	{0x7DBF, KTimeBase	},	// SW6
	{0x7EFB, KTrigLvl		},	// SW7
	{0x7D7F, KHold		},	// SW8
	{0x7FF7, KAux		},	// SW_PB
	
	{0x7DE7, KAux_VPos		},	// Aux_SW2
	{0x7EF6, KAux_Mode		},	// Aux_SW3
	{0x7DD7, KAux_HPos		},	// Aux_SW4
	{0x7EF5, KAux_Slope		},	// Aux_SW5
	{0x7DB7, KAux_TB			},	// Aux_SW6
	{0x7EF3, KAux_Level		},	// Aux_SW7
	{0x7D77, KAux_Hold		},	// Aux_SW8
	
	{0xFDEF, KVPos | 0x80		},	// SW2 Hold
	{0xFEFE, KTrigMode| 0x80	},	// SW3 Hold
	{0xFDDF, KHPos | 0x80		},	// SW4 Hold
	{0xFEFD, KTrigEdge | 0x80	},	// SW5 Hold
	{0xFDBF, KTimeBase | 0x80	},	// SW6 Hold
	{0xFEFB, KTrigLvl | 0x80	},	// SW7 Hold
	{0xFD7F, KHold | 0x80		},	// SW8 Hold
	{0xFFF7, KAux | 0x80		},	// SW_PB Hold
	
	{0, 		0			}	// End of table
	};

volatile	U16	KScanBuf;
volatile	U8	enc_state;
volatile	U8	KFlags;

U16	KScan;		
U8	KCount;
U8	KHCount;	// Key hold counter
U8	KTimeChk;	// Key time check 
U8	KeyCode;

U8	Prev_Cpl;
U8	Prev_VSEN1;
U8	Prev_VSEN2;

U8	MasterCmd;

// ==========================================================
//	Function Definitions
// ==========================================================
// Map scan code to key code
U8	KeyConvert(U16 KScancode, KeyMap *keymap)
{
 U16 tmp0;
 
 // -- Looking for match command code
 while((tmp0 = pgm_read_word((U8 *)keymap)) != 0) {
	if(tmp0 == KScancode) {
		// -- Match found
		return (pgm_read_byte((U8 *)keymap + 2));
		}
	else {
		// -- Proceed to next entry
		keymap = (KeyMap *)((U8 *)keymap+ sizeof(KeyMap));
		}
 	}

 return 0;
}

void	KeyCodeProc(void)
{
 if(KeyCode != 0) {
	if(KeyCode == (KAux | 0x80)) {
		// Power OFF
		while(1) {
			BitClr(Power_Port, (1 << Power_Bit));
			}
		}

	if(KeyCode == (KTrigLvl | 0x80)) {
		// LCD backlight ON/OFF
		BitXor(Backlight_Port, (1 << Backlight_Bit));
		KeyCode = 0;
		return;
		}
	
 	u0putc(KeyCode);
	KeyCode = 0;
	}
}

void CmdInit(void)
{
 KeyCode = 0;
 KFlags = 0x00;

 MasterCmd = 0;
}

void KeyScan(void)
{
 U8	tmp0, tmp1;
 
 // Scan
 KScanBuf = NoKey;

 // Read COL 1
 tmp0 = 0x7F;
 KeyPortOut = (tmp0 & KeyPortOut_Bits) | (KeyPortOut & ~KeyPortOut_Bits);
 Delay(10);
 // Read keypad input
 tmp1 = KeyPortIn1 & KeyPortIn1_Bits;
 if(tmp1 != KeyPortIn1_Bits) {
	// Bit[9:8] = Col[1:0].  Bit[7:0] = {HOLD,  SEC/DIV, HPOS, VPOS, AUX, LEVEL, SLOPE, MODE}
	KScanBuf &= ((((U16)tmp0 >> 6) | 0x7C) << 8) |((tmp1 << 4) | 0x0F);
	}
 // Read COL 0
 tmp0 = 0xBF;
 KeyPortOut = (tmp0 & KeyPortOut_Bits) | (KeyPortOut & ~KeyPortOut_Bits);
 Delay(10);
 // Read keypad input
 tmp1 = KeyPortIn1 & KeyPortIn1_Bits;
 if(tmp1 != KeyPortIn1_Bits) {
	// Bit[9:8] = Col[1:0].  Bit[7:0] = {HOLD, MENU, S/DIV, CH1, AUX, ALT, TRIGGER, CH2}
	KScanBuf &= ((((U16)tmp0 >> 6) | 0x7C) << 8) |(tmp1 | 0xF0);
	}
// 	}

 // Read SW_PB
 if(!BitTest(PushButton_Port, (1 << PushButton_Bit))) {
 	// Bit[3] = Pushbutton
	KScanBuf &= 0xFFF7;
 	}
 
 // -- Debouncing
 if((KScanBuf == NoKey) || (KScanBuf != KScan)) {
 	KScan = KScanBuf;
	KCount = 0;
	KHCount = 0;
	KTimeChk = KH_val;
 	}
 else {
	KCount++;
	if(KCount > KD_val) {
		if(KCount == 255) {
			KCount = KD_val;
			if(++KHCount == KTimeChk) {
				// Key-holding found
				KeyCode = KeyConvert((U16)KScan | 0x8000, KMap);

				}
			}
		}
	else if(KCount == KD_val) {
		// Key-hitting found
		KeyCode = KeyConvert((U16)KScan, KMap);
		}
	}

 // Process encoder
 tmp0 = KeyPortIn2 & KeyPortIn2_Bits;
 tmp0 >>= 6;
 enc_state <<= 2;
 enc_state |= tmp0;
 tmp0 = pgm_read_byte(&enc_table[enc_state & 0x0F]);
 if(tmp0 == 1) {
	BitSet(KFlags, (1<<Enc_CCW));
	}
 if(tmp0 == 0xFF) {
	BitSet(KFlags, (1<<Enc_CW));
	}
 
 }

void	DetectSwitch(void)
{
 U8	tmp0;

 // Detect Cpl switch (SW1)
 tmp0 = ADC_Poll(ADC_CPLSEL);
 if(tmp0 > LevelHigh) {
	tmp0 = KCpl_DC;
 	}
 else if(tmp0 < LevelLow) {
	tmp0 = KCpl_GND;
 	}
 else {
	tmp0 = KCpl_AC;
 	}

 if(tmp0 != Prev_Cpl) {
 	Prev_Cpl = tmp0;
	tmp0 |= KCpl;
	u0putc(tmp0);
 	Delay(10000);
	}

 
 // Detect VSEN1 switch (SW2)
 tmp0 = ADC_Poll(ADC_VSENSEL1);
 if(tmp0 > LevelHigh) {
	tmp0 = KSEN1_10mV;
 	}
 else if(tmp0 < LevelLow) {
	tmp0 = KSEN1_1V;
 	}
 else {
	tmp0 = KSEN1_01V;
 	}
 
 if(tmp0 != Prev_VSEN1) {
 	Prev_VSEN1 = tmp0;
	tmp0 = Prev_VSEN1 * 3 + Prev_VSEN2;
	tmp0 |= KVSen;
	u0putc(tmp0);
 	Delay(10000);
 	}
 
 // Detect VSEN2 switch (SW3)
 tmp0 = ADC_Poll(ADC_VSENSEL2);
 if(tmp0 > LevelHigh) {
	tmp0 = KSEN2_X1;
 	}
 else if(tmp0 < LevelLow) {
	tmp0 = KSEN2_X5;
 	}
 else {
	tmp0 = KSEN2_X2;
 	}

 if(tmp0 != Prev_VSEN2) {
 	Prev_VSEN2 = tmp0;
	tmp0 = Prev_VSEN1 * 3 + Prev_VSEN2;
	tmp0 |= KVSen;
	u0putc(tmp0);
 	}

}

