//////////////////////////////////////////////////////////////////////////////
//
// 	Filename:	DSO.c
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
#include 	"DSO.h"
#include	"Screen.h"
#include	"Board.h"
#include	"Command.h"

// ===========================================================
// 	File Scope Global variables
// ===========================================================
//

U8	PrevSample;
U8	PrevVPos;
U8	PrevTLvl;
U8	PrevHPos;
U8	SampleCount;

U8	WaveBuf[WLen * (Vmax/8)];	

U8	Signature_EE1 __attribute__ ((section (".eeprom"))) = {EE_signature1};
U8	Signature_EE2 __attribute__ ((section (".eeprom"))) = {EE_signature2};
U8	EEversion __attribute__ ((section (".eeprom"))) = {0x00};

DSO		MyDso;
DSO		MyDso_EE __attribute__ ((section (".eeprom")));
PROGMEM	DSO MyDso_ROM	= {
		VS_05V,		// Vsen
		CP_DC,		// Couple
		0,			// VPos
		80, 			// HPos
		TB_1ms, 		// Timebase
		TM_Auto,		// TrigMode
		TE_Falling,	// TrigEdge
		128,			// TrigLvl
		0,			// Status
		SampleBuf1,
		(U16)(SampleBuf1 + 64)
		};


PROGMEM 	FocusBox FBox[7] = {
		{0, 0, 23},		// dummy
		{105, 23, 23},		// Timebase
		{117, 32, 11},		// H position
		{105, 32, 11},		// V position
		{105, 43, 23},		// Trig mode
		{105, 52, 11},		// Trig edge
		{117, 52, 11}		// Trig level
	};

PROGMEM 	U8 TBString[8][6] = {
	"20ms",				//	TB_20ms,
	"10ms",				//	TB_10ms,
	" 5ms",				//	TB_5ms,
	" 2ms",				//	TB_2ms,
	" 1ms",				//	TB_1ms,
	".5ms",				//	TB_05ms,
	".2ms",				//	TB_02ms,
	".1ms"				//	TB_01ms,
						//	TB_max
	};

// Locations (x, y) of each adjustable item
PROGMEM 	U8 IndicatorLoc[(FC_Last + 1) * 2] = {
	0,	0,	// dummy
	106,	24,	// Timebase
	118,	33,	// HPos
	106,	33,	// VPos
	106,	44,	// Trig Mode
	106,	53,	// Trig edge
	118,	53,	// Trig level
	106, 15	// V Sensitivity
	};

PROGMEM 	U8 TMString[3][5] = {
	"AUTO",			// AUT
	"NORM",			// NOR
	"SING"			// SIG
	};

PROGMEM	U8	VsenString[9][5] = {
	{"  5V"},		// 	VS_5V,
	{"  2V"},		// 	VS_2V,
	{"  1V"},		// 	VS_1V,
	{"0.5V"},		// 	VS_05V,
	{"0.2V"},		// 	VS_02V,
	{"0.1V"},		// 	VS_01V,
	{"50mV"},	// 	VS_50mV,
	{"20mV"},	// 	VS_20mV,
	{"10mV"}	// 	VS_10mV,
	};

PROGMEM 	U16	TBconstants[8] = {
	// -- Constants for ADC clock in FlashMode
	40000,		//	TB_20ms,	// 500Hz	2ms
	20000,		//	TB_10ms,	// 1K	1ms
	10000,		//	TB_5ms,		// 2K	500us
	4000,		//	TB_2ms,		// 5K	200us
	2000,		//	TB_1ms,		// 10K	100us
	1000,		//	TB_05ms,	// 20K	50us
	400,			// 	TB_02ms,	// 50K	20us (400 cycles)
	200			// 	TB_01ms,	// 100K	10us (200 cycles)
	};

