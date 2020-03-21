#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#define STDCAL(type) type __cdecl
typedef struct stack
{
	char* stack;
	size_t top;
	size_t size;
	char output[200];
}stack;
STDCAL(void) stack_init(stack* stkobj)
{
	stkobj->stack = (char*)malloc(256);
	stkobj->size = 256;
	stkobj->top = 0;
	memset(stkobj->output,'\0',200);
}
STDCAL(void) stack_push(stack* stackobj,char ch)
{
	if(stackobj->size <= stackobj->top + 1){
		while(stackobj->size <= stackobj->top + 1)stackobj->size += stackobj->size >> 1;
		stackobj->stack = (char*)realloc(stackobj->stack,stackobj->size);
	}
	char* ret = 0;
	ret = stackobj->stack + stackobj->top;
	*ret = ch;
	stackobj->top += 1;
}
STDCAL(char) stack_pop(stack* stackobj)
{
	if(stackobj->top == -1)
	{
		stackobj->top = 0;
		return '\0';
	}
	char ret = *(stackobj->stack + stackobj->top -1);
	char* modifier = stackobj->stack + stackobj->top -1;
	*modifier = '\0';
	stackobj->top -= 1;
	return ret;
}
STDCAL(int) comp_priority(char token1,char token2)
{
	int tk1 = 0;
	int tk2 = 0;
	if(token1 == '+' || token1 == '-')tk1 = 1;
	else tk1 = 2;
	if(token2 == '+' || token2 == '-')tk2 = 1;
	else tk2 = 2;
	return tk1>=tk2 ? 1 : 0;
}
STDCAL(void) Infix2Postfix(char* infix,stack* stkobj)
{
	size_t iter = 0;
	char* s_ptr = infix;
	memset(stkobj->output,'\0',200);
	while (*s_ptr != '\0')
	{
		if(*s_ptr >= 'a' && *s_ptr <= 'z')//变量
		{
			stkobj->output[iter] = *s_ptr;
			s_ptr++;
			iter++;
		}
		else
		{
			char ch = *(stkobj->stack + stkobj->top -1); //栈中符号
			if(ch =='\0')
			{
				stack_push(stkobj,*s_ptr);
				s_ptr++;
				goto out;
			}
			else
			{
				while(comp_priority(ch,*s_ptr))
				{
					stkobj->output[iter] = ch;
					stack_pop(stkobj);
					ch = *(stkobj->stack + stkobj->top -1);
					iter++;
					if(ch =='\0')
					{
						stack_push(stkobj,*s_ptr);
						s_ptr++;
						goto out;
					}
				}
				stack_push(stkobj,*s_ptr);
				s_ptr++;
			}
		}
		out:;
	}
	char ch = stack_pop(stkobj);
	while (ch != '\0')
	{
		stkobj->output[iter] = ch;
		iter++;
		ch = stack_pop(stkobj);
	}
}
int main(int argc, char const *argv[])
{
	stack test;
	test.stack = (char*)malloc(256);
	test.size = 256;
	test.top  = 0;
	memset(test.output,'\0',200);
	Infix2Postfix("a+b*c+d+h/e",&test);
	printf("%s",test.output);
	free(test.stack);
	return 0;
}
