#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/init.h>
#include <linux/slab.h>
#include <linux/spinlock.h>
#include <linux/rcupdate.h>
#include <linux/kthread.h>
#include <linux/delay.h>

struct foo
{
    int a;
    struct rcu_head rcu;
};

static struct foo* g_ptr;

//读者线程
static void myrcu_header_thread(void* data)
{
    struct foo* p =NULL;

    while(1)
    {
        msleep(200);
        rcu_read_lock();
        /*
           用于获取被RCU保护的指针，读者线程要访问RCU保护的共享数据，
           需要使用该函数创建一个新的指针，并且指向被RCU保护的指针
        */
        p = rcu_dereference(g_ptr);
        if(p)
            printk("%s: read a=%d\n",__func__,p->a);
        rcu_read_unlock();
    }
}

static void myrcu_del(struct rcu_head* rh)
{
    struct foo* p = container_of(rh, struct foo, rcu);
    printk("%s: a=%d\n", __func__ ,p->a);
    kfree(p);
}

//写者线程
static void myrcu_writer_thread(void* p)
{
    struct foo* new;
    struct foo* old;

    int value = (unsigned long) p;

    while(1)
    {
        msleep(400);
        struct foo* new_ptr = kmalloc(sizeof(struct foo), GFP_KERNEL);
        old = g_ptr;
        printk("%s: write to new %d\n", __func__ ,value);
        *new_ptr = *old;
        new_ptr -> a = value;
        /*
           写者线程完成新数据的修改之后，调用该函数可以让被RCU保护的指针指向新创建的数据
           （publish 了更新后的数据）
        */
        rcu_assign_pointer(g_ptr,new_ptr);
        /*
            注册一个回调函数，当所有现存的读访问完成之后，调用这个回调函数销毁旧数据
        */
        call_rcu(&old->rcu,myrcu_del);
        value++;
    }
}

static int __init my_test_init(void)
{
    struct task_struct* reader_thread;
    struct task_struct* writer_thread;

    int value = 5;

    printk("figo: my module init\n");
    g_ptr = kzalloc(sizeof(struct foo), GFP_KERNEL);

    reader_thread = kthread_run(myrcu_header_thread,NULL,"rcu_reader");
    writer_thread = kthread_run(myrcu_writer_thread,(void*)(unsigned long) value, "rcu_writer");

    return 0;
}

static void __exit my_test_exit(void)
{
    printk("goodbye\n");
    if(g_ptr)
        kfree(g_ptr);
}

MODULE_LICENSE("GPL");
module_init(my_test_init)
module_exit(my_test_exit);
MODULE_AUTHOR("Dragon");