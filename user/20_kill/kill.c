#include <sys/types.h>
#include <stdio.h>
#include <signal.h>
 
 
int main()
{
		
pid_t pid=0;
int i;
printf("input pid you want to send SIGINT:");
scanf("%d",&pid);
while(1)
{
	/*
	for(i=1 ; i<64 ;i++)
	{
		kill(pid,i);

	}
	*/
	kill(pid,40);
	//sleep(1);
	//printf("*****[%d]\n",i++);
}

		
return 0;		
}
