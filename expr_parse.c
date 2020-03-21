/*
 TODO: 解析表达式  计算表达式的值
*/
#include "expr_parse.h"
#include <stdlib.h>
#define STDCAL(type) _cdecl type
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