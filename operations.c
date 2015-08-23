/*
*x86 operations
*Author: sunimalr@comp.nus.edu.sg
*/

#include "IA32.h"
#include <stdio.h>
#include <stdlib.h>
#include "operations.h"

/*
*leal operation. opcode : 0x8d
*Has four addressing modes depending on the MOD value of MODRM byte.
*/
void op_leal(instr ins, unsigned int input, FILE *file)
{
	ins=parse_mod_rm(ins, getnext(file), file);
	
	if(ins.MODRM.mod==0x01)//Memory location is calculated by BASE + SCALE*INDEX + DISPLACEMENT
    {
		printf("\nLEAL");
        printf(" %s ",get_register_name(ins.MODRM.reg));
		printf("[%s + %s*%d + 0x%x]",get_register_name(ins.SIB.base),get_register_name(ins.SIB.index),ins.SIB.scale,ins.SIB.displacement);
		set_register_content(ins.MODRM.reg,mem_retrieve(get_register_content(ins.SIB.base)+get_register_content(ins.SIB.index)*ins.SIB.scale+ins.SIB.displacement));
	}
    else if(ins.MODRM.mod==0x00)//Memory location is calculated by BASE + INDEX*SCALE
    {
		printf("\nLEAL");
        printf(" %s ",get_register_name(ins.MODRM.reg));
        printf("[%s + %s*%d]",get_register_name(ins.SIB.base),get_register_name(ins.SIB.index),ins.SIB.scale);
		set_register_content(ins.MODRM.reg,mem_retrieve(get_register_content(ins.SIB.base)+get_register_content(ins.SIB.index)*ins.SIB.scale));
    }
    else if(ins.MODRM.mod==0x03)//Only registers are invloved in the operation
    {
		printf("\nLEAL");
        printf(" %s ",get_register_name(ins.MODRM.reg));
        printf(", %s",get_register_name(ins.MODRM.rm));
		set_register_content(ins.MODRM.reg,mem_retrieve(get_register_content(ins.MODRM.rm)));
    }
    else if(ins.MODRM.mod==0x02)//Same as 0x01, but displacement is 32bit
    {
        printf("\nLEAL");
        printf(" %s ",get_register_name(ins.MODRM.reg));
        printf("[%s + %s*%d + 0x%x]",get_register_name(ins.SIB.base),get_register_name(ins.SIB.index),ins.SIB.scale,ins.SIB.displacement);
		set_register_content(ins.MODRM.reg,mem_retrieve(get_register_content(ins.SIB.base)+get_register_content(ins.SIB.index)*ins.SIB.scale+ins.SIB.displacement));
    }
}

/*
* andl and subl operations.
* Opcode : 0x83
* andl is executed when MODRM Reg=0x04
* subl is executed when MODRM Reg=0x05
*/
void op_andl_subl(instr ins, unsigned int input, FILE *file)
{
	ins=parse_mod_rm(ins, getnext(file), file);
	if(ins.MODRM.reg==0x4)//ANDL
	{
		unsigned int tmp=getnext(file);
		if(tmp & (1<<7))//Checking the sign bit
        {
            tmp=tmp|0xFFFFFF00;//Sign extending (2's complement)
        }
		unsigned int ans=(tmp & get_register_content(ins.MODRM.rm));
		printf("\nANDL");
		printf(" %s ",get_register_name(ins.MODRM.rm));
		printf(" %x ",tmp);
		set_register_content(ins.MODRM.rm,ans);
		set_eflags("of",0);//Overflow and carry flags are cleared
		set_eflags("cf",0);
		set_sflag(ans);//SF, ZF and PF flags are set according to the answer
		set_zflag(ans);
		set_pflag(ans);
				
	}
	
	if(ins.MODRM.reg==0x05)//SUBL
	{
		unsigned int tmp=getnext(file);
		if(tmp & (1<<7))//Checking sign bit
		{
			tmp=tmp|0xFFFFFF00;//Sign extension (2's complement)
		}
		unsigned int ans=(get_register_content(ins.MODRM.rm)-tmp);
        printf("\nSUBL");
        printf(" %s ",get_register_name(ins.MODRM.rm));
        printf(" %x ",tmp);
		set_register_content(ins.MODRM.rm,ans);
		if((tmp & (1<<31))==(ans & (1<<31)))//Checking for overflow and carry.
		{
			if((tmp & (1<<31))!=(get_register_content(ins.MODRM.rm) & (1<<31)))
			{
				set_eflags("of",1);
				set_eflags("cf",1);
			}
		}
			else
			{
				set_eflags("of",0);
				set_eflags("cf",0);
			}
		set_sflag(ans);//SF, ZF and PF flags are set according to the answer
        set_zflag(ans);
        set_pflag(ans);
	}
}

/*
* pushl operation. 
* Opcode : 0x50
*/
void op_pushl(instr ins, unsigned int input, FILE *file)
{
	//Pushes the value is the specified register onto the stack
	printf("\nPUSHL %s",get_register_name((input & 0x07)));
	stack_push(get_register_content(input & 0x07));		
}

