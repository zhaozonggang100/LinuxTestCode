#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>

/*
例子：#define barrier() __asm__ __volatile__("": : :"memory")
#define __asm__ asm
#define __volatile__ volatile
__asm__(汇编语句模板: 输出部分: 输入部分: 破坏描述部分)
*/
int main()
{
	char *p = NULL;
	int cr0 = 10;
	printf(" i = %d \n",cr0);
	__asm__ __volatile__("movl %0, %%cr0"::"a" (cr0)); 
	return 0;
}