PROGMEM 	U8 Str_HOLD[5] = {"HOLD"};
PROGMEM 	U8 Str_Space4[5] = {"    "};

	
// ===========================================================
// 	Function Definitions
// ===========================================================
//
void	ShowWave(void)
{
 U8 tmp0, tmp1, tmp2, tmp3;
 S16 tmp4, tmp5;
 U8 *ptmp1, *ptmp2;

 if(!BitTest(MyDso.Status, (1<<Hold))) {
 	ptmp1 = SampleBuf1;
 	ptmp2 = ptmp1 + WBsize;
 	*ptmp2++ = *ptmp1++;
	 tmp0 = 255;
 	while(tmp0) {
 		*ptmp2++ = *ptmp1++;
		tmp0--;
 		}
 	}

 if(BitTest(MyDso.Status, (1<<EnterHold))) {
 	BitSet(MyDso.Status, (1<<Hold));
	BitClr(MyDso.Status, (1<<EnterHold));
 	BitClr(TIMSK , (1<<TICIE1));	
	KeyState = KS_Hold;
 	UpdateHold();
 	}

 ptmp1 = (U8 *)(WBsize + MyDso.SampleStart + MyDso.HPos);
 if(ptmp1 >= (SampleBuf1 + 2 * WBsize)) {
	ptmp1 = (U8*)((U16)ptmp1 - WBsize);
 	}

 // -- Wave update 
 wClr();

 tmp5 = MyDso.VPos;
 tmp0 = 0;
 while(tmp0 < 100) {
 	// Get a sample
 	tmp4 = (S16)*ptmp1;
	tmp4 += tmp5;
	if(tmp4 > Lmax) {
		tmp1 = Lmax;
		}
	else if(tmp4 < Lmin) {
		tmp1 = Lmin;
		}
	else {
		tmp1 = tmp4;
		}

/*
	if(tmp1 > Lmax) {
		tmp1 = Lmax;
		}
	if(tmp1 < Lmin) {
		tmp1 = Lmin;
		}
*/

	if(tmp0 == 0) {
		// -- Make previous point the same as current 
		PrevSample = tmp1;
		}

	tmp2 = Min(tmp1, PrevSample);
	tmp3 = Max(tmp1, PrevSample);
		
	PrevSample = tmp1;

	// -- Draw the vertival line segment between two points
	wVLine(tmp0, Lmax - tmp3, tmp3 - tmp2 + 1);
	
	ptmp1 += 1;
	if((U16)ptmp1 >= ((U16)SampleBuf1 + 2 * WBsize)) {
		ptmp1 = SampleBuf1 + WBsize;
		}
	tmp0++;
	}

 wDisplay();
}

// ----------------------------------------------------------------------------
//	Scope Initialization
// ----------------------------------------------------------------------------
//
void	ScopeInit(void)
{
 // Clear wave buffer
 wClr();
 
 LoadEEprom();

 MyDso.SampleStart = SampleBuf1;
 MyDso.TrigAddress = (U16)(SampleBuf1 + 64);
 MyDso.Status = 0;
 
 Focus = FC_TBRate;
 PrevFocus = FC_TBRate;
 PrevVPos = MyDso.VPos;
 PrevTLvl = MyDso.TrigLvl;
 PrevHPos = MyDso.HPos;


 BitSet(MyDso.Status, (1<<DoCapture));

 // Update slide switch display
 uPutc(Uart1, 0xF0);
}

void	ScopePanel(void)
{
 ClearDBuf();

 Grid();
 Put_9x7(
 	pgm_read_byte((U8 *)IndicatorLoc + FC_VPos*2),
 	pgm_read_byte((U8 *)IndicatorLoc + FC_VPos*2 + 1),
	UpDown);	// VPos adjustment symbol
 Put_9x7(
 	pgm_read_byte((U8 *)IndicatorLoc + FC_HPos*2),
 	pgm_read_byte((U8 *)IndicatorLoc + FC_HPos*2 + 1),
	LeftRight);	// HPos adjustment symbol
 Put_9x7(
 	pgm_read_byte((U8 *)IndicatorLoc + FC_Triglvl*2),
 	pgm_read_byte((U8 *)IndicatorLoc + FC_Triglvl*2 + 1),
	UpDown);	// Triglevel adjustment symbol
 
 VLine(24 + Xofs, 61, 3);
 VLine(75 + Xofs, 61, 3);

 UpdateCpl();
 UpdateVSen();
 UpdateTrigMode();
 UpdateTimeBase();
 UpdateTrigEdge();
 UpdateVPos();
 UpdateTLvl();
 UpdateHPos();
 UpdateHold();
 
 Hightlight(Focus);

 Display(DispBuf);

}


