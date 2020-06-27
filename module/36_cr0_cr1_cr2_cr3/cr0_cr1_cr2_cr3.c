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
	printk("zhaozonggang_01 read_cr0()=[%lx] \n",read_cr0());//
	//printk("zhaozonggang_01 read_cr1()=[%lx] \n",read_cr1());//
	printk("zhaozonggang_01 read_cr2()=[%lx] \n",read_cr2());//
	printk("zhaozonggang_01 read_cr3()=[%lx] \n",read_cr3());//

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
