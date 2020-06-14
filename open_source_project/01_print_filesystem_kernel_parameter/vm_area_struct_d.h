#ifndef _VM_AREA_STRUCT_D_H
#define _VM_AREA_STRUCT_D_H

static int print_vm_area_struct_fun(struct seq_file *seq, void *v,struct mm_struct *mm_struct_g)
{

	struct vm_area_struct *vm_area_struct_g;
	unsigned long int vm_len=0  ;
	unsigned long int hiwater_rss , all_phy_,file_page,anon_page,swap_page,shmem_page;
	
	vm_area_struct_g = mm_struct_g->mmap;
	hiwater_rss = atomic_long_read(&mm_struct_g->hiwater_rss);
	file_page = atomic_long_read(&mm_struct_g->rss_stat.count[0]);
	anon_page = atomic_long_read(&mm_struct_g->rss_stat.count[1]);
	swap_page = atomic_long_read(&mm_struct_g->rss_stat.count[2]);
	shmem_page = atomic_long_read(&mm_struct_g->rss_stat.count[3]);
	all_phy_ = (file_page +  anon_page) * 4;
	seq_printf(seq, "***********vm_area_struct***********\n");
	seq_printf(seq, "vm_area_struct address: 0x%lx\n",mm_struct_g);
	seq_printf(seq, "hiwater_rss : %ld\n",hiwater_rss);
	seq_printf(seq, "all_phy_    : %ld\n",all_phy_);
	seq_printf(seq, "file_page   : %ld\n",file_page);
	seq_printf(seq, "anon_page   : %ld\n",anon_page);
	seq_printf(seq, "swap_page   : %ld\n",swap_page);
	seq_printf(seq, "shmem_page  : %ld\n",shmem_page);
	//计算vmd
	while(vm_area_struct_g)
	{
		
		vm_len = vm_area_struct_g->vm_end - vm_area_struct_g->vm_start + vm_len ;
		//printk("vm_len [%ld]  [%ld] [%ld]\n   ",i++, vm_len ,vm_len/1024);
		
		//printk(" [%ld]  [%lx] \n",vm_area_struct_g->vm_end , vm_area_struct_g->vm_start);
		vm_area_struct_g = vm_area_struct_g->vm_next;
	}
	seq_printf(seq, "vm_addr   : %ld\n",vm_len/1024);
	return 0; 
}

#endif 