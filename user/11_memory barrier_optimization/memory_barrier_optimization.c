#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>

#define barrier() __asm__ __volatile__("": : :"memory")
volatile int ready;
//int ready;
int buf[100];

void test(int i)
{
	buf[i] = 50;
	//barrier();
	ready = 1;
	ready++;
	ready++;
	ready++;
	ready++;
	printf("printf :%d\n",ready);
}
int main()
{
	int m=10;
	//printf("[1] %d %d %d \n",m, buf[m],ready);
	test(m);
	//printf("[1] %d %d %d \n",m, buf[m],ready);
	
}