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

struct class *my_class;
static char *vmalloc_area = NULL;  

#define MAP_PAGE_COUNT 10  
#define MAPLEN (PAGE_SIZE*MAP_PAGE_COUNT)  
#define MAP_DEV_MAJOR 240
#define MAP_DEV_NAME "mapnopage"

void map_vopen(struct vm_area_struct *vma);
void map_vclose(struct vm_area_struct *vma);
//int (*fault)(struct vm_area_struct *vma, struct vm_fault *vmf);
int map_fault(struct vm_area_struct *vma,struct vm_fault *vmf);

static struct vm_operations_struct map_vm_ops = {
    .open = map_vopen,
    .close = map_vclose,
    .fault = map_fault,
};


static int tdd_open(struct inode *inode ,struct file *file){
	//打印用户进程的信息
	//[39495.246194] open  file ok -->a.out -->72919 
	printk("open  file ok -->%s -->%d \n",current->comm,current->pid);
	return 0;
}

static int tdd_mmap(struct file *file, struct vm_area_struct *vma)  
{
	unsigned long offset = vma->vm_pgoff << PAGE_SHIFT;  
	unsigned long size = vma->vm_end - vma->vm_start; 
	/*
	[39946.465650] offset 0 -- size 40960 
	*/
	printk("offset %lu -- size %lu \n",offset ,size);
	vma->vm_flags |= VM_LOCKONFAULT;//锁定不被交换出去
	
	if(offset == 0)
	{
		/*
		//重新定义一个操作函数集合 操作vm
		原先有个默认的vm_ops结构体，替换掉默认的 使用map_vm_ops。
		*/
		vma->vm_ops = &map_vm_ops;
	}
	return 0;
}

static int tdd_close(struct inode *inode ,struct file *file){
	printk("enter file close\n");
	return 0;
}
void map_vopen(struct vm_area_struct *vma)  
{  
    printk("mapping vma is opened..\n");
} 
 
void map_vclose(struct vm_area_struct *vma)  
{  
    printk("mapping vma is closed..\n");
}
/*
//将用内核空间的地址（物理地址） 映射到用户空间的线性地址
第一步找到物理页框
*/
int map_fault(struct vm_area_struct *vma,struct vm_fault *vmf)  
{
	struct page *page;
	void *page_ptr;
	unsigned long offset, virt_start, pfn_start;
	
	offset = vmf->virtual_address - vma->vm_start;
	virt_start = (unsigned long)vmalloc_area + (unsigned long)(vmf->pgoff << PAGE_SHIFT);
	pfn_start = (unsigned long)vmalloc_to_pfn((void *)virt_start);
	//offset[0] virt_start[18446659146109161472] pfn_start[235806] 
	printk("offset[%lu] virt_start[%lu] pfn_start[%lu] \n",offset ,virt_start,pfn_start);
	if((vma==NULL)||(vmalloc_area==NULL)){
		printk("return VM_FAULT_SIGBUS!\n");
		return VM_FAULT_SIGBUS;
	}
	//offset大小必须和申请的一致可以
	if(offset >=MAPLEN){
		printk("return VM_FAULT_SIGBUS!");
		return VM_FAULT_SIGBUS;
	}
	
	page_ptr=vmalloc_area + offset;
	page=vmalloc_to_page(page_ptr);//得到物理页面对应的地址
	printk("page_ptr[%p] page[%p] vmf->page[%p]\n",page_ptr ,page,vmf->page);
	//page_ptr[ffffb2c2408c1000] page[ffffef5c40e64780] vmf->page[          (null)]
	get_page(page);//增加page的引用次数。
	vmf->page=page; //非常重要完成缺页中断的 页帧号的建立
	printk("%s: map 0x%lx (0x%016lx) to 0x%lx , size: 0x%lx, page:%ld \n", __func__, virt_start, pfn_start << PAGE_SHIFT, vmf->virtual_address,PAGE_SIZE,vmf->pgoff);
    //map_fault: map 0xffffb2c2408c1000 (0x000000003991e000) to 0x7f6266aab000 , size: 0x1000, page:0 
	
	
	
	page_ptr=NULL;
	return 0;
}

struct file_operations tdd_fops={
	.owner = THIS_MODULE ,
	.open = tdd_open,
	.mmap = tdd_mmap,
	.release = tdd_close ,
	
};



static int __init my_init(void)
{
	unsigned long virt_addr;
	int i = 1;
	
    printk("*****kenel ko*****\n");
	if(register_chrdev(123,"Dragon", &tdd_fops))
		printk("register fail \n");
	else 
		printk("register ok \n");
	vmalloc_area=vmalloc(MAPLEN);
	virt_addr = (unsigned long)vmalloc_area;
	//printk("vmalloc_area[%p] virt_addr[%p]  page_size=%d  \n",vmalloc_area,virt_addr ,PAGE_SIZE);
	//vmalloc_area[ffffc900009ee000] virt_addr[ffffc900009ee000]
	for(virt_addr = (unsigned long)vmalloc_area; virt_addr < (unsigned long)vmalloc_area + MAPLEN; virt_addr += PAGE_SIZE)
   {
	  // printk("vmalloc_to_page((void *)virt_addr)) = %d \n",vmalloc_to_page((void *)virt_addr));
	   /*
	vmalloc_to_page()求出虚拟地址对应的page号
	找到vmalloc()所分配内存的虚拟地址所映射的物理页，并返回该页的指针描述符
	[38250.706499] vmalloc_to_page((void *)virt_addr)) = 21532352 
	[38250.706500] vmalloc_to_page((void *)virt_addr)) = 21532288 
	[38250.706500] vmalloc_to_page((void *)virt_addr)) = 21323840 
	[38250.706500] vmalloc_to_page((void *)virt_addr)) = 21323776 
	[38250.706501] vmalloc_to_page((void *)virt_addr)) = 20724544 
	[38250.706501] vmalloc_to_page((void *)virt_addr)) = 20724480 
	[38250.706501] vmalloc_to_page((void *)virt_addr)) = 43007040 
	[38250.706502] vmalloc_to_page((void *)virt_addr)) = 43006976 
	[38250.706502] vmalloc_to_page((void *)virt_addr)) = 24384832 
	[38250.706502] vmalloc_to_page((void *)virt_addr)) = 24384768
	   */
	   SetPageReserved(vmalloc_to_page((void *)virt_addr)); 
	/*
	#define SetPageReserved(page) set_bit(PG_reserved,&(page)->flags)
	设置物理页面不被交换
	*/
           sprintf((char *)virt_addr, "test %d",i++);             
   }
	
	
	my_class = class_create(THIS_MODULE,"MY_CLASS");
	if(IS_ERR(my_class))
	{
		printk("error:auto reg \n");
		return -1;
	}
	device_create(my_class,NULL,MKDEV(123,1),NULL,"Dragon");
	printk("Dragon ok\n");
    return 0;
}

static void  __exit my_exit(void)
{
	device_destroy(my_class,MKDEV(123,1));
    class_destroy(my_class);
	
	unregister_chrdev(123,"Dragon");
    printk("module exit hello\n");
}

module_init(my_init);
module_exit(my_exit);
MODULE_AUTHOR("Dragon");
MODULE_LICENSE("GPL");
