#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/init.h>
#include <linux/mm_types.h>
#include <linux/slab.h>
#include <linux/kthread.h>
#include <linux/delay.h>
#include <linux/interrupt.h>

//定义一个时间 全局变量
struct timer_list timer;
unsigned int pending;

static void mem_timefunc(unsigned long dummy)
{
	
	
	pending = local_softirq_pending();//检查是否有软中断 未处理
	printk("Dragon:%x \n",pending);
	//重新设置超时函数&并启动函数
	timer.expires = jiffies + 100;
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
	timer.expires = jiffies + 100;
	
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
module_exit(my_oops_exit);
