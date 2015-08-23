/*
*x86 emulator
*Author: sunimalr@comp.nus.edu.sg
*/
#include "IA32.h"
#include <stdio.h>
#include <stdlib.h>
#include "operations.h"
#include <string.h>

void parse_prefix(unsigned int input);
void parse_opcode(instr ins, unsigned int input);

//Initial values of registers
	unsigned int eax=0xbf8db144;
   	unsigned int ecx=0x88c5cffb;
    unsigned int edx=0x1;
    unsigned int ebx=0xae5ff4;
    unsigned int esp=0xbf8db0bc;
    unsigned int ebp=0xbf8db118;
    unsigned int esi=0x9a0ca0;
    unsigned int edi=0x0;
    unsigned int eip=0x8048354;
    unsigned int eflags=0x246;
    unsigned int cs=0x73;
    unsigned int ss=0x7b;
    unsigned int ds=0x7b;
    unsigned int es=0x7b;
    unsigned int fs=0x0;
    unsigned int gs=0x33;


instr newins;//type defined in IA32.h

FILE *file;

unsigned int getnext(FILE *fp)
{
char c;
char inhex[2];
unsigned int ret;
char *ptr;
	if(fscanf(file,"%s",inhex)!=EOF)
	{
		ret=strtoul(inhex, &ptr, 16);
		eip=eip+1;
		return ret;	
	}
	else
	{
		printf("Done!\n");
		fclose(fp);
		exit(0);
	}
}

int main(int argc, char **argv)
{	
	file = fopen(argv[1],"r");
	printf("\nPress ENTER key to step forward!\n");
	char i=getchar();
	while((i!='n')||(i!='N'))
	{
		unsigned int val=getnext(file);
		parse_prefix(val);
		printf("\nPress ENTER key to continue!\n");
		i=getchar();
	}
	return 0;
}

//Parsing the prefix
void parse_prefix(unsigned int input)
{
	instr ins;
	switch(input)
	{
		case 0xf0:
		case 0xf3:
		case 0xf2:
			ins.prefixmask=input;
			parse_opcode(ins,getnext(file)); 
			break;
		case 0x2e:
		case 0x36:
		case 0x3e:
		case 0x26:
		case 0x64:
		case 0x65:	
		case 0x66:
			ins.prefixmask=input;
			parse_opcode(ins,getnext(file));
			break;
		case 0x67:
			ins.prefixmask=input;
			parse_opcode(ins,getnext(file));
			break;
		default:
			parse_opcode(ins, input);
	}
}

/*
*Parsing the opcode.
*Depending on the opcode, the correct routine is called
*/
void parse_opcode(instr ins, unsigned int input)
{
	printf("\n----------------------------------------------------------------------------\n");
	printf("Opcode : %x\n",input);
	switch(input)
	{
		case 0x8d:
			op_leal(ins,input,file);
			break;
		case 0x83:
			op_andl_subl(ins,input,file);
			break;
		case 0x50:
			op_pushl(ins,input,file);
			break;
		case 0xc1:
			op_shift(ins,input,file);
			break;
		case 0x34:
			op_xor(ins,input,file);
			break;
		case 0x89:
			op_mov(ins, input,file);
			break;
		case 0x40:
		case 0x41:
		case 0x42:
		case 0x43:
		case 0x44:
		case 0x45:
		case 0x46:
		case 0x47:
			op_inc(ins,input,file);
			break;
		case 0x80:
			op_add_or_adc_sbb_and_sub_xor_cmp(ins,input,file);
			break;
		case 0x58:
		case 0x59:
		case 0x5a:
		case 0x5b:
		case 0x5c:
		case 0x5d:
		case 0x5e:
		case 0x5f:
			op_popl(ins,input,file);
			break;
	}

	print_mem_reg();//Printing memory and register contents
	print_stack_top();
}

/*
*This function returns the register name when the hex code is passed as argument.
*/
char* get_register_name(int code)
{
	switch(code)
	{
		case 0x00:
			return("eax");
			break;
		case 0x01:
			return("ecx");
			break;
	
		case 0x02:
			return("edx");
			break;

		case 0x03:
			return("ebx");
			break;

		case 0x04:
			return("esp");
			break;

		case 0x05:
			return("ebp");
			break;

		case 0x06:
			return("esi");
			break;

		case 0x07:
			return("edi");
			break;
	}
}

