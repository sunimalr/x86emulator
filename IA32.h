/*
*Data structure to hold a IA32 instruction. 
*Author: sunimalr@comp.nus.edu.sg
*/


#ifndef IA32
#define IA32

extern unsigned int eax;
extern unsigned int ecx;
extern unsigned int edx;
extern unsigned int ebx;
extern unsigned int esp;
extern unsigned int ebp;
extern unsigned int esi;
extern unsigned int edi;
extern unsigned int eip;
extern unsigned int eflags;
extern unsigned int cs;
extern unsigned int ss;
extern unsigned int ds;
extern unsigned int es;
extern unsigned int fs;
extern unsigned int gs;

typedef struct IA32Inst
{ 
    unsigned int opcode, prefixmask;
	char ilen;
	//InterpreterFunction-routine
	
	struct
	{
		int mode;
		int Rbase;
		int Rindex;
		int shmt;
		long displacement;
	} operandRM;

	struct
	{
		int mod;
		int reg;
		int rm;
	} MODRM;

	struct
	{
		int scale;
		int index;
		int base;
		unsigned int displacement;
	} SIB;	
	
	char sourcereg;
	char destreg;
	char *opmode;

	struct
	{
		int mode;
		char regname;
		long immediate;
	} operandRI;
} instr;

struct list_el {
	unsigned int address;
	unsigned int val;
	struct list_el * next;
};
typedef struct list_el item;

#endif
