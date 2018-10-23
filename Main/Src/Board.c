//////////////////////////////////////////////////////////////////////////////
//
// 	Filename:	Board.c
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

#include "Common.h"
#include "Screen.h"
#include	"Command.h"
#include "Board.h"
#include	"DSO.h"


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
void	Misc_Init(void)
{
// MCU Control Register - MCUCR
//	SRE:	1 = External SRAM enable [MCUCR.7 = 0]
//	SRW10:	Wait state select bit, combined with SRW11 bit in XMCRA register [MCUCR.6 = 0]
//	SE:	
//	SM0:	
//	SM1:	
//	SM2:	
//	IVSEL:	
//	IVCE:	
//
 MCUCR = (0<<SRE)|(1<<SRW10)|(0<<SE)|(0<<SM0)
 		|(0<<SM1)|(0<<SM2)|(0<<IVSEL)|(0<<IVCE);		

// External Memory Control Register - XMCRA
//	SRL[2:0]:	Wait state sector limit [XMCRA.[6:4] = 000] (See datasheet)
//	SRW0[1:0]:	Wait state select bits for lower sector [XMCRA.[3:2] = 00]
//					00 = No wait state
//					01 = wait 1 cycle during read/write strobe
//					10 = wait 2 cycle during read/write strobe
//					11 = wait 2 cycle during read/write and 1 cycle before new address
//	SRW11:		Wait state select bit (with SRW10 in MCUCR) for upper sector [XMCRA.1 = 0]
//				[SRW11, SRW10]
//					00 = No wait state
//					01 = wait 1 cycle during read/write strobe
//					10 = wait 2 cycle during read/write strobe
//					11 = wait 2 cycle during read/write and 1 cycle before new address
 XMCRA = (0<<SRL2)|(0<<SRL2)|(0<<SRL2)|(1<<SRW01)
 		|(1<<SRW00)|(1<<SRW11);

// External Memory Control Register - XMCRB
//	XMBK:		1 = Enable external memory bus keeper [XMCRb.7 = 0] 
//	XMM[2:0]:	External memory high mask [XMCRb.[2:0] = 000]
//					000 = 8 bit high address, none port pin released
//					001 = 7 bit high address, PC7 released
//					010 = 6 bit high address, PC7 - PC6 released
//					011 = 5 bit high address, PC7 - PC5 released
//					100 = 4 bit high address, PC7 - PC4 released
//					101 = 3 bit high address, PC7 - PC3 released
//					110 = 2 bit high address, PC7 - PC2 released
//					111 = No address high bit, full Port C released
//
 XMCRB = (0<<XMBK)|(1<<XMM2)|(1<<XMM1)|(1<<XMM0);

// Special Function IO Register - SFIOR
//	TSM:	1 = activates the Timer/Counter Synchronization mode.
//	ACME:	1 = Switch off ADC and ADC multiplxer selects comparator negative input.
//			0 = AIN1 (PE3) connects to comparator negative input.
//	PUD:	1 = Disable port pull-up globally, 0 = enable port pull-up.
//	PSR0:	1 = Reset Timer0 prescaler. Clear by HW if TSM = 0.
//	PSR321:	1 = Reset Timer3/2/1 prescaler simultaneously. Clear by HW if TSM = 0.
//
// SFIOR defaults are all bits 0.
//
// ACME set to 1 for using ADC multiplxer for comparator

 SFIOR = (0<<TSM)|(1<<ACME)|(0<<PUD)|(0<<PSR0)|(0<<PSR321);

// External Interrupt Control Register A 每 EICRA
//	ISC31, ISC30 - ISC00, ISC00: External Interrupt 3 - 0 Sense Control Bits
//
//	ISCn1 ISCn0 Description
//		0 0 = The low level of INTn generates an interrupt request.
//		0 1 = Reserved
//		1 0 = The falling edge of INTn generates asynchronously an interrupt request.
//		1 1 = The rising edge of INTn generates asynchronously an interrupt request.
//
//	Falling edge trigs interrupt
 EICRA = (1<<ISC31)|(0<<ISC30)|(1<<ISC21)|(0<<ISC20)
 		|(1<<ISC11)|(0<<ISC10)|(1<<ISC01)|(0<<ISC00);

// External Interrupt Control Register B 每 EICRB
//	ISC71, ISC70 - ISC41, ISC40: External Interrupt 7 - 4 Sense Control Bits
//
//	ISCn1 ISCn0 Description
//		0 0 = The low level of INTn generates an interrupt request.
//		0 1 = Any logical change on INTn generates an interrupt request
//		1 0 = The falling edge between two samples of INTn generates an interrupt request.
//		1 1 = The rising edge between two samples of INTn generates an interrupt request.
 EICRB = (0<<ISC71)|(0<<ISC70)|(0<<ISC61)|(0<<ISC60)
 		|(0<<ISC51)|(0<<ISC50)|(0<<ISC41)|(0<<ISC40); 

// External Interrupt Mask Register 每 EIMSK
//	INT7 - INT0: External Interrupt Request 7 - 0 Enable. 1 = Enabled
//
 EIMSK = (0<<INT7)|(0<<INT6)|(0<<INT5)|(0<<INT4)
 		|(0<<INT3)|(0<<INT2)|(0<<INT1)|(0<<INT0);
// EIMSK = (0<<INT7)|(0<<INT6)|(0<<INT5)|(0<<INT4)
// 		|(0<<INT3)|(0<<INT2)|(0<<INT1)|(0<<INT0);

// External Interrupt Flag Register 每 EIFR
//	INTF7 - INTF0: External Interrupt Flags 7 - 0
//
 EIFR = (1<<INTF7)|(1<<INTF6)|(1<<INTF5)|(1<<INTF4)
 		|(1<<INTF3)|(1<<INTF2)|(1<<INTF1)|(1<<INTF0);

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

 // Port A default to input mode 
 PORTA = 
 	(1<<PA7)|(1<<PA6)|(1<<PA5)|(1<<PA4)|(1<<PA3)|(1<<PA2)|(1<<PA1)|(1<<PA0);
 DDRA = 
 	  (0<<DDA7)|(0<<DDA6)|(0<<DDA5)|(0<<DDA4)
 	|(0<<DDA3)|(0<<DDA2)|(0<<DDA1)|(0<<DDA0);

// PB7: Output, ADC clock
// PB6: 
// PB5: 
// PB4: Output, PWM for trig level (OC0)
// PB3: 
// PB2: 
// PB1: Output, LED
// PB0: 

 PORTB = 
 	(0<<PB7)|(1<<PB6)|(1<<PB5)|(0<<PB4)|(1<<PB3)|(1<<PB2)|(0<<PB1)|(1<<PB0);
 DDRB = 
 	  (1<<DDB7)|(0<<DDB6)|(0<<DDB5)|(1<<DDB4)
 	|(0<<DDB3)|(0<<DDB2)|(1<<DDB1)|(0<<DDB0);

 // PC[7:0]:	LCD data port
 PORTC = 
 	(0<<PC7)|(0<<PC6)|(0<<PC5)|(0<<PC4)|(0<<PC3)|(0<<PC2)|(0<<PC1)|(0<<PC0);
 DDRC = 
 	  (1<<DDC7)|(1<<DDC6)|(1<<DDC5)|(1<<DDC4)
 	|(1<<DDC3)|(1<<DDC2)|(1<<DDC1)|(1<<DDC0);

 // PD7:	Output, LCD_DI 
 // PD6:	
 // PD5:	
 // PD4:	
 // PD3:	Output, TXD for KEYCMD to key scan processor (M48)
 // PD2:	Input, RXD for KEYDAT from key scanning processor (M48)
 // PD1:	
 // PD0:	
 //
PORTD = 
 	(1<<PD7)|(1<<PD6)|(1<<PD5)|(1<<PD4)|(1<<PD3)|(1<<PD2)|(1<<PD1)|(1<<PD0);
 DDRD = 
 	  (1<<DDD7)|(0<<DDD6)|(0<<DDD5)|(0<<DDD4)
 	|(1<<DDD3)|(0<<DDD2)|(0<<DDD1)|(0<<DDD0);

 // PE7:	
 // PE6:	
 // PE5:	
 // PE4:	
 // PE3:	
 // PE2:	Input, trig level (AIN0)
 // PE1:	
 // PE0:	
 PORTE = 
 	(1<<PE7)|(1<<PE6)|(1<<PE5)|(1<<PE4)|(1<<PE3)|(0<<PE2)|(1<<PE1)|(1<<PE0);
 DDRE = 
 	  (0<<DDE7)|(0<<DDE6)|(0<<DDE5)|(0<<DDE4)
 	|(0<<DDE3)|(0<<DDE2)|(0<<DDE1)|(0<<DDE0);

 // PF7:	
 // PF6:	
 // PF5:	
 // PF4:	Input, trig source
 // PF3:	
 // PF2:	
 // PF1:	
 // PF0:	
 PORTF = 
 	(0<<PF7)|(0<<PF6)|(0<<PF5)|(0<<PF4)|(0<<PF3)|(0<<PF2)|(0<<PF1)|(1<<PF0);
 DDRF = 
 	  (0<<DDF7)|(0<<DDF6)|(0<<DDF5)|(0<<DDF4)
 	|(0<<DDF3)|(0<<DDF2)|(0<<DDF1)|(0<<DDF0);

// PG4: Output, LCD E control
// PG3: Output, LCD WR control
// PG2: Output, LCD RES control
// PG1: Output, LCD CS2 control
// PG0: Output, LCD CS1 control

 PORTG = 
 	(0<<PG4)|(0<<PG3)|(0<<PG2)|(0<<PG1)|(0<<PG0);
 DDRG = 
 	(1<<DDG4)|(1<<DDG3)|(1<<DDG2)|(1<<DDG1)|(1<<DDG0);

}

