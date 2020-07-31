#include <linux/times.h>
#include <linux/init.h>
#include <linux/module.h>
#include <linux/slab.h>
#include <linux/sched.h>
#include <linux/blkdev.h>
#include <linux/string.h>
#include <linux/mm.h>
#include <linux/mempool.h>
#include <linux/writeback.h>

 
static struct kmem_cache *kmem = NULL;
static mempool_t *pool = NULL;

static void *elememt = NULL;


static int __init my_init(void)
{
    kmem = kmem_cache_create("my_buffer",1000,0,SLAB_HWCACHE_ALIGN,NULL);
	if(kmem == NULL)
	{
		printk("kmem failed \n");
	}
	else
	{
		pool = mempool_create(5,mempool_alloc_slab,mempool_free_slab,kmem);
		if(pool == NULL)
		{
			printk("mempool faile\n");
			return 0;
		}
		else
		{
			printk("min_nr = %d\n",pool->min_nr);
			elememt = mempool_alloc(pool , GFP_KERNEL);
			printk("%0lx\n",(unsigned long )elememt);
		}
	}
    return 0;
}

static void  __exit my_exit(void)
{
	if(elememt != NULL)
	{
		mempool_free(elememt,pool);
	}
	if(pool != NULL)
	{
		mempool_destroy(pool);
		
	}
	if(kmem != NULL)
	{
		kmem_cache_destroy(kmem);
	}
    printk("module exit hello\n");
}

module_init(my_init);
module_exit(my_exit);
MODULE_AUTHOR("Dragon");
MODULE_LICENSE("GPL");
