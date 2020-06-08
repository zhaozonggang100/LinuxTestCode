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





static int __init my_init(void)
{
	/*
	struct mm_struct 0xffff93aa36280f80
		mmap = 0xffff93aa001123c0,
	struct vm_area_struct 0xffff93aa001123c0
		struct vm_area_struct {
		vm_start = 4194304, 
		vm_end = 4198400, 
		vm_next = 0xffff93aa001120c0, 
		vm_prev = 0x0, 
	*/
    struct mm_struct *mm ;
	unsigned long addr ;
	struct vm_area_struct *vma ,*vma01;
	mm = current->mm;//获取当前进程的mm_struct结构体指针
	addr = mm->mmap->vm_next->vm_start + 1;
	//vma01 = mm->mmap;
	vma01 = (struct vm_area_struct *)0xffff93aa001123c0;
	/*
	计算有多少个vma_area_struct 区域
	*/  
	//链表的头地址
	while(vma01)
	{
		printk("add:[%lx] len[%lx] start[%lx] end[%lx]\n",vma01->vm_next,vma01->vm_end - vma01->vm_start ,\
										vma01->vm_start,vma01->vm_end);
		vma01 = vma01->vm_next;
	}
	printk("pid: %d\n",current->pid);
	printk("addr: %lx \n",addr);
	
	vma = find_vma(mm, addr);
	if(vma != NULL)
	{
		printk("vm_start= %lx \n",vma->vm_start);
		printk("vm_end= %lx \n",vma->vm_end);
	}
	else
	{
		printk("failed!!");
	}
    return 0;
}

static void  __exit my_exit(void)
{
	/*
	if(vmalloc_area != NULL)
		vfree(vmalloc_area);
	*/
    printk("module exit hello\n");
}

module_init(my_init);
module_exit(my_exit);
MODULE_AUTHOR("Dragon");
MODULE_LICENSE("GPL");
