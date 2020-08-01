#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/module.h>
#include <linux/interrupt.h>
#include <asm/current.h>

static int irq = 1;					
static char * devname = "dragon";			
			

//module_param(irq,int,0644);
//module_param(devname,charp,0644);
			
struct myirq
{
    int devid;
};

struct myirq mydev={1119};
		

static irqreturn_t dragon_fun(int irq, void *data)
{
	//printk("dragon woring [%d][%s][%d]",data->devid,current->comm,current->pid);
	printk("dragon woring ");
	return IRQ_HANDLED;
}
//内核模块初始化函数
static int __init myirq_init(void)
{
	int ret;
    printk("myirq zhaozonggang working ...\n");
	/*
	irq 中断号(硬件中断号)
	myirq_handler 上半部 中断回调可以为空(NULL)
	IRQF_SHARED 中断类型
	devname 中断设备名字
	*/    
	//注册申请一个中断线
	
	/*
	可以使用键盘中断 来模拟 中断；
	如下操作：
	    键盘的中断号是：2
		键盘是输入设备/dev/inpurt/event1 
		注意一点：键盘按下释放 会触发两次
	*/
	ret = request_threaded_irq(irq, NULL, dragon_fun,IRQF_TRIGGER_LOW | IRQF_ONESHOT,devname, &mydev);
    printk("%s zhaozonggang request IRQ:%d success...\n",devname,irq);
    return 0;
}

//内核模块退出函数
static void __exit myirq_exit(void)
{
    printk("Module is leaving...\n");
    free_irq(irq,&mydev);
    printk("Free the irq:%d..\n",irq);
}

MODULE_LICENSE("GPL");
module_init(myirq_init);
module_exit(myirq_exit);