/*
* shift operation
* Opcode : 0xc1
* Depending on the value of MODRM reg, the mode of shift varies
*/
void op_shift(instr ins, unsigned int input, FILE *file)
{	ins=parse_mod_rm(ins, getnext(file), file);
	if(ins.MODRM.reg==0x07)//SAR operation
	{
		unsigned int tmp=getnext(file);
		unsigned int reg_val=get_register_content(ins.MODRM.rm);
		set_eflags("cf",(reg_val & (1<<(tmp-1))));//Carry flag contains the last bit shifted out by the operation
		unsigned int ans=reg_val>>tmp;
		set_register_content(ins.MODRM.rm,ans);
		printf("\nSAR %s, 0x%x",get_register_name(ins.MODRM.rm),tmp);
		set_eflags("of",0);//Overflow flag is cleared
		set_sflag(ans);//PF. SF, and ZF are set according to the answer.
		set_pflag(ans);
		set_zflag(ans);
	}
}

/*
* xor operation
* Opcode : 0x34
*/
void op_xor(instr ins, unsigned int input, FILE *file)
{
	unsigned int tmp=getnext(file);
	unsigned int ans=get_register_content(0x00)^tmp;//AL register is used for this operation. AL is the lower part of the EAX register.
	set_register_content(0x00,ans);
	printf("\nXOR AL, 0x%x",tmp);
	set_eflags("of",0);//Overflow and carry bits are cleared
	set_eflags("cf",0);
	set_sflag(ans);//PF, SF and ZF are set according to the answer
    set_zflag(ans);
    set_pflag(ans);
}

/*
* movl operation
* Opcode : 0x89
*  
*/
void op_mov(instr ins, unsigned int input, FILE *file)
{
	ins=parse_mod_rm(ins, getnext(file), file);
	printf("\nMOV %s, %s",get_register_name(ins.MODRM.rm),get_register_name(ins.MODRM.reg));
	set_register_content(ins.MODRM.rm,get_register_content(ins.MODRM.reg));	
	//No flags affected
}

/*
* inc operation
* Opcode : 0x40 - 0x47
*  
*/
void op_inc(instr ins, unsigned int input, FILE *file)
{
	printf("\nINC %s",get_register_name(input & 0x07));
	unsigned int ans=(get_register_content(input & 0x07)+1);//Increment by 1
	set_register_content((input & 0x07),ans);
	
	if(((get_register_content(input & 0x07) & (1<<31))==0) && ((ans & (1<<31))==1))//Overflow and Carry are calculated
    {
    	set_eflags("of",1);
    	set_eflags("cf",1);
    }
    else
    {
    	set_eflags("of",0);
    	set_eflags("cf",0);
    }
	set_sflag(ans);//eflags are set according to answer
	set_zflag(ans);
	set_pflag(ans);	
}

