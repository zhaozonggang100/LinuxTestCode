#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/init.h>
#include <linux/mm.h>
#include <linux/fs.h>
#include <linux/slab.h>
#include <linux/proc_fs.h>
#include <linux/seq_file.h>
#include <linux/mmzone.h>

//内核模块初始化函数
static int __init myirq_init(void)
{
	/*
	//2.对于加运算
	//对结构体指针进行"+"运算,如:mem_map + 3;结果为mem_map所在地址加上3个mem_map_t结构体大小结构块之后的最终地址
	//例:pfn_to_page()函数:将物理页帧号转换为管理该页的mem_map_t类型指针page
	#define pfn_to_page(pfn) ((mem_map + (pfn)) - PHYS_PFN_OFFSET)
	//变换一种形式可以更容易理解:(mem_map + (pfn - PHYS_PFN_OFFSET))
	//其中index = pfn - PHYS_PFN_OFFSET表示物理页帧号pfn对应的偏移索引号index
	
	*/
	
	struct page *my_page;
	int i = 0;
	extern struct page *mem_map;
	printk("*****start******");
	printk("*PHYS_PFN_OFFSET[%lx]  mem_map[%lx]\n",mem_map,mem_map);
	//my_page = mem_map + (0) - PHYS_PFN_OFFSET ;
	my_page = pfn_to_page(i);
	printk("page[%lx] \n",my_page);
	return 0;
} 

//内核模块退出函数
static void __exit myirq_exit(void)
{
	printk("*****end******");

}

MODULE_LICENSE("GPL");
module_init(myirq_init);
module_exit(myirq_exit);