#ifndef _LOADAVG_D_H
#define _LOADAVG_D_H

#include <linux/seq_file.h>
static int print_kernel_global_variables(struct seq_file *seq, void *v);

#define FSHIFT		11		/* nr of bits of precision */
#define FIXED_1		(1<<FSHIFT)	/* 1.0 as fixed-point */
#define LOAD_INT(x) ((x) >> FSHIFT)
#define LOAD_FRAC(x) LOAD_INT(((x) & (FIXED_1-1)) * 100)


static unsigned long *ptr_avenrun;


static int print_kernel_global_variables(struct seq_file *seq, void *v)
{
	//uptime uproc 命令通过avenrun数组获取值
	unsigned long zhao = 2048;
    ptr_avenrun = (void *)kallsyms_lookup_name("avenrun");//获取内核变量的地址
	printk("avenrun1 = 0x%p ",ptr_avenrun);
	//printk("avenrun2 = %lx ",ptr_avenrun);
	seq_printf(seq,"Load: %lu.%02lu, %lu.%02lu, %lu.%02lu\n",
			LOAD_INT(ptr_avenrun[0]), LOAD_FRAC(ptr_avenrun[0]),
			LOAD_INT(ptr_avenrun[1]), LOAD_FRAC(ptr_avenrun[1]),
			LOAD_INT(ptr_avenrun[2]), LOAD_FRAC(ptr_avenrun[2]));
	seq_printf(seq,"zhao=%lu \n",zhao*100);
	seq_printf(seq,"zhao=%lu \n",LOAD_INT(zhao));
	seq_printf(seq,"zhao=%lu \n",LOAD_FRAC(zhao));
	return 0;
}
 
#endif 