//-----------------------------------------------------------------------------
// UART0_Init
//-----------------------------------------------------------------------------
//
void Uart0_Init (void)
{
 // UCSR0A - USART Control and Status Register A
 //	RXC0:	1 = Receive complete [UCSR0A.7]
 //	TXC0:	1 = Transmit complete [UCSR0A.6]
 //	UDRE0:	1 = USART Data Register Empty [UCSR0A.5]
 //	FE0:		1 = Frame Error [UCSR0A.4]
 //	DOR0:	1 = Data Over Run [UCSR0A.3]
 //	UPE0:	1 = USART Parity Error [UCSR0A.2]
 //	U2X0:	1 = Double USART transmission speed (for asynchronous only) [UCSR0A.1]
 //	MPCM0:	1 = Multiprocessor Communication Mode [UCSR0A.0]
 //
 // UCSR0A defaults are all bits zero.
 UCSR0A = (0<<TXC0)|(0<<U2X0)|(0<<MPCM0);

 // UCSR0B - USART Control and Status Register B
 //	RXCIE0:	1 = Receive complete interrupt enable [UCSR0B.7]
 //	TXCIE0:	1 = Transmit complete interrupt enable [UCSR0B.6]
 //	UDRIE0:	1 = USART Data Register Empty interrupt enable [UCSR0B.5]
 //	RXEN0:	1 = Receiver enable [UCSR0B.4]
 //	TXEN0:	1 = Transmitter enable [UCSR0B.3]
 //	UCSZ02:	Character size, combined with UCSZ0.[1:0] in UCSR0C [UCSR0B.2]
 //	RXB80:	1 = Receive Date Bit 8 [UCSR0B.1]
 //	TXB80:	1 = Transmit Date Bit 8 [UCSR0B.0]
 //
 // UCSR0B defaults are all bits zero.

 // -- Enable receiver
 // -- Enable transmitter
 UCSR0B = (0<<RXCIE0)|(0<<TXCIE0)|(0<<UDRIE0)|(1<<RXEN0)
 			|(1<<TXEN0)|(0<<UCSZ02)|(0<<TXB80);

 // UCSR0C - USART Control and Status Register C
 //	UMSEL0:	1 = Asynchronous, 0 = Synchronous [UCSR0C.6]
 //	UPM0.[1:0]:	Parity Mode [UCSR0C.[5:4]]
 //				00 = Disabled
 //				01 = Reserved
 //				10 = Enabled, even parity
 //				11 = Enabled, odd parity
 //	USBS0:	Stop bit select, 0 = 1 stop bit, 1 = 2 stop bits [UCSR0C.3]
 //	UCSZ0.[1:0]:	Character size, combined with UCSZ02 in UCSR0B [UCSR0C.2]
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
 UCSR0C = (0<<UMSEL0)|(0<<UPM01)|(0<<UPM00)|(0<<USBS0)
 			|(1<<UCSZ01)|(1<<UCSZ00)|(0<<UCPOL0);

// Setup baudrate
 UBRR0H = (((F_CPU / 16) / Uart0_Baud) - 1) >> 8;
 UBRR0L = (((F_CPU / 16) / Uart0_Baud) - 1) & 0x00FF;
 
}

