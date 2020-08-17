
//sys/bus/platform 查看总线注册位置
#include <linux/init.h>
#include <linux/module.h>
//地址：https://zhuanlan.zhihu.com/p/129129087
/*驱动注册的头文件，包含驱动的结构体和注册和卸载的函数*/
#include <linux/platform_device.h>
 




struct resource led_resource[] = {
	[0] = {
			.start = 0x01,
			.end = 0x01 + 8 - 1,
			.flags = IORESOURCE_MEM,
		},
	[1] = {
		.start = 8888,
		.end = 8888,
		.flags = IORESOURCE_IRQ,
	},
	[2] = {
		.start = 0x34,
		.end = 0x34+7,
		.flags = IORESOURCE_MEM,
	},
	
};



struct platform_device led_pdev ={
	.name = "dragon",
	.id = -1,
	.num_resources = ARRAY_SIZE(led_resource),
	.resource = led_resource,

	
};
 
static int hello_init(void)
{	
	platform_device_register(&led_pdev);

	return 0;
}
 
static void hello_exit(void)
{
	printk(KERN_EMERG "HELLO WORLD exit!\n");
	
	platform_device_unregister(&led_pdev);
}
 
module_init(hello_init);
module_exit(hello_exit);
MODULE_AUTHOR("Dragon");
MODULE_LICENSE("GPL");

