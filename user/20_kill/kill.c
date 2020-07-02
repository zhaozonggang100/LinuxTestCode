#include <sys/types.h>
#include <stdio.h>
#include <signal.h>
 
 
int main()
{
		
pid_t pid=0;
printf("input pid you want to send SIGINT:");
scanf("%d",&pid);
kill(pid,SIGINT);
		
return 0;		
}
