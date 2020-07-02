#include <linux/module.h>
#include <linux/kthread.h>
#include <linux/delay.h>

static struct task_struct * slam_thread = NULL;
static int is_signal_exited = 0;

static int slam_func(void *data)
{
	printk("<xinu>%s()!\n",__func__);
	allow_signal(SIGKILL);
	mdelay(1000);
	//检查当前进程是否有信号处理，返回不为0表示有信号需要处理。
	while(!signal_pending(current) && !kthread_should_stop())
	{
		printk("<xinu>jiffies(%lu)\n",jiffies);
		printk("zhaozonggang [%d]\n",signal_pending(current));
		set_current_state(TASK_INTERRUPTIBLE);
		schedule_timeout(msecs_to_jiffies(5000));
	}

	printk("<xinu>leave slam_func!\n");
	is_signal_exited = 1;

	return 0;
}

static __init int kthread_signal_example_init(void)
{
	slam_thread = kthread_run(slam_func, NULL, "slam");
	printk("<xinu>kthread_signal_example_init()!\n");

	return 0;
}

static __exit void kthread_signal_example_exit(void)
{
	if(!is_signal_exited && !IS_ERR(slam_thread))
	{
		kthread_stop(slam_thread);
	}

	printk("<xinu>%s()!\n", __FUNCTION__);
}

module_init(kthread_signal_example_init);
module_exit(kthread_signal_example_exit);
MODULE_AUTHOR("Dragon");
MODULE_LICENSE("GPL");