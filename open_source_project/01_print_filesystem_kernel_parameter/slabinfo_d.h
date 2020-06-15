#ifndef _SLABINFO_D_H
#define _SLABINFO_D_H

#include "linux/slab_def.h"


static int print_slabinfo(struct seq_file *seq, void *v)
{
	struct kmem_cache *s = list_entry(v, struct kmem_cache, list);
	//if (v == slab_caches.next)
	//	print_slabinfo_header(seq);
	//cache_show(s, m);
	 s = (struct kmem_cache *)kallsyms_lookup_name("kmem_cache");//获取内核变量的地址
	seq_printf(seq, "kmem_cache_add:%p \n",s);
	seq_printf(seq, "kmem_cache_add:%lx \n",s);
	return 0;
}

#endif 