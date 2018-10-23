//////////////////////////////////////////////////////////////////////////////
//
// 	Filename:	Board.c
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
//-----------------------------------------------------------------------------
// 	Includes
//-----------------------------------------------------------------------------

#include <inttypes.h>
#include <avr/io.h>
#include	"avr/wdt.h"
#include	"avr/Pgmspace.h"
#include <avr/interrupt.h>
#include "Common.h"
#include "Board.h"
#include "Command.h"

// ===========================================================
//	File Scope Variables
// ===========================================================
//
// ===========================================================
//	Function Definitions
// ===========================================================

//-----------------------------------------------------------------------------
// Misc_Init
//-----------------------------------------------------------------------------
#define	IVSEL	1
#define	IVCE	0

void	Misc_Init(void)
{
// MCU Control Register - MCUCR
//	PUD:	1 = Disable port pull-up globally, 0 = enable port pull-up. [MCUCR.4 = 0]
//	IVSEL:	
//	IVCE:	
//
 MCUCR = (0<<PUD)|(0<<IVSEL)|(0<<IVCE);		

// Clock Prescaler Register - CLKPR
//	CLKPCE:		1 = Clock prescaler change enable. CLKPS bits must be updated within 4 cycles
//				[CLKPR.8 (0)]
//	CLKPS[3:0]:	Clock prescaler select bits [Default value determined by CKDIV8 fuse]
//					0000 = 1
//					0001 = 1/2
//					0010 = 1/4
//					0011 = 1/8
//					0100 = 1/16
//					0101 = 1/32
//					0110 = 1/64
//					0111 = 1/128
//					1000 = 1/256
//					1001-1111 = Reserved
 CLKPR = (1<<CLKPCE)|(0<<CLKPS3)|(0<<CLKPS2)|(0<<CLKPS1)|(0<<CLKPS0);
 CLKPR = (0<<CLKPCE)|(0<<CLKPS3)|(0<<CLKPS2)|(0<<CLKPS1)|(0<<CLKPS0);

 // TSM: 			Timer/Counter Synchronization Mode
 // PSRASY:		Prescaler Reset Timer/Counter2
 // PSRSYNC: 	Prescaler Reset
 GTCCR = (0<<TSM)|(0<<PSRASY)|(0<<PSRSYNC);

}

//-----------------------------------------------------------------------------
// PORT_Init
//-----------------------------------------------------------------------------
//
// This routine configures the crossbar and GPIO ports.
//
void Port_Init(void)
{

//  Note: pull-up function requires PUD in SFIOR (SFIOR.2) to be zero (default value).
//		(See Misc_Init())
//
//  PORTx (input):  1=pull-up on, 0=pull-up off
//  PORTx (output): 1=output high (source), 0=output low (sink)
//  DDRx: 1=output, 0=input

//  PORTx and DDRx defaults are all bits zero.

 // PB7: 	Input, rotary encoder terminal B
 // PB6: 	Input, rotary encoder terminal A
 // PB5: 	
 // PB4: 	
 // PB3: 
 // PB2:  Output, Test signal
 // PB1: 	Output, LCD backlight
 // PB0:	Output, PWROFF
 PORTB = 
 	(1<<PB7)|(1<<PB6)|(0<<PB5)|(0<<PB4)|(0<<PB3)|(1<<PB2)|(0<<PB1)|(1<<PB0);
 DDRB = 
 	  (0<<DDB7)|(0<<DDB6)|(1<<DDB5)|(0<<DDB4)
 	|(0<<DDB3)|(1<<DDB2)|(0<<DDB1)|(1<<DDB0);

 // PC6:	
 // PC5:	Input, VSENSEL2
 // PC4:	Input, VSENSEL1
 // PC3:	Input, keypay row 3
 // PC2:	Input, keypad row 2
 // PC1:	Input, keypad row 1
 // PC0:	Input, keypad row 0
 PORTC = 
 	(1<<PC6)|(0<<PC5)|(0<<PC4)|(1<<PC3)|(1<<PC2)|(1<<PC1)|(1<<PC0);
 DDRC = 
 	    (0<<DDC6)|(0<<DDC5)|(0<<DDC4)
 	  |(0<<DDC3)|(0<<DDC2)|(0<<DDC1)|(0<<DDC0);

 // PD7: 	Output, keypad column 1
 // PD6: 	Output, keypad column 0
 // PD5: 	Input, PWR switch
 // PD4: 	
 // PD3: 	
 // PD2: 	
 // PD1:	Output, Uart TXD (output keyscan data)
 // PD0: Input, Uart RXD (receive commands from master)
 PORTD = (1<<PD7)|(1<<PD6)|(1<<PD5)|(0<<PD4)
 		|(0<<PD3)|(0<<PD2)|(1<<PD1)|(1<<PD0);
 DDRD = 
 	  (1<<DDD7)|(1<<DDD6)|(0<<DDD5)|(0<<DDD4)
 	|(0<<DDD3)|(0<<DDD2)|(0<<DDD1)|(0<<DDD0);
}

