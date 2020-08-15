//参考实验https://www.jianshu.com/p/f67517b3a150

#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/init.h>
#include <linux/mm_types.h>
#include <linux/slab.h>
#include <linux/kthread.h>
#include <linux/delay.h>

/*
使用
per_cpu 
DEFINE_PER_CPU
*/

static DEFINE_PER_CPU(long,cpuvar) = 10;

static long __percpu *cpualloc;

static int __init my_oops_init(void)
{
	int cpu ;
	printk("dragon init \n");
	for_each_possible_cpu(cpu){
		per_cpu(cpuvar , cpu) = 15;
		printk("01:[%d] [%ld] \n",cpu ,get_cpu_var(cpuvar));
		put_cpu_var(cpuvar);
	}
	
	__this_cpu_write(cpuvar, 20);
	
	cpualloc = alloc_percpu(long);
	
	//为这个变量设置所有CPU变量
	for_each_possible_cpu(cpu){
		*per_cpu_ptr(cpualloc, cpu)= 100;
		printk("02:[%d] [%ld] \n",cpu ,*per_cpu_ptr(cpualloc, cpu));
	}
	
	return 0;
}

static void __exit my_oops_exit(void)
{
	int  cpu;
	printk("goodbye\n");
	
	for_each_possible_cpu(cpu){
		printk("03:[%d] [%ld] \n",cpu ,per_cpu(cpuvar, cpu));
		printk("03:[%d] [%ld] \n",cpu,*per_cpu_ptr(cpualloc, cpu));
	}
	free_percpu(cpualloc);
	printk("goodbye ok \n");

	
	
}

module_init(my_oops_init);
module_exit(my_oops_exit);
MODULE_AUTHOR("Dragon");
MODULE_LICENSE("GPL");
