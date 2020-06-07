module_name=read_kernel_parameter
obj-m := $(module_name).o
myHello := module
kdir := /lib/modules/4.19.122/build
myinclude := /root/
all:
	make -I $(myinclude) -C $(kdir) SUBDIRS=$(PWD) modules
clean:
	rm -rf *.ko Module.* *.mod.* modules.* *.o
i:
	insmod $(module_name).ko
r:
	rmmod $(module_name).ko
ir:
	insmod $(module_name).ko
	sleep 1
	rmmod $(module_name).ko