/*
//-----------------------------------------------------------------------------
// Comp_Init
//-----------------------------------------------------------------------------
//

void Comp_Init(void)
{
}

*/

//-----------------------------------------------------------------------------
// Timer0_Init
//-----------------------------------------------------------------------------
//
void Timer0_Init(void)
{
 // COM0A[1:0]:	Compara Match Output A mode
 //				Non-PWM Mode:
 //					00 = Normal port operation, OC0A disconnected
 //					01 = Toggle OC0A on Compare Match
 //					10 = Clear OC0A on Compare Match
 //					11 = Set OC0A on Compare Match
 //				Fast PWM Mode:
 //					00 = Normal port operation, OC0A disconnected.
 //					01 = WGM02 = 0: Normal Port Operation, OC0A Disconnected
 //						 WGM02 = 1: Toggle OC0A on Compare Match
 //					10 = Clear OC0A on Compare Match, set OC0A at TOP
 //					11 = Set OC0A on Compare Match, clear OC0A at TOP
 //				Phase Correct PWM Mode:
 //					00 = Normal port operation, OC0A disconnected
 //					01 = WGM02 = 0: Normal Port Operation, OC0A Disconnected
 //						 WGM02 = 1: Toggle OC0A on Compare Match.
 //					10 = Clear OC0A on Compare Match when up-counting. Set OC0A on
 //						 Compare Match when down-counting.
 //					11 = Set OC0A on Compare Match when up-counting. Clear OC0A on
 //						 Compare Match when down-counting.
 // COM0B[1:0]:	Compara Match Output B mode
 //				Non-PWM Mode:
 //					00 = Normal port operation, OC0B disconnected
 //					01 = Toggle OC0B on Compare Match
 //					10 = Clear OC0B on Compare Match
 //					11 = Set OC0B on Compare Match
 //				Fast PWM Mode:
 //					00 = Normal port operation, OC0B disconnected.
 //					01 = Reserved
 //					10 = Clear OC0B on Compare Match, set OC0B at TOP
 //					11 = Set OC0B on Compare Match, clear OC0B at TOP
 //				Phase Correct PWM Mode:
 //					00 = Normal port operation, OC0B disconnected
 //					01 = Reserved
 //					10 = Clear OC0B on Compare Match when up-counting. Set OC0B on
 //						 Compare Match when down-counting.
 //					11 = Set OC0B on Compare Match when up-counting. Clear OC0B on
 //						 Compare Match when down-counting.
 // WGM0[1:0]:	Waveform Generation Mode
 //				Mode	WGM0[2:0]	T/C Mode  	TOP  	Update_OCRx		TOV0_Set
 //				0		000			Normal		0xFF		Immidiate		Max
 //				1		001			PWM, PC		0xFF		TOP				BOTTOM
 //				2		010			CTC			OCRA	Immidiate		MAX
 //				3		011			Fast PWM		0xFF		TOP				MAX
 //				4		100			Reserved		-		-				-
 //				5		101			PWM	, PC		OCRA	TOP				BOTTOM
 //				6		110			Reserved		-		-				-
 //				7		111			Fast PWM		OCRA	TOP				TOP
 //				(MAX = 0xFF, BOTTOM = 0x00)
 //
 // -- Set Timer0 to CTC mode
 TCCR0A = (0<<COM0A1)|(0<<COM0A0)|(0<<COM0B1)|(0<<COM0B0)
 		|(1<<WGM01)|(0<<WGM00);

 // FOC0A:		Force Output Compare A
 // FOC0B:		Force Output Compare B
 // WGM02: 		Waveform Generation Mode
 // CS0[2:0]: 	Clock Select
 //					000 = No clock source (Timer/Counter stopped)
 //					001 = clkio/1 (No prescaling)
 //					010 = clkio/8 (From prescaler)
 //					011 = clkio/64 (From prescaler)
 //					100 = clkio/256 (From prescaler)
 //					101 = clkio/1024 (From prescaler)
 //					110 = External clock source on T0 pin. Clock on falling edge
 //					111 = External clock source on T0 pin. Clock on rising edge
 //
 // -- 1/8 prescaler. 1us clock @8M osc
 TCCR0B = (0<<FOC0A)|(0<<FOC0B)|(0<<WGM02)|(0<<CS02)|(1<<CS01)|(0<<CS00);

 TCNT0 = 0x00;
 OCR0A = 100;  // 100us for keypad scanning
 OCR0B = 0x00;

 // OCIE0B: 	Timer/Counter0 Output Compare Match B Interrupt Enable
 // OCIE0A: 	Timer/Counter0 Output Compare Match A Interrupt Enable
 // TOIE0: 	Timer/Counter0 Overflow Interrupt Enable
 //
 // -- Enable TOV interrupt
 TIMSK0 = (0<<OCIE0B)|(1<<OCIE0A)|(0<<TOIE0);

 // OCF0B: 	Timer/Counter 0 Output Compare B Match Flag
 // OCF0A: 	Timer/Counter 0 Output Compare A Match Flag
 // TOV0: 	Timer/Counter0 Overflow Flag
 TIFR0 = (1<<OCF0B)|(1<<OCF0A)|(1<<TOV0);

}