/*
*Returns the content of a register when the register code is passed as argument
*/
unsigned int get_register_content(int code)
{
	switch(code)
	{
		case 0x00:
			return eax;
			break;
		case 0x01:
			return ecx;
			break;
	
		case 0x02:
			return edx;
			break;

		case 0x03:
			return ebx;
			break;

		case 0x04:
			return esp;
			break;

		case 0x05:
			return ebp;
			break;

		case 0x06:
			return esi;
			break;

		case 0x07:
			return edi;
			break;
	}
}

/*
* Stores content in registers
*/
void set_register_content(int code, unsigned int data)
{
	switch(code)
	{
		case 0x00:
			eax=data;
			break;
		case 0x01:
			ecx=data;
			break;
	
		case 0x02:
			edx=data;
			break;

		case 0x03:
			ebx=data;
			break;

		case 0x04:
			esp=data;
			break;

		case 0x05:
			ebp=data;
			break;

		case 0x06:
			esi=data;
			break;

		case 0x07:
			edi=data;
			break;
	}
}

/*
*Sets flags located in the eflags registers
*/
void set_eflags(char *flag, int val)
{
	if(strcmp("of",flag)==0)
	{
		if(val==1)
		{
			eflags=eflags | 0x800;//Setting the flag
		}
		if(val==0)
		{
			eflags=eflags & 0xFFFFF7FF;//Clearing the flag
		}
	}
	if(strcmp("cf",flag)==0)
    {
        if(val==1)
        {
            eflags=eflags | 0x1;
        }
        if(val==0)
        {
            eflags=eflags & 0xFFFFFFFE;
        }
    }
	if(strcmp("pf",flag)==0)
    {
        if(val==1)
        {
            eflags=eflags | 0x4;
        }
        if(val==0)
        {
            eflags=eflags & 0xFFFFFFFB;
        }
    }
	if(strcmp("af",flag)==0)
    {
        if(val==1)
        {
            eflags=eflags | 0x10;
        }
        if(val==0)
        {
            eflags=eflags & 0xFFFFFFEF;
        }
    }
	if(strcmp("zf",flag)==0)
    {
        if(val==1)
        {
            eflags=eflags | 0x40;
        }
        if(val==0)
        {
            eflags=eflags & 0xFFFFFFBF;
        }
    }
	if(strcmp("sf",flag)==0)
    {
        if(val==1)
        {
            eflags=eflags | 0x80;
        }
        if(val==0)
        {
            eflags=eflags & 0xFFFFFF7F;
        }
    }
	if(strcmp("tf",flag)==0)
    {
        if(val==1)
        {
            eflags=eflags | 0x100;
        }
        if(val==0)
        {
            eflags=eflags & 0xFFFFFEFF;
        }
    }
	if(strcmp("if",flag)==0)
    {
        if(val==1)
        {
            eflags=eflags | 0x200;
        }
        if(val==0)
        {
            eflags=eflags & 0xFFFFFDFF;
        }
    }
	if(strcmp("df",flag)==0)
    {
        if(val==1)
        {
            eflags=eflags | 0x400;
        }
        if(val==0)
        {
            eflags=eflags & 0xFFFFFBFF;
        }
    }
	if(strcmp("nt",flag)==0)
    {
        if(val==1)
        {
            eflags=eflags | 0x4000;
        }
        if(val==0)
        {
            eflags=eflags & 0xFFFFBFFF;
        }
    }
}

/*
*Sets the zero flag depending on the input
*/
void set_zflag(unsigned int ans)
{
	if(ans==0)//setting zero flag if result is zero
        {
            set_eflags("zf",1);
        }
        else
        {
            set_eflags("zf",0);
        }

}

/*
* Sets the sign flag depending on the sign of the input.
*/
void set_sflag(unsigned int ans)
{
	if((ans>>31)==1)//setting sign flag if result is negative
        {
            set_eflags("sf",1);
        }
        else
        {
            set_eflags("sf",0);
        }
}

/*
* Sets the parity flag if the number of ones in lower 8 bits is an even number. Otherwise clears.
*/
void set_pflag(unsigned int ans)//parity flag
{
	int itr=1;
	int count=0;
	for(itr;itr<=8;itr++)
	{
		if(ans & (1<<itr))
		{
			count++;
		}
	}
	if(count%2==0)
	{
		set_eflags("pf",1);
	}
	else
	{
		set_eflags("pf",0);
	}
}

