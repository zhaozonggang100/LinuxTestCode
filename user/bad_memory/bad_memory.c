#include <stdio.h>
#include <malloc.h>
#include <time.h>
int main()
{
	char *p;
	char *ppp ;
	p = malloc(10);
	p[100] = 7;//内存越界
	free(p);
	//free(p);//重复释放
	//int *p1 ;
	//*p1 = 22;//非法指针
	
	while(1)
	{
		sleep(1);
		ppp = malloc(1000);
		printf("nihao %p \n",ppp);
	}
	/*
	int *ptr = malloc(sizeof(int)*10);  
    ptr[10] = 7; // 内存越界  
    memcpy(ptr +1, ptr, 5); // 踩内存  
    free(ptr);   
    free(ptr);// 重复释放  
    int *p1;  
    *p1 = 1; // 非法指针
	*/
	printf("ha ha \n");
	return 0;
}