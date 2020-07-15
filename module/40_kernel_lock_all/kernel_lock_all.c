#include <linux/module.h>
#include <linux/kthread.h>
#include <linux/delay.h>
#include <linux/semaphore.h>

#define count_d 2

//声明一个自旋锁
spinlock_t lock;

//声明一个互斥锁
struct mutex mutex;

//声明一个信号量
struct semaphore sema;

//声明读写锁
rwlock_t rwlock;


static struct task_struct *slam_thread[count_d];
unsigned long num = 0;
 
static int slam_func(void *data)
{
	
	unsigned int i =0;
#if 0 	

	spin_lock(&lock);//获取自旋锁
	for(i=0 ; i< 10000001 ;i++)
	{
		num++; 
	} 
	spin_unlock(&lock);//spin_lock和spin_trylock都用该接口解锁

#elif 0
	printk("no lock");
	for(i=0 ; i< 10001011 ;i++)
	{
		num++; 
	}

#elif 0
	
	//获取互斥锁
	mutex_lock(&mutex);
	for(i=0 ; i< 10001011 ;i++)
	{
		num++; 
	}
	mutex_unlock(&mutex); 
	
#elif 0
	
	//使用信号量保持同步
	down(&sema);//获取信号量 资源--操作
	printk("sema [%d]",sema.count);
	for(i=0 ; i< 10001011 ;i++)
	{
		num++; 
	}
	up(&sema);//释放信号量 资源++操作
	printk("sema [%d]",sema.count);
	
#elif 0
	
	//使用读锁保持同步
	read_lock(&rwlock);
	for(i=0 ; i< 10001011 ;i++)
	{
		num++; 
	}
	read_unlock(&rwlock);

#elif 0
	
	//使用写锁保持同步
	write_lock(&rwlock);
	for(i=0 ; i< 10001011 ;i++)
	{
		num++; 
	}
	write_unlock(&rwlock);
	
#elif 1
	
	//使用写锁保持同步
	write_lock(&rwlock);
	for(i=0 ; i< 10001011 ;i++)
	{
		num++; 
	}
	write_unlock(&rwlock);



#endif	
	
	
	
	return 0;
}

static __init int kthread_signal_example_init(void)
{
	int i =0;
	printk("start num = %lx\n",num);
	//初始自旋锁
	spin_lock_init(&lock);
	
	//初始化互斥锁
	mutex_init(&mutex);
	
	//初始化信号量[初始化5个资源]
	sema_init(&sema ,5);
	
	//初始化读写锁
	rwlock_init(&rwlock);
	
	for(i = 0; i<count_d ;i++)
	{
		slam_thread[i] = kthread_run(slam_func, NULL, "Dragon");
		printk("i = %d",i);
	}
	
	
	
	
	
  
	return 0;
}

static __exit void kthread_signal_example_exit(void)
{

	printk("end   num = %ld\n",num);
	printk("**exit***\n");
}

module_init(kthread_signal_example_init);
module_exit(kthread_signal_example_exit);
MODULE_AUTHOR("Dragon");
MODULE_LICENSE("GPL");