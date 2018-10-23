//////////////////////////////////////////////////////////////////////////////
//
// 	Filename:	Command.h
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


// -- Command Entry Structure
typedef	struct	{
	U8 	CmdCode;
	U8	NextState;
	CmdFuncPointer	CmdAction;
//	void	(*CmdAction)();
	}CmdEntry ;

enum	KeyStates {
	KS_Run,				// Initial state
	KS_Hold,			// Hold state
	};

enum	Type_Focus {
	FC_First,
	FC_TBRate,
	FC_HPos,
	FC_VPos,
	FC_TrigMode,
	FC_TrigEdge,
	FC_Triglvl,
	FC_Last
	};

extern	enum	Type_Focus 	Focus;
extern	enum	Type_Focus 	PrevFocus;
extern	volatile	U8	KeyCode;
extern	volatile	U8	KeyParam;
extern	enum	KeyStates  KeyState;

// ====================================================
//	Function Prototype Declarations
// ====================================================
//
void	CmdProc(void);
void	*CmdAnalysis(U8 Ccode, CmdEntry *CmdTab, U8 *PState);
void	StateRun(void);
void	StateHold(void);
void CmdInit(void);
void	KF_SelVPos(void);
void	KF_SelHPos(void);
void	KF_SelTimeBase(void);
void	KF_SelTrigMode(void);
void	KF_SelTrigEdge(void);
void	KF_SelTrigLevel(void);
void	KF_SelTrigSrc(void);
void	KF_AdjUp(void);
void	KF_AdjDown(void);
void	KF_Hold(void);
void	KF_ExitHold(void);
void	UpdateFocus(void);
void	ParamAdj(U8 Dir);
void Dummy(void);
void Dummy1(U8 Dir);

#endif