//-----------------------------------------------------------------------------
// Timer1_Init
//-----------------------------------------------------------------------------
//
void Timer1_Init (void)
{
 // COM1A[1:0]:	Compara Match Output A mode
 // COM1B[1:0]:	Compara Match Output B mode
 //				Non-PWM Mode:
 //					00 = Normal port operation, OC1A/OC1B disconnected
 //					01 = Toggle OC1A/OC1B on Compare Match
 //					10 = Clear OC1A/OC1B on Compare Match (Set output to low level).
 //					11 = Set OC1A/OC1B on Compare Match (Set output to high level).
 //				Fast PWM Mode:
 //					00 = Normal port operation, OC1A/OC1B disconnected
 //					01 = WGM13:0 = 14 or 15: Toggle OC1A on Compare Match, 
 //						 OC1B disconnected (normal port operation). For all other 
 //						 WGM1 settings, normal port operation, OC1A/OC1B disconnected.
 //					10 = Clear OC1A/OC1B on Compare Match, set OC1A/OC1B at TOP
 //					11 = Set OC1A/OC1B on Compare Match, clear OC1A/OC1B at TOP
 //				Phase Correct PWM Mode:
 //					00 = Normal port operation, OC1A/OC1B disconnected
 //					01 = WGM13:0 = 8, 9, 10 or 11: Toggle OC1A on Compare Match,
 //						 OC1B disconnected (normal port operation). For all other 
 //						 WGM1 settings, normal port operation, OC1A/OC1B disconnected.
 //					10 = Clear OC1A/OC1B on Compare Match when upcounting.
 //						 Set OC1A/OC1B on Compare Match when downcounting.
 //					11 = Set OC1A/OC1B on Compare Match when upcounting.
 //						 Clear OC1A/OC1B on Compare Match when downcounting.
 // WGM1[1:0]:	Waveform Generation Mode
 //				Mode	WGM1[3:0]	T/C Mode  	TOP  	Update_OCRx		TOV1_Set
 //				0		0000		Normal		0xFFFF	Immidiate		Max
 //				1		0001		PWM, PC8	0x00FF	TOP				BOTTOM
 //				2		0010		PWM, PC9	0x01FF	TOP				BOTTOM
 //				3		0011		PWM, PC10	0x03FF	TOP				BOTTOM
 //				4		0100		CTC			OCR1A	Immediate 		MAX	
 //				5		0101		Fast PWM 8	0x00FF	TOP				TOP
 //				6		0110		Fast PWM 9	0x01FF	TOP				TOP
 //				7		0111		Fast PWM 10	0x03FF	TOP				TOP
 //				8		1000		PWM, PFC	ICR1	BOTTOM			BOTTOM
 //				9		1001		PWM, PFC	OCR1A	BOTTOM			BOTTOM
 //				10		1010		PWM, PC		ICR1	TOP				BOTTOM
 //				11		1011		PWM, PC		OCR1A	TOP				BOTTOM
 //				12		1100		CTC			ICR1	Immediate 		MAX	
 //				13		1101		Reserved		-		-				-
 //				14		1110		Fast PWM		ICR1	TOP				TOP
 //				15		1111		Fast PWM		OCR1A	TOP				TOP
//				
// -- WGM1 = 1100 for CTC with ICR1
// -- Select 1/8 prescaler for 1us basic clock 
// -- Set ICR1 to 500 - 1 for 1KHz signal with 50% duty cycle
//
TCCR1A = (0<<COM1A1)|(0<<COM1A0)|(0<<COM1B1)|(1<<COM1B0)
 		|(0<<WGM11)|(0<<WGM10);

 // ICNC1: 		Input Capture Noise Canceler
 // ICES1: 		Input Capture Edge Select, 0 = falling edge, 1 = rising edge
 // WGM1[3:2]:	Waveform Generation Mode
 // CS1[2:0]: 	Clock Select
 //					000 = No clock source (Timer/Counter stopped)
 //					001 = clkio/1 (No prescaling)
 //					010 = clkio/8 (From prescaler)
 //					011 = clkio/64 (From prescaler)
 //					100 = clkio/256 (From prescaler)
 //					101 = clkio/1024 (From prescaler)
 //					110 = External clock source on T1 pin. Clock on falling edge
 //					111 = External clock source on T1 pin. Clock on rising edge
 //
 TCCR1B = (0<<ICNC1)|(0<<ICES1)|(1<<WGM13)|(1<<WGM12)
 		|(0<<CS12)|(1<<CS11)|(0<<CS10);

 // FOC1A: 	Force Output Compare for Channel A
 // FOC1B: 	Force Output Compare for Channel B
 TCCR1C = (0<<FOC1A)|(0<<FOC1B);

 TCNT1H = 0x00;
 TCNT1L = 0x00;

 OCR1A = 500 - 1;

// OCR1BH = 0 >> 8;
// OCR1BL = 0xFE & 0x00ff;
 OCR1A = 500 - 1;

// ICR1H = 0xFF;
// ICR1L = 0xFF;
 ICR1 = 500 - 1;

 // ICIE1: 	Timer/Counter1 Input Capture Interrupt Enable
 // OCIE1B: 	Timer/Counter1 Output Compare Match B Interrupt Enable
 // OCIE1A: 	Timer/Counter1 Output Compare Match A Interrupt Enable
 // TOIE1: 	Timer/Counter1 Overflow Interrupt Enable
 //
 TIMSK1 = (0<<ICIE1)|(0<<OCIE1B)|(0<<OCIE1A)|(0<<TOIE1);

 // ICF1: 	Timer/Counter1, Input Capture Flag
 // OCF1B: 	Timer/Counter 1 Output Compare B Match Flag
 // OCF1A: 	Timer/Counter 1 Output Compare A Match Flag
 // TOV1: 	Timer/Counter1 Overflow Flag
 TIFR1 = (1<<ICF1)|(1<<OCF1B)|(1<<OCF1A)|(1<<TOV1);
 
}