//-----------------------------------------------------------------------------
// UART1_Init
//-----------------------------------------------------------------------------
//
void Uart1_Init (void)
{
 // UCSR1A - USART Control and Status Register A
 //	RXC1:	1 = Receive complete [UCSR1A.7]
 //	TXC1:	1 = Transmit complete [UCSR1A.6]
 //	UDRE1:	1 = USART Data Register Empty [UCSR1A.5]
 //	FE1:		1 = Frame Error [UCSR1A.4]
 //	DOR1:	1 = Data Over Run [UCSR1A.3]
 //	UPE1:	1 = USART Parity Error [UCSR1A.2]
 //	U2X1:	1 = Double USART transmission speed (for asynchronous only) [UCSR1A.1]
 //	MPCM1:	1 = Multiprocessor Communication Mode [UCSR1A.0]
 //
 // UCSR1A defaults are all bits zero.
 UCSR1A = (0<<TXC1)|(0<<U2X1)|(0<<MPCM1);

 // UCSR1B - USART Control and Status Register B
 //	RXCIE1:	1 = Receive complete interrupt enable [UCSR1B.7]
 //	TXCIE1:	1 = Transmit complete interrupt enable [UCSR1B.6]
 //	UDRIE1:	1 = USART Data Register Empty interrupt enable [UCSR1B.5]
 //	RXEN1:	1 = Receiver enable [UCSR1B.4]
 //	TXEN1:	1 = Transmitter enable [UCSR1B.3]
 //	UCSZ12:	Character size, combined with UCSZ1.[1:0] in UCSR1C [UCSR1B.2]
 //	RXB81:	1 = Receive Date Bit 8 [UCSR1B.1]
 //	TXB81:	1 = Transmit Date Bit 8 [UCSR1B.0]
 //
 // UCSR1B defaults are all bits zero.

 // -- Enable receiver
 // -- Enable transmitter
 // -- Enable receiption interrupt
 UCSR1B = (1<<RXCIE1)|(0<<TXCIE1)|(0<<UDRIE1)|(1<<RXEN1)
 			|(1<<TXEN1)|(0<<UCSZ12)|(0<<TXB81);

 // UCSR1C - USART Control and Status Register C
 //	UMSEL1:	1 = Asynchronous, 0 = Synchronous [UCSR1C.6]
 //	UPM1.[1:0]:	Parity Mode [UCSR1C.[5:4]]
 //				00 = Disabled
 //				01 = Reserved
 //				10 = Enabled, even parity
 //				11 = Enabled, odd parity
 //	USBS1:	Stop bit select, 0 = 1 stop bit, 1 = 2 stop bits [UCSR1C.3]
 //	UCSZ1.[1:0]:	Character size, combined with UCSZ12 in UCSR1B [UCSR1C.2]
 //				000 = 5-bit
 //				001 = 6-bit
 //				010 = 7-bit
 //				011 = 8-bit
 //				100 = Reserved
 //				101 = Reserved
 //				110 = Reserved
 //				111 = 9-bit
 //	UCPOL1:	Clock pority, for synchronous mode only [UCSR1C.0] (See datasheet)
 //
 UCSR1C = (0<<UMSEL1)|(0<<UPM11)|(0<<UPM10)|(0<<USBS1)
 			|(1<<UCSZ11)|(1<<UCSZ10)|(0<<UCPOL1);

// Setup baudrate
 UBRR1H = (((F_CPU / 16) / Uart1_Baud) - 1) >> 8;
 UBRR1L = (((F_CPU / 16) / Uart1_Baud) - 1) & 0x00FF;
 
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
// Output one byte to specified port - polling method
//-----------------------------------------------------------------------------
//
void	uPutc(U8 UartNum, U8 ch)
{
 switch(UartNum) {
 	case 0:
 		while(!BitTest(UCSR0A, (1<<UDRE0))) {
 			}
 		UDR0 = ch;
		break;
 	case 1:
 		while(!BitTest(UCSR1A, (1<<UDRE1))) {
 			}
 		UDR1 = ch;
		break;
	default:
		break;
 	}

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
// Output zero terminated string to Uart 0 or Uart 1 - polling method
//-----------------------------------------------------------------------------
//
void	uputs(U8 *s, U8 UartNum)
{
 while(*s != 0) {
 	uPutc(UartNum, *s);
	s++;
 	}
}

// Outptu a ROM string
void	uputs_ROM(U8 *s, U8 UartNum)
{
 U8	tmp;

 tmp = pgm_read_byte(s);
 while(tmp != 0) {
 	uPutc(UartNum, tmp);
	s++;
	tmp = pgm_read_byte(s);
 	}
}

ISR(SIG_UART1_RECV)
{

 KeyCode = UDR1;
 
}

//-----------------------------------------------------------------------------
// Comp_Init
//-----------------------------------------------------------------------------
//
void Comp_Init(void)
{
 // Special Function IO Register - SFIOR
 //	ACME:	1 = Analog Comparator Multiplexer Enable
//			 When this bit is written logic one and the ADC is switched off 
//			(ADEN in ADCSRA is zero), the ADC multiplexer selects the 
//			negative input to the Analog Comparator. When this bit is written 
//			logic zero, AIN1 is applied to the negative input of the Analog Comparator.
 //
 // Current settings:
 //	-- ACME = 0 to use AIN1 as comparator input
 //	(Actual setting is done in Misc_Init())

// ACSR - Analog Comparator Control and Status Register
// 	ACD: 	1 = Analog Comparator Disable (ACSR.7)
//	ACBG: 	1 = fixed bandgap reference voltage applies positive input. Otherwise (ACSR.6)
//			AIN0 is connected to positive input. 
//	ACO: 	Analog Comparator Output with 1-2 clock delay. (ACSR.5)
//	ACI: 	Analog Comparator Interrupt Flag. Set by hardware based on ACS[1:0] (ACSR.4)
//	ACIE: 	1 = Analog Comparator Interrupt Enable (ACSR.3)
//	ACIC: 	1 = Analog Comparator Input Capture Enable (ACSR.2)
//	ACIS[1:0]: 	Analog Comparator Interrupt Mode Select (ACSR.[1:0])
//		00 	-	Comparator Interrupt on Output Toggle.
//		01	-	Reserved
//		10	-	Comparator Interrupt on Falling Output Edge.
//		11	-	Comparator Interrupt on Rising Output Edge.
//
// Current Settings:
//	ACIS[1:0] = 10 - Falling edge trigger interrupt
 ACSR = (0 << ACD)|(0 << ACBG)|(0 << ACO)|(1 << ACI)|(0 << ACIE)
 		|(0 <<ACIC)|(0 << ACIS1)|(0 << ACIS0);
 
}

//-----------------------------------------------------------------------------
// ADC0_Init
//-----------------------------------------------------------------------------
//
void ADC_Init (void)
{
 //ADMUX - ADC Multiplexer Selection Register
 //	Bit 7:6 每 REFS1:0: Reference Selection Bits
 //				0 0 - AREF, Internal Vref turned off.
 //				0 1 - AVCC with external capacitor at AREF pin.
 //				1 0 - Reserved
 //				1 1 - Internal 2.56V Voltage Reference with external capacitor at AREF pin.
 //	Bit 5 每 ADLAR: ADC Left Adjust Result. 1 = Left adjust
 //	Bits 4:0 每 MUX4:0: Analog Channel and Gain Selection Bits
 //				MUX4..0 	Single Ended 	Pos Diff. Input		Neg Diff. Input 	Gain
 //				00000 	ADC0
 //				00001 	ADC1
 //				00010 	ADC2
 //				00011 	ADC3 
 //				00100 	ADC4
 //				00101 	ADC5
 //				00110 	ADC6
 //				00111 	ADC7
 //				01000 				ADC0 			ADC0 			10x
 //				01001  				ADC1 			ADC0 			10x
 //				01010  				ADC0  			ADC0 			200x
 //				01011  				ADC1  			ADC0 			200x
 //				01100  				ADC2  			ADC2 			10x
 //				01101  				ADC3  			ADC2 			10x
 //				01110  				ADC2  			ADC2 			200x
 //				01111  				ADC3  			ADC2 			200x
 //				10000  				ADC0  			ADC1 			1x
 //				10001  				ADC1  			ADC1 			1x
 //				10010 				ADC2  			ADC1 			1x
 //				10011  				ADC3  			ADC1 			1x
 //				10100  				ADC4  			ADC1 			1x
 //				10101  				ADC5  			ADC1 			1x
 //				10110  				ADC6  			ADC1 			1x
 //				10111  				ADC7  			ADC1 			1x
 //				11000  				ADC0  			ADC2 			1x
 //				11001  				ADC1  			ADC2 			1x
 //				11010  				ADC2  			ADC2 			1x
 //				11011  				ADC3  			ADC2 			1x
 //				11100  				ADC4  			ADC2 			1x
 //				11101  				ADC5  			ADC2 			1x
 //				11110 	1.22V (VBG)  				
 //				11111 	0V (GND)
 //
 // 
 ADMUX = (0<<REFS1)|(0<<REFS0)|(0<<ADLAR)
 			|(0<<MUX4)|(0<<MUX3)|(1<<MUX2)|(0<<MUX1)|(0<<MUX0); 

 //ADCSRA - ADC Control and Status Register A 
 //	Bit 7 每 ADEN: ADC Enable. 1 = enable.
 //	Bit 6 每 ADSC: ADC Start Conversion. 1 = start conversion
 //	Bit 5 每 ADATE: ADC Auto Trigger Enable. 1 = auto trigger enabled
 //	Bit 4 每 ADIF: ADC Interrupt Flag
 //	Bit 3 每 ADIE: ADC Interrupt Enable. 1 = enabled
 //	Bits [2:0] 每 ADPS2:0: ADC Prescaler Select Bits
 //					ADPS[2:0] 	Division Factor
 //					0 0 0 		2
 //					0 0 1 		2
 //					0 1 0 		4
 //					0 1 1 		8
 //					1 0 0 		16
 //					1 0 1 		32
 //					1 1 0 		64
 //					1 1 1 		128
 ADCSRA = (0<<ADEN)|(0<<ADSC)|(0<<ADATE)|(0<<ADIF)
 			|(0<<ADIE)|(0<<ADPS2)|(0<<ADPS1)|(0<<ADPS0);

 //DCSRB - ADC Control and Status Register B
 //	Bits 7:3 每 Res: Reserved Bits
 //	Bit 2:0 每 ADTS[2:0]: ADC Auto Trigger Source
 //				ADTS[2:0] 	Trigger Source
 //				0 0 0 		Free Running mode
 //				0 0 1		Analog Comparator
 //				0 1 0 		External Interrupt Request 0
 //				0 1 1 		Timer/Counter0 Compare Match
 //				1 0 0 		Timer/Counter0 Overflow
 //				1 0 1 		Timer/Counter1 Compare Match B
 //				1 1 0 		Timer/Counter1 Overflow
 //				1 1 1 		Timer/Counter1 Capture Event
 ADCSRB = (0<<ADTS2)|(0<<ADTS1)|(0<<ADTS0); 
 
}


//-----------------------------------------------------------------------------
// Timer_Init
//-----------------------------------------------------------------------------
//
void Timer_Init (void)
{
 // -------------------------------------------------------------------------
 //	Timer/Counter 0
 //

 // TCCR0 - Timer/Counter Control Register
 //	FOC0: 		Force Output Compare
 //	WGM0[1:0]: 	Waveform Generation Mode
 //			Mode		TOP			Update of OCR0	TOV0 Set
 //		00 - Normal		0xFF 		Immediate 		MAX
 //		01 - PWM, PC		0xFF 		TOP 				BOTTOM
 //		10 - CTC 		OCR0 		Immediate 		MAX
 //		11 - Fast PWM 	0xFF 		TOP 				MAX
 //
 //	COM0[1:0]: 	Compare Match Output Mode
 //		Non-PWM Mode:
 //			00 - Normal port operation, OC0 disconnected.
 //			01 - Toggle OC0 on Compare Match.
 //			10 - Clear OC0 on Compare Match.
 //			11 - Set OC0 on Compare Match.
 //
 //		Fast PWM Mode:
 //			00 - Normal port operation, OC0 disconnected.
 //			01 - Reserved
 //			10 - Clear OC0 on Compare Match, set OC0 at TOP.
 //			11 - Set OC0 on Compare Match, clear OC0 at TOP.
 //
 //		Phase Correct PWM Mode:
 //			00 - Normal port operation, OC0 disconnected.
 //			01 - Reserved
 //			10 - Clear OC0 on Compare Match when up-counting. Set OC0 on Compare
 //				Match when downcounting.
 //			11 - Set OC0 on Compare Match when up-counting. Clear OC0 on Compare
 //				Match when downcounting.
 //	CS02:0: 		Clock Select
 //			000 - No clock source (Timer/counter stopped).
 //			001 - clkI/O/(No prescaling)
 //			010 - clkI/O/8 (From prescaler)
 //			011 - clkI/O/32 (From prescaler)
 //			100 - clkI/O/64 (From prescaler)
 //			101 - clkI/O/128 (From prescaler)
 //			110 - clkI/O/256 (From prescaler)
 //			111 - clkI/O/1024 (From prescaler)
 //
 // Current settings:
 // 	Mode = Fast PWM (WGM0[1:0] = 11)
 //	1/1 prescaling (CS0[2:0] = 001)
 //	Clear OC0 (PB4) at compare match (COM0[1:0] = 10)
 TCCR0 = (0<<FOC0)|(1<<WGM00)|(1<<COM01)|(0<<COM00)
 		|(1<<WGM01)|(0<<CS02)|(0<<CS01)|(1<<CS00);

 // TCNT0 - Timer/Counter Register
 //	
 TCNT0 = 0x00;

 // OCR0 - Output Compare Register
 //
 OCR0 = 0x80;


 
 // -------------------------------------------------------------------------
 //	Timer/Counter 1
 //
  // TCCRnA - Timer/Counter1(3) Control Register A
 // COMnA[1:0]: 	Compare Output Mode for Channel A
 // COMnB[1:0]: 	Compare Output Mode for Channel B
 // COMnC[1:0]: 	Compare Output Mode for Channel C
 //	Non-PWM Mode:
 //		00 - Normal port operation, OCnA/OCnB/OCnC disconnected.
 //		01 - Toggle OCnA/OCnB/OCnC on Compare Match.
 //		10 - Clear OCnA/OCnB/OCnC on Compare Match (Set output to low level).
 //		11 - Set OCnA/OCnB/OCnC on Compare Match (Set output to high level).
 //
 //	Fast PWM Mode:
 //		00 - Normal port operation, OCnA/OCnB/OCnC disconnected.
 //		01 - WGMn3:0 = 15: Toggle OCnA on Compare Match, OCnB/OCnC disconnected 
 //			(normal port operation). For all other WGMn settings, normal port operation,
//			OCnA/OCnB/OCnC disconnected.
 //		10 - Clear OCnA/OCnB/OCnC on Compare Match, set OCnA/OCnB/OCnC at TOP.
 //		11 - Set OCnA/OCnB/OCnC on Compare Match, clear OCnA/OCnB/OCnC at TOP.
 //
 //	Phase Correct and Phase and Frequency Correct PWM Mode:
 //		00 - Normal port operation, OCnA/OCnB/OCnC disconnected.
 //		01 - WGMn3:0 = 9 or 11: Toggle OCnA on Compare Match, OCnB/OCnC 
 //			disconnected (normal port operation). Forr all other WGMn settings, normal 
 //			port operation, OCnA/OCnB/OCnC disconnected.
 //		10 - Clear OCnA/OCnB/OCnC on Compare Match when upcounting. 
 //			Set OCnA/OCnB/OCnC on Compare Match when downcounting.
 //		11 - Set OCnA/OCnB/OCnC on Compare Match when up-counting.
 //			Clear OCnA/OCnB/OCnC on Compare Match when downcounting.
 //
 // WGMn[1:0]: 	Waveform Generation Mode. Combined with WGMn[3:2]  in the TCCRnB
 //				Mode			TOP			Update of OCRnX		TOVn Set
 //                      --------------------------------------------------------------
 //		0000 - 	Normal 			0xFFFF 		Immediate 			MAX
 //		0001 - 	PWM, PC8 		0x00FF 		TOP 					BOTTOM
 //		0010 - 	PWM, PC9 		0x01FF 		TOP 					BOTTOM
 //		0011 - 	PWM, PC10		0x03FF 		TOP 					BOTTOM
 //		0100 - 	CTC 			OCRnA 		Immediate 			MAX
 //		0101 - 	Fast PWM, 8-bit 	0x00FF 		TOP 					TOP
 //		0110 - 	Fast PWM, 9-bit 	0x01FF 		TOP 					TOP
 //		0111 - 	Fast PWM, 10-bit 	0x03FF 		TOP 					TOP
 //		1000 - 	PWM, P&F C		ICRn 		BOTTOM 				BOTTOM
 //		1001 - 	PWM, P&F C		OCRnA 		BOTTOM 				BOTTOM
 //		1010 - 	PWM, PC			ICRn		TOP 					BOTTOM
 //		1011 - 	PWM, PC			OCRnA 		TOP 					BOTTOM
 //		1100 - 	CTC 			ICRn 		Immediate 			MAX
 //		1101 - 	(Reserved) 		每 			每 					每
 //		1110 - 	Fast PWM 		ICRn 		TOP 					TOP
 //		1111 - 	Fast PWM 		OCRnA 		TOP 					TOP
 //
 //	CTC mode using ICR1 (WGM3[3:0] = 1100)
 TCCR1A = (0<<COM1A1)|(0<<COM1A0) |(0<<COM1B1) |(0<<COM1B0) 
 		|(0<<COM1C1) |(0<<COM1C0) |(0<<WGM11) |(0<<WGM10);
 
 // TCCRnB - Timer/Counter1(3) Control Register B
 //	ICNCn: 		Input Capture Noise Canceler
 //	ICESn: 		Input Capture Edge Select. 0 = falling, 1 = rising.
 //	WGMn[3:2]: 	Waveform Generation Mode
 //	CSn2:0: 		Clock Select
 //		000 - No clock source (Timer/counter stopped).
 //		001 - clkI/O/1 (No prescaling)
 //		010 - clkI/O/8 (From prescaler)
 //		011 - clkI/O/64 (From prescaler)
 //		100 - clkI/O/256 (From prescaler)
 //		101 - clkI/O/1024 (From prescaler)
 //		110 - External clock source on Tn pin. Clock on falling edge.
 //		111 - External clock source on Tn pin. Clock on rising edge.
 //
 // -- No prescaling (CS1[2:0] = 001)
 TCCR1B = (0<<ICNC1)|(0<<ICES1)|(1<<WGM13)|(1<<WGM12)
 		|(0<<CS12)|(0<<CS11)|(1<<CS10);

 // TCCRnC - Timer/Counter1(3) Control Register C
 //	FOCnA: 	Force Output Compare for Channel A
 //	FOCnB: 	Force Output Compare for Channel B
 //	FOCnC: 	Force Output Compare for Channel C
 //
 TCCR1C = (0<<FOC1A)|(0<<FOC1B)|(0<<FOC1C);

 // TCNT1H and TCNT1L - Timer/Counter1
 //
 TCNT1H = 0x00;
 TCNT1L = 0x00;

 // OCR1AH and OCR1AL - Output Compare Register 1A
 // 
 OCR1AH = 0x00;
 OCR1AL = 0x00;

 // OCR1BH and OCR1BL - Output Compare Register 1 B
 // 
 OCR1BH = 0x00;
 OCR1BL = 0x00;

 // OCR1CH and OCR1CL - Output Compare Register 1 C
 // 
 OCR1CH = 0x00;
 OCR1CL = 0x00;

 // ICR1H and ICR1L - Input Capture Register 1
 //
 ICR1H = 0x00;
 ICR1L = 0x00;

 // ----------------------------------------------------------------------
 //	Timer/Counter 2
 //
 // TCCR2 - Timer/Counter Control Register
 //	FOC2: 		Force Output Compare
 //	WGM2[1:0]: 	Waveform Generation Mode
 //			Mode		TOP			Update of OCR2	TOV2 Set
 //		00 - Normal		0xFF 		Immediate 		MAX
 //		01 - PWM, PC		0xFF 		TOP 				BOTTOM
 //		10 - CTC 		OCR2 		Immediate 		MAX
 //		11 - Fast PWM 	0xFF 		TOP 				MAX
 //
 //	COM2[1:0]: 	Compare Match Output Mode
 //		Non-PWM Mode:
 //			00 - Normal port operation, OC2 disconnected.
 //			01 - Toggle OC2 on Compare Match.
 //			10 - Clear OC2 on Compare Match.
 //			11 - Set OC2 on Compare Match.
 //
 //		Fast PWM Mode:
 //			00 - Normal port operation, OC2 disconnected.
 //			01 - Reserved
 //			10 - Clear OC2 on Compare Match, set OC2 at TOP.
 //			11 - Set OC2 on Compare Match, clear OC2 at TOP.
 //
 //		Phase Correct PWM Mode:
 //			00 - Normal port operation, OC2 disconnected.
 //			01 - Reserved
 //			10 - Clear OC2 on Compare Match when up-counting. Set OC2 on Compare
 //				Match when downcounting.
 //			11 - Set OC2 on Compare Match when up-counting. Clear OC2 on Compare
 //				Match when downcounting.
 //	CS22:0: 		Clock Select
 //			000 - No clock source (Timer/counter stopped).
 //			001 - clkI/O/(No prescaling)
 //			010 - clkI/O/8 (From prescaler)
 //			011 - clkI/O/64 (From prescaler)
 //			100 - clkI/O/256 (From prescaler)
 //			101 - clkI/O/1024 (From prescaler)
 //			110 - External clock source on T2 pin. Clock on falling edge.
 //			111 - External clock source on T2 pin. Clock on rising edge.
 //
 //	CTC mode (WGM2[1:0] = 10)
 //	Toggle OC2 on compare match (COM2[1:0] = 01)
 //	NO prescaling (CS2[2:0] = 001)
 //	Toggle OC2 every 5us - 100KHz square wave to generate neg voltage for LCD
 //		(OCR2 = 100)
 TCCR2 = (0<<FOC2)|(0<<WGM20)|(0<<COM21)|(0<<COM20)
 		|(1<<WGM21)|(0<<CS22)|(0<<CS21)|(1<<CS20);

 // TCNT2 - Timer/Counter Register
 //	
 TCNT2 = 0x00;

 // OCR2 - Output Compare Register
 //
 OCR2 = 1;

 
 // ----------------------------------------------------------------------
 //	Timer/Counter 3 (See descriptions in Timer/Counter 1 for meanings of settings)
 //
  // TCCRnA - Timer/Counter1(3) Control Register A
 // COMnA[1:0]: 	Compare Output Mode for Channel A
 // COMnB[1:0]: 	Compare Output Mode for Channel B
 // COMnC[1:0]: 	Compare Output Mode for Channel C
 //	Non-PWM Mode:
 //		00 - Normal port operation, OCnA/OCnB/OCnC disconnected.
 //		01 - Toggle OCnA/OCnB/OCnC on Compare Match.
 //		10 - Clear OCnA/OCnB/OCnC on Compare Match (Set output to low level).
 //		11 - Set OCnA/OCnB/OCnC on Compare Match (Set output to high level).
 //
 //	Fast PWM Mode:
 //		00 - Normal port operation, OCnA/OCnB/OCnC disconnected.
 //		01 - WGMn3:0 = 15: Toggle OCnA on Compare Match, OCnB/OCnC disconnected 
 //			(normal port operation). For all other WGMn settings, normal port operation,
//			OCnA/OCnB/OCnC disconnected.
 //		10 - Clear OCnA/OCnB/OCnC on Compare Match, set OCnA/OCnB/OCnC at TOP.
 //		11 - Set OCnA/OCnB/OCnC on Compare Match, clear OCnA/OCnB/OCnC at TOP.
 //
 //	Phase Correct and Phase and Frequency Correct PWM Mode:
 //		00 - Normal port operation, OCnA/OCnB/OCnC disconnected.
 //		01 - WGMn3:0 = 9 or 11: Toggle OCnA on Compare Match, OCnB/OCnC 
 //			disconnected (normal port operation). Forr all other WGMn settings, normal 
 //			port operation, OCnA/OCnB/OCnC disconnected.
 //		10 - Clear OCnA/OCnB/OCnC on Compare Match when upcounting. 
 //			Set OCnA/OCnB/OCnC on Compare Match when downcounting.
 //		11 - Set OCnA/OCnB/OCnC on Compare Match when up-counting.
 //			Clear OCnA/OCnB/OCnC on Compare Match when downcounting.
 //
 // WGMn[1:0]: 	Waveform Generation Mode. Combined with WGMn[3:2]  in the TCCRnB
 //				Mode			TOP			Update of OCRnX		TOVn Set
 //                      --------------------------------------------------------------
 //		0000 - 	Normal 			0xFFFF 		Immediate 			MAX
 //		0001 - 	PWM, PC8 		0x00FF 		TOP 					BOTTOM
 //		0010 - 	PWM, PC9 		0x01FF 		TOP 					BOTTOM
 //		0011 - 	PWM, PC10		0x03FF 		TOP 					BOTTOM
 //		0100 - 	CTC 			OCRnA 		Immediate 			MAX
 //		0101 - 	Fast PWM, 8-bit 	0x00FF 		TOP 					TOP
 //		0110 - 	Fast PWM, 9-bit 	0x01FF 		TOP 					TOP
 //		0111 - 	Fast PWM, 10-bit 	0x03FF 		TOP 					TOP
 //		1000 - 	PWM, P&F C		ICRn 		BOTTOM 				BOTTOM
 //		1001 - 	PWM, P&F C		OCRnA 		BOTTOM 				BOTTOM
 //		1010 - 	PWM, PC			ICRn		TOP 					BOTTOM
 //		1011 - 	PWM, PC			OCRnA 		TOP 					BOTTOM
 //		1100 - 	CTC 			ICRn 		Immediate 			MAX
 //		1101 - 	(Reserved) 		每 			每 					每
 //		1110 - 	Fast PWM 		ICRn 		TOP 					TOP
 //		1111 - 	Fast PWM 		OCRnA 		TOP 					TOP
 //
 TCCR3A = (0<<COM3A1)|(0<<COM3A0) |(0<<COM3B1) |(0<<COM3B0) 
 		|(0<<COM3C1) |(0<<COM3C0) |(0<<WGM31) |(1<<WGM30);
 
 // TCCRnB - Timer/Counter1(3) Control Register B
 //	ICNCn: 		Input Capture Noise Canceler
 //	ICESn: 		Input Capture Edge Select
 //	WGMn[3:2]: 	Waveform Generation Mode
 //	CSn2:0: 		Clock Select
 //		000 - No clock source (Timer/counter stopped).
 //		001 - clkI/O/1 (No prescaling)
 //		010 - clkI/O/8 (From prescaler)
 //		011 - clkI/O/64 (From prescaler)
 //		100 - clkI/O/256 (From prescaler)
 //		101 - clkI/O/1024 (From prescaler)
 //		110 - External clock source on Tn pin. Clock on falling edge.
 //		111 - External clock source on Tn pin. Clock on rising edge.
 //
 TCCR3B = (0<<ICNC3)|(0<<ICES3)|(0<<WGM33)|(1<<WGM32)
 		|(0<<CS32)|(0<<CS31)|(1<<CS30);

 // TCCRnC - Timer/Counter1(3) Control Register C
 //	FOCnA: 	Force Output Compare for Channel A
 //	FOCnB: 	Force Output Compare for Channel B
 //	FOCnC: 	Force Output Compare for Channel C
 //
 TCCR3C = (0<<FOC3A)|(0<<FOC3B)|(0<<FOC3C);

 // TCNT3H and TCNT3L - Timer/Counter3
 //
 TCNT3H = 0x00;
 TCNT3L = 0x00;
 // OCR3AH and OCR3AL - Output Compare Register 3 A
 // 
 OCR3AH = 0x00;
 OCR3AL = 0x60;	

 // OCR3BH and OCR3BL - Output Compare Register 3 B
 // 
 OCR3BH = 0x00;
 OCR3BL = 0x00;

 // OCR3CH and OCR3CL - Output Compare Register 3 C
 // 
 OCR3CH = 0x00;
 OCR3CL = 0x00;

 // ICR3H and ICR3L - Input Capture Register 3
 //
 ICR3H = 0x00;
 ICR3L = 0x00;

 // -------------------------------------------------------------------
 //	Interrupt settings for Timer 1/2/3
 //
 // TIMSK - Timer/Counter Interrupt Mask Register
 //	OCIE2: 	1 = Timer/Counter2 Output Compare Match Interrupt Enable [TIMSK.7]
 //	TOIE2: 	1 = Timer/Counter2 Overflow Interrupt Enable [TIMSK.6]
 //	TICIE1: 	1 = Timer/Counter1, Input Capture Interrupt Enable [TIMSK.5]
 //	OCIE1A: 	1 = Timer/Counter1, Output Compare A Match Interrupt Enable [TIMSK.4]
 //	OCIE1B: 	1 = Timer/Counter1, Output Compare B Match Interrupt Enable [TIMSK.3]
 //	TOIE1: 	1 = Timer/Counter1, Overflow Interrupt Enable [TIMSK.2]
 // 	OCIE0: 	1 = Timer/Counter0 Output Compare Match Interrupt Enable [TIMSK.1]
 //	TOIE0: 	1 = Timer/Counter0 Overflow Interrupt Enable [TIMSK.0]
 TIMSK = (0<<OCIE2)|(0<<TOIE2)|(0<<TICIE1)|(0<< OCIE1A)
 	      	|(0<<OCIE1B)|(0<< TOIE1)|(0<< OCIE0)|(0<< TOIE0);

 // ETIMSK - Extended Timer/Counter Interrupt Mask Register
 //	TICIE3: 	Timer/Counter3, Input Capture Interrupt Enable
 //	OCIE3A: 	Timer/Counter3, Output Compare A Match Interrupt Enable
 //	OCIE3B: 	Timer/Counter3, Output Compare B Match Interrupt Enable
 //	TOIE3: 	Timer/Counter3, Overflow Interrupt Enable
 //	OCIE3C: 	Timer/Counter3, Output Compare C Match Interrupt Enable
 //	OCIE1C: 	Timer/Counter1, Output Compare C Match Interrupt Enable
 //
 ETIMSK = (0<<TICIE3)|(0<<OCIE3A)|(0<<OCIE3B)|(0<<TOIE3)
 		|(0<<OCIE3C)|(0<<OCIE1C);

 // TIFR - Timer/Counter Interrupt Flag Register
 //	OCF2: 	Output Compare Flag 2
 //	TOV2: 	Timer/Counter2 Overflow Flag
 //	ICF1: 	Timer/Counter1, Input Capture Flag
 //	OCF1A: 	Timer/Counter1, Output Compare A Match Flag
 //	OCF1B: 	Timer/Counter1, Output Compare B Match Flag
 //	TOV1: 	Timer/Counter1, Overflow Flag
 //	OCF0: 	Output Compare Flag 0
 //	TOV0: 	Timer/Counter0 Overflow Flag
 TIFR = (1<<OCF2)|(1<<TOV2)|(1<<ICF1)|(1<<OCF1A)
 		|(1<<OCF1B)|(1<<TOV1)|(1<<OCF0)|(1<<TOV0);

 // ETIFR - Extended Timer/Counter Interrupt Flag Register
 //	ICF3: 	Timer/Counter3, Input Capture Flag
 //	OCF3A: 	Timer/Counter3, Output Compare A Match Flag
 //	OCF3B: 	Timer/Counter3, Output Compare B Match Flag
 //	TOV3: 	Timer/Counter3, Overflow Flag
 //	OCF3C: 	Timer/Counter3, Output Compare C Match Flag
 //	OCF1C: 	Timer/Counter1, Output Compare C Match Flag
 //	
 ETIFR = (1<<ICF3)|(1<<OCF3A)|(1<<OCF3B)|(1<<TOV3)|(1<<OCF3C)|(1<<OCF1C);

}

void	LCD_Init(void)
{
 // Reset LCD panel
 BitClr(LCD_RES_Port, (1<<LCD_RES_Bit));
 Delay(1000);

 BitSet(LCD_RES_Port, (1<<LCD_RES_Bit));
 Delay(100);

 // Set display ON
 while(!(LCD_Write(LCD_I_type, LCD_I_On, LCD_Block_Left))) {
 	}
 while(!(LCD_Write(LCD_I_type, LCD_I_On, LCD_Block_Right))) {
 	}

}

U8	LCD_Read(U8 type, U8 block)
{
 U8 tmp;

 // Select block
 if(block == LCD_Block_Left) {
 	// Left block
 	BitSet(LCD_CS1_Port, (1<<LCD_CS1_Bit));
 	BitClr(LCD_CS2_Port, (1<<LCD_CS2_Bit));
 	}
 else {
 	// Right block
 	BitSet(LCD_CS2_Port, (1<<LCD_CS2_Bit));
 	BitClr(LCD_CS1_Port, (1<<LCD_CS1_Bit));
 	} 	
 
 if(type == LCD_I_type) {
 	// Read status
	BitClr(LCD_DI_Port, (1<<LCD_DI_Bit));
 	}
 else {
 	// Read data
	BitSet(LCD_DI_Port, (1<<LCD_DI_Bit));
 	}
 
 	BitSet(LCD_WR_Port, (1<<LCD_WR_Bit));

 	// Set E signal to HIGH
 	BitSet(LCD_E_Port, (1<<LCD_E_Bit));
 
 	// Set Port A to input mode
 	LCD_Data_Dir = 0x00;
	// Delay
 	Delay(LCD_Delay);

 	// Read
 	tmp = LCD_Data_In;
 	Delay(LCD_Delay);

 	// Set Port A to output mode
 	LCD_Data_Dir = 0xFF;
 	BitClr(LCD_CS1_Port, (1<<LCD_CS1_Bit));
	BitClr(LCD_CS2_Port, (1<<LCD_CS2_Bit));
 	// Set E signal to LOW
 	BitClr(LCD_E_Port, (1<<LCD_E_Bit));
 	Delay(LCD_Delay);

 	return(tmp);
 
}

U8	LCD_Write(U8 type, U8 data, U8 block)
{
// U8 tmp;
 
 // Check for status
// tmp = LCD_Read(LCD_I_type, block);
// if( tmp & 0x80) {
 	// LCD busy
// 	return(0x00);
// 	}
 
 // -- type = 0/1 for control/date
 // 
 if(type == LCD_I_type) {
 	// Write instruction
 	BitClr(LCD_DI_Port, (1<<LCD_DI_Bit));
	BitClr(LCD_WR_Port, (1<<LCD_WR_Bit));
 	}
 else {
 	// Write data
 	BitClr(LCD_WR_Port, (1<<LCD_WR_Bit));
 	BitSet(LCD_DI_Port, (1<<LCD_DI_Bit));
 	} 	

 // Select block
 if(block == LCD_Block_Left) {
 	// Left block
 	BitSet(LCD_CS1_Port, (1<<LCD_CS1_Bit));
 	BitClr(LCD_CS2_Port, (1<<LCD_CS2_Bit));
 	}
 else {
 	// Right block
 	BitSet(LCD_CS2_Port, (1<<LCD_CS2_Bit));
 	BitClr(LCD_CS1_Port, (1<<LCD_CS1_Bit));
 	} 	

 // Set E signal to HIGH
 BitSet(LCD_E_Port, (1<<LCD_E_Bit));

 LCD_Data_Port = data;
 Delay(LCD_Delay);

 // Set E signal to LOW
 BitClr(LCD_E_Port, (1<<LCD_E_Bit));
 BitClr(LCD_CS1_Port, (1<<LCD_CS1_Bit));
 BitClr(LCD_CS2_Port, (1<<LCD_CS2_Bit));
 Delay(LCD_Delay);

 return (0xFF);
	
}

void EE_Write(U8 *Addr, U8 Data)
{
 // Wait for completion of previous write 
 while(EECR & (1<<EEWE)) {
 	}
 // Set up address and data registers 
 EEAR = (U16)Addr;
 EEDR = Data;
 cli();
 // Write logical one to EEMWE
 BitSet(EECR, (1<<EEMWE));
 // Start eeprom write by setting EEWE 
 BitSet(EECR, (1<<EEWE)); 
 sei();
}

U8 	EE_Read(U8 *Addr)
{
 // Wait for completion of previous write 
 while(EECR & (1<<EEWE)) {
 	}
 // Set up address registers 
 EEAR = (U16)Addr;
 // Start eeprom read by writing EERE 
 BitSet(EECR, (1<<EERE));
 // Return data from data register 
 return EEDR;
}

