@256
D=A
@SP
M = D
@7
D=A
@SP
M=M+1
A=M
M=D
// END OF PUSH/POP COMMAND
@8
D=A
@SP
M=M+1
A=M
M=D
// END OF PUSH/POP COMMAND
// ARITHMETIC COMMAND: add
// DYNAMIC POP COMMAND temp 0
@5
D = A
@0
A = D + A
D = A
@R13
M=D
@SP
A=M
D=M
@R13
A=M
M=D
@SP
M=M-1
// END OF PUSH/POP COMMAND
// DYNAMIC POP COMMAND temp 1
@5
D = A
@1
A = D + A
D = A
@R13
M=D
@SP
A=M
D=M
@R13
A=M
M=D
@SP
M=M-1
// END OF PUSH/POP COMMAND
@6
D=M
@5
M = M+D
// DYNAMIC PUSH COMMAND temp 0
@SP
M = M+1
@temp
D = A
@0
A = D+A
D = M
@SP
A = M
M = D
// END OF PUSH/POP COMMAND
// END OF ARITHMETIC COMMAND: add
