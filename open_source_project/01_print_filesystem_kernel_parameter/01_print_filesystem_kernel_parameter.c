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
//#include <linux/stdlib.h>

//static struct dentry *seq_file_demo_dir;
//static int int_pid;
struct pid *pid_g;
struct task_struct *task_g;
struct mm_struct *mm_struct_g;
struct vm_area_struct *vm_area_struct_g;
int pid_g_g = 0;
//module_param(int_pid,int,0644);

 
static int print_task_struct_fun(struct seq_file *seq, void *v)
{
	//seq_printf(seq, "%s", "skb status info start:\n");
	seq_printf(seq, "***********task_struct***********\n");
	seq_printf(seq, "task_struct address: 0x%lx\n",task_g);
	seq_printf(seq, "pid        : %d\n",task_g->pid);
	seq_printf(seq, "name       : %s\n",task_g->comm);
	seq_printf(seq, "maj_flt    : %d\n",task_g->maj_flt);
	seq_printf(seq, "min_flt    : %d\n",task_g->min_flt);
	return 0;
} 

static int print_vm_area_struct_fun(struct seq_file *seq, void *v)
{

	unsigned long int hiwater_rss , all_phy_,file_page,anon_page,swap_page,shmem_page;
	hiwater_rss = atomic_read(&mm_struct_g->hiwater_rss);
	file_page = atomic_read(&mm_struct_g->rss_stat.count[0]);
	anon_page = atomic_read(&mm_struct_g->rss_stat.count[1]);
	swap_page = atomic_read(&mm_struct_g->rss_stat.count[2]);
	shmem_page = atomic_read(&mm_struct_g->rss_stat.count[3]);
	all_phy_ = (file_page +  anon_page) * 4;
	seq_printf(seq, "***********vm_area_struct***********\n");
	seq_printf(seq, "vm_area_struct address: 0x%lx\n",mm_struct_g);
	seq_printf(seq, "hiwater_rss : %ld\n",hiwater_rss);
	seq_printf(seq, "all_phy_    : %ld\n",all_phy_);
	seq_printf(seq, "file_page   : %ld\n",file_page);
	seq_printf(seq, "anon_page   : %ld\n",anon_page);
	seq_printf(seq, "swap_page   : %ld\n",swap_page);
	seq_printf(seq, "shmem_page  : %ld\n",shmem_page);
	//计算vmd
	unsigned long int vm_len=0 , i=0 ;
	while(vm_area_struct_g)
	{
		
		vm_len = vm_area_struct_g->vm_end - vm_area_struct_g->vm_start + vm_len ;
		printk("vm_len [%ld]  [%ld] [%ld]\n   ",i++, vm_len ,vm_len/1024);
		
		printk(" [%ld]  [%lx] \n",vm_area_struct_g->vm_end , vm_area_struct_g->vm_start);
		vm_area_struct_g = vm_area_struct_g->vm_next;
	}
	seq_printf(seq, "vm_addr   : %ld\n",vm_len/1024);
	return 0; 
}

static int print_all_process(struct seq_file *seq, void *v)
{
	int kernel_count = 0;
	int user_count = 0;
	struct task_struct *p;
	p = &init_task;
	for_each_process(p)
	{
		if(p->mm == NULL)
		{  
			
			printk("comm=%s , pid=%d \n",p->comm,p->pid);
			kernel_count++;
			
		}
		else
		{
			printk("*****************comm=%s , pid=%d \n",p->comm,p->pid);
			user_count++;
		}
	}
	seq_printf(seq, "kernel_process  : %d \n",kernel_count);
	seq_printf(seq, "user_process    : %d \n",user_count);
 
	return 0;
}
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
		print_task_struct_fun(seq,NULL);
		//打印vm_area_struct
		print_vm_area_struct_fun(seq,NULL);
		//遍历所用进程
		//print_all_process(seq,NULL);
        //seq_printf(seq, "Hello World\n");
		//seq_printf(seq,(char *)&int_pid);
		seq_printf(seq, "************end************\n");
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
   strcpy( dtm, "Saturaturday March 25 1989" );
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