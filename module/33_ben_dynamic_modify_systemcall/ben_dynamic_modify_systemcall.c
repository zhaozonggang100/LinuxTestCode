#include <linux/module.h>
#include <linux/init.h>
#include <linux/types.h>
#include <linux/syscalls.h>
#include <linux/delay.h> 
#include <linux/sched.h>
#include <linux/version.h>
#include <linux/kallsyms.h>

MODULE_LICENSE("GPL");

void **syscall_table;

asmlinkage long (*orig_sys_ioctl)(unsigned int fd, unsigned int cmd,unsigned long arg);

struct mm_struct *init_mm_p;

static int syscall_nr = __NR_clone;

asmlinkage long my_new_ioctl(unsigned int fd, unsigned int cmd,unsigned long arg)
{
	printk(KERN_INFO "hello, i have hacked this sysall\n");
	
	return orig_sys_ioctl(fd, cmd, arg);
}

pte_t *walk_pagetable(unsigned long address)
{
	pgd_t *pgd = NULL;
	pte_t *pte;
	pmd_t *pmd;
	
	/* pgd */
	pgd = pgd_offset(init_mm_p, address);
	if (pgd == NULL || pgd_none(*pgd))
		return NULL;
	//printk("ben: pgd=0x%llx\n", *pgd);
	
	pmd = pmd_offset(pud_offset(pgd, address), address);
	if (pmd == NULL || pmd_none(*pmd) || !pmd_present(*pmd))
		return NULL;
	//printk("ben: pmd=0x%llx\n", *pmd);
	if (((pmd_val(*pmd) & (PMD_TYPE_SECT | PMD_SECT_USER)) ==  \
								(PMD_TYPE_SECT | PMD_SECT_USER)) || !pmd_present(*pmd)) 
	{
		return NULL;
	} 
	else if (pmd_val(*pmd) & PMD_TYPE_SECT) 
	{
		return (pte_t *)pmd;
	}
	
	pte = pte_offset_kernel(pmd, address);
	if ((pte == NULL) || pte_none(*pte))
		return NULL;
	//printk("ben: pte=0x%llx\n", *pte);
	return pte;

}





static int __init hack_syscall_init(void)
{
	pte_t *p_pte;
	pte_t pte; /* old pte */
	pte_t pte_new; 
	
	syscall_table = (void **) kallsyms_lookup_name("sys_call_table");
	if (! syscall_table) 
	{
		printk(KERN_ERR "ERROR: Cannot find the system call table address.\n"); 
		return -1;
	}
	
	printk(KERN_INFO "Found the sys_call_table at %16lx.\n", (unsigned long) syscall_table);
	
	init_mm_p = (struct mm_struct *)kallsyms_lookup_name("init_mm");
	if (! init_mm_p) 
	{
		printk(KERN_ERR "ERROR: Cannot find init_mm\n");
		return -1;
	}
	
	printk(KERN_INFO "replace system call ...\n");
	
	p_pte = walk_pagetable((unsigned long)syscall_table + 8 * syscall_nr);
	if (!p_pte)
		return -1;
	
	pte = *p_pte;
	
	printk(KERN_INFO "walk_pagetable get pte=0x%llX", pte_val(pte)); 
	
	pte_new = pte_mkyoung(pte);
	pte_new = pte_mkwrite(pte_new);
	
	printk(KERN_INFO "mkwrite pte=0x%llX", pte_new); 
	
	set_pte_at(init_mm_p, (unsigned long)syscall_table, p_pte, pte_new);
	
	p_pte = walk_pagetable((unsigned long)syscall_table);
	printk(KERN_INFO "walk_pagetable: pte=0x%llx", pte_val(*p_pte)); 
	
	printk(KERN_INFO "got sys_call_table[%d] at %16lx.\n", syscall_nr, (unsigned long)orig_sys_ioctl);
	
	orig_sys_ioctl = syscall_table[syscall_nr];
	syscall_table[syscall_nr] = my_new_ioctl;
	
	printk(KERN_INFO "got sys_call_table[%d] at %16lx.\n", syscall_nr,(unsigned long)syscall_table[syscall_nr]);
	set_pte_at(init_mm_p, (unsigned long)syscall_table, p_pte, pte);
	
	return 0;
}



static void __exit hack_syscall_exit(void)
{
	printk(KERN_INFO "syscall_release\n");
}


module_init(hack_syscall_init);
module_exit(hack_syscall_exit);