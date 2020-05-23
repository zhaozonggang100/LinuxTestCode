#include <linux/kthread.h>
#include <linux/time.h>
#include <linux/mm.h>
#include <linux/module.h>
#include <linux/nmi.h>
#include <linux/init.h>
#include <linux/uaccess.h>
#include <linux/highmem.h>
#include <linux/interrupt.h>
#include <linux/capability.h>
#include <linux/completion.h>
#include <linux/kernel_stat.h>
#include <linux/debug_locks.h>
#include <linux/perf_event.h>
#include <linux/security.h>
#include <linux/notifier.h>
#include <linux/profile.h>
#include <linux/freezer.h>
#include <linux/vmalloc.h>
#include <linux/blkdev.h>
#include <linux/delay.h>
#include <linux/pid_namespace.h>
#include <linux/smp.h>
#include <linux/threads.h>
#include <linux/timer.h>
#include <linux/rcupdate.h>
#include <linux/cpu.h>
#include <linux/cpuset.h>
#include <linux/percpu.h>
#include <linux/proc_fs.h>
#include <linux/seq_file.h>
#include <linux/sysctl.h>
#include <linux/syscalls.h>
#include <linux/times.h>
#include <linux/tsacct_kern.h>
#include <linux/kprobes.h>
#include <linux/delayacct.h>
#include <linux/unistd.h>
#include <linux/pagemap.h>
#include <linux/hrtimer.h>
#include <linux/tick.h>
#include <linux/debugfs.h>
#include <linux/ctype.h>
#include <linux/ftrace.h>
#include <linux/slab.h>
#include <linux/init_task.h>
#include <linux/binfmts.h>
#include <linux/context_tracking.h>

struct class *my_class;

static ssize_t tdd_read(struct  file *file ,char *buffer ,size_t count ,loff_t *offset){
	printk("enter file read");
	return 0;
}
static int tdd_open(struct inode *inode ,struct file *file){
	printk("enter file open");
	return 0;
}

static ssize_t tdd_write(struct file *file ,const char *buffer ,size_t count ,loff_t *offset ){
	printk("enter file write ");
	return 0;
}

static int tdd_close(struct inode *inode ,struct file *file){
	printk("enter file close");
	return 0;
}
struct file_operations tdd_fops={
	.owner = THIS_MODULE ,
	.read = tdd_read ,
	.open = tdd_open,
	.write = tdd_write,
	.release = tdd_close ,
};

static int __init my_init(void)
{
    printk("*****kenel ko*****\n");
	if(register_chrdev(123,"Dragon", &tdd_fops))
		printk("register fail \n");
	else 
		printk("register ok \n");
	my_class = class_create(THIS_MODULE,"MY_CLASS");
	if(IS_ERR(my_class))
	{
		printk("error:auto reg \n");
		return -1;
	}
	device_create(my_class,NULL,MKDEV(123,1),NULL,"zzg");
	printk("zzg ok\n");
    return 0;
}

static void  __exit my_exit(void)
{
	dev_t devno = MKDEV (hello_major,hello_minor);
	cdev_del (&cdev);
	device_destroy(my_class,MKDEV(adc_major, 0)); 
	class_destroy(my_class);
	
	unregister_chrdev(123,"Dragon");
    printk("module exit hello\n");
}

module_init(my_init);
module_exit(my_exit);
MODULE_AUTHOR("Dragon");
MODULE_LICENSE("GPL");
