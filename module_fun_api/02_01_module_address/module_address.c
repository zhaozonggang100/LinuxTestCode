#include <linux/module.h>
#include <linux/init.h>
#include <linux/types.h>
#include <linux/syscalls.h>
#include <linux/delay.h> 
#include <linux/sched.h>
#include <linux/version.h>
#include <linux/kallsyms.h>
 
int d_module(void)
{
	return 0;
}
 
static int __init init_mod(void)
{
	struct module * ret;
	//unsigned long addr = (unsigned long )d_module;
	unsigned long addr = (unsigned long )0xffffffff99eb0460;
	
	preempt_disable();
	ret = __module_address(addr);
	preempt_enable();
	if( ret != NULL)
	{
		printk("name : %s \n",ret->name);
		//printk("size : %d \n",ret->core_size);
		
	}
	else
	{
		printk("****************\n");
	}

    return 0;
}

static void __exit exit_mod(void)
{
    printk(KERN_INFO "Kernel Module uninstall!\n");
}

module_init(init_mod);
module_exit(exit_mod);

//将变量设置为文件系统关联，这样才能传递参数
//module_param(syscall_tbl_addr, ulong, 0640);
//MODULE_PARM_DESC(syscall_tbl_addr,"system call table address, need to be provided when the module is installed");

MODULE_AUTHOR("Dragon");
MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("System call modify");
