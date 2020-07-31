#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/init.h>
#include <linux/mm_types.h>
#include <linux/slab.h>
#include <linux/kthread.h>
#include <linux/delay.h>
#include <linux/workqueue.h>  
#include <asm/current.h>

//https://www.cnblogs.com/ljin/p/9935972.html


//定义一个时间 全局变量  
struct timer_list timer;

//定义工作队列

//工作以队列结构组织成工作队列（workqueue），其数据结构为workqueue_struct，
static struct workqueue_struct *test_wq = NULL;    

//把推后执行的任务叫做工作（work），描述它的数据结构为work_struct
static struct work_struct work;


int timer_count = 0;
int workqueue_count = 0;

static void work_handler(struct work_struct *data)  
{  
       while(1){
		   printk("zhaozonggang work queue [%s] [%d]\n",current->comm ,current->pid); 
		   
	   }
	   mdelay(1000);
} 





static int __init my_oops_init(void)
{
	int ret = 123;
#if 0
	/*创建工作队列workqueue_struct，该函数会为cpu创建内核线程*/
    test_wq = create_workqueue("Dragon_mul");   
      
	
#elif 1    
	/*创建工作队列workqueue_struct，该函数会为cpu创建内核线程*/
	test_wq = create_singlethread_workqueue("Dragon_single");   

	
#endif


    /*初始化工作work_struct，指定工作函数*/
    INIT_WORK(&work,work_handler);

    /*将工作加入到工作队列中，最终唤醒内核线程*/
    queue_work(test_wq, &work);

	while(1)
	{
		mdelay(1000);
		printk(KERN_ERR "-----%s-----\n",__func__);
    }





	return 0;
}

static void __exit my_oops_exit(void)
{
	del_timer(&timer);
	printk("goodbye\n");
	
}

module_init(my_oops_init);
module_exit(my_oops_exit)
MODULE_LICENSE("GPL");
MODULE_AUTHOR("Dragon");