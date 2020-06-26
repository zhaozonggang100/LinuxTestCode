#include <linux/uaccess.h>
#include <linux/module.h>
#include <linux/init.h>
#include <linux/syscalls.h>
#include <linux/printk.h>
#include <linux/slab.h>
#include <linux/preempt.h>
#include <linux/mm.h>
#include <asm/io.h>

#define D_NR_mycall 40
#define sys_call_table_adress 0xffffffffab000200

unsigned long * sys_call_table = 0;
static int (*anything_saved)(void);

long sys_mycall(void)
{
	printk("dragon[%d][%s]\n",current->pid,current->comm);
	return current->pid;
}


static int __init init_mod(void)
{
	printk("*******start******\n");
	sys_call_table = (unsigned long *)sys_call_table_adress;
	anything_saved = (int(*)(void))sys_call_table[D_NR_mycall];
	sys_call_table[D_NR_mycall] = (unsigned long)&sys_mycall;
	return 0;
}

static void __exit exit_mod(void)
{
	sys_call_table[D_NR_mycall] = (unsigned long )anything_saved;
	printk("*******exit******\n");
	
}

module_init(init_mod);
module_exit(exit_mod);
MODULE_AUTHOR("Dragon");
MODULE_LICENSE("GPL");