; //////////////////////////////////////////////////////////////////////////////
; //
; // 	Filename:	Zmon.S
; //	Version:		
; //
; //	Author:		Liu, Zemin   (jyetek@gmail.com)
; //	Company:	JYE Tech
; //	Web: 		www.jyetech.com
; //
; //-----------------------------------------------------------------------------
; //
; // 	Target: 		ATmega64
; // 	Tool chain: 	WinAvr (GCC-AVR)
; //
; //-----------------------------------------------------------------------------


#include <avr/io.h>
#include <avr/Sfr_defs.h>

	.section	.data
	
       .section 	.text
        
	.global	zmon   
zmon:
       
       ret                    		;back to pgm

 	.global	delay1us
delay1us: 	
 	nop
 	nop
 	nop
 	nop
 	nop
 	
 	nop
 	nop
 	nop
 	nop
 	nop
 	
 	nop
 	nop
 	nop
	ret 	


.end




