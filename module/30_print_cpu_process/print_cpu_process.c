#include<linux/init.h>
#include<linux/module.h>
#include<linux/kernel.h>
 

static int __init export_symbol_init(void)
{
        printk("Init!\n");
        return 0;
}
 
static void __exit export_symbol_exit(void)
{
        printk("Exit!\n");
}

module_init(export_symbol_init);
module_exit(export_symbol_exit);
MODULE_AUTHOR("Dragon");
MODULE_LICENSE("GPL");