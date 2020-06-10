#include <linux/init.h>
#include <linux/module.h>
#include <linux/seq_file.h>
#include <linux/debugfs.h>
#include <linux/fs.h>
 
#include <linux/uaccess.h>
 
static struct dentry *seq_file_demo_dir;
 
static int seq_file_demo_show(struct seq_file *seq, void *v)
{   
	int i = 0;
    printk("%s %d \n",__FUNCTION__,__LINE__);
	for(i = 0;i< 100 ;i++)
	{
	  seq_printf(seq, "Hello World\n");//向文件写数据
	}

    return 0;
}
static ssize_t seq_file_demo_write(struct file *file,const char __user *buf,
				   size_t count, loff_t *ppos)
{
	//u32 reg, val;
	//char kbuf[25];
	//if (copy_from_user(kbuf, buf, count))
	//	return -EFAULT;
	//写函数执行到此需要写参数到文件
	printk("%s %d \n",__FUNCTION__,__LINE__);
	//seq_printf(seq, "Hello World\n");
	//printk("%s %d \n",kbuf,count);
	return count;
}
static int seq_file_demo_open(struct inode *inode, struct file *file)
{
	printk("%s %d \n",__FUNCTION__,__LINE__);
	//single_open实现了所有的迭代器 自己仅仅实现一个show就可以了
    return single_open(file, &seq_file_demo_show, NULL);
}
 
static const struct file_operations seq_file_demo_fops = {
        .owner = THIS_MODULE,
        .open = seq_file_demo_open,
        .read = seq_read,
		.write = seq_file_demo_write,
        .llseek = seq_lseek,
        .release = single_release,
};
 
static int __init seq_file_demo_init(void)
{
	printk("%s %d \n",__FUNCTION__,__LINE__);
    seq_file_demo_dir = debugfs_create_file("seq_file_demo", 0444, NULL,NULL, &seq_file_demo_fops);
    return 0;
}
 
static void __exit seq_file_demo_exit(void)
{
	    printk("%s %d \n",__FUNCTION__,__LINE__);
        if (seq_file_demo_dir)
                debugfs_remove(seq_file_demo_dir);
}
 
module_init(seq_file_demo_init);
module_exit(seq_file_demo_exit);
MODULE_LICENSE("GPL");