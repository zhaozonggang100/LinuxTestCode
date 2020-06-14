#ifndef _TASK_STRUCT_D_H
#define _TASK_STRUCT_D_H


static int print_task_struct_fun(struct seq_file *seq, void *v, struct task_struct *task_g);

static int print_task_struct_fun(struct seq_file *seq, void *v ,struct task_struct *task_g)
{
	//seq_printf(seq, "%s", "skb status info start:\n");
	seq_printf(seq, "***********task_struct***********\n");
	seq_printf(seq, "task_struct address: 0x%lx\n",task_g);
	seq_printf(seq, "pid        : %ld\n",task_g->pid);
	seq_printf(seq, "name       : %s\n",task_g->comm);
	seq_printf(seq, "maj_flt    : %ld\n",task_g->maj_flt);
	seq_printf(seq, "min_flt    : %ld\n",task_g->min_flt);
	return 0;
} 






#endif 