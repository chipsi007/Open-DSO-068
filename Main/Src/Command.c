//////////////////////////////////////////////////////////////////////////////
//
// 	Filename:	Command.c
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
#include "Common.h"
#include "Board.h"
#include "Command.h"
#include "DSO.h"
#include	"Screen.h"

// ===========================================================
// 	File Scope Global variables
// ===========================================================
//

enum	Type_Focus 	Focus, PrevFocus;
enum	KeyStates  	KeyState;

volatile	U8	KeyCode;
volatile	U8	KeyParam;
		
// -- Adjustment control table
// -- Adjustments available for Run State
PROGMEM	void (*AdjTable[FC_Last])(U8) = {
		Dummy1,		// Dummy entry
		AdjTimeBase,
		AdjHPos,
		AdjVPos,
		AdjTrigMode,
		AdjTrigEdge,
		AdjTrigLvl,
		};


// ==========================================================
//	Function Definitions
// ==========================================================
//
void	CmdProc(void)
{
 // Special handling for slide switch codes
 if((KeyCode & 0xF0) == KCpl) {
	// Special handling for couple
	KeyParam = KeyCode & 0x0F;
	KeyCode = KCpl;
	}
 if((KeyCode & 0xF0) == KVSen) {
	// Special handling for vertical sensitivity
	KeyParam = KeyCode & 0x0F;
	KeyCode = KVSen;
	}

 switch(KeyState) {
	case KS_Run:
		StateRun();
		break;

	case KS_Hold:
		StateHold();
	default:
		break;

 	}

 KeyCode = 0;
}

void	StateHold(void)
{
 switch(KeyCode) {
	case KHPos:
		KF_SelHPos();
		break;
		
	case KTrigMode:
		KF_SelTrigMode();
		break;
		
	case KAdjCW:
		KF_AdjUp();
		break;
		
	case KAdjCCW:
		KF_AdjDown();
		break;
		
	case KCpl:
		DoCpl();
		break;
		
	case KVSen:
		DoVsen();
		break;
		
	case KHold:
		KeyState = KS_Run;
		KF_ExitHold();
	default:
		break;
		
 	}
}

void	StateRun(void)
{
 switch(KeyCode) {
	case KVPos:
		KF_SelVPos();
		break;
		
	case KHPos:
		KF_SelHPos();
		break;
		
	case KTimeBase:
		KF_SelTimeBase();
		break;
		
	case KTrigMode:
		KF_SelTrigMode();
		break;
		
	case KTrigEdge:
		KF_SelTrigEdge();
		break;
		
	case KTrigLvl:
		KF_SelTrigLevel();
		break;
		
	case KAdjCW:
		KF_AdjUp();
		break;
		
	case KAdjCCW:
		KF_AdjDown();
		break;
		
	case KCpl:
		DoCpl();
		break;
		
	case KVSen:
		DoVsen();
		break;
		
	case KHold:
		KeyState = KS_Hold;
		KF_Hold();
	default:
		break;
		
 	}
}

void CmdInit(void)
{
 KeyState = KS_Run;
 KeyCode = 0;

 Focus = FC_TBRate;
 PrevFocus = FC_TBRate;

}

void	KF_SelVPos(void)
{
 Focus = FC_VPos;
 UpdateFocus();
}

void	KF_SelHPos(void)
{
 Focus = FC_HPos;
 UpdateFocus();
}

void	KF_SelTimeBase(void)
{
 Focus = FC_TBRate;
 UpdateFocus();
 
}

void	KF_SelTrigMode(void)
{
 Focus = FC_TrigMode;
 UpdateFocus();
}

void	KF_SelTrigEdge(void)
{
 Focus = FC_TrigEdge;
 UpdateFocus();
 KF_AdjUp();
}

void	KF_SelTrigLevel(void)
{
 Focus = FC_Triglvl;
 UpdateFocus();
}

void	KF_AdjUp(void)
{
 switch(KeyState) {
 	case KS_Run:
		KeyParam = 1;
 		ParamAdj(UP);
		break;
		
	case KS_Hold:
		if((Focus == FC_HPos)||(Focus == FC_TrigMode)) {
			KeyParam = 1;
 			ParamAdj(UP);
			}
	default:
		break;
		
 	}
}


void	KF_AdjDown(void)
{
 switch(KeyState) {
 	case KS_Run:
		KeyParam = 1;
 		ParamAdj(DOWN);
		break;
		
	case KS_Hold:
		if((Focus == FC_HPos)||(Focus == FC_TrigMode)) {
			KeyParam = 1;
 			ParamAdj(DOWN);
			}
	default:
		break;
		
 	}
}

void	KF_Hold(void)
{
 BitSet(MyDso.Status, (1<<Hold));
 BitClr(TIMSK , (1<<TICIE1));	
 UpdateHold();
 }

void	KF_ExitHold(void)
{
 BitClr(MyDso.Status, (1<<Hold));
 UpdateHold();
  
 BitSet(MyDso.Status, (1<<DoCapture));
}

void	UpdateFocus(void)
{ 
 // -- Clear highlight at old position
 Hightlight(PrevFocus);

 // -- Set highlight at new position
 Hightlight(Focus);

 // -- Save current position
 PrevFocus = Focus;

 //Update all symbol display
 PutBox(105, 23, 23, 39);
 
}

void	ParamAdj(U8 Dir)
{

 (*(void (*)())pgm_read_word(&AdjTable[Focus]))(Dir);
 
}


void Dummy(void)
{

}

void Dummy1(U8 Dir)
{

}

