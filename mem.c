#include <stdlib.h>
#include <stdio.h>
#include "IA32.h"
#include "operations.h"

/*
*This file emulates the memory for the x86 emulator.
* COnsists of a linked list implementation that can be used as memory and a linked list stack implementation.
*/

item * curr, * head;

/*
*Stores data value with the corresponding address in the linked list.
*/
void mem_store(unsigned int addr, unsigned int data)
{
	if(head==NULL)
	{
		head=(item *)malloc(sizeof(item));
		head->val=data;
		head->address=addr;
		head->next=NULL;
	} 	
	else
	{
		curr = (item *)malloc(sizeof(item));
		curr->val=data;
		curr->address=addr;
		curr->next=head;
		head=curr;
	}
}

/*
* Searches through the linked list to find the required memory address. If not found, returns zero and creates a memory location for the corresponding addess in the linked list.
*/
unsigned int mem_retrieve(unsigned int addr)
{
	curr = head;
	while(curr)
	{
		if(curr->address==addr)
		{
			return curr->val;
		}
		curr=curr->next;
	}
	mem_store(addr,0);
	return 0;
}

/*
*Prints contents of all registers.
*/
void print_mem_reg()
{
	printf("\n-----------Register Contents--------------\n");
	printf("EAX 	: %x \n",eax);
	printf("ECX 	: %x \n",ecx);
	printf("EBX 	: %x \n",ebx);
	printf("EDX	: %x \n",edx);
	printf("ESP 	: %x \n",esp);
	printf("EBP 	: %x \n",ebp);
	printf("ESI 	: %x \n",esi);
	printf("EDI 	: %x \n",edi);
	printf("EIP 	: %x \n",eip);
	printf("EFLAGS	: %x \n",eflags);
	printf("CS 	: %x \n",cs);
	printf("SS 	: %x \n",ss);
	printf("DS 	: %x \n",ds);
	printf("ES 	: %x \n",es);
	printf("FS 	: %x \n",fs);
	printf("GS 	: %x \n",gs);

	printf("\n-----------Memory Contents--------------\n");
	curr = head;
   	while(curr) 
	{
		printf("Address: %x  Value: %x\n",curr->address,curr->val);
		curr=curr->next;
	}


}

item * stack_ele, * stack_top;

//Stack implementation
void stack_push(unsigned int element)
{
	if(stack_top==NULL)
	{
		stack_top=(item *)malloc(sizeof(item));
        stack_top->val=element;
        stack_top->next=NULL;
	}	
	else
	{
		stack_ele=(item *)malloc(sizeof(item));
		stack_ele->val=element;
		stack_ele->next=stack_top;
		stack_top=stack_ele;
	}
} 

unsigned int stack_pop()
{
	if(stack_top!=NULL)
	{
		stack_ele=stack_top;
		stack_top=stack_top->next;
		return stack_ele->val;
	}
	else
	{
		printf("\nSTACK EMPTY!!\n");
		return 0;
	}	
}
//Prints the contants of the top element of the stack
void print_stack_top()
{
	if(stack_top!=NULL)
	{
		printf("Stack Top: %x \n",stack_top->val);

	}
	else
	{
		printf("Stack Empty!\n");

	}
}	
