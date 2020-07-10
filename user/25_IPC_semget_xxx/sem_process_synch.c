#include <stdio.h>  
#include <stdlib.h>  
#include <unistd.h>  
#include <string.h>  
#include <signal.h>
#include <math.h>
#include <sys/time.h>
#include <sys/types.h>   
#include <netinet/in.h>  
#include <pthread.h>
#include <semaphore.h>

int ar[10] = {1,2,3,4,5,6,7,8,9,10};
sem_t s;

void *do_bank(void *arg)
{
	int i;
	//P(s);
	sem_wait(&s);
	//printf("[%d]starting....\n",*(int *)arg);
	printf("[%d]starting....\n",(int )arg);
	for(i =0 ;i<5 ;i++)
	{
		//printf("%d ",*(int *)arg);
		fflush(stdout);//强制刷新
		sleep(1);
	}
	printf("done.\n");
	sem_post(&s);
	//V(s);
}
int main()
{
	int res,i;
	res = sem_init(&s,0,2);//设置3个共享资源
	
	pthread_t t[5];
	void *thread_result;
	for(i=0 ;i<10 ;i++)
	{
		//res = pthread_create(&t[i] , NULL, do_bank ,(void *)&ar[i]);
		res = pthread_create(&t[i] , NULL, do_bank ,(void *)i);
	}
	
	for(i=0 ;i<10 ;i++)
	{
		pthread_join(t[i],NULL);
	}
	
	
	sem_destroy(&s);
	
	return 0;
}