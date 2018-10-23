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
 	
	ret 	


.end

