/*
 * Documentation/filesystem/seq_file.txt
 */
 #include <linux/kernel.h>
#include <linux/module.h>
#include <linux/init.h>
#include <linux/mm.h>
#include <linux/fs.h>
#include <linux/slab.h>
#include <linux/proc_fs.h>
#include <linux/seq_file.h>

//#define DEBUG_SEQ

#ifdef DEBUG_SEQ
#define log_seq(...) printk(__VA_ARGS__)
#else
#define log_seq(...)
#endif

static void *ct_seq_start(struct seq_file *s, loff_t *pos)
{
        int *count = s->private;
	printk("zhaozonggang start 01 \n");
        if ((long long)*pos < *count) {
               // printk("start pos %lld\n", (long long)*pos);
                return pos;
        }

        return NULL;
}

static void *ct_seq_next(struct seq_file *s, void *v, loff_t *pos)
{
        int *count = s->private;

        //log_seq("%s\n", __func__);
	printk("zhaozonggang next 01 \n");
        ++*pos;
        if ((long long)*pos < *count) {
                //printk("next pos %lld\n", (long long)*pos);
                return pos;
        }

        return NULL;
}

static void ct_seq_stop(struct seq_file *s, void *v)
{
        //log_seq("%s\n", __func__);
		printk("zhaozonggang stop 01 \n");
}

static int ct_seq_show(struct seq_file *s, void *v)
{
        loff_t *pos = v;

        //log_seq("%s\n", __func__);
        printk("zhaozonggang show 01 \n");
		
        seq_printf(s, "%lld\n", (long long)*pos);
        return 0;
}

static const struct seq_operations ct_seq_ops = {
        .start = ct_seq_start,
        .next = ct_seq_next,
        .stop = ct_seq_stop,
        .show = ct_seq_show
};

static int ct_open(struct inode *inode, struct file *file)
{
        int ret;
        struct seq_file *s;

		printk("zhaozonggang open 01 \n");

        ret = seq_open(file, &ct_seq_ops);
		printk("zhaozonggang open 02 \n");
        s = file->private_data;
        s->private = (void *)kmalloc(sizeof(int), GFP_KERNEL);
        *((int *)s->private) = 5;

        return ret;
}

static int ct_close(struct inode *inode, struct file *file)
{
        struct seq_file *s = file->private_data;
		printk("zhaozonggang close 02 \n");
        kfree(s->private);
        return seq_release(inode, file);
}

static const struct file_operations ct_file_ops = {
        .owner = THIS_MODULE,
        .open = ct_open,
        .read = seq_read,
//      .write = seq_write,
        .llseek = seq_lseek,
        .release = ct_close
};

static int __init ct_init(void)
{
        struct proc_dir_entry *entry;

        entry = proc_create("hh0101h", 0, NULL, &ct_file_ops);
        return 0;
}

static void __exit ct_exit(void)
{
        remove_proc_entry("hh0101h", NULL);
}

module_init(ct_init);
module_exit(ct_exit);
/*
整体看来，用户态调用一次读操作，seq_file流程为：
该函数调用struct seq_operations结构提顺序为：
start->show->next->show…->next->show->next->stop->start->stop来读取文件

*/
