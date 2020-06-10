#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/module.h>
#include <linux/interrupt.h>
#include <linux/proc_fs.h>
#include <linux/string.h>
#include <asm/uaccess.h>
#include <linux/seq_file.h>




atomic_t my_atomic;

//内核模块初始化函数
static int __init myirq_init(void)
{
	int i = 5;
	printk(" [%d]\n",atomic_read(&my_atomic));
	atomic_set(&my_atomic, i );
	printk(" [%d]\n",atomic_read(&my_atomic));
	return 0;
} 

//内核模块退出函数
static void __exit myirq_exit(void)
{
	printk("*****end******");

	//#remove_proc_entry("pfcount", proc_pf);
	//#remove_proc_entry("pf",0);
}

MODULE_LICENSE("GPL");
module_init(myirq_init);
module_exit(myirq_exit);