//-----------------------------------------------------------------------------
// Timer2_Init
//-----------------------------------------------------------------------------
//
void Timer2_Init (void)
{
 // COM2A[1:0]:	Compara Match Output A mode
 //				Non-PWM Mode:
 //					00 = Normal port operation, OC2A disconnected
 //					01 = Toggle OC2A on Compare Match
 //					10 = Clear OC2A on Compare Match
 //					11 = Set OC2A on Compare Match
 //				Fast PWM Mode:
 //					00 = Normal port operation, OC2A disconnected.
 //					01 = WGM22 = 0: Normal Port Operation, OC2A Disconnected
 //						 WGM22 = 1: Toggle OC2A on Compare Match
 //					10 = Clear OC2A on Compare Match, set OC2A at TOP
 //					11 = Set OC2A on Compare Match, clear OC2A at TOP
 //				Phase Correct PWM Mode:
 //					00 = Normal port operation, OC2A disconnected
 //					01 = WGM22 = 0: Normal Port Operation, OC2A Disconnected
 //						 WGM22 = 1: Toggle OC0A on Compare Match.
 //					10 = Clear OC2A on Compare Match when up-counting. Set OC2A on
 //						 Compare Match when down-counting.
 //					11 = Set OC2A on Compare Match when up-counting. Clear OC2A on
 //						 Compare Match when down-counting.
 // COM2B[1:0]:	Compara Match Output B mode
 //				Non-PWM Mode:
 //					00 = Normal port operation, OC2B disconnected
 //					01 = Toggle OC2B on Compare Match
 //					10 = Clear OC2B on Compare Match
 //					11 = Set OC2B on Compare Match
 //				Fast PWM Mode:
 //					00 = Normal port operation, OC2B disconnected.
 //					01 = Reserved
 //					10 = Clear OC2B on Compare Match, set OC2B at TOP
 //					11 = Set OC2B on Compare Match, clear OC2B at TOP
 //				Phase Correct PWM Mode:
 //					00 = Normal port operation, OC2B disconnected
 //					01 = Reserved
 //					10 = Clear OC2B on Compare Match when up-counting. Set OC2B on
 //						 Compare Match when down-counting.
 //					11 = Set OC2B on Compare Match when up-counting. Clear OC2B on
 //						 Compare Match when down-counting.
 // WGM2[1:0]:	Waveform Generation Mode
 //				Mode	WGM2[2:0]	T/C Mode  	TOP  	Update_OCRx		TOV0_Set
 //				0		000			Normal		0xFF		Immidiate		Max
 //				1		001			PWM, PC		0xFF		TOP				BOTTOM
 //				2		010			CTC			OCRA	Immidiate		MAX
 //				3		011			Fast PWM		0xFF		TOP				MAX
 //				4		100			Reserved		-		-				-
 //				5		101			PWM	, PC		OCRA	TOP				BOTTOM
 //				6		110			Reserved		-		-				-
 //				7		111			Fast PWM		OCRA	TOP				TOP
 //				(MAX = 0xFF, BOTTOM = 0x00)
 //
 TCCR2A = (0<<COM2A1)|(0<<COM2A0)|(0<<COM2B1)|(0<<COM2B0)
 		|(0<<WGM21)|(0<<WGM20);

 // FOC2A:		Force Output Compare A
 // FOC2B:		Force Output Compare B
 // WGM22: 		Waveform Generation Mode
 // CS2[2:0]: 	Clock Select
 //					000 = No clock source (Timer/Counter stopped)
 //					001 = clkT2s/1 (No prescaling)
 //					010 = clkT2s/8 (From prescaler)
 //					011 = clkT2s/32 (From prescaler)
 //					100 = clkT2s/64 (From prescaler)
 //					101 = clkT2s/128 (From prescaler)
 //					110 = clkT2s/256 (From prescaler)
 //					111 = clkT2s/1024 (From prescaler)
 //
 // -- Clock: 1/1 
 TCCR2B = (0<<FOC2A)|(0<<FOC2B)|(0<<WGM22)|(0<<CS22)|(0<<CS21)|(1<<CS20);

 TCNT2 = 0x00;
 OCR2A = 0x00;
 OCR2B = 0x00;

 // OCIE2B: 	Timer/Counter2 Output Compare Match B Interrupt Enable
 // OCIE2A: 	Timer/Counter2 Output Compare Match A Interrupt Enable
 // TOIE2: 	Timer/Counter2 Overflow Interrupt Enable
 //
 TIMSK2 = (0<<OCIE2B)|(0<<OCIE2A)|(0<<TOIE2);

 // OCF2B: 	Timer/Counter 2 Output Compare B Match Flag
 // OCF2A: 	Timer/Counter 2 Output Compare A Match Flag
 // TOV2: 	Timer/Counter 2 Overflow Flag
 TIFR2 = (1<<OCF2B)|(1<<OCF2A)|(1<<TOV2);

 // EXCLK: 	Enable External Clock Input, 1 = enable external clock in Asynchronous Mode
 // AS2: 		Asynchronous Timer/Counter2, 0 - T2 clocked with clkio, 1 = ext. clock (TOSC1)
 ASSR = (0<<EXCLK)|(0<<AS2);
 
}

