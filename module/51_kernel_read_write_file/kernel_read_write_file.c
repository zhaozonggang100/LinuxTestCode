//https://www.cnblogs.com/arnoldlu/p/8879800.html

#include <linux/module.h>
#include <linux/init.h>
#include <linux/fs.h>
#include <linux/uaccess.h>

static char buf[] ="你好";
static char buf1[10];
 
int __init hello_init(void)
{
    struct file *fp;
    mm_segment_t fs;
    loff_t pos;
    printk("hello enter/n");
    
	//内核打开文件
	fp = filp_open("/root/dragon_kernel_file",O_RDWR | O_CREAT,0644);
    if (IS_ERR(fp)){
        printk("create file error/n");
        return -1;
    }
	//获取当前线程的thread_info->addr_limit。
    fs = get_fs();
	
	//将能访问的空间thread_info->addr_limit扩大到KERNEL_DS。
    set_fs(KERNEL_DS);
    pos =0;
    
	//调用vfs_write写内容
	vfs_write(fp, buf, sizeof(buf), &pos);
    pos =0;
	
	//调用vfs_read读取内容
    vfs_read(fp, buf1, sizeof(buf), &pos);
    printk("read: %s/n",buf1);
    filp_close(fp,NULL);
	
	//切换回原来的值
    set_fs(fs);
    return 0;
}
void __exit hello_exit(void)
{
    printk("hello exit/n");
}
 
module_init(hello_init);
module_exit(hello_exit);
 
MODULE_LICENSE("GPL");