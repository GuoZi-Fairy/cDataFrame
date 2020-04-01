#include <stdio.h>
#include <ctype.h>

typedef union __data
{
	double p_num;
	long long i_num;
}data;
typedef struct __num
{
	data data;
	char type;// 0 为pnum小数 1为inum整数
}num;
int main(int argc, char const *argv[])
{
    
    return 0;
}