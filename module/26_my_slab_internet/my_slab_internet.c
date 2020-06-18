#include <linux/module.h>
#include <linux/init.h>
#include <linux/kernel.h>
#include <linux/mm.h>
#include <linux/errno.h>
#include <linux/slab.h>
#include <linux/gfp.h>
#include <linux/slub_def.h>

#include <linux/delay.h>

struct slab_test{
    int val;
};

static int num;
struct kmem_cache *test_cachep = NULL;
struct slab_test *object1 = NULL, *object2 = NULL ;

void slab_ctor(void *cachep){
	/*
	ctor 和 dtor 参数定义了一个可选的对象构造器和析构器。
	构造器和析构器是用户提供的回调函数。当从缓存中分配新对象时，可以通过构造器进行初始化
	*/
	//申请一个页面并且初始化次数= 1*page/size
    printk("slab_ctor is called! object %d has been inited!\n", num);
    num++;
}

static int __init my_init(void)
{
	printk("slab test module init\n");
    num = 0;
	test_cachep = kmem_cache_create("zhaogang", 123, 0, SLAB_HWCACHE_ALIGN, slab_ctor);
    if(!test_cachep)
		return -ENOMEM;
	
	printk("add:[%p] size[%d] [%ld] \n",test_cachep,kmem_cache_size(test_cachep),sizeof(struct slab_test));
	printk("kmem_cache_cpu add:[%p] \n",test_cachep->cpu_slab);

	ssleep(20);
	
	object1 = kmem_cache_alloc(test_cachep, GFP_KERNEL);
	if(!object1)
        return -ENOMEM;
    else
        printk("1111111 object1 :%p \n",object1);
	
	
/*	
    object2 = kmem_cache_alloc(test_cachep, GFP_KERNEL);
	if(!object2)
        return -ENOMEM;
    else
        printk("2222222 object2 :%p \n",object2);
	
*/
    return 0;
	
}

static void  __exit my_exit(void)
{
	if(object1)
	{
		kmem_cache_free(test_cachep, object1);
	}
	
	
	/*
	if(object2)
	{
		kmem_cache_free(test_cachep, object2);
	｝
	*/  
	
   // kmem_cache_free(test_cachep, object2);
    if(test_cachep)
        kmem_cache_destroy(test_cachep);
    printk("module exit hello\n");
	
}

module_init(my_init);
module_exit(my_exit);
MODULE_AUTHOR("Dragon");
MODULE_LICENSE("GPL");