// Clear wave buffer
void	wClr(void) 
{
 U8	*ptmp;
 U16	tmp1;

 ptmp = WaveBuf;
 tmp1 = WLen * (Vmax/8);
 while(tmp1) {
	*ptmp++ = 0x00;
	tmp1--;
 	} 
}

// Draw a vertical line at (x, y) of length "len"
void	wVLine(U8 x, U8 y, U8 len)
{
 U8 *ptmp; 
 U8 tmp1, tmp2;
 
 if(len == 0) return;

 ptmp = WaveBuf + (U16)(((y & 0x3F) >> 3) * WLen)+ x;

 // -- Find the end point of the line
 len = y + len - 1; 
 if(len >= Vmax) {
 	len = Vmax - 1; 
	}

 // -- Find the number of bytes that include the line
 tmp2 = (len >> 3) - (y >> 3);
 switch(tmp2)
 	{ case 0: // Line start and end in the same byte
 		tmp1 = pgm_read_byte((U8 *)Bitmap1 + (y & 0x07));
		tmp2 = pgm_read_byte((U8 *)Bitmap2 + (len & 0x07));
		// -- Combine the two bitmap and write to display buffer
		tmp1 &= tmp2;
  		*ptmp |= tmp1;
		break;

   	  case 1: // Line falls in two bytes
 		// -- Create bitmap with 1s starts at the line's starting point   
 		tmp1 = pgm_read_byte((U8 *)Bitmap1 + (y & 0x07));
  		*ptmp |= tmp1;
		ptmp += WLen;
		tmp1 = pgm_read_byte((U8 *)Bitmap2 + (len & 0x07));
  		*ptmp |= tmp1;
		break;

	  default: // Line in multiple bytes
 		tmp1 = pgm_read_byte((U8 *)Bitmap1 + (y & 0x07));
  		*ptmp |= tmp1;
		ptmp += WLen;
	  
		// -- Write all middle bytes
	  	while(tmp2 > 1) {
  			*ptmp |= 0xFF;
			ptmp += WLen;
	  		tmp2--;
			}

 		// -- Create bitmap with 1s starts at the line's ending point
		tmp1 = pgm_read_byte((U8 *)Bitmap2 + (len & 0x07));
  		*ptmp |= tmp1;
		break;

	}
 
}

// Copy waveform data from Wavebuf to LCD
void	wDisplay(void)
{
 U8	*ptmp, *ptmp1;
 U8	tmp1, tmp2;

 ptmp = DispBuf + Xofs;
 ptmp1 = WaveBuf;

 LCD_Write(LCD_I_type, LCD_I_Page0, LCD_Block_Left);
 LCD_Write(LCD_I_type, LCD_I_Y0 + Xofs, LCD_Block_Left);
 LCD_Write(LCD_I_type, LCD_I_Page0, LCD_Block_Right);
 LCD_Write(LCD_I_type, LCD_I_Y0, LCD_Block_Right);

 // Transfer Block left & right data
 tmp1 = 0;
 while(tmp1 < 8) {
 	tmp2 = 0;
	while(tmp2 < (64 - Xofs)) {
		LCD_Write(LCD_D_type, *ptmp|*ptmp1, LCD_Block_Left);
		ptmp++;
		ptmp1++;
		tmp2++;
		}
 	tmp2 = 0;
	while(tmp2 < (WLen - 64 + Xofs)) {
		LCD_Write(LCD_D_type, *ptmp|*ptmp1, LCD_Block_Right);
		ptmp++;
		ptmp1++;
		tmp2++;
		}
	 // Increase X address
	tmp1++;

	LCD_Write(LCD_I_type, LCD_I_Page0|tmp1, LCD_Block_Left);
	LCD_Write(LCD_I_type, LCD_I_Page0|tmp1, LCD_Block_Right);
	LCD_Write(LCD_I_type, LCD_I_Y0 + Xofs, LCD_Block_Left);
	LCD_Write(LCD_I_type, LCD_I_Y0, LCD_Block_Right);
	ptmp += (Hmax - WLen);
	
 	}

}

