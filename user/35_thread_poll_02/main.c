#include "threadpool.h"

void *process(void *arg)
{
    printf("thread 0x%x working on task %d\n ",(unsigned int)pthread_self(),*(int *)arg);
    sleep(1);
    printf("task %d is end\n",*(int *)arg);
    return NULL;
}


int main(void)
{
    threadpool_t *thp = threadpool_create(3,100,100);    /*线程池里最小3个线程，最大100个，队列最大值12*/
    printf("pool inited");

    int *num = (int *)malloc(sizeof(int)*20);
    //int num[20];
    int i;
    for (i=0;i<10;i++)
    {
        num[i]=i;
        printf("add task %d\n",i);
        threadpool_add(thp,process,(void*)&num[i]);
    }
    sleep(10);
    threadpool_destroy(thp);
}