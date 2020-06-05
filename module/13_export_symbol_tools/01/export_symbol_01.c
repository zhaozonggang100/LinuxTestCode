#include<linux/init.h>
#include<linux/module.h>
#include<linux/kernel.h>
 
static int function_one(void)
{
        printk("****11111******: %s...\n",__func__);
        return 0;
}
static int function_one01(void)
{
        printk("****000000000000******: %s...\n",__func__);
        return 0;
}

static int __init export_symbol_init(void)
{
        printk("111111111111111 one,Init!\n");
        return 0;
}
 
static void __exit export_symbol_exit(void)
{
        printk("11111111111111111,Exit!\n");
}

EXPORT_SYMBOL(function_one);
module_init(export_symbol_init);
module_exit(export_symbol_exit);
