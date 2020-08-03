#include <linux/init.h>
#include <linux/module.h>
#include <linux/platform_device.h>
#include <linux/fs.h>
#include <linux/io.h>
#include <linux/cdev.h>
#include <linux/device.h>
#include <linux/of.h>
#include <linux/uaccess.h>
#include <linux/of_address.h>
#include <linux/of_irq.h>
 



 
static int hello_init(void)
{
	
	struct device_node *nd;
	printk("***********start**********\n");
	nd = of_find_node_by_path("/backlight");
	if(nd == NULL)
	{
		printk("*faild*\n");
	}
	return 0;
}
 
static void hello_exit(void)
{
	printk("***********end**********\n");

}
 
module_init(hello_init);
module_exit(hello_exit);
MODULE_AUTHOR("Dragon");
MODULE_LICENSE("GPL");