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
	struct page *p;
	unsigned long i, pfn , valid = 0,locked=0,dirty=0,swapcache=0,slab=0,reserved=0;
	unsigned long private = 0,uptodate=0,active=0,writeback=0,referenced=0,mappedtodisk=0;
	int free01 = 0;
	
	unsigned long num_physpages;
	num_physpages = get_num_physpages();
	printk("num_physpages[%lu] \n",num_physpages);
	for(i = 0 ;i < num_physpages; i++ )
	{
		pfn = i+ 0;
		if(!pfn_valid(pfn))
			continue;
		valid++;
		p = pfn_to_page(pfn);
		if(!p)
			continue;
		if(!page_count(p))
		{
			free01++;
			continue;
		}
		if(PageLocked(p)){
			locked++;
		}
		if(PageReserved(p)){
			reserved++;
		}
		if(PageSwapCache(p)){
			swapcache++;
		}
		if(PageReferenced(p)){
			referenced++;
		}
		if(PageUptodate(p)){
			uptodate++;
		}
		if(PageDirty(p)){
			dirty++;
		}		
		if(PageSlab(p)){
			slab++;
		}
		if(PagePrivate(p)){
			private++;
		}
		if(PageActive(p)){
			active++;
		}	
		if(PageWriteback(p)){
			writeback++;
		}
		if(PageMappedToDisk(p)){
			mappedtodisk++;
		}		
	}
	printk("slab[%lu] locked[%lu] dirty[%lu]\n",slab,locked,dirty);
    return 0;
}

static void  __exit my_exit(void)
{
	
    printk("module exit hello\n");
}

module_init(my_init);
module_exit(my_exit);
MODULE_AUTHOR("Dragon");
MODULE_LICENSE("GPL");