//-----------------------------------------------------------------------------
// ADC Initialization
//-----------------------------------------------------------------------------
//
void ADC_Init (void)
{
 // REFS[1:0]:	Reference Selections [ADMUX.[7:6] = 00]
 //					00 = AREF, internal Vref turned off
 //					01 = AVcc with external cap at AREF pin
 //					10 = Reserved
 //					11 = Internal 1.1V Vref with external cap at AREF pin
 // ADLAR:		ADC Left Adjust Result, 1 = Left adjusted [ADMUX.5 = 0]
 // ADMUX.4:		Reserved
 // MUX[3:0]:		Analog channel selection [ADMUX.[3:0] = 0000]
 //					0000 = ADC0
 //					0001 = ADC1
 //					0010 = ADC2
 //					0011 = ADC3
 //					0100 = ADC4
 //					0101 = ADC5
 //					0110 = ADC6
 //					0111 = ADC7
 //					1000 - 1101 = Reserved
 //					1110 = 1.1V (internal reference)
 //					1111 = 0V (GND)
 //
 // -- Use AVcc as reference
 // -- Select channel ADC7
 ADMUX = (0<<REFS1)|(1<<REFS0)|(1<<ADLAR)
 		|(0<<MUX3)|(1<<MUX2)|(1<<MUX1)|(1<<MUX0);

 // ADEN:	ADC Enable, 1 = enable ADC [ADCSRA.7 = 0]
 // ADSC:	ADC Start Conversion, 1 = to start conversion [ADCSRA.6 = 0]
 // ADATE:	ADC Auto Trig Enable, 1 = auto trigged by selected source [ADCSRA.5 = 0]
 // ADIF:		ADC	 interrupt flag, 1 = ADC completed [ADCSRA.4 = 0]
 // ADIE:	ADC interrupt enable, 1 = enable ADC interrupt [ADCSRA.3 = 0]
 // ADPS[2:0]: ADC Prescaler Selection [ADCSRA.[2:0] = 000]
 //				000 = 1/2 of system clock
 //				001 = 1/2
 //				010 = 1/4
 //				011 = 1/8
 //				100 = 1/16
 //				101 = 1/32
 //				110 = 1/64
 //				111 = 1/128
 // 
 // -- Enable ADC
 // -- Set ADC clock to 1/32 of F_CPU, about 8000000/32 = 250KHz
 ADCSRA = (1<<ADEN)|(0<<ADSC)|(0<<ADATE)|(1<<ADIF)
 		|(0<<ADIE)|(1<<ADPS2)|(0<<ADPS1)|(1<<ADPS0);

 // ACME:
 // ADTS[2:0]: ADC Auto Trig Source select [ADCSRB.[2:0] = 000]
 //				000 = Free running
 //				001 = Analog comparator
 //				010 = External interrupt 0
 //				011 = Timer/Counter 0 compare match A
 //				100 = Timer/Counter 0 overflow
 //				101 = Timer/Counter 1 compare match B
 //				110 = Timer/Counter 1 overflow
 //				111 = Timer/Counter 1 capture event
 
 ADCSRB= (0<<ACME)|(0<<ADTS2)|(0<<ADTS1)|(0<<ADTS0);

 // ADC[5:0]D: 	Digital Input Disable [ADC[5:0]D = 000000]
 DIDR0 = (1<<ADC5D)|(1<<ADC4D)|(0<<ADC3D)|(0<<ADC2D)
 		|(0<<ADC1D)|(0<<ADC0D);
 
}

