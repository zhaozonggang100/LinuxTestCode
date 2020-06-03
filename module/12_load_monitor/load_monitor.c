/**
 * Boyou Xie's load monitor module
 *
 * Copyright (C) 2018 Baoyou Xie.
 *
 * Author: Baoyou Xie <baoyou.xie@gmail.com>
 *
 * License terms: GNU General Public License (GPL) version 2
 */

#include <linux/version.h>
#include <linux/module.h>
#include <linux/hrtimer.h>
#include <linux/ktime.h>
#include <linux/kallsyms.h>
#include <linux/sched.h>
#include <linux/tracepoint.h>
#include <linux/stacktrace.h>
//#include <linux/sched/task.h> /*init_task 头文件*/
//#include <linux/sched/signal.h> /*do_each_thread 头文件*/

#include "load.h" //这个文件是空的 为什么？

struct hrtimer timer;

static unsigned long *ptr_avenrun;

#define FSHIFT		11		/* nr of bits of precision */
#define FIXED_1		(1<<FSHIFT)	/* 1.0 as fixed-point */
#define LOAD_INT(x) ((x) >> FSHIFT)
#define LOAD_FRAC(x) LOAD_INT(((x) & (FIXED_1-1)) * 100)

#define BACKTRACE_DEPTH 20

/*
LOAD_INT(a) = avenrun[0]/(2^11) + 1/200
LOAD_FRAC(a) = ((avenrun[0]%(2^11) + 2^11/200) * 100) / (2^11)
             = (((avenrun[0]%(2^11)) * 100 + 2^10) / (2^11)
             = ((avenrun[0]%(2^11) * 100) / (2^11) + 1/2
*/

/*#if defined(UBUNTU_1604)
extern struct task_struct init_task;
#define next_task(p) \
	list_entry_rcu((p)->tasks.next, struct task_struct, tasks)
#define do_each_thread(g, t) \
	for (g = t = &init_task ; (g = t = next_task(g)) != &init_task ; ) do
#define while_each_thread(g, t) \
	while ((t = next_thread(t)) != g)
static inline struct task_struct *next_thread(const struct task_struct *p)
{
	return list_entry_rcu(p->thread_group.next,
			      struct task_struct, thread_group);
}
#endif*/
static void print_all_task_stack(void)
{
	struct task_struct *g, *p;
	unsigned long backtrace[BACKTRACE_DEPTH];
	struct stack_trace trace;//stack_trace 保存进程调用栈的信息

	memset(&trace, 0, sizeof(trace));
	memset(backtrace, 0, BACKTRACE_DEPTH * sizeof(unsigned long));
	trace.max_entries = BACKTRACE_DEPTH;//最大调用栈的深度
	trace.entries = backtrace;

	printk("!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!\n");
	printk("\tLoad: %lu.%02lu, %lu.%02lu, %lu.%02lu\n",
		LOAD_INT(ptr_avenrun[0]), LOAD_FRAC(ptr_avenrun[0]),
		LOAD_INT(ptr_avenrun[1]), LOAD_FRAC(ptr_avenrun[1]),
		LOAD_INT(ptr_avenrun[2]), LOAD_FRAC(ptr_avenrun[2]));
	printk("dump all task: balabala\n");

       	rcu_read_lock();//遍历线程加锁rcu read copy upload 实用于 读多 写少的进程

	printk("dump running task.\n");
	do_each_thread(g, p) { //遍历线程
		if (p->state == TASK_RUNNING) {
			printk("running task, comm: %s, pid %d\n",
				p->comm, p->pid);
			memset(&trace, 0, sizeof(trace));
			memset(backtrace, 0, BACKTRACE_DEPTH * sizeof(unsigned long));
			trace.max_entries = BACKTRACE_DEPTH;
			trace.entries = backtrace;
			save_stack_trace_tsk(p, &trace);
			print_stack_trace(&trace, 0);
		}
	} while_each_thread(g, p);

	printk("dump uninterrupted task.\n");
	do_each_thread(g, p) {
		if (p->state & TASK_UNINTERRUPTIBLE) {
			printk("uninterrupted task, comm: %s, pid %d\n",
				p->comm, p->pid);
			memset(&trace, 0, sizeof(trace));
			memset(backtrace, 0, BACKTRACE_DEPTH * sizeof(unsigned long));
			trace.max_entries = BACKTRACE_DEPTH;
			trace.entries = backtrace;
			save_stack_trace_tsk(p, &trace);
			print_stack_trace(&trace, 0);
		}
	} while_each_thread(g, p);

	rcu_read_unlock();
}

static void check_load(void)
{
	static ktime_t last;//ktime用于保存时间的数据结果
	u64 ms;
	int load = LOAD_INT(ptr_avenrun[0]); /* 最近1分钟的Load值 */
	printk("time:%ld ",ktime_get());
	if (load < 1)
		return;

	/**
	 * 如果上次打印时间与当前时间相差不到20秒，就直接退出
	 */
	ms = ktime_to_ms(ktime_sub(ktime_get(), last));//ktime_get() 获取当前系统时间 last上次打印的时间。
	if (ms < 20 * 1000)
		return;

	last = ktime_get();
	print_all_task_stack();//打印出所有线程栈
}

static enum hrtimer_restart monitor_handler(struct hrtimer *hrtimer)
{
	enum hrtimer_restart ret = HRTIMER_RESTART;

	check_load();//检查当前系统负载值

	hrtimer_forward_now(hrtimer, ms_to_ktime(10));//hrtimer从现在开始起推迟10ms到期

	return ret;//返回定时器重启的信号
}

static void start_timer(void)
{
	//hrtimer属于高精度定时器
	hrtimer_init(&timer, CLOCK_MONOTONIC, HRTIMER_MODE_PINNED);
	timer.function = monitor_handler;
	hrtimer_start_range_ns(&timer, ms_to_ktime(10)/*10ms到期*/,	0, HRTIMER_MODE_REL_PINNED);
}
static int load_monitor_init(void)
{
	//uptime uproc 命令通过avenrun数组获取值
        ptr_avenrun = (void *)kallsyms_lookup_name("avenrun");//获取内核变量的地址
		printk("avenrun = %lx ",ptr_avenrun);
	if (!ptr_avenrun)
		return -EINVAL;
        
        start_timer();//定时器
        
	printk("load-monitor loaded.\n");

	return 0;
}

static void load_monitor_exit(void)
{
	hrtimer_cancel(&timer);

        printk("load-monitor unloaded.\n");
}

module_init(load_monitor_init)
module_exit(load_monitor_exit)

MODULE_DESCRIPTION("Baoyou Xie's load monitor module");
MODULE_AUTHOR("Baoyou Xie <baoyou.xie@gmail.com>");
MODULE_LICENSE("GPL v2");
