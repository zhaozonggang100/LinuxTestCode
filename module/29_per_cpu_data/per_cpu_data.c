#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/cpu.h>
#include <linux/slab.h>
#include <linux/mutex.h>
#include <linux/bitmap.h>
#include <linux/vmalloc.h>
#include <linux/timer.h>
#include <linux/device.h>
#include <linux/firmware.h>


unsigned long my_percpu[NR_CPUS];
static int __init export_symbol_init(void)
{
	struct cpu *point = NULL;
	int id = 0, node = -1;
	int i = 0;
	printk("***********init**********!\n");
	printk("*NR_CPUS: %d \n",NR_CPUS);
	id = get_cpu();//得到正在使用CPU的ID号
	my_percpu[id]++;
	printk("ID:%d  my[%lu]\n",id,my_percpu[id]);
	put_cpu();
	for(i=0; i< 10 ;i++)
	{
		node = smp_processor_id();//返回unsigned int 
		point->node_id = cpu_to_node(node);
		printk("i=[%d] node=%d \n ",i,node);
	}
    return 0;
}
 
static void __exit export_symbol_exit(void)
{
        printk("***********exit**********!\n");
}

module_init(export_symbol_init);
module_exit(export_symbol_exit);