// -- Do ADC on selected channel and return the result
U8	ADC_Poll(U8 channel)
{
 ADMUX &= 0xF0;
 ADMUX |= channel;
 Delay(50);
 
 // -- Start ADC
 BitSet(ADCSRA, (1<<ADSC));
 while(!BitTest(ADCSRA, (1<<ADIF))) {
 	// Wait for ADC to complete
 	}
 // -- ADIF has to be manually reset if not using ADC interrupt
 BitSet(ADCSRA, (1<<ADIF));
// return (ADCL|(ADCH<<8));
 return (ADCH);
}

//-----------------------------------------------------------------------------
// UART0_Init
//-----------------------------------------------------------------------------
//
void Uart0_Init (void)
{
 // UCSR0A - USART Control and Status Register A
 //	RXC0:	1 = Receive complete [UCSR0A.7]
 //	TXC0:	1 = Transmit complete [UCSR0A.6 = 0]
 //	UDRE0:	1 = USART Data Register Empty [UCSR0A.5]
 //	FE0:		1 = Frame Error [UCSR0A.4]
 //	DOR0:	1 = Data Over Run [UCSR0A.3]
 //	UPE0:	1 = USART Parity Error [UCSR0A.2]
 //	U2X0:	1 = Double USART transmission speed (for asynchronous only) [UCSR0A.1 = 0]
 //	MPCM0:	1 = Multiprocessor Communication Mode [UCSR0A.0 = 0]
 //
 // UCSR0A defaults are all bits zero.
 UCSR0A = (0<<TXC0)|(0<<U2X0)|(0<<MPCM0);

 // UCSR0B - USART Control and Status Register B
 //	RXCIE0:	1 = Receive complete interrupt enable [UCSR0B.7 = 0]
 //	TXCIE0:	1 = Transmit complete interrupt enable [UCSR0B.6 = 0]
 //	UDRIE0:	1 = USART Data Register Empty interrupt enable [UCSR0B.5 = 0]
 //	RXEN0:	1 = Receiver enable [UCSR0B.4 = 1]
 //	TXEN0:	1 = Transmitter enable [UCSR0B.3 = 1]
 //	UCSZ02:	Character size, combined with UCSZ0.[1:0] in UCSR0C [UCSR0B.2 = 0]
 //	RXB80:	1 = Receive Date Bit 8 [UCSR0B.1]
 //	TXB80:	1 = Transmit Date Bit 8 [UCSR0B.0 = 0]
 //
 // UCSR0B defaults are all bits zero.

 // -- Enable receiver
 // -- Enable transmitter
 // -- Enable receiption interrupt
 UCSR0B = (1<<RXCIE0)|(0<<TXCIE0)|(0<<UDRIE0)|(1<<RXEN0)
 			|(1<<TXEN0)|(0<<UCSZ02)|(0<<TXB80);

 // UCSR0C - USART Control and Status Register C
 //	UMSEL0.[1:0]:USART Mode Select [UCSR0C.[7:6] = 00]
 //				00 = Asynchronous
 //				01 = Synchronous
 //				10 = Reserved
 //				11 = Master SPI
 //	UPM0.[1:0]:	Parity Mode [UCSR0C.[5:4] = 00]
 //				00 = Disabled
 //				01 = Reserved
 //				10 = Enabled, even parity
 //				11 = Enabled, odd parity
 //	USBS0:	Stop bit select, 0 = 1 stop bit, 1 = 2 stop bits [UCSR0C.3 = 0]
 //	UCSZ0.[1:0]:	Character size, combined with UCSZ02 in UCSR0B [UCSR0C.[2:1] = 11] 
 //				000 = 5-bit
 //				001 = 6-bit
 //				010 = 7-bit
 //				011 = 8-bit
 //				100 = Reserved
 //				101 = Reserved
 //				110 = Reserved
 //				111 = 9-bit
 //	UCPOL0:	Clock pority, for synchronous mode only [UCSR0C.0] (See datasheet)
 //
 // -- Select 8N1 format
 UCSR0C = (0<<UMSEL01)|(0<<UMSEL00)|(0<<UPM01)|(0<<UPM00)
 		  |(0<<USBS0)|(1<<UCSZ01)|(1<<UCSZ00)|(0<<UCPOL0);

// Setup baudrate
 UBRR0H = (((F_CPU / 16) / Uart0_Baud) - 1) >> 8;
 UBRR0L = (((F_CPU / 16) / Uart0_Baud) - 1) & 0x00FF;
 
}


