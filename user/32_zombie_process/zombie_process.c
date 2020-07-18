#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <time.h>
 
 
int main(int argc, char *argv[])
{
	int ret;
	if(fork() == 0)
	{
		printf("son [%d]\n",getpid());
		sleep(2);
	}
	else
	{
		printf("father [%d]\n",getpid());
		while(1)
		{
			sleep(1);
			//只有执行此函数 才能回收内存子进程的PCB
			//ret = wait();//返回的是子进程的PID
			printf("ret : %d\n",ret);
		}
	}
	return 0;
}