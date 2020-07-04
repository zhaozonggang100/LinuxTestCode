#include <stdio.h>

union dragon_union{
	int i;
	char c;
	int i1;
	char c1;
	long l;
	
};
//union只能一个变量有效！！！！
int main()
{
	union dragon_union dragon_union01;
	//printf("size:%d\n",sizeof(dragon_union));
	printf("size:%d\n",sizeof(dragon_union01));
	//dragon_union01.i=14444;
	//dragon_union01.i1=11;
	dragon_union01.c=12;
	//dragon_union01.c1=13;
	//dragon_union01.l=145;
	printf("dragon_union01.i:%d\n",dragon_union01.i);
	printf("dragon_union01.i:%d\n",dragon_union01.l);
	printf("dragon_union01.i:%d\n",dragon_union01.c);
	return 0;
}