/*
* add, or, adc, sbb, and, sub, xor, cmp operations
* Opcode : 0x80
*/
extern void op_add_or_adc_sbb_and_sub_xor_cmp(instr ins, unsigned int input,FILE *file)
{
	ins=parse_mod_rm(ins, getnext(file), file);
	if(ins.MODRM.reg==0x07)//If reg is 0x07 it is CMP operation
	{// The addessing mode varies depending on the mod value of the MODRM byte
		unsigned int tmp;
		unsigned int op1,op2,diff;
		printf("\nCMP");
		if(ins.MODRM.mod==0x01)
    	{	
        	printf("[%s + %s*%d + 0x%x]",get_register_name(ins.SIB.base),get_register_name(ins.SIB.index),ins.SIB.scale,ins.SIB.displacement);//Memory location is calculated by BASE + SCALE*INDEX + DISPLACEMENT
			tmp=getnext(file);
			if(tmp & (1<<7))
            {
                tmp=tmp|0xFFFFFF00;//Sign extension
            }

			op1=mem_retrieve(get_register_content(ins.SIB.base)+get_register_content(ins.SIB.index)*ins.SIB.scale+ins.SIB.displacement);
            op2=tmp;
            diff=op1-op2;
			printf(" 0x%x ",tmp);
			if((op2 & (1<<31))==(diff & (1<<31)))//Checking for overflow
        	{
            if((op2 & (1<<31))!=(op1 & (1<<31)))
            {
                set_eflags("of",1);
                set_eflags("cf",1);
			}
       		}
        	else
        	{
            set_eflags("of",0);
            set_eflags("cf",0);
        	}
        	set_sflag(diff);
        	set_zflag(diff);
        	set_pflag(diff);	
		}		
    	else if(ins.MODRM.mod==0x00)
   		{   //Memory location is calculated by BASE + SCALE*INDEX
			tmp=getnext(file);
        	printf("[%s + %s*%d]",get_register_name(ins.SIB.base),get_register_name(ins.SIB.index),ins.SIB.scale);
			if(tmp & (1<<7))
            {
                tmp=tmp|0xFFFFFF00;
            }
			printf(" 0x%x ",tmp);
            op1=mem_retrieve(get_register_content(ins.SIB.base)+get_register_content(ins.SIB.index)*ins.SIB.scale);
            op2=tmp;
			diff=op1-op2;
			if((op2 & (1<<31))==(diff & (1<<31)))
            {
            if((op2 & (1<<31))!=(op1 & (1<<31)))
            {
                set_eflags("of",1);
                set_eflags("cf",1);
            }
            }
            else
            {
            	set_eflags("of",0);
            	set_eflags("cf",0);
            }
            set_sflag(diff);
            set_zflag(diff);
            set_pflag(diff);
		}
    	else if(ins.MODRM.mod==0x03)
    	{//Only registers are invloved in the operation
			tmp=getnext(file);
			if(tmp & (1<<7))
            {
                tmp=tmp|0xFFFFFF00;
            }
			op1=get_register_content(ins.MODRM.rm);
			op2=tmp;
			diff=op1-op2;
            if((op2 & (1<<31))==(diff & (1<<31)))
            {
            if((op2 & (1<<31))!=(op1 & (1<<31)))
            {
                set_eflags("of",1);
                set_eflags("cf",1);
            }
            }
            else
            {
                set_eflags("of",0);
                set_eflags("cf",0);
            }
            set_sflag(diff);
            set_zflag(diff);
            set_pflag(diff);
        	printf(", %s",get_register_name(ins.MODRM.rm));
    		printf(" 0x%x ",tmp);
		}
    	else if(ins.MODRM.mod==0x02)
    	{//Memory location is calculated by BASE + SCALE*INDEX + DISPLACEMENT
        	printf("[%s + %s*%d + 0x%x]",get_register_name(ins.SIB.base),get_register_name(ins.SIB.index),ins.SIB.scale,ins.SIB.displacement);
			tmp=getnext(file);
            if(tmp & (1<<7))
            {
                tmp=tmp|0xFFFFFF00;
            }

            op1=mem_retrieve(get_register_content(ins.SIB.base)+get_register_content(ins.SIB.index)*ins.SIB.scale+ins.SIB.displacement);
            op2=tmp;
            diff=op1-op2;
            printf(" 0x%x ",tmp);
            if((op2 & (1<<31))==(diff & (1<<31)))
            {
            	if((op2 & (1<<31))!=(op1 & (1<<31)))
            	{
               		set_eflags("of",1);
                	set_eflags("cf",1);
            	}
            }
            else
            {
            	set_eflags("of",0);
            	set_eflags("cf",0);
            }
            set_sflag(diff);
            set_zflag(diff);
            set_pflag(diff);
		}
	}
}

/*
* popl operation
* Opcode : 0x5f
*/
void op_popl(instr ins, unsigned int input, FILE *file)
{
    printf("\nPOPL %s",get_register_name((input & 0x07)));
    set_register_content((input & 0x07),stack_pop());
	set_register_content(0x04,get_register_content(0x04)+4);
}

/*
* Parsing the MODRM byte
*/
instr parse_mod_rm(instr ins, unsigned int input, FILE *file)
{
	int mod_val=(input >> 6);
	int reg_val=(input & 0x38) >> 3 ;
	int rm_val=(input & 0x07);
	printf("MODRM=%x\tMOD=%x\t REG=%x\t RM=%x\n",input,mod_val,reg_val,rm_val);
	
	ins.MODRM.mod=mod_val;
	ins.MODRM.reg=reg_val;
	ins.MODRM.rm=rm_val;
	switch(ins.MODRM.mod)
	{
		case 0x00:
		case 0x01:
		case 0x02:
			if(rm_val==0x04)
			{ //If mod is either 00, 01 or 02 and rm is 04, the SIB byte should be evaluated.
				ins=parse_sib(ins, getnext(file),file);
				printf("disp = %x\n",ins.SIB.displacement);
			}
			else 
			if(rm_val==0x05)
			{
				//Reads 32 bit value
			}	
			break;		
	}

	return ins;	
		
}

instr parse_sib(instr ins, unsigned int input, FILE *file)
{
	printf("\nSIB=%x\t",input);
	int scale_val=(input >> 6);
	int index_val=(input & 0x38) >> 3 ;
	int base_val=(input & 0x07);
	printf("SCALE=%x\tINDEX=%x\tBASE=%x\t",scale_val,index_val,base_val);

	ins.SIB.base=base_val;
	ins.SIB.index=index_val;
	ins.SIB.scale=scale_val;
	
	if(ins.MODRM.mod==0x01)//If mod is 0x01, displacement is 8bit.
	{
		ins.SIB.displacement=getnext(file);
	}
	else if(ins.MODRM.mod==0x02)//If mod is 0x02, displacement is 32 bit
	{
		ins.SIB.displacement = (getnext(file)<<24) | (getnext(file)<<16)| (getnext(file)<<8) | getnext(file);
	}
	else if(ins.MODRM.mod==0x03)//If mod is 0x03, this does not apply
	{
	}
	else if(ins.MODRM.mod==0x00)//If mod is 0x00, there is no displacement.
	{
		ins.SIB.displacement=0x00;
	}
	return ins;
}






