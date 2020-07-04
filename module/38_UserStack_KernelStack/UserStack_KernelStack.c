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

/*
说明：void * stack ;
*/
#define BACKTRACE_DEPTH 2 //打印多少行的堆栈信息
static int __init my_init(void)
{
  
	void * d_stack = NULL;
	struct pt_regs *d_pt_regs;
	
	struct stack_trace d_trace;
	unsigned long backtrace[BACKTRACE_DEPTH];
	
	
	//unsigned long task01 = 0xffff90c635fc8000;
	
	d_stack = current->stack;
	printk("d_stack01 = %p",d_stack);
	d_stack = (unsigned long)task_stack_page(current);
	printk("d_stack02 = %p",d_stack);
	printk("THREAD_SIZE = %lx",THREAD_SIZE);
	printk("TOP_OF_KERNEL_STACK_PADDING = %lx",TOP_OF_KERNEL_STACK_PADDING);
	/*
	有函数计算公式的出是：
	[ 3857.141708] d_stack02 = ffffa201826b4000
	[ 3857.141708] THREAD_SIZE = 4000
	[ 3857.141709] TOP_OF_KERNEL_STACK_PADDING = 0
	[ 3857.141709] d_pt_regs = ffffa201826b7f58
	0xffffa201826b4000 + 0x4000 - 0xA8(pt_regs结构体大小) = 0xffffa201826b7f58
	*/
	d_pt_regs = task_pt_regs(current);//获取pt_stack指针 && 经过验证完全OK
	printk("d_pt_regs = %p",d_pt_regs);
	
	printk("d_pt_regs->ss = %lx",d_pt_regs->ss);
	
	
	/***************************************************************/
	//开始打印内核堆栈
	printk("****start kernel trace*****************\n");
	printk("pid [%d] \n",current->pid);
	
	memset(&d_trace, 0, sizeof(d_trace));
	memset(backtrace, 0, BACKTRACE_DEPTH * sizeof(unsigned long));
	d_trace.max_entries = BACKTRACE_DEPTH;//最大调用栈的深度
	d_trace.entries = backtrace;
	save_stack_trace_tsk(current, &d_trace);
	print_stack_trace(&d_trace, 0);
	
	
	/*
	void print_stack_trace(struct stack_trace *trace, int spaces)
	{
		int i;

		if (WARN_ON(!trace->entries))
			return;

		for (i = 0; i < trace->nr_entries; i++)
			printk("%*c%pS\n", 1 + spaces, ' ', (void *)trace->entries[i]);
	}
	EXPORT_SYMBOL_GPL(print_stack_trace);
	*/
	
	/*
	arch/x86/include/asm/processor.h

	struct pt_regs * task_pt_regs(struct task_struct *p);


	#define task_pt_regs(task) \
	({ \
	unsigned long __ptr = (unsigned long)task_stack_page(task); \
	__ptr += THREAD_SIZE - TOP_OF_KERNEL_STACK_PADDING; \
	((struct pt_regs *)__ptr) - 1; \
	})
	*/
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
