//参考实验https://www.jianshu.com/p/f67517b3a150
//per-cpu是每个CPU私有的数据

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
/*
per-cpu 静态和动态分配
1 静态分配
        DEFINE_PER_CPU(long,cpuvar) = 10;
		DECLARE_PER_CPU(long,cpuvar) = 10;
2 动态分配
        cpualloc = alloc_percpu(long);
	释放函数：
		free_percpu(cpualloc);
*/


/*对于动态定义的per-cpu变量使用如下函数访问：
						get_cpu_var
						put_cpu_

*/
static DEFINE_PER_CPU(long,cpuvar) = 10;

static long __percpu *cpualloc;

static int __init my_oops_init(void)
{
	int cpu ;
	printk("dragon init \n");
	/*
	for_each_online_cpu()//现在在线的CPU个数 循环遍历
	for_each_possible_cpu()//可以支持cpu的个数
	for_each_present_cpu()
	*/
	for_each_online_cpu(cpu){
		//获取数据 并赋值
		per_cpu(cpuvar , cpu) = 15;
		/*对于静态定义的per-cpu变量 可以使用：
										get_cpu_var()//访问变量
										put_cpu_var()//修改变量
		*/
		printk("01:[%d] [%ld] \n",cpu ,get_cpu_var(cpuvar));
		//和get_cpu_var配套使用 恢复抢占
		put_cpu_var(cpuvar);
	}
	//	修改其中一个perCPU 数据为20
	__this_cpu_write(cpuvar, 20);
	
	cpualloc = alloc_percpu(long);
	
	//为这个变量设置所有CPU变量
	for_each_online_cpu(cpu){
		//获取数据并赋值
		*per_cpu_ptr(cpualloc, cpu)= 100;
		printk("02:[%d] [%ld] \n",cpu ,*per_cpu_ptr(cpualloc, cpu));
	}
	
	return 0;
}

static void __exit my_oops_exit(void)
{
	int  cpu;
	printk("goodbye\n");
	
	for_each_online_cpu(cpu){
		//per_cpu(cpuvar, cpu)仅仅 获取数据
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
