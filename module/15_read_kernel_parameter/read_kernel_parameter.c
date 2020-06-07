#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/module.h>
#include <linux/interrupt.h>
#include <linux/proc_fs.h>
#include <linux/string.h>
#include <asm/uaccess.h>
#include <linux/seq_file.h>
extern unsigned long volatile pfcount;


static int hello_proc_show(struct seq_file *m, void *v) {
	seq_printf(m, "Hello proc!\n");
	seq_printf(m, "zhaozonggang\n");
	seq_printf(m, "****\n");
	//seq_printf(m, (const char *)&pfcount);
	
	return 0;
}


static int hello_proc_open(struct inode *inode, struct file *file) {
 
	return single_open(file, hello_proc_show, NULL);
}

static const struct file_operations hello_proc_fops = {
 .owner = THIS_MODULE,
 .open = hello_proc_open,
 .read = seq_read,
 .llseek = seq_lseek,
 .release = single_release,
};

//内核模块初始化函数
static int __init myirq_init(void)
{
	printk("*****strart******");
	printk("count:%lu ",pfcount);
	proc_create("hello_proc",0,NULL,&hello_proc_fops);
	//#proc_pf = proc_mkdir("pf",0);
	//#proc_create("pfcount",0,NULL,get_pfcount);
	//proc_create("capi/capi20", 0, NULL, &capi20_proc_fops);
	return 0;
} 

//内核模块退出函数
static void __exit myirq_exit(void)
{
	printk("*****end******");
	printk("count:%lu ",pfcount);
	remove_proc_entry("hello_proc", NULL);
	//#remove_proc_entry("pfcount", proc_pf);
	//#remove_proc_entry("pf",0);
}

MODULE_LICENSE("GPL");
module_init(myirq_init);
module_exit(myirq_exit);