#include <linux/fs.h>
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

//static struct dentry *seq_file_demo_dir;
//static int int_pid;
struct pid *pid_g;
struct task_struct *task_g;
struct mm_struct *mm_struct_g;
struct vm_area_struct *vm_area_struct_g;
//module_param(int_pid,int,0644);

 
static int print_task_struct_fun(struct seq_file *seq, void *v)
{
	//seq_printf(seq, "%s", "skb status info start:\n");
	seq_printf(seq, "***********task_struct***********\n");
	seq_printf(seq, "task_struct address: 0x%lx\n",task_g);
	seq_printf(seq, "pid        : %d\n",task_g->pid);
	seq_printf(seq, "maj_flt    : %d\n",task_g->maj_flt);
	seq_printf(seq, "min_flt    : %d\n",task_g->min_flt);
	return 0;
}

static int print_vm_area_struct_fun(struct seq_file *seq, void *v)
{
	seq_printf(seq, "***********vm_area_struct***********\n");
	seq_printf(seq, "vm_area_struct address: 0x%lx\n",mm_struct_g);
	seq_printf(seq, "hiwater_rss : %d\n",atomic_read(&mm_struct_g->hiwater_rss));
	seq_printf(seq, "file_page   : %d\n",atomic_read(&mm_struct_g->rss_stat.count[0]));
	seq_printf(seq, "anon_page   : %d\n",atomic_read(&mm_struct_g->rss_stat.count[1]));
	seq_printf(seq, "swap_page   : %d\n",atomic_read(&mm_struct_g->rss_stat.count[2]));
	seq_printf(seq, "shmem_page  : %d\n",atomic_read(&mm_struct_g->rss_stat.count[3]));
	return 0;
}

static int seq_file_demo_show(struct seq_file *seq, void *v)
{
	    //先获取进程的task_struct
		pid_g = find_get_pid(1);
		task_g = pid_task(pid_g,PIDTYPE_PID);
		mm_struct_g = task_g->mm;
		//打印task_struct
		print_task_struct_fun(seq,NULL);
		//打印vm_area_struct
		print_vm_area_struct_fun(seq,NULL);
        //seq_printf(seq, "Hello World\n");
		//seq_printf(seq,(char *)&int_pid);
		seq_printf(seq, "*********2************\n");
        return 0;
}

static int seq_file_demo_open(struct inode *inode, struct file *file)
{
        return single_open(file, &seq_file_demo_show, NULL);
}

static ssize_t seq_file_demo_write(struct file *file, 
				const char *buf ,size_t count, loff_t *pos)
{
    //int val;
    //ssize_t (*write)(struct file *, const char *, size_t, loff_t *);
/*
   int day, year;
   char weekday[20], month[20], dtm[100];
   strcpy( dtm, "Saturday March 25 1989" );
   sscanf( dtm, "%s %s %d  %d", weekday, month, &day, &year );

   printf("%s %d, %d = %s\n", month, day, year, weekday );


    if(sscanf(buffer, "%d", &val) != 1)
        return -EINVAL;

    if(val == 1)
        printk("proc_test1 write true  [%d]\n",val);
    else 
        printk("proc_test1 write false [%d]\n",val);

    return val;
	*/
 /*       char *ptr;
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
*/
/*
参考：drvices/tty/sysrq.c
ok 信息只能传递 一个字符
	if (count) {
		int c;
		if (get_user(c, buf))
		//if(sscanf(buf, "%d", &c) != 1)
			printk("*****\n");
			printk("c:[%d]\n",c);
	    if (get_user(c, buf))
		//if(sscanf(buf, "%d", &c) != 1)
			printk("*****\n");
			printk("c:[%d]\n",c);
		   //__handle_sysrq(c, false);
	}
	return count;
	*/
	if(count)
	{
		char *ptr;
		int buflen = count;
		if(copy_from_user(ptr , buf ,buflen))
		{
			printk("********\n");
			return -EFAULT;
		}
	} 
	printk("########\n");
	return count;
		

	
	
}

static const struct file_operations hello_proc_fops = {
        .owner = THIS_MODULE,
        .open = seq_file_demo_open,
        .read = seq_read,
		.write = seq_file_demo_write,
        .llseek = seq_lseek,
        .release = single_release,
};

static int __init seq_file_demo_init(void)
{
	proc_create("hello_proc",0777,NULL,&hello_proc_fops);
    return 0;
}

static void __exit seq_file_demo_exit(void)
{
	remove_proc_entry("hello_proc",NULL);
}

module_init(seq_file_demo_init);
module_exit(seq_file_demo_exit);
MODULE_LICENSE("GPL");