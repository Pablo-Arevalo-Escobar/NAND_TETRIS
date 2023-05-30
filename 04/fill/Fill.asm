@R0
M = 0
@R2
M = D
(LOOP)
	@R0
	D = M 
	@8192
	D = D-A
	@END
	D;JGE
	
	
	
	@KBD
	D = M
	@25
	D;JGT 
	@SCREEN
	D = A
	@R0
	D = D + M
	@R1
	A = D
	M = A
	D = 0
	M = D
	@34
	0;JMP
	@SCREEN
	D = A
	@R0
	D = D + M
	@R1
	A = D
	M = A
	D = 0
	M = !D
	@R0
	M = M + 1
	@LOOP
	0;JMP
(END)
@END
@0
0;JMP // Infinite loop



