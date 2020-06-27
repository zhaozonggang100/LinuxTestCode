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
	
	const char *name = "sys_call_table";
	struct kernel_symbol *ksymbol;
	struct module * owner;
	const unsigned long *crc;
	bool gplok = true;
	bool warn = true;
	
	ksymbol = find_symbol(name ,&owner ,&crc, gplok ,warn);
	
	
	if( ksymbol != NULL)
	{
		printk("name : %s \n",ksymbol->name);
		//printk("size : %d \n",ret->core_size);
		
	}
	else
	{
		printk("****************\n");
	}
	if(owner != NULL)
	{
		printk("#########");
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