//-----------------------------------------------------------------------------
// Output one byte to Uart 0 - polling method
//-----------------------------------------------------------------------------
//
void	u0putc(U8 ch)
{
 while(!BitTest(UCSR0A, (1<<UDRE0))) {
 	}
 UDR0 = ch;
}



//-----------------------------------------------------------------------------
// Receive one byte from Uart 0 - polling method
//-----------------------------------------------------------------------------
//
U8	u0getc(void)
{
 while(!BitTest(UCSR0A, (1<<RXC0))) {
 	}
 return (UDR0);
}

//-----------------------------------------------------------------------------
// Receive state check
//	Return 1 if received data ready, otherwise return 0.
//-----------------------------------------------------------------------------
//
U8	u0rcheck(void)
{
 if(BitTest(UCSR0A, (1<<RXC0))) {
 	return 1;
 	}
 return 0;
}



//-----------------------------------------------------------------------------
// Output zero terminated string to Uart 0 - polling method
//-----------------------------------------------------------------------------
//
void	uputs(U8 *s)
{
 while(*s != 0) {
 	u0putc(*s);
	s++;
 	}
}

ISR(SIG_USART_RECV)
{
 MasterCmd = UDR0;
 
}

// Timer0 overflow ISR
ISR(SIG_OUTPUT_COMPARE0A)
{
 BitSet(KFlags, (1<<KScanRdy));
}


// -----------------------------------------------------------------
// EEPROM operations
//
U8 EEPROM_read(U16 uiAddress)
{
 /* Wait for completion of previous write */
 while(EECR & (1<<EEPE));

 /* Set up address register */
 EEARL= uiAddress;
 EEARH = uiAddress>>8;

 /* Start eeprom read by writing EERE */
 EECR |= (1<<EERE);
 /* Return data from Data Register */
 return EEDR;
}

void EEPROM_write(U16 uiAddress, U8 ucData)
{
 /* Wait for completion of previous write */
 while(EECR & (1<<EEPE));

 /* Set up address and Data Registers */
 EEARL= uiAddress;
 EEARH = uiAddress>>8;
 EEDR = ucData;

 cli();
 
 /* Write logical one to EEMPE */
 EECR |= (1<<EEMPE);
 /* Start eeprom write by setting EEPE */
 EECR |= (1<<EEPE);

 sei();
}


