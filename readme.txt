=================================
DSO 068 Learning Edition (LE)

(C) JYE Tech Ltd. All right reserved

Web:     www.jyetech.com
Forum:   www.jyetech.com/forum
Email:   jyetek@gmail.com
=================================


1. Purposes
-----------
DSO 068 Learning Edition was created to serve three purposes:

1 ) To provide a DSO programming example for students and beginners
2 ) To provide a base for DIYers to add their own features to DSO 068
3 ) To provide a programming template for everyone who want to use DSO 068 board for 
    their own projects.


2. Contents
-----------
A. Main Firmware [for ATmega64 (U4)]

 1 ) 113-06805-010.c -- The main program
 2 ) Board.c         -- Functions for initialization and lower level operation
 3 ) Board.h         -- Header file
 4 ) Command.c       -- Functions for key command analysis and execution
 5 ) Command.h       -- Header file
 6 ) Common.c        -- Basic type definitions and general functions
 7 ) Common.h        -- Header file
 8 ) DSO.c           -- Functions of oscilloscope core 
 9 ) DSO.h           -- Header file
10 ) Screen.c        -- Functions for screen related operations
11 ) Screen.h        -- Header file 
12 ) Zmon.s          -- Assembler subroutines
13 ) makefile        -- Winavr script file for building the project

B. Keypad Firmware [for ATmega48 (U5)]

 1 ) 113-06804-010.c -- The main program
 2 ) Board.c         -- Functions for initialization and lower level operation
 3 ) Board.h         -- Header file
 4 ) Command.c       -- Functions for keypad analysis
 5 ) Command.h       -- Header file
 6 ) Common.c        -- Basic type definitions and general functions
 7 ) Common.h        -- Header file 
 8 ) Zmon.s          -- Assembler subroutines
 9 ) makefile        -- Winavr script file for building the project

3. Limitations
--------------

1 ) Timebase was limited to 0.1ms/div - 20ms/div.
2 ) Maximun sampling rate was limited to 100Ksps.
3 ) No other features.


4, Applicable Models
--------------------
This Learning Edition is applicable to the "F" version PCB (PN: 109-06800-00F" ) 


5. How To Build
---------------
The project was developped under WinAVR(GCC). Under Windows environment please follow the steps 
below to build the project:

1 ) Download WinAVR from http://winavr.sourceforge.net/. Install it and have environment 
    variables setup properly as instructed in the accompanied documents.
2 ) Unzip all files in this package to a folder.
3 ) Open a command line window under Windows. Switch to the folder that contains the unzipped files by "CD" command. Type and execute "make all" from the folder.


6. Fuse Settings
----------------
A. Main controller (U4, ATmega64)
   Extended byte: - 0xFF
   High byte:     - 0xC3 (set this byte to 0xC2 if bootloader is used) 
   Low byte:      - 0x2E

B. Keypad controller (U5, ATmega48)
   Extended byte: - 0xFF
   High byte:     - 0xD6 
   Low byte:      - 0xE2


7. Support
----------
It is strongly recommended to present questions and seek help in JYE Tech Forum (http://www.jyetech.com/forum).
Alternatively, users can also write JYE Tech directly at jyetek@gmail.com. But due to very limited 
time only selected emails can be answered.


8. License
----------
DSO 068 Learning Edition is an open source software to implement digital storage oscilloscope
on JYE Tech DSO 068 board. It is a free software and is opened for education, research, and commercial
developments under license policy of the following terms.

Copyright (C)2014 JYE Tech Ltd., All right reserved

1 ) DSO 068 Learning Edition is a free software and there is NO WARRANTY.
2 ) You can use, modify, and redistribute it for personal, non-profit, or commercial product UNDER
    YOUR OWN RESPONSIBILITY.
3 ) If it is used in commercial product your product must not be in the name of "JYE Tech" and must not
    contain "DSO 068" in its title.
4 ) Redistributions of source codes must retain the above copyright notice.



