#include <linux/kernel.h>  
#include <linux/module.h>  
#include <linux/init.h>  
#include <linux/slab.h>  
#include <linux/list.h>

#include <linux/delay.h>

struct student  
{  
   char name[100];  
   int num;  
   struct list_head list;  
};
 
struct list_head  student_list;
struct list_head  *pos;
struct student   *pstudent;
struct student   *tmp_student;
	
static int __init my_init(void)
{
	int i;
	INIT_LIST_HEAD(&student_list); //初始化链表 
	pstudent = kmalloc(sizeof(struct student)*5 ,GFP_KERNEL );
	
	for(i = 0 ;i<5 ;i++)
	{
		sprintf(pstudent[i].name, "zhaozonggang-%d",i);
		pstudent[i].num = i+1;
		list_add(&(pstudent[i].list), &student_list);//添加节点
	}
	
    list_for_each(pos,&student_list) //遍历所有的结构体 
    {  
        tmp_student= list_entry(pos,struct student,list);//计算结构体的首地址
		printk("tmp_student %p \n",tmp_student);
		
        printk("<0>student%d name: %s\n",tmp_student->num,tmp_student->name);  
    }  
	ssleep(50);
    return 0;
}

static void  __exit my_exit(void)
{
	int i ;  
    for(i=0;i<5;i++)  
    {  
        list_del(&(pstudent[i].list));      
    }  
          
    kfree(pstudent);  
    printk("module exit hello\n");
}

module_init(my_init);
module_exit(my_exit);
MODULE_AUTHOR("Dragon");
MODULE_LICENSE("GPL");
