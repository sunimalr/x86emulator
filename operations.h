//x86 operations header

#ifndef X86OPERATIONS
#define X86OPERATIONS

extern void op_leal(instr ins,unsigned int input,FILE *file);
extern void op_andl_subl(instr ins, unsigned int input,FILE *file);
extern void op_pushl(instr ins, unsigned int input, FILE *file);
extern void op_shift(instr ins, unsigned int input, FILE *file);
extern void op_xor(instr ins, unsigned int input, FILE *file);
extern void op_mov(instr ins, unsigned int input, FILE *file);
extern void op_inc(instr ins, unsigned int input, FILE *file);
extern void op_add_or_adc_sbb_and_sub_xor_cmp(instr ins, unsigned int input,FILE *file);
extern void op_popl(instr ins, unsigned int input, FILE *file);
extern unsigned int getnext(FILE *fp);
extern instr parse_mod_rm(instr ins, unsigned int input, FILE *file);
extern instr parse_sib(instr ins, unsigned int input,FILE *file);
//extern unsigned int get_memory_contents(unsigned int address);
extern char* get_register_name(int code);
extern unsigned int get_register_content(int code);
extern void set_register_content(int code, unsigned int data);
extern void mem_store(unsigned int addr, unsigned int data);
extern unsigned int mem_retrieve(unsigned int addr);
extern void print_mem_reg();
extern void set_eflags(char *flag, int val);
extern void set_zflag(unsigned int ans);
extern void set_sflag(unsigned int ans);
extern void set_pflag(unsigned int ans);
extern void stack_push(unsigned int element);
extern unsigned int stack_pop();
extern void print_stack_top();

#endif
