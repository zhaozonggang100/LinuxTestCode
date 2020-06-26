#include <linux/uaccess.h>
#include <linux/module.h>
#include <linux/init.h>
#include <linux/syscalls.h>
#include <linux/printk.h>
#include <linux/slab.h>
#include <linux/preempt.h>
#include <linux/mm.h>
#include <asm/io.h>

static unsigned long syscall_tbl_addr;
static unsigned long saved_syscall_addr;
static int syscall_nr = 56;

// 系统调用实现
int syscall_impl(const char __user *msg, size_t len)
{
    char *k_msg = NULL;
	 printk("**************** :\n");
    if (!msg || !len)
        return -EINVAL;
    
    k_msg = kmalloc(len, GFP_KERNEL);
    if (!k_msg)
        return -ENOMEM;
    
    if (copy_from_user(k_msg, msg, len)) {
        kfree(k_msg);
        return -EFAULT;
    }
	
    printk("zhaozonggang add[%p][%d] : %s\n", syscall_impl,len, k_msg);
    kfree(k_msg);
    // do something
    return 0;
}

// 修改系统调用表，任意选择上述介绍的一种

unsigned long syscall_table_modify(unsigned long *tbl, unsigned nr, unsigned long new_addr)
{
    unsigned long   old_addr;
	unsigned char   *p;

#ifndef X86_CR0_WP
  #define X86_CR0_WP    (1UL << 16)
#endif
	preempt_disable();//关闭内核抢占
	printk("zhaozonggang_01 X86_CR0_WP [%lx] \n",X86_CR0_WP);//X86_CR0_WP = 0x10000
	//‭1000 0000 0000 0101 0000 0000 0011 0011‬
	printk("zhaozonggang_01 read_cr0() [%lx] \n",read_cr0());//cr0 = 80050033
	write_cr0(read_cr0() & ~X86_CR0_WP);//去掉写保护(修改第16位为0)
	printk("zhaozonggang_02 X86_CR0_WP [%lx] \n",X86_CR0_WP);//X86_CR0_WP = 0x10000
	//‭1000 0000 0000 0100 0000 0000 0011 0011‬
	printk("zhaozonggang_02 read_cr0() [%lx] \n",read_cr0());//cr0 = 80040033
	p = (unsigned char *)&tbl[nr];//取出数组的地址
	printk("zhaozonggang_03 p [%p] \n",p);
	printk("zhaozonggang_03 p [%lx] \n",*(unsigned long *)p);//取出数组的内容 
	printk("zhaozonggang_03 p [%lx] \n",tbl[nr]);//取出数组的内容 
	
	old_addr = ACCESS_ONCE(*(unsigned long *)p); //保存修改系统调用号的槽位中的函数地址
	printk("zhaozonggang_04 old_addr [%lx] \n",old_addr);//
	printk("zhaozonggang_04 new_addr [%lx] \n",new_addr);//
	ACCESS_ONCE(*((unsigned long *)p)) = new_addr;//重新把系统调用表 函数地址重写
	smp_mb();
	write_cr0(read_cr0() | X86_CR0_WP);//增加写保护
	preempt_enable();//打开内核抢占

	return old_addr;

}
static int __init init_mod(void)
{
	syscall_tbl_addr = (unsigned long)0xffffffffb8e00200;//获取首地址 ffffffffb8e00200 R sys_call_table
    printk(KERN_INFO "Kernel Module install!\n");
    if (!syscall_tbl_addr) {
        printk(KERN_ERR "Parameter was invalid\n");
        return -EINVAL;
    }
    saved_syscall_addr = syscall_table_modify((unsigned long *)syscall_tbl_addr,syscall_nr, (unsigned long)syscall_impl);
    return 0;
}

static void __exit exit_mod(void)
{
    printk(KERN_INFO "Kernel Module uninstall!\n");
    saved_syscall_addr = syscall_table_modify((unsigned long *)syscall_tbl_addr, 
            syscall_nr, (unsigned long)saved_syscall_addr);
    WARN_ON(saved_syscall_addr != (unsigned long)syscall_impl);
}

module_init(init_mod);
module_exit(exit_mod);

//将变量设置为文件系统关联，这样才能传递参数
module_param(syscall_tbl_addr, ulong, 0640);
MODULE_PARM_DESC(syscall_tbl_addr,"system call table address, need to be provided when the module is installed");

MODULE_AUTHOR("zhoukai");
MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("System call modify");