void	AdjVPos(U8 Dir)
{
 if(Dir == UP) {
 	// -- Increase Vpos
 	MyDso.VPos += KeyParam;
 	}
 else {
 	// -- Decrease Vpos
	MyDso.VPos -= KeyParam;
 	}
 if(MyDso.VPos> VPosMax) {
 	MyDso.VPos = VPosMax;
 	}
 if(MyDso.VPos < VPosMin) {
 	MyDso.VPos = VPosMin;
 	}

 EE_Write((U8*)&MyDso_EE.VPos, MyDso.VPos);

 UpdateVPos();
 UpdateTLvl();
}

void	AdjTimeBase(U8 Dir)
{
 if(Dir == UP) {
 	// -- Increase TimeBase rate
 	if(++MyDso.TimeBase > TB_01ms) {
 		MyDso.TimeBase = TB_01ms;
 		}
 	}
 else {
 	// -- Decrease TimeBase rate
 	if(--MyDso.TimeBase < TB_20ms) {
 		MyDso.TimeBase = TB_20ms;
 		}	
 	}
 
 EE_Write((U8*)&MyDso_EE.TimeBase, MyDso.TimeBase);
 
 Hightlight(Focus);

 UpdateTimeBase();
 
 Hightlight(Focus);

 BitClr(TIMSK , (1<<TICIE1));

 if(!BitTest(MyDso.Status, (1<<Hold))) {
 	MyDso.Status = 0;
 	BitSet(MyDso.Status, (1<<DoCapture));
 	}
 
 PutBox(105, 23, 23, 9);
 }

void	AdjHPos(U8 Dir)
{
 if(Dir == UP) {
	MyDso.HPos += KeyParam;
 	// -- Ensure HPosition not too close to the WB end
 	if(MyDso.HPos >= (WBsize - WDsize)) {
 		MyDso.HPos = (WBsize - WDsize);
 		}
 	}
 else {
 	// -- Ensure HPosition not too close to the WB start 
 	if(MyDso.HPos <= KeyParam) {
 		MyDso.HPos = 0;
 		}
 	else {
		MyDso.HPos -= KeyParam;
 		}
 	}

 EE_Write((U8*)&MyDso_EE.HPos, MyDso.HPos);

 UpdateHPos();
 PutBox(24, 61, 52, 3);

 if(BitTest(MyDso.Status, (1<<Hold))) {
 	// Shift waveform. 
	ShowWave();
 	wDisplay();
 	}
 
}

void	AdjTrigMode(U8 Dir)
{
 if(Dir == UP) {
 	if(++MyDso.TrigMode > TM_Single) {
		MyDso.TrigMode = TM_Single;
		}
 	}
 else {
 	if(--MyDso.TrigMode < TM_Auto) {
		MyDso.TrigMode = TM_Auto;
 		}
 	}

 EE_Write((U8*)&MyDso_EE.TrigMode, MyDso.TrigMode);

 Hightlight(Focus);

 UpdateTrigMode();

 Hightlight(Focus);

 PutBox(105, 43, 23, 9);
}

void	AdjTrigEdge(U8 Dir)
{
 Dir = Dir;

 MyDso.TrigEdge ^= 0x01;

 EE_Write((U8*)&MyDso_EE.TrigEdge, MyDso.TrigEdge);

 Hightlight(Focus);

 UpdateTrigEdge();
 
 Hightlight(Focus);
 
 PutBox(105, 52, 11, 9);
 
}

