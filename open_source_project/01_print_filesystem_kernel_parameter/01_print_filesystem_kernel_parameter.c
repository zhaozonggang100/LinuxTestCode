#include <linux/init.h>
#include <linux/module.h>
#include <linux/seq_file.h>
#include <linux/debugfs.h>
#include <linux/fs.h>
#include <linux/proc_fs.h>

//static struct dentry *seq_file_demo_dir;
static int int_pid;

//module_param(int_pid,int,0644);

static int seq_file_demo_show(struct seq_file *seq, void *v)
{
        seq_printf(seq, "Hello World\n");
		seq_printf(seq,(char *)&int_pid);
		seq_printf(seq, "Hello World123\n");
        return 0;
}

static int seq_file_demo_open(struct inode *inode, struct file *file)
{
        return single_open(file, &seq_file_demo_show, NULL);
}

static ssize_t seq_file_demo_write(struct file *file, const char __user *buffer,size_t count, loff_t *pos)
{
    int val;

    if(sscanf(buffer, "%d", &val) != 1)
        return -EINVAL;

    if(val == 1)
        printk("proc_test1 write true  [%d]\n",val);
    else 
        printk("proc_test1 write false [%d]\n",val);

    return val;
}

static const struct file_operations hello_proc_fops = {
        .owner = THIS_MODULE,
        .open = seq_file_demo_open,
        .read = seq_read,
		.write = seq_file_demo_write,
        .llseek = seq_lseek,
        .release = single_release,
};

static int __init seq_file_demo_init(void)
{
	proc_create("hello_proc",0,NULL,&hello_proc_fops);
    return 0;
}

static void __exit seq_file_demo_exit(void)
{
	remove_proc_entry("hello_proc",NULL);
}

module_init(seq_file_demo_init);
module_exit(seq_file_demo_exit);
MODULE_LICENSE("GPL");