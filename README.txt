Simple x86 instruction decoder.
-----------------------------------------------------------------------------------

HOW TO COMPILE AND RUN:(ON LINUX)

compile: gcc emulator.c operations.c mem.c -o emu
run: ./emu <input file>

example: (if the sample-input.txt and executable are at the same directory)
./emu sample-input.txt 

NOTE: ENTER key should be pressed to step into each instruction.

---------------------------------------README--------------------------------------
This x86 emulator supports following x86 opcodes:

0x8d LEA
0x83 AND
0x83 SUB
0x50 PUSH
0xc1 SAR
0x34 XOR
0x89 MOV
0x45 INC
0x80 CMP
0x5f POP

The source file structure:

1) IA32.h header contains the data structure that is used to store information regaring 
an x86 instruction.
2) operations.h header contains declarations of all operations.
3) mem.c contains implementation of memory and stack using a linked list.
4) operations.c contains all the definitions of operations. (operations' routines)
5) emulator.c contains the main interpreter loop which reads input from the text file 
and directs to the corresponding routine. 
6) some sample input files are also included.

Output :

The assembly instruction being decoded and is printed on standard output with decoding details
of MODRM and SIB bytes.
Also, register and memory contents are printed on standard output.

Assumptions:

1) Followed the convention that in two operand instructions like sub, 1st operand is 
deducted from the 2nd operand.
2) Input literals are provided in 2's complement arithmatic.
3) unsigned int data type has been used for 32 bit variables. (I checked with sizeof() command in gcc and found out that unsigned int has a capacity of 4 bytes.)

