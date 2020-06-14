#ifndef _ALL_PROCESS_D_H
#define _ALL_PROCESS_D_H



static int print_all_process(struct seq_file *seq, void *v)
{
	int kernel_count = 0;
	int user_count = 0;
	struct task_struct *p;
	p = &init_task;
	for_each_process(p)
	{
		if(p->mm == NULL)
		{  
			
			printk("comm=%s , pid=%d \n",p->comm,p->pid);
			kernel_count++;
			
		}
		else
		{
			printk("*****************comm=%s , pid=%d \n",p->comm,p->pid);
			user_count++;
		}
	}
	seq_printf(seq, "kernel_process  : %d \n",kernel_count);
	seq_printf(seq, "user_process    : %d \n",user_count);
 
	return 0;
}


#endif 