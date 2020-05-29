
/* 参考drivers\input\keyboard\gpio_keys.c */

#include <linux/module.h>
#include <linux/version.h>

#include <linux/init.h>
#include <linux/fs.h>
#include <linux/interrupt.h>
#include <linux/irq.h>
#include <linux/sched.h>
#include <linux/pm.h>
#include <linux/sysctl.h>
#include <linux/proc_fs.h>
#include <linux/delay.h>
#include <linux/platform_device.h>
#include <linux/input.h>
#include <linux/irq.h>

static struct input_dev *buttons_dev;

static int buttons_init(void)
{
	//int i;
	
	/* 1. 分配一个input_dev结构体 */
	buttons_dev = input_allocate_device();;

	/* 2. 设置 */
	/* 2.1 能产生哪类事件 */
	set_bit(EV_KEY, buttons_dev->evbit);
	//set_bit(EV_REP, buttons_dev->evbit);
	
	/* 2.2 能产生这类操作里的哪些事件: L,S,ENTER,LEFTSHIT */
	set_bit(KEY_A, buttons_dev->keybit);//注册了一个按键事件
	set_bit(700, buttons_dev->keybit);//注册了一个按键事件
	
	//测试验证下，可以设置多少个按键值
	
	//set_bit(KEY_S, buttons_dev->keybit);
	//set_bit(KEY_ENTER, buttons_dev->keybit);
	//set_bit(KEY_LEFTSHIFT, buttons_dev->keybit);

	/* 3. 注册 */
	//会在/dev/input产生一个event
	//crw-rw----  1 root input 13, 70 5月  29 21:47 event6
	input_register_device(buttons_dev);
	
	/* 4. 硬件相关的操作 */
	//init_timer(&buttons_timer);
	//buttons_timer.function = buttons_timer_function;
	//add_timer(&buttons_timer);
	
	//for (i = 0; i < 4; i++)
	//{
	//	request_irq(pins_desc[i].irq, buttons_irq, IRQT_BOTHEDGE, pins_desc[i].name, &pins_desc[i]);
	//}
	
	return 0;
}

static void buttons_exit(void)
{
	//int i;
	//for (i = 0; i < 4; i++)
	//{
	//	free_irq(pins_desc[i].irq, &pins_desc[i]);
	//}

	//del_timer(&buttons_timer);
	input_unregister_device(buttons_dev);
	input_free_device(buttons_dev);
	printk("exit input subsystem \n");
}



module_init(buttons_init);

module_exit(buttons_exit);

MODULE_LICENSE("GPL");



