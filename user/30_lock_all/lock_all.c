#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <errno.h>
#include <unistd.h>
#include <time.h>
#include <sys/time.h>  




#define COUNT 16
unsigned long sum =  0;

//添加互斥锁
pthread_mutex_t mutex;
pthread_mutex_t mutex01 = PTHREAD_MUTEX_INITIALIZER;//静态创建锁

//添加自旋锁
pthread_spinlock_t spinlock;

//添加读写锁
pthread_rwlock_t rwlock;


//时间回调函数
void sysLocalTime()  
{  
    time_t             timesec;  
    struct tm         *p;  

    time(&timesec);
    p = localtime(&timesec);  

    printf("%d%d%d%d%d%d\n", 1900+p->tm_year, 1+p->tm_mon, p->tm_mday, p->tm_hour, p->tm_min, p->tm_sec);  

} 

void sysUsecTime()  
{  
    struct timeval    tv;  
    struct timezone tz;  
    struct tm         *p;  

    gettimeofday(&tv, &tz);  
    //printf("tv_sec:%ld\n",tv.tv_sec);  
    //printf("tv_usec:%ld\n",tv.tv_usec);  
    //printf("tz_minuteswest:%d\n",tz.tz_minuteswest);  
    //printf("tz_dsttime:%d\n",tz.tz_dsttime);  
    p = localtime(&tv.tv_sec);  
    printf("time_now:%d/%d/%d %d:%d:%d.%3ld\n", 1900+p->tm_year, 1+p->tm_mon, p->tm_mday, p->tm_hour, p->tm_min, p->tm_sec, tv.tv_usec);  
}


void * thread(void *arg)
{

	int i=0;
#if 1
	//自旋锁 感觉耗时   死等 不做切换，临界区尽量小
	pthread_spin_lock(&spinlock);
    for(i=0; i< 100000000 ;i++)
	{
		sum += 1;
	}
	pthread_spin_unlock(&spinlock);
	
#elif 0

	//使用互斥锁 
	/*
	1 操作时间长 临界区大
	*/
	pthread_mutex_lock(&mutex);
    for(i=0; i< 100000000 ;i++)
	{
		sum += 1;
	}
	pthread_mutex_unlock(&mutex);
	
	
#elif 0

	//使用读的方式 加锁
	printf("....rdlock...\n");
	pthread_rwlock_rdlock(&rwlock);
	for(i=0; i< 100000000 ;i++)
	{
		sum += 1;
	}
	pthread_rwlock_unlock(&rwlock);


#elif 0

	//使用写的方式 加锁
	printf("....wlock...\n");
	pthread_rwlock_wrlock(&rwlock);
	for(i=0; i< 100000000 ;i++)
	{
		sum += 1;
	}
	pthread_rwlock_unlock(&rwlock);
	
	
	

#elif 0
	
	for(i=0; i< 100000000 ;i++)
	{
		sum += 1;
	}

#endif

	
}
int main()
{
	//初始化时间函数
	sysLocalTime();
	
	//打印此时的函数
	sysUsecTime();
	
	printf("before ... sum = %lu \n",sum);
	pthread_t pthread[COUNT];
	int ret;
	void *retval[COUNT];
	int i=0;
	
	//初始化互斥锁
	pthread_mutex_init(&mutex, NULL);
	
	//初始化自旋锁
	pthread_spin_init(&spinlock,PTHREAD_PROCESS_SHARED);
	
	//初始化读写锁
	pthread_rwlock_init(&rwlock, NULL);
	
	for(i =0; i<COUNT ;i++)
	{
		ret = pthread_create(&pthread[i],NULL,thread,NULL);

	}
	/*
	代码中如果没有pthread_join主线程会很快结束从而使整个进程结束，
	从而使创建的线程没有机会开始执行就结束了。加入pthread_join后，
	主线程会一直等待直到等待的线程结束自己才结束，使创建的线程有机会执行。
	*/
	
	
	//如果没有pthread_join主线程很快就退出了，导致子线程死亡
	for(i = 0; i< COUNT ; i++)
	{
		pthread_join(pthread[i],&retval[i]);//等待线程的结束-
	}
	//销毁一个互斥锁
	pthread_mutex_destroy(&mutex);
	//销毁一个自旋锁
	pthread_spin_destroy(&spinlock);
	//销毁读写锁
	pthread_rwlock_destroy(&rwlock);
	
	printf("after....sum [%lu]\n",sum);
	sysUsecTime();
	return 0;
}
