#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/sem.h>
 
union semun
{
    int val;
    struct semid_ds *buf;
    unsigned short *arry;
};
 
static int sem_id = 0;
static int set_semvalue();
static void del_semvalue();
static int semaphore_p();
static int semaphore_v();
 

 
static int set_semvalue()
{
    //用于初始化信号量，在sem_union的val字段中设置信号量初值。使用信号量之前必须先初始化！
    union semun sem_union;
    sem_union.val = 1;
	//semctl 初始化信号量 函数
	/*
	semctl(sem_id, sem_num, command, semun)：控制信号量。
	commond中有：SETVAL 初始化信号量为一个值，该值再semun结构体的val字段；
	IPC_RMID用于删除一个无需继续使用的信号量
	*/
    if(semctl(sem_id, 0, SETVAL, sem_union) == -1)
        return 0;
    return 1;
}



static void del_semvalue()
{
    //删除信号量
	//IPC_RMID 删除操作
    union semun sem_union;
    if(semctl(sem_id, 0, IPC_RMID, sem_union) == -1)
        printf("Failed to delete semaphore\n");
}
 
static int semaphore_p()
{
    //对信号量做减1操作，即加锁 P（sv）
    struct sembuf sem_b;
    sem_b.sem_num = 0;
    sem_b.sem_op = -1;   //P()
    sem_b.sem_flg = SEM_UNDO;
	
	/*
	semop(sem_id, sem_opa, num_sem_ops)：改变信号量的值，改变操作在sem_opa中，sem_opa是sumbuf结构体对象，使用方法如下：
	
	struct sembuf
	{  
		short sem_num;   //除非使用一组信号量，否则它为0  
		short sem_op;    //信号量在一次操作中需要改变的数据，通常是两个数，一个是-1，即P操作（加锁）；一个是+1，即V操作（解锁）
		short sem_flg;   //通常为SEM_UNDO,使操作系统跟踪信号，并在进程没有释放该信号量而终止时，操作系统释放信号量  
	};
		
	
	*/
    if(semop(sem_id, &sem_b, 1) == -1)
    {
        printf("semaphore_p failed\n");
        return 0;
    }
    return 1;
}
 
static int semaphore_v()
{
    //这是一个释放操作，它使信号量变为可用，即解锁 V（sv）
    struct sembuf sem_b;
    sem_b.sem_num = 0;
    sem_b.sem_op = 1;   //V()
    sem_b.sem_flg = SEM_UNDO;
    if(semop(sem_id, &sem_b, 1) == -1)
    {
        printf("semaphore_v failed\n");
        return 0;
    }
    return 1;
}


int main(int argc, char *argv[])
{
    char message = 'X';
    int i = 0;
 
    //创建信号量 
	//(key_t)1234 键值  1(个数)
    sem_id = semget((key_t)1234, 1, 0666 | IPC_CREAT);
 
    
	
	
	
	if(argc > 1)
    {
        //程序第一次被调用，初始化信号量
        if(!set_semvalue())
        {
            fprintf(stderr, "Failed to initialize semaphore\n");
            exit(EXIT_FAILURE);
        }
        //设置要输出到屏幕中的信息，即其参数的第一个字符
        message = argv[1][0];
        sleep(2);
    }
	
	printf("***********\n");
    for(i = 0; i < 5; ++i)
    {
        //进入临界区
        if(!semaphore_p())//进入临界区 p操作 相当于加锁资源减少1 
            exit(EXIT_FAILURE);
        
		
		//向屏幕中输出数据
        printf("zhaozonggang %d\n", message++);
        //清理缓冲区，然后休眠随机时间
        fflush(stdout);
        //离开临界区，休眠随机时间后继续循环
        if(!semaphore_v())//离开临界区，v操作 相当与资源加1
            exit(EXIT_FAILURE);
        sleep(1);
    }
 
    sleep(3);
    printf("***********[%d]\n", getpid());
 
    if(argc > 1)
    {
        //如果程序是第一次被调用，则在退出前删除信号量
        sleep(1);
        del_semvalue();
    }
    exit(EXIT_SUCCESS);
}