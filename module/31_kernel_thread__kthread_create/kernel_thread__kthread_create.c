#include <linux/module.h>
#include <linux/init.h>
#include <linux/kmod.h>
#include <linux/sched.h>
#include <linux/delay.h>
#include <linux/pid.h>
#include <linux/wait.h>
#include <linux/kthread.h>
 

int my_kernel_thread(void *arg)
{
        int n = 0;
 
        while(1)
        {
                printk("%s: %d\n",__func__,n++);
                ssleep(3);
        }
 
        return 0;

}





static int __init my_init(void)
{
	struct task_struct *re;  
	printk("*******Dragon**********\n");
   // kernel_thread(my_kernel_thread,NULL,CLONE_FS | CLONE_FILES); //此方法创建没有导出kernel_thread无法使用
	// kthread_run(dmatest_work, NULL, "dmatest");
	re = kthread_create_on_node(my_kernel_thread,NULL,-1,"dragon");//创建进程
	printk("*******pid[%d]**********\n",re->pid);
	wake_up_process(re);//唤醒进程
	
    return 0;
}

static void  __exit my_exit(void)
{
 
	
    printk("module exit hello\n");
}

module_init(my_init);
module_exit(my_exit);
MODULE_AUTHOR("Dragon");
MODULE_LICENSE("GPL");
