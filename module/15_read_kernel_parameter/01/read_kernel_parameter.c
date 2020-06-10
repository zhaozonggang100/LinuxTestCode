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

		printk("start.....\n");
        if ((long long)*pos < *count) {
               // printk("start pos %lld\n", (long long)*pos);
                return pos;
        }

        return NULL;
}

static void *ct_seq_next(struct seq_file *s, void *v, loff_t *pos)
{
        int *count = s->private;

		printk("next.....\n");
        ++*pos;
        if ((long long)*pos < *count) {
                //printk("next pos %lld\n", (long long)*pos);
                return pos;
        }

        return NULL;
}

static void ct_seq_stop(struct seq_file *s, void *v)
{
       // log_seq("%s\n", __func__);
	   printk("stop.....\n");
	   return ;

}

static int ct_seq_show(struct seq_file *s, void *v)
{
        loff_t *pos = v;

		printk(" show \n");
        seq_printf(s, "%s\n", "hello wold");
        return 0;
}



static const struct seq_operations ct_seq_ops = {
    .start = ct_seq_start,
    .next = ct_seq_next,
    .stop = ct_seq_stop,
    .show = ct_seq_show
};

static int ct_open(struct inode *inode, struct file *file) {
	int ret ;
	struct seq_file *s;
	printk("enter ct_open \n");
	ret  = seq_open(file,&ct_seq_ops);
	s = file->private_data; //什么作用？？？？？？
	s->private = (void *)kmalloc(sizeof(int), GFP_KERNEL);
    *((int *)s->private) = 5;
	return 0;
	
}

static const struct file_operations hello_proc_fops = {
	.owner = THIS_MODULE,
	.open = ct_open,
	.read = seq_read,
//	.write = seq_write ,
	.llseek = seq_lseek,
	.release = seq_release,
};

//内核模块初始化函数
static int __init myirq_init(void)
{
	printk("*****strart******");
	proc_create("hello_proc",0,NULL,&hello_proc_fops);
	//#proc_pf = proc_mkdir("pf",0);
	//#proc_create("pfcount",0,NULL,get_pfcount);
	//proc_create("capi/capi20", 0, NULL, &capi20_proc_fops);
	return 0;
} 

//内核模块退出函数
static void __exit myirq_exit(void)
{
	printk("*****end******");
	remove_proc_entry("hello_proc", NULL);
	//#remove_proc_entry("pfcount", proc_pf);
	//#remove_proc_entry("pf",0);
}

MODULE_LICENSE("GPL");
module_init(myirq_init);
module_exit(myirq_exit);