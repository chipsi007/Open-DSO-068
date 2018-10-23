//////////////////////////////////////////////////////////////////////////////
//
// 	Filename:	DSO.h
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

#ifndef	DSO_h

#define	DSO_h

#include	"Common.h"

// =======================================================
// Display parameters
#define	Lmax		158	// Maximun diaplayable wave level
#define	Lmin		98	// Minimun diaplayable wave level
#define	WBsize		256	// Wave buffer size
#define	WDsize		100	// Wave display size
#define	HPStart		24	// HPos indicator start position
#define	HPEnd		75	// HPos indicator end position

// Special indicator locations
#define	HOLDx	105			// "HOLD" location
#define	HOLDy	0

#define	CPLx	106
#define	CPLy	9

#define	VSenx	106
#define	VSeny	15

#define	VPosMax	127
#define	VPosMin		-127

#define	CplMax		CP_GND
#define	CplMin		CP_DC

#define	HPosMin		0

#define	TrigModeMax	TM_Single
#define	TrigModeMin		TM_Auto

#define	TrigEdgeMax	TE_Rising
#define	TrigEdgeMin		TE_Falling

#define	TrigLvlMax		256
#define	TrigLvlMin		0

// =======================================================
//	Scope control type definitions
// =======================================================
//
// -- V sensitivity type
typedef	enum 	{
	VS_5V,
	VS_2V,
	VS_1V,
	VS_05V,
	VS_02V,
	VS_01V,
	VS_50mV,
	VS_20mV,
	VS_10mV
	}TY_VSen ;

// -- Time base type
typedef	enum	 {
	TB_20ms,	// 500Hz	2ms
	TB_10ms,	// 1K	1ms
	TB_5ms,		// 2K	500us
	TB_2ms,		// 5K	200us
	TB_1ms,		// 10K	100us
	TB_05ms,	// 20K	50us
	TB_02ms,	// 50K	20us (400 cycles)
	TB_01ms	// 100K	10us (200 cycles)
	} TY_TimeBase;

// -- Trig Mode type
typedef	enum 	{
	TM_Auto,
	TM_Normal,
	TM_Single
	}TY_TrigMode ;


// -- Trig Edge type
typedef 	enum 	 {
	TE_Falling,
	TE_Rising,
	}TY_TrigEdge;

typedef	struct 	{
	S8	Vsen;			
	S8	Cpl;				
	S16	VPos;			
	S16	HPos;			
	S8	TimeBase;		
	S8	TrigMode;		
	S8	TrigEdge;		
	S16	TrigLvl;			
	U8	Status;				
	U8 	*SampleStart;		
	U16	TrigAddress;	
	} DSO;


// -- Coupling type
typedef	enum 	{
	CP_DC,
	CP_AC,
	CP_GND
	}TY_Cpl ;

// -- Panel layout parameter structures
//  Highlight bar location and size (Y size is fixed to 9)
typedef	struct	{
	U8	x;
	U8	y;
	U8	xsize;	// 
	} FocusBox;

typedef	enum	{
	ScopeMode,
	FMeterMode
	} SystemMode;

// -- Adjustment direction
#define	UP			1
#define	DOWN		0

extern	FocusBox FBox[];
extern	U8 	IndicatorLoc[];
extern	U8	SampleBuf[];
extern	U8	WaveBuf[];	
extern	U8	PrevTLvl;
extern	DSO		MyDso;
extern	DSO		MyDso_EE;

extern	U8 	SysMode;	

// MyDso.Status bit allocation
#define	DoCapture		7
#define	CaptureDone	6
#define	Trigged			5
#define	Abort			4
#define	Hold			3
#define	DummyRead		2
#define	EnterHold		1

#define	EE_signature1	0xFA
#define	EE_signature2	0x5F
#define	EepromVersion	0x00

// -- Function Prototypes --

void	ScopeInit(void);
void	ScopePanel(void);

void	ShowWave(void);

void	AdjVPos(U8 Dir);
void	AdjTimeBase(U8 Dir);
void	AdjHPos(U8 Dir);
void	AdjTrigMode(U8 Dir);
void	AdjTrigEdge(U8 Dir);
void	AdjTrigLvl(U8 Dir);
void	DoCpl(void);
void	DoVsen(void);

void	StartCapture(void);
void	WaveformProc(void);
void	PrepareCapture(void);

void	wClr(void);
void	wVLine(U8 x, U8 y, U8 len);
void	wDisplay(void);
void	UpdateVSen(void);
void	UpdateTimeBase(void);
void UpdateTrigMode(void);
void	UpdateTrigEdge(void);
void	UpdateVPos(void);
void	UpdateTLvl(void);
void	UpdateHPos(void);
void	UpdateHold(void);
void	UpdateCpl(void);
void	SetTLvl(void);
void LoadEEprom(void);
void	EepromReset(void);

#endif

