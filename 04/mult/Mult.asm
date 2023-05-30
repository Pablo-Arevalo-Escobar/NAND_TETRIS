// This file is part of www.nand2tetris.org
// and the book "The Elements of Computing Systems"
// by Nisan and Schocken, MIT Press.
// File name: projects/04/Mult.asm

// Multiplies R0 and R1 and stores the result in R2.
// (R0, R1, R2 refer to RAM[0], RAM[1], and RAM[2], respectively.)
//
// This program only needs to handle arguments that satisfy
// R0 >= 0, R1 >= 0, and R0*R1 < 32768.

// Put your code here.
// This program performs multiplication by adding R0 to itself R1 times

//Initalize counter to 0
//Initalize the sum to 0
@R3
M = 0
@R2 
M = 0

(LOOP)
	//Load the counter and check if it is greater than the multiple 
	@R3
	D=M
	@R1
	D=D-M
	@END
	D;JGE // if (counter is greater than multiple goto END)
	
	//Add R0 to R2, increment counter (R3)
	@R0
	D=M
	@R2
	M = D+M 
	@R3
	M=M+1
	@LOOP
	0;JMP
(END)
@END