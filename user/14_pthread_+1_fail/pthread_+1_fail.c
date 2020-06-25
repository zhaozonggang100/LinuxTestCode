#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <errno.h>
#include <unistd.h>
#include <time.h>

#define COUNT 16

unsigned long sum =  0;
//volatile unsigned long sum =  0;
pthread_mutex_t mymutex=PTHREAD_MUTEX_INITIALIZER;

void * thread(void *arg)
{
	pthread_mutex_lock(&mymutex);
	int i=0;
	for(i = 0;i<10000; i++)
	{
		sum+=1;
	}
	pthread_mutex_unlock(&mymutex);
}
int main()
{
	printf("before ... sum = %lu \n",sum);
	pthread_t pthread[COUNT];
	int ret;
	void *retval[COUNT];
	int i=0;
	for(i =0; i<COUNT ;i++)
	{
		ret = pthread_create(&pthread[i],NULL,thread,NULL);
		//sleep(1);
		if(ret != 0)
		{
			perror("cause:");
			printf("creat ptread %d failed \n",i+1);
		}
	}
	sleep(2);
	
	/*
	代码中如果没有pthread_join主线程会很快结束从而使整个进程结束，
	从而使创建的线程没有机会开始执行就结束了。加入pthread_join后，
	主线程会一直等待直到等待的线程结束自己才结束，使创建的线程有机会执行。
	*/
	
	/*
	//如果没有pthread_join主线程很快就退出了，导致子线程死亡
	for(i = 0; i< COUNT ; i++)
	{
		pthread_join(pthread[i],&retval[i]);//等待线程的结束-
	*/
	printf("after....sum [%lu]\n",sum);
	return 0;
}
