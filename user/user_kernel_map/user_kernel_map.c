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
#include <linux/string.h>

struct class *my_class;

static char *device_buffer;
#define device_buffer_size (1 * PAGE_SIZE)
#define cmd_buffer 1

static ssize_t tdd_read(struct  file *file ,char *buffer ,size_t count ,loff_t *offset){
	
	char *ptr = "nihaochina";
	int cnt = strlen(ptr);
	int res;
	if(cnt < count)
	{
		cnt = count ;
	}
	printk("enter file read count = %ld | cnt %d \n",count,cnt);
	
	//cnt使用最大的数值
	//将内核数据放进buffer，由用户进程使用（buffer属于内核和用户交互的缓存区）
	res = copy_to_user((void *)buffer,(void *)ptr,cnt);
	
	return cnt;
}
static int tdd_open(struct inode *inode ,struct file *file){
	printk("enter file open");
	return 0;
}

static ssize_t tdd_write(struct file *file ,const char *buffer ,size_t count ,loff_t *offset ){
	char *ptr;
	int cnt = 10 ,res;
	printk("enter file write size= %ld ",count);
	//printk("buffer:%s\n",buffer);
	ptr = (char *)kmalloc(100,GFP_KERNEL);
	printk("enter file ptr = %p  ",ptr);
	if(cnt < count)
	{
		cnt = count ;
	}
	//复制用户数据到内核空间 ptr属于内核空间地址 buffer属于用户write到内核的数据
	//（buffer属于内核和用户交互的缓存区）
	res = copy_from_user((void *)ptr,(void *)buffer,cnt);
	printk("uesr->kernel:%s\n",ptr);

	return cnt;
}

static int tdd_close(struct inode *inode ,struct file *file){
	printk("enter file close\n");
	return 0;
}
// long (*unlocked_ioctl) (struct file *, unsigned int, unsigned long);
//cmd 来自应用层ioctl(fd,cmd1,cmd2,cmd3 ...);
static long tdd_ioctl(struct inode *inode, unsigned int cmd, unsigned long arg){
	printk("enter file ioctl cmd=%ld  arg=%ld \n",cmd,arg);
	switch(cmd){
		case 1://传输一个数字 选择命令咋执行
		printk("ioctl cmd 1\n");
		break;
		
		case 2:
		printk("ioctl cmd 2\n");
		break;
	}
	
	return 0;
}
struct file_operations tdd_fops={
	.owner = THIS_MODULE ,
	.read = tdd_read ,
	.open = tdd_open,
	.write = tdd_write,
	.unlocked_ioctl = tdd_ioctl,
	.release = tdd_close ,
	
};

static int __init my_init(void)
{
    
	printk("*****kenel ko*****\n");
	device_buffer = kmalloc(device_buffer_size, GFP_KERNEL);
	printk("*****device_buffer[%p] page_size[%d]*****\n",device_buffer,PAGE_SIZE);
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
	device_create(my_class,NULL,MKDEV(123,1),NULL,"Dragon");
	printk("Dragon ok\n");
    return 0;
}

static void  __exit my_exit(void)
{
	device_destroy(my_class,MKDEV(123,1));
    class_destroy(my_class);
	
	unregister_chrdev(123,"Dragon");
    printk("module exit hello\n");
}

module_init(my_init);
module_exit(my_exit);
MODULE_AUTHOR("Dragon");
MODULE_LICENSE("GPL");
