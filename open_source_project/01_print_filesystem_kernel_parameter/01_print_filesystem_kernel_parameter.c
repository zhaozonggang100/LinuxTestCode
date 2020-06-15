#include <linux/fs.h>
#include <linux/kthread.h>
#include <linux/time.h>
#include <linux/mm.h>
#include <linux/module.h>
#include <linux/init.h>
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

#include "loadavg_d.h"
#include "task_struct_d.h"
#include "vm_area_struct_d.h"
#include "all_process_d.h"
#include "cpu_loadavg_d.h"
#include "slabinfo_d.h"


struct pid *pid_g;
struct task_struct *task_g;
struct mm_struct *mm_struct_g;
struct vm_area_struct *vm_area_struct_g;
int pid_g_g = 0;
//module_param(int_pid,int,0644);

 


static int seq_file_demo_show(struct seq_file *seq, void *v)
{
	    //先获取进程的task_struct
		if(pid_g_g == 0)
		{
			seq_printf(seq, "input <pid>\n");
			return 0;
		}
		pid_g = find_get_pid(pid_g_g);
		
		if(pid_g == NULL)
		{
			seq_printf(seq, "pid is missing \n");
			return 0;
		}
		task_g = pid_task(pid_g,PIDTYPE_PID);
		mm_struct_g = task_g->mm;
		vm_area_struct_g = mm_struct_g->mmap;
		//打印task_struct
		print_task_struct_fun( seq, NULL, task_g);
		//打印vm_area_struct
		print_vm_area_struct_fun(seq,NULL,mm_struct_g);
		//遍历所用进程
		print_all_process(seq,NULL);
		//打印内核全局变量
		print_kernel_global_variables(seq,NULL);
		//seq_printf(seq,(char *)&int_pid);
		seq_printf(seq, "************end************\n");
		//打印cpu参数属性。
		print_cpu_loadavg( seq, NULL);
		print_slabinfo(seq, NULL);
        return 0;
}

static int seq_file_demo_open(struct inode *inode, struct file *file)
{
        return single_open(file, &seq_file_demo_show, NULL);
}

static ssize_t seq_file_demo_write(struct file *file, 
				const char *buf ,size_t count, loff_t *pos)
{

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
		char ptr[27];
		int buflen ;
		memset(ptr,0,27);
		if(count > 26)
			buflen = 26;
		else
			buflen = count;
		if(!copy_from_user((char *)ptr , buf ,buflen))
		{
			printk("********\n");
			if(kstrtouint(ptr,10,&pid_g_g))
			{
				return -EFAULT;
			}
			
			return buflen;
		}
		else
		{
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