//////////////////////////////////////////////////////////////////////////////
//
// 	Filename:	Command.h
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
#ifndef	Command_h
#define	Command_h

#include	"Common.h"

// ============== Key Analysis Definitions =====================

// -- Key Code Definitions
#define	KVoid			0x00

#define	KVPos			0x01
#define	KHPos			0x02
#define	KTimeBase		0x03
#define	KTrigMode		0x04
#define	KTrigEdge		0x05
#define	KTrigLvl			0x06
#define	KHold			0x07
#define	KAux			0x08

#define	KAdjCW			0x09
#define	KAdjCCW		0x0A

#define	KAux_VPos		0x60
#define	KAux_HPos		0x61
#define	KAux_TB		0x62
#define	KAux_Mode		0x63
#define	KAux_Slope		0x64
#define	KAux_Level		0x65
#define	KAux_Hold		0x66

// Special states
#define	KCpl			0x40
#define	KCpl_DC			0	
#define	KCpl_AC			1
#define	KCpl_GND		2

#define	KVSen			0x50
#define	KSEN1_1V		0
#define	KSEN1_01V		1
#define	KSEN1_10mV	2
#define	KSEN2_X5		0
#define	KSEN2_X2		1
#define	KSEN2_X1		2


#define	NoKey			0x7FFF	

#define	KD_val			100		// 
#define	KH_val			10		// 

// KFlag bit allocation
#define	KReady		7
#define	KScanRdy	6
#define	Enc_CW		5	// Encoder clockwise
#define	Enc_CCW	4	// Encoder counter clockwise


// -- Key code conversion 
typedef	struct	{
	U16 	ScanCode;
	U8	Keycode;
	}KeyMap ;

extern	U8	KeyCode;
extern	volatile	U8	KFlags;
extern	U8	Prev_Cpl;
extern	U8	Prev_VSEN1;
extern	U8	Prev_VSEN2;
extern	U8	MasterCmd;

// ====================================================
//	Function Prototype Declarations
// ====================================================
//
void	KeyCodeProc(void);
void CmdInit(void);
void KeyScan(void);
void	DetectSwitch(void);

#endif

