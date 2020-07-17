#include <stdio.h>

#define GPKCON 0x7f008800
#define GPKDAT 0x7f008808
 
void main()
{
    int i = 10;
    int a = i;
 
    printf("i = %d", a);
 /*
     __asm__ (
		"" 正常语法
		"" 正常语法
        :  输出部分
		:  输入部分
		:  破坏部分
    );
 */
    __asm__ ( 
		"ldr eax, =0x1110000\n" 
		"str eax, [%0]\n"
		"ldr eax, =0xa0\n"
		"str eax, [%1]\n"
        :
		:"=a"(GPKCON),"r"(GPKDAT)
		:"=eax"
    );
	
	return 0;
}