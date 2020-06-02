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

 
#define MAPLEN (PAGE_SIZE*10)  
char *vmalloc_area; 

static int __init my_init(void)
{
    struct page *to_page, *to_page_1 ;
	unsigned long to_pfn ;
	vmalloc_area=vmalloc(8092);
	printk("Dragon vmalloc successfully ok\n");
	//to_page为虚拟地址vmalloc_area所映射的物理页框
	to_page = vmalloc_to_page(vmalloc_area);
	to_pfn = vmalloc_to_pfn(vmalloc_area);
	to_page_1 = pfn_to_page(to_pfn);
	printk("to_page_address = %lx to_pfn= %ld to_page_1= %lx\n",to_page,to_pfn,to_page_1);
	//[38023.888027] to_page_address = ffffef5c40a50ec0 to_pfn= 169019
	
	to_page = vmalloc_to_page(vmalloc_area+4097);
	//to_pfn 虚拟地址对应的物理页框号(第几个页框)
	to_pfn = vmalloc_to_pfn(vmalloc_area+4097);
	to_page_1 = pfn_to_page(to_pfn+1);//由页框号求出对应的page地址
	printk("to_page_address = %lx to_pfn= %ld to_page_1= %lx\n",to_page,to_pfn,to_page_1);
	//[38023.888029] to_page_address = ffffef5c406b8ec0 to_pfn= 110139
	  
    return 0;
}

static void  __exit my_exit(void)
{
	if(vmalloc_area != NULL)
		vfree(vmalloc_area);
	
    printk("module exit hello\n");
}

module_init(my_init);
module_exit(my_exit);
MODULE_AUTHOR("Dragon");
MODULE_LICENSE("GPL");
