
///sys/bus/platform 查看总线注册位置
#include <linux/init.h>
#include <linux/module.h>
//地址：https://zhuanlan.zhihu.com/p/129129087
/*驱动注册的头文件，包含驱动的结构体和注册和卸载的函数*/
#include <linux/platform_device.h>
 
#define DRIVER_NAME "hello_ctl"

 
static int hello_probe(struct platform_device *pdv){
	
	printk("zhaozonggang init is ok \n");
	//拿到pdev资源 进行初始化 操作
	//获取到内存资源
	//参数3 相同资源的 第几个
	struct resource *addr_res =  platform_get_resource(pdv, IORESOURCE_MEM, 0); //拿到第0 个资源
	//ioremap(addr_res->start,addr_res->end - addr_res->start +1);

	volatile unsigned long *gpc0_conf;
	volatile unsigned long *gpc0_data;
	
	gpc0_conf = ioremap(addr_res->start,resource_size(addr_res));
	gpc0_data = gpc0_conf + 1; 


	//获取中断资源
	
	//struct resource *irq_res = platform_get_resource(pdev, IORESOURCE_IRQ, 0);
	//printk("",irq_res->start);
	int irqno =  platform_get_irq(pdv, 0);


	/*
	1 拿到资源、
	2 给用户提供接口
	*/
	return 0;
}
 
static int hello_remove(struct platform_device *pdv){
	
	return 0;
}
 
static void hello_shutdown(struct platform_device *pdv){
	
	;
}
 
static int hello_suspend(struct platform_device *pdv){
	
	return 0;
}
 
static int hello_resume(struct platform_device *pdv){
	
	return 0;
}


/*

struct platform_device_id {
	char name[PLATFORM_NAME_SIZE];
	kernel_ulong_t driver_data;
};*/
const struct platform_device_id led_id_table[] = {
	{"zhaozonggang" ,   0x1111},
	{"dragon" ,       0x2222},
	{"dragon01" ,       0x3333},
	
};

struct platform_driver hello_driver = {
	.probe = hello_probe, //匹配成功 拿到device的 pdev
	.remove = hello_remove,//
	.shutdown = hello_shutdown,
	.suspend = hello_suspend,
	.resume = hello_resume,
	.id_table = led_id_table,//优先使用这个里面的名字进行配对
	.driver = {
		.name = DRIVER_NAME,//随便写 可以用于匹配name
		.owner = THIS_MODULE, 
	}
	
};


 
static int hello_init(void)
{
	int DriverState ;
	
	printk(KERN_EMERG "HELLO WORLD enter!\n");
	DriverState = platform_driver_register(&hello_driver);
	
	
	printk(KERN_EMERG "\tDriverState is %d\n",DriverState);
	return 0;
}
 
static void hello_exit(void)
{
	printk(KERN_EMERG "HELLO WORLD exit!\n");
	
	platform_driver_unregister(&hello_driver);	
}
 
module_init(hello_init);
module_exit(hello_exit);
MODULE_AUTHOR("Dragon");
MODULE_LICENSE("GPL");

