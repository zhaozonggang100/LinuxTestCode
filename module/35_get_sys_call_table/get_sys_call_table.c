#include <linux/module.h>
#include <linux/init.h>
#include <linux/types.h>
#include <linux/syscalls.h>
#include <linux/delay.h> 
#include <linux/sched.h>
#include <linux/version.h>
#include <linux/kallsyms.h>
 

 
static int __init init_mod(void)
{
	void ** sys_call_table_address;
	sys_call_table_address = (void **)kallsyms_lookup_name("sys_call_table");
	sys_call_table_address = (unsigned long *)sys_call_table_address;
	printk("sys_call_table_address=%p\n",sys_call_table_address);
	printk("sys_call_table_address[0]=[%p][%p]\n",&sys_call_table_address[0],sys_call_table_address[0]);
	printk("sys_call_table_address[1]=[%p][%p]\n",&sys_call_table_address[1],sys_call_table_address[1]);
	printk("sys_call_table_address[2]=[%p][%p]\n",&sys_call_table_address[2],sys_call_table_address[2]);
	printk("sys_call_table_address[3]=[%p][%p]\n",&sys_call_table_address[3],sys_call_table_address[3]);
	printk("sys_call_table_address[4]=[%p][%p]\n",&sys_call_table_address[4],sys_call_table_address[4]);

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

MODULE_AUTHOR("zhoukai");
MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("System call modify");
