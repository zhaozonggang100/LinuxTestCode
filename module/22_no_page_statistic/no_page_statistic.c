#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/init.h>
#include <linux/mm.h>
#include <linux/fs.h>
#include <linux/slab.h>
#include <linux/proc_fs.h>
#include <linux/seq_file.h>



//内核模块初始化函数
static int __init myirq_init(void)
{
	
	struct pid *pid;
	struct task_struct *task;
	printk("*****strart******");
	pid = find_get_pid(15542);//由pid查找到task_struct结构体
	task = pid_task(pid,PIDTYPE_PID);
	printk("min_flt =%ld maj_flt=%ld \n",task->min_flt,task->maj_flt);
	return 0;
} 

//内核模块退出函数
static void __exit myirq_exit(void)
{
	printk("*****end******");

}

MODULE_LICENSE("GPL");
module_init(myirq_init);
module_exit(myirq_exit);