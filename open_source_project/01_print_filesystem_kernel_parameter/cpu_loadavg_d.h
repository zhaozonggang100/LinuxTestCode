#ifndef _CPU_LOADAVG_D_H
#define _CPU_LOADAVG_D_H



static int print_cpu_loadavg(struct seq_file *seq, void *v)
{
	/*
	seq_put_decimal_ull(p, "cpu  ", cputime64_to_clock_t(user));
	seq_put_decimal_ull(p, " ", cputime64_to_clock_t(nice));
	seq_put_decimal_ull(p, " ", cputime64_to_clock_t(system));
	seq_put_decimal_ull(p, " ", cputime64_to_clock_t(idle));
	seq_put_decimal_ull(p, " ", cputime64_to_clock_t(iowait));
	seq_put_decimal_ull(p, " ", cputime64_to_clock_t(irq));
	seq_put_decimal_ull(p, " ", cputime64_to_clock_t(softirq));
	seq_put_decimal_ull(p, " ", cputime64_to_clock_t(steal));
	seq_put_decimal_ull(p, " ", cputime64_to_clock_t(guest));
	seq_put_decimal_ull(p, " ", cputime64_to_clock_t(guest_nice));
	seq_putc(p, '\n');
	*/
	//计算cpu所用的时间
	int i ;
	u64 user, nice, system, irq, softirq, steal;
	u64 guest, guest_nice;
	
    for_each_possible_cpu(i)
    {
		//i是输出参数[0-127] 本代码中
        printk("possible cpu :%d\n", i);
    }
	
    for_each_online_cpu(i)
    {
		/*使用此函数 计算cpu时间值
		[22851.246190] online cpu :0
		[22851.246190] online cpu :1
		*/
		user = kcpustat_cpu(i).cpustat[CPUTIME_USER];
		nice = kcpustat_cpu(i).cpustat[CPUTIME_NICE];
		system = kcpustat_cpu(i).cpustat[CPUTIME_SYSTEM];
		//idle = get_idle_time(i);
		//iowait = get_iowait_time(i);
		irq = kcpustat_cpu(i).cpustat[CPUTIME_IRQ];
		softirq = kcpustat_cpu(i).cpustat[CPUTIME_SOFTIRQ];
		steal = kcpustat_cpu(i).cpustat[CPUTIME_STEAL];
		guest = kcpustat_cpu(i).cpustat[CPUTIME_GUEST];
		guest_nice = kcpustat_cpu(i).cpustat[CPUTIME_GUEST_NICE];
		
		seq_printf(seq, "zhaozonggang %llu %llu\n", user,cputime64_to_clock_t(user));//求出cpu时间
		
        //printk("online cpu :%d \n", i);
		//printk("%llu %llu  %llu %llu %llu %llu  \n", user,nice,system,irq,softirq,steal);
    }
 
    for_each_present_cpu(i)
    {
		/*
		[22851.246191] present cpu :0
		[22851.246191] present cpu :1
		*/
        seq_printf(seq,"present cpu :%d\n", i);
    }
	
	//计算进程所占用的CPU
	
	return 0;
}


#endif 