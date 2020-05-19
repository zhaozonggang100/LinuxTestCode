#include <stdio.h>
#include <malloc.h>
#include <time.h>
int main()
{
	char *p = NULL;
	while(1)
	{
		sleep(30);
		p = malloc(100*1024);
		printf("padd： %p \n",p);
	}
	return 0;
}