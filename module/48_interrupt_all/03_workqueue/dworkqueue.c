#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/init.h>
#include <linux/mm_types.h>
#include <linux/slab.h>
#include <linux/kthread.h>
#include <linux/delay.h>
#include <linux/workqueue.h>  
#include <asm/current.h>

//定义一个时间 全局变量  
struct timer_list timer;

//定义工作队列

static struct workqueue_struct *queue=NULL;  
static struct work_struct   work;  

int timer_count = 0;
int workqueue_count = 0;

static void work_handler(struct work_struct *data)  
{  
       printk("zhaozonggang work queue [%s] [%d]\n",current->comm ,current->pid);  
	   
	   //此延时函数容易造成死机
	   mdelay(1000);
} 


static void mem_timefunc(unsigned long dummy)
{
	  
	
	
	if(timer_count < 4)
	{
		/*创建一个单线程的工作队列*/ 
		//queue=create_singlethread_workqueue("hello dragon"); 
		printk("Dragon:%d \n",timer_count);
		
		INIT_WORK(&work,work_handler);
		//把结构体放入 内核线程，唤醒队列
		schedule_work(&work);
		timer_count++;
	}
  
	
	
	//重新设置超时函数&并启动函数
	timer.expires = jiffies + 1000;
	add_timer(&timer);

}


static int __init my_oops_init(void)
{
	int ret = 123;
	
	/*
	设置的超时时间
	cat .config | grep HZ 
	CONFIG_HZ=1000 1s滴答1000次
	timer.expires = jiffies + xxx;   // xxx表示多少个滴答后超时
    timer.expires = jiffies + 2*HZ;  // HZ等于CONFIG_HZ，2*HZ就相当于2秒
    */
	timer.expires = jiffies + 2000;
	
	//设置定时器
	//mem_timefunc 用于回调
	//setup_timer(&timer, mem_timefunc, (unsigned long)gvma); 
	setup_timer(&timer, mem_timefunc, (unsigned long)&ret); 
	//向内核添加定时器
	add_timer(&timer);



	return 0;
}

static void __exit my_oops_exit(void)
{
	del_timer(&timer);
	printk("goodbye\n");
	
}

module_init(my_oops_init);
module_exit(my_oops_exit)