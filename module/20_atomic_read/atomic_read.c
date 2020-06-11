#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/module.h>
#include <linux/interrupt.h>
#include <linux/proc_fs.h>
#include <linux/string.h>
#include <asm/uaccess.h>
#include <linux/seq_file.h>
#include <linux/pid.h>
#include <linux/sched.h>
#include <linux/mm_types.h>


atomic_t my_atomic;

//内核模块初始化函数
static int __init myirq_init(void)
{
	int i = 5;
	unsigned long  swap, anon, file, shmem;
	struct task_struct *task;
	struct mm_struct *mm ;
	struct vm_area_struct *vma ;
	struct pid *pid;
	//vma = (struct vm_area_struct *)0xffff93aa001123c0;
	pid = find_get_pid(13822);//由pid查找到task_struct结构体
	task = pid_task(pid,PIDTYPE_PID);
	mm = get_task_mm(task);
	vma = mm->mmap;
	printk("PAGE_SHIFT= [%lx] [%d]\n",PAGE_SHIFT,PAGE_SHIFT);
	printk("PAGE_SHIFT= [%lx] [%d]\n",PAGE_SHIFT,PAGE_SHIFT);

	printk("pid=[%d] task=[%lx] mm[%lx] vma[%lx] \n",pid, task, mm, vma);
	printk("****read****\n");
	file = atomic_long_read(&mm->rss_stat.count[0]);//MM_FILEPAGES
	anon = atomic_long_read(&mm->rss_stat.count[1]);//MM_ANONPAGES
	swap = atomic_long_read(&mm->rss_stat.count[2]);//MM_SWAPENTS
	shmem = atomic_long_read(&mm->rss_stat.count[3]);//MM_SHMEMPAGES
	
	printk("RSS = [%lu]\n",(file+anon)<<2);
	
	printk(" file[%d]\n",file);
	printk(" anon[%d]\n",anon);
	printk(" swap[%d]\n",swap);
	printk(" shmem[%d]\n",shmem);
	
	file = atomic_read(&mm->rss_stat.count[0]);//MM_FILEPAGES
	anon = atomic_read(&mm->rss_stat.count[1]);//MM_ANONPAGES
	swap = atomic_read(&mm->rss_stat.count[2]);//MM_SWAPENTS
	shmem = atomic_read(&mm->rss_stat.count[3]);//MM_SHMEMPAGES
	
	printk(" file[%d]\n",file);
	printk(" anon[%d]\n",anon);
	printk(" swap[%d]\n",swap);
	printk(" shmem[%d]\n",shmem);
	
	printk(" [%d]\n",atomic_read(&my_atomic));
	atomic_set(&my_atomic, i );
	printk(" [%d]\n",atomic_read(&my_atomic));
	return 0;
} 

//内核模块退出函数
static void __exit myirq_exit(void)
{
	printk("*****end******");

	//#remove_proc_entry("pfcount", proc_pf);
	//#remove_proc_entry("pf",0);
}

MODULE_LICENSE("GPL");
module_init(myirq_init);
module_exit(myirq_exit);