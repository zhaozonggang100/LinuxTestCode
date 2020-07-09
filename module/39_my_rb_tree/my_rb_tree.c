#include <linux/kthread.h>
#include <linux/time.h>
#include <linux/mm.h>
#include <linux/module.h>
#include <linux/nmi.h>
#include <linux/init.h>
#include <linux/uaccess.h>
#include <linux/highmem.h>
#include <linux/interrupt.h>
#include <linux/capability.h>
#include <linux/completion.h>
#include <linux/kernel_stat.h>
#include <linux/debug_locks.h>
#include <linux/perf_event.h>
#include <linux/security.h>
#include <linux/notifier.h>
#include <linux/profile.h>
#include <linux/freezer.h>
#include <linux/vmalloc.h>
#include <linux/blkdev.h>
#include <linux/delay.h>
#include <linux/pid_namespace.h>
#include <linux/smp.h>
#include <linux/threads.h>
#include <linux/timer.h>
#include <linux/rcupdate.h>
#include <linux/cpu.h>
#include <linux/cpuset.h>
#include <linux/percpu.h>
#include <linux/proc_fs.h>
#include <linux/seq_file.h>
#include <linux/sysctl.h>
#include <linux/syscalls.h>
#include <linux/times.h>
#include <linux/tsacct_kern.h>
#include <linux/kprobes.h>
#include <linux/delayacct.h>
#include <linux/unistd.h>
#include <linux/pagemap.h>
#include <linux/hrtimer.h>
#include <linux/tick.h>
#include <linux/debugfs.h>
#include <linux/ctype.h>
#include <linux/ftrace.h>
#include <linux/slab.h>
#include <linux/init_task.h>
#include <linux/binfmts.h>
#include <linux/string.h>

#include <linux/rbtree.h>
#include <linux/rbtree_augmented.h>

#define  alen 9


#define CHECK_INSERT 0    // "插入"动作的检测开关(0，关闭；1，打开)
#define CHECK_DELETE 0    // "删除"动作的检测开关(0，关闭；1，打开)
#define LENGTH(a) ( (sizeof(a)) / (sizeof(a[0])) )

typedef int Type;

struct my_node {
    struct rb_node rb_node;    // 红黑树节点
    Type key;                // 键值
    // ... 用户自定义的数据
};

/*
 * 查找"红黑树"中键值为key的节点。没找到的话，返回NULL。
 */
struct my_node *my_search(struct rb_root *root, Type key)
{
    struct rb_node *rbnode = root->rb_node;

    while (rbnode!=NULL)
    {
        struct my_node *mynode = container_of(rbnode, struct my_node, rb_node);

        if (key < mynode->key)
            rbnode = rbnode->rb_left;
        else if (key > mynode->key)
            rbnode = rbnode->rb_right;
        else
            return mynode;
    }

    return NULL;
}

/*
 * 将key插入到红黑树中。插入成功，返回0；失败返回-1。
 */
int my_insert(struct rb_root *root, Type key)
{
	
	/**
	
	1-searching for the place to insert thenew node
	2-inserting the node
	3-rebalancing ("recoloring") the tree
	*/
    struct my_node *mynode; // 新建结点
    struct rb_node **tmp = &(root->rb_node), *parent = NULL;

    /* Figure out where to put new node */
    while (*tmp)
    {
        struct my_node *my = container_of(*tmp, struct my_node, rb_node);//container_of查找地址函数
		//printk("my_node add[%lx][%lx]\n",my,*tmp);
        parent = *tmp;
        if (key < my->key)
            tmp = &((*tmp)->rb_left);
        else if (key > my->key)
            tmp = &((*tmp)->rb_right);
        else
            return -1;
    }

    // 如果新建结点失败，则返回。
    if ((mynode=kmalloc(sizeof(struct my_node),GFP_KERNEL)) == NULL)
        return -1;
    mynode->key = key;

/*
rb_link_node(&people->node, parent, new);
parent是父节点，new是要插入的父节点的子节点的地址。
rb_link_node()插入节点：将parent的子节点指向新节点，将新节点的父设置为parent，将新节点的左右子节点设置为NULL。

*/


    /* Add new node and rebalance tree. */
    rb_link_node(&mynode->rb_node, parent, tmp);//初始化新节点
	
	/*
	rb_insert_color()：进行上色旋转，保持为红黑树
	*/
    rb_insert_color(&mynode->rb_node, root);

    return 0;
}

/*
 * 删除键值为key的结点
 */
void my_delete(struct rb_root *root, Type key)
{
    struct my_node *mynode;

    // 在红黑树中查找key对应的节点mynode
    if ((mynode = my_search(root, key)) == NULL)
        return ;

    // 从红黑树中删除节点mynode
    rb_erase(&mynode->rb_node, root);
    kfree(mynode);
}

/*
 * 打印"红黑树"
 */
static void print_rbtree(struct rb_node *tree, Type key, int direction)
{
    if(tree != NULL)
    {
        if(direction==0)    // tree是根节点
		{
            printk("%2d(黑色) is root\n", key);
			//printk("dragon[%d]\n",rb_is_black(tree));
		}
        else                // tree是分支节点
            printk("%2d(%s) is %2d's %6s child\n", key, rb_is_black(tree)?"黑":"红", key, direction==1?"右" : "左");

        if (tree->rb_left)
            print_rbtree(tree->rb_left, rb_entry(tree->rb_left, struct my_node, rb_node)->key, -1);
        if (tree->rb_right)
            print_rbtree(tree->rb_right,rb_entry(tree->rb_right, struct my_node, rb_node)->key,  1);
    }
} 



void my_print(struct rb_root *root)
{
    if (root!=NULL && root->rb_node!=NULL)

    print_rbtree(root->rb_node, rb_entry(root->rb_node, struct my_node, rb_node)->key,  0);
}


static int __init my_init(void)
{
	
	//int a[] = {10, 40, 30, 60, 90, 70, 20, 50, 80};
	int a[] = {275, 711, 260, 515, 442, 800, 900, 50, 270, 20, 30};
    int i, ilen=LENGTH(a);
    struct rb_root mytree = RB_ROOT;

    printk("== 原始数据: ");
    for(i=0; i<ilen; i++)
        printk("%d ", a[i]);
    printk("\n");

    for (i=0; i < ilen; i++)
    {
        my_insert(&mytree, a[i]);
		printk("== 添加节点: %d\n", a[i]);
		printk("== 树的详细信息start: \n");
		my_print(&mytree);
		printk("== 树的详细信息end: \n");
    }

	
	
    return 0;
}

static void  __exit my_exit(void)
{
	 
    printk("module exit hello\n");
}

module_init(my_init);
module_exit(my_exit);
MODULE_AUTHOR("Dragon");
MODULE_LICENSE("GPL");