void	AdjTrigLvl(U8 Dir)
{ 
 S16 tmp0;
 
 tmp0 = MyDso.TrigLvl;
 
 if(Dir == UP) {
 	// -- Increase trig level
 	tmp0+= KeyParam;
 	if(tmp0 > TrigLvlMax) {
 		tmp0 = TrigLvlMax;
 		}
 	}
 else {
	tmp0  -= KeyParam; 	
 	if(tmp0 < TrigLvlMin) {
 		tmp0 = TrigLvlMin;
 		}
 	}

 MyDso.TrigLvl = tmp0;
 EE_Write((U8*)&MyDso_EE.TrigLvl, MyDso.TrigLvl);
 EE_Write((U8*)&MyDso_EE.TrigLvl + 1, MyDso.TrigLvl>> 8);

 UpdateTLvl();
}

void	DoCpl(void)
{
 MyDso.Cpl = KeyParam;
 UpdateCpl();
}

void	DoVsen(void)
{
 MyDso.Vsen = KeyParam;
 UpdateVSen();
 
}

void	UpdateVSen(void)
{
 Puts_ROM(VSenx, VSeny, (U8 *)(VsenString + MyDso.Vsen), 5);

 PutBox(106, 15, 21, 21);
 }

void	UpdateTimeBase(void)
{
 Puts_ROM(
 	pgm_read_byte((U8 *)IndicatorLoc + FC_TBRate*2),
 	pgm_read_byte((U8 *)IndicatorLoc + FC_TBRate*2 + 1),
 	(U8 *)(TBString + MyDso.TimeBase),
 	5);

}

void	UpdateTrigMode(void)
{
 Puts_ROM(
 	pgm_read_byte((U8 *)IndicatorLoc + FC_TrigMode*2),
 	pgm_read_byte((U8 *)IndicatorLoc + FC_TrigMode*2 + 1),
 	(U8 *)(TMString + MyDso.TrigMode - TM_Auto),
 	5);

}

void	UpdateTrigEdge(void)
{
 Put_9x7(
 	pgm_read_byte((U8 *)IndicatorLoc + FC_TrigEdge*2),
 	pgm_read_byte((U8 *)IndicatorLoc + FC_TrigEdge*2 + 1),
 	MyDso.TrigEdge ? RisingEdge : FallingEdge);
 	
}


void	UpdateVPos(void)
{
 U8	tmp0;
 
 // Clear previous mark
 Triangle(MarkVPos, Lmax - PrevVPos, 1);
 
 // Put current mark
 tmp0 = MyDso.VPos + 128;
 if(tmp0 > Lmax) {
 	tmp0 = Lmax;
 	}
 else if(tmp0 < Lmin) {
 	tmp0 = Lmin;
 	}

 Triangle(MarkVPos, Lmax - tmp0, 0);

 PrevVPos = tmp0;

 PutBox(0, 0, 2, 61);
 
}

void	UpdateTLvl(void)
{
 S16 tmp0;
 
 // Clear previous mark
 Triangle(MarkTLvl, Lmax - PrevTLvl, 1);
 // Put current mark
 tmp0 = MyDso.TrigLvl + MyDso.VPos;
 if(tmp0 > Lmax) {
 	tmp0 = Lmax;
 	}
 else if(tmp0 < Lmin) {
 	tmp0 = Lmin;
 	}
 Triangle(MarkTLvl, Lmax - tmp0, 0);
 PutBox(103, 0, 2, 61);

 PrevTLvl = tmp0;

 SetTLvl();

}


void	UpdateHPos(void)
{
 // Clear previous mark
 rHLine(HPStart + (PrevHPos/5) + Xofs, 62, 20);

 
 // Put current mark
 HLine(HPStart + (MyDso.HPos/5) + Xofs, 62, 20);

 PrevHPos = MyDso.HPos;
 
}

void	UpdateHold(void)
{
 if(BitTest(MyDso.Status, (1<<Hold))) {
 	Puts_ROM(HOLDx, HOLDy, Str_HOLD, 5);
 	}
 else {
 	Puts_ROM(HOLDx, HOLDy, Str_Space4, 5);
 	}
 
 PutBox(HOLDx, HOLDy, 23, 8);
 
}

void	UpdateCpl(void)
{

 switch(MyDso.Cpl) {
 	case CP_AC: 
 		// First digit
 		PutChar5x5(CPLx, CPLy, 0);
 		// Second digit
 		PutChar5x5(CPLx + 6, CPLy, 1);
 		// Third digit
 		PutChar5x5(CPLx + 12, CPLy, 5);
		break;

 	case CP_DC: 
 		// First digit
 		PutChar5x5(CPLx, CPLy, 2);
 		// Second digit
 		PutChar5x5(CPLx + 6, CPLy, 1);
 		// Third digit
 		PutChar5x5(CPLx + 12, CPLy, 5);
		break;
		
 	case CP_GND: 
 		// First digit
 		PutChar5x5(CPLx, CPLy, 3);
 		// Second digit
 		PutChar5x5(CPLx + 6, CPLy, 4);
 		// Third digit
 		PutChar5x5(CPLx + 12, CPLy, 2);
		break;

	default:
		break;
 	}

 PutBox(106, 9, 17, 13);
 }

void	SetTLvl(void)
{
 U16 tmp;

 tmp = MyDso.TrigLvl;
 tmp = (tmp<<1) + tmp;
 tmp >>= 3;
 tmp += 32;
 
 OCR0 = tmp;
}

// =======================================================
//	waveforms are displayed after a full buffer sampling had being	done.
//
void	StartCapture(void)
{
 U8 tmp0;

 // Prepare for flash capture
 PrepareCapture();

 // Setup Timer 1 for sampling. Sample is done at each interrupt of Timer 1
 // Start Timer 1 at CTC mode and enable its interrupt [WGM1[3:0] = 0b1100]
 TCCR1A = (0<<COM1A1)|(0<<COM1A0) |(0<<COM1B1) |(0<<COM1B0) 
 		|(0<<COM1C1) |(0<<COM1C0) |(0<<WGM11) |(0<<WGM10);
 TCCR1B = (0<<ICNC1)|(0<<ICES1)|(1<<WGM13)|(1<<WGM12)
 		|(0<<CS12)|(0<<CS11)|(1<<CS10);
 BitClr(ACSR, (1<<ACIC));
 // Load timing constant based on timebase.
 ICR1 = pgm_read_word(&TBconstants[MyDso.TimeBase]);

 // Warm up ADC to clear samples already inside ADC pipeline
 tmp0 = 0;
 while(tmp0 < 8) {
 	BitClr(ADC_CLK_Port, (1 << ADC_CLK_Bit));
	BitSet(ADC_CLK_Port, (1 << ADC_CLK_Bit));
	tmp0++;
 	}
 
 BitSet(TIFR , (1<<ICF1));		// Reset pending interrupt flag if any
 BitSet(TIMSK , (1<<TICIE1));	// Input capture interrupt is used
							// because ICR was chosen as TOP
 }

void	WaveformProc(void)
{
 // Valid capture
 if(BitTest(MyDso.Status, (1<<Trigged))) {
 	BitClr(MyDso.Status, (1<<Trigged));	

	MyDso.SampleStart = (U8 *)(MyDso.TrigAddress -WBsize/2);
	if(MyDso.SampleStart < SampleBuf1) {
		MyDso.SampleStart += WBsize;
		}

	// Enter HOLD state if in single mode
	if(MyDso.TrigMode == TM_Single) {
		BitSet(MyDso.Status, (1<<EnterHold));
		}
	}
 else {
	MyDso.SampleStart = SampleBuf1;
	}

 // Display waveform
 ShowWave();

 BitSet(MyDso.Status, (1<<DoCapture));
 
}

void	PrepareCapture(void)
{
 // Sample buffer
 MyDso.SampleStart = SampleBuf1;
 BitSet(MyDso.Status, (1<<DummyRead));
 SampleCount = WBsize/2;

 // ---- Select effective trig edge -----
 // Trig is detected by checking at the ACI bit in ACSR register.
 //	ACIS[1:0] selects the edge that set ACI bit.
 BitClr(ACSR, (1<<ACIS1)|(1<<ACIS0));	// Reset both bits to 0 first
 if(MyDso.TrigEdge == TE_Falling) {
 	// Note that signal is applied to IN- input of comparator
 	//	So a falling edge cause a rising edge at the comparator
 	BitSet(ACSR, (1<<ACIS1)|(1<<ACIS0));
 	}
 else {
 	BitSet(ACSR, (1<<ACIS1));
 	}

}

//  ISR
ISR(SIG_INPUT_CAPTURE1)
{
 BitClr(PORTB, (1<<PB1));		// LED on

 // Do ADC to get one sample
 BitClr(ADC_CLK_Port, (1 << ADC_CLK_Bit));
 
 *MyDso.SampleStart = ADC_DataIn;

 BitSet(ADC_CLK_Port, (1 << ADC_CLK_Bit));

 if(!BitTest(MyDso.Status, (1 << Trigged))) {
	 // Check for trig
	 if(BitTest(ACSR, (1 << ACI))) {
		if(!BitTest(MyDso.Status, (1 << DummyRead))) {
			BitSet(MyDso.Status, (1 << Trigged));
			MyDso.TrigAddress = (U16)MyDso.SampleStart;
			}
	 	}
 	}
 else {
	if(SampleCount == 0) {
		// Capture done
		BitSet(MyDso.Status, (1 << CaptureDone));
 		BitClr(TIMSK , (1<<TICIE1));	
		return;
		}
	else {
		SampleCount--;
		}
 	}

 if(++MyDso.SampleStart >= SampleBuf1 + 256) {
 	MyDso.SampleStart = SampleBuf1;
	if(BitTest(MyDso.Status, (1 << DummyRead))) {
		BitClr(MyDso.Status, (1 << DummyRead));
		BitSet(ACSR, (1 << ACI));
		}
	else {
		if(MyDso.TrigMode == TM_Auto) {
			// Capture done
			BitSet(MyDso.Status, (1 << CaptureDone));
			BitClr(TIMSK , (1<<TICIE1));	
			}
		}
 	}

 BitSet(PORTB, (1<<PB1));	// LED off

}

void LoadEEprom(void)
{
 U8	tmp,  tmp1, tmp2;

  // Looking for EEPROM signature
 tmp = EE_Read((U8 *)&Signature_EE1);
 tmp1 = EE_Read((U8 *)&Signature_EE2);
 tmp2 = EE_Read((U8 *)&EEversion);
 
 if((tmp == EE_signature1) && (tmp1 == EE_signature2) && (tmp2 == EepromVersion)) {
 	// Load setting from EEPROM
 	tmp = 0;
	while(tmp < sizeof(DSO)) {
		*((U8*)&MyDso + tmp) = EE_Read((U8 *)&MyDso_EE + tmp);
		tmp++;
		}

 	}
 else {
 	EepromReset();
 	}
}


void	EepromReset(void)
{
 U8	tmp;
 
 // Load setting from Flash
 tmp = 0;
 while(tmp < sizeof(DSO)) {
	*((U8*)&MyDso + tmp) 
		= pgm_read_byte((U8 *)&MyDso_ROM + tmp);
	tmp++;
	}

 // Setup EEPROM
 tmp = 0;
 while(tmp < sizeof(DSO)) {
	EE_Write((U8*)&MyDso_EE + tmp, *((U8*)&MyDso + tmp));
	tmp++;
	}

 // Write signature & version
 EE_Write((U8*)&Signature_EE1, EE_signature1);
 EE_Write((U8*)&Signature_EE2, EE_signature2);
 EE_Write((U8*)&EEversion, EepromVersion);

}



