#include<linux/init.h>
#include<linux/kernel.h>
#include<linux/module.h>
 
static int function_two(void)
{
        extern int function_one(void);//声明使用外部的函数
        function_one();//这个函数来自哪里？来自其他module的导出
        printk("*****222222222222******** %s...\n",__func__);
        return 0;
}
 
static int __init export_symbol_init(void)
{
        printk("2222222222222222222222,Init!\n");
        function_two();
        return 0;
}
 
static void __exit export_symbol_exit(void)
{
        printk("2222222222222222222222 two,Exit!\n");
}
 
module_init(export_symbol_init);
module_exit(export_symbol_exit);
