/*
 TODO: 解析表达式  计算表达式的值
*/
#ifndef CSVREADER_H
    #define CSVREADER_H
    #include "csvReader.h"
#endif
#include "expr_parse.h"
#include <stdlib.h>
#include <assert.h>
#include <ctype.h>
#define STDCAL(type) _cdecl type
#define STACKSIZE 200
typedef enum exprBool
{
	exprFalse,
	exprTrue,
}exprBool;
typedef struct TRstack
{
	char stack[STACKSIZE];
	size_t top;
	size_t size;
	size_t LP_count;//每当压入( 则加一 , 用于判定栈中有无(
}TRstack;

typedef union data
{
	double p_num;
	long i_num;
}data;
typedef struct num
{
	data data;
	char type;// 0 为pnum小数 1为inum整数
}num;
typedef struct OPstack
{
	num stack[STACKSIZE];
	size_t top;
	char pastfix[STACKSIZE];
}OPstack;

#define PUSH(StackObj,data) do{\
	StackObj.stack[StackObj.top] = data;\
	StackObj.top += 1;\
}while(0)

#define POP(StackObj,data) do{\
	data = StackObj.stack[StackObj.top -1];\
	StackObj.top -= 1;\
}while(0)
#define TOP(StackObj) (StackObj.stack[StackObj.top -1])
static STDCAL(int) parser_interge(const char* one_cell)//判断是否为整数
{
    if(*one_cell=='-')one_cell++;
    while (*one_cell!='\0')
    {
        if(!(*one_cell<='9'&&*one_cell>='0'))return -1;
        one_cell++;
    }
    return 0;
}
static STDCAL(int) parser_float(const char* one_cell)//判断是否为浮点数
{
    int count = 0;
    if(*one_cell=='-')one_cell++;
    while (*one_cell!='\0'&&count<=1)
    {
        if(*one_cell=='.')count++;
        else if(!(*one_cell<='9'&&*one_cell>='0'))return -1;
        one_cell++;
    }
    if(count!=1)return -1;
    return 0;
}
STDCAL(num) parse_num(const char** pptr)//当遇到一个数字时进入该进程
{
	num ret;
	char buf[51] = {'\0'};
	size_t bufIter = 0;
	const char* parser = *pptr;
	while (((*parser <='9' && *parser >= '0') || *parser == '.' || *parser == '-') && bufIter < 50)
	{
		buf[bufIter] = *parser;
		bufIter++;
		parser++;
	}
	if(!parser_interge(buf))
	{
		ret.type = 1;
		ret.data.i_num = atoi(buf);
		assert(ret.data.i_num != 0);
	}
	else if(!parser_float(buf))
	{
		ret.type = 0;
		ret.data.p_num = atof(buf);
		assert(ret.data.p_num != 0.0);
	}
	assert(bufIter < 50);

	*pptr = parser - 1;
	return ret;
}
STDCAL(int) Prec_get(char op)
{
	int ret = 0;
	switch (op)
	{
	case '\0':
		ret = 0;
		break;
	case '+':
	case '-':
		ret = 5;
		break;
	case '*':
	case '/':
	case '%':
		ret = 6;
		break;
	case '(':
	case ')':
		ret = 128;
	default:
		ret = 0;
		break;
	}
	return ret;
}
STDCAL(exprBool) Prec_judge(char op1, char op2)
{
	int prec1 = 0;
	int prec2 = 0;
	prec1 = Prec_get(op1);
	prec2 = Prec_get(op2);
	return prec1 >= prec2 ? exprTrue : exprFalse;
}
STDCAL(void) cacl(OPstack* stkobj,char op)//计算栈中元素与op的映射
{
	putchar(op);
	return;
}
STDCAL(void) eval(const char* expr)
{
	OPstack numStack;//计算栈
	numStack.top = 0;
	memset(numStack.pastfix,'\0',STACKSIZE);
	TRstack infixStack;//中缀转后缀栈
	infixStack.LP_count = 0;
	infixStack.size =STACKSIZE;
	infixStack.top = 0;
	memset(infixStack.stack,'\0',STACKSIZE);
	////////////////////////////////////////////////////init
	const char* parser = expr;
	char ch = '\0';
	while(*parser != '\0')
	{
		ch = *parser;
		switch (ch)
		{
			case '-':
			{
				if (parser == expr) goto num_token; //若第一个位置是-则认为是负数标识
				else if (isdigit(*(parser-1)) || *(parser-1) ==')') goto operater_token;//若-前面是数字或右括号(左括号不满足)则认为他是一个操作符  多一个分支是为了避免访问位置内存
				else goto num_token;
				break;
			}
			case '0':
			case '1':
			case '2':
			case '3':
			case '4':
			case '5':
			case '6':
			case '7':
			case '8':
			case '9':
			num_token: //用于处理 - 是负数标识时会跳转至此
			{ 
					num OpNum = parse_num(&parser);
					PUSH(numStack,OpNum);
					if(OpNum.type == 0)printf("%lf",OpNum.data.p_num);
					else if(OpNum.type == 1)printf("%ld",OpNum.data.i_num);
				break;
			}
			case '(':
			{
				infixStack.LP_count++;
				PUSH(infixStack,'(');
				break;
			}
			case ')':
			{
				assert(infixStack.LP_count > 0);
				char pop_ret = '\0';
				while(TOP(infixStack) != '(')
				{
					POP(infixStack,pop_ret);
					cacl(&numStack,pop_ret);
				}
				POP(infixStack,pop_ret);
				break;
			}
			case '+':
			case '/':
			case '*':
			case '%':
			{
				operater_token: //用于处理 - 时运算符时会跳转至此
					while(Prec_judge(TOP(infixStack),ch))
					{
						char pop_ret = '\0';
						POP(infixStack,pop_ret);
						cacl(&numStack,pop_ret);
					}
					PUSH(infixStack,ch);
				break;
			}
		}
		parser++;
	}
	while(infixStack.top > 0)
	{
		char pop_ret = '\0';
		POP(infixStack,pop_ret);
		cacl(&numStack,pop_ret);
	}
}
int main(int argc, char const *argv[])
{
	eval("(-10+20)+30*500");
	return 0;
}
