#include <linux/init.h>
#include <linux/thread_info.h>
#include <linux/module.h>
#include <linux/sched.h>
#include <linux/errno.h>
#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/slab.h>
#include <linux/input.h>
#include <linux/init.h>
#include <linux/serio.h>
#include <linux/delay.h>
#include <linux/clk.h>
#include <linux/miscdevice.h>
#include <linux/io.h>
#include <linux/ioport.h>
#include <linux/vmalloc.h>
#include <linux/dma-mapping.h>
#include <linux/export.h>
#include <linux/gfp.h>

MODULE_LICENSE("GPL");
MODULE_AUTHOR("bunfly");

unsigned long vmalloc_to_pfn(const void *vmalloc_addr);

int test_init()
{
    int ret = 0;
    unsigned char *vmalloc_virt, *normal_virt, *phys;
    unsigned long pfn;

    printk("KERNEL SPACE: init\n");

    printk("KERNEL SPACE: read from d0003000: %s\n", 0xd0003000);
    //读出d0003000虚拟地址里面的数据
 
    normal_virt = kmalloc(40, GFP_KERNEL);
    //kmalloc分配一段虚拟地址，大小，睡眠不可中断

    phys = virt_to_phys(normal_virt);
    printk("KMAKLLOC: kmalloc virt: %p <==> phys: %p\n", normal_virt, phys);

    kfree(normal_virt);

    //分配页
    //--------------------------------------
    vmalloc_virt = vmalloc(500);
     //vmalloc分配内存
 
    pfn = vmalloc_to_pfn(vmalloc_virt);
    //页分配
    phys = (pfn << 12) | ((unsigned long)vmalloc_virt & 0xfff);
    //将页地址转换成物理地址
     normal_virt = phys_to_virt(phys);
     //再将物理地址转换成虚拟地址
     printk("VMALLOC: vmalloc_virt = %p, normal_virt = %p\n", vmalloc_virt, normal_virt);
 
     //dma分配
     normal_virt = dma_alloc_coherent(NULL, 1024, &phys, GFP_KERNEL);
     printk("DMA: vmalloc_virt = %p, normal_virt = %p\n", vmalloc_virt,phys);
     dma_free_coherent(NULL, 1024, normal_virt, phys);
 
     vfree(vmalloc_virt);
     return 0;
 }

void test_exit()
 {
    printk("KERNEL SPACE: exit\n");
 }

module_init(test_init);
module_exit(test_exit);
