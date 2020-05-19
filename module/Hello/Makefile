module_name=Hello
obj-m := $(module_name).o
myHello := module
kdir := /lib/modules/4.9.39/build
myinclude := /root/
all:
	make -I $(myinclude) -C $(kdir) SUBDIRS=$(PWD) modules
clean:
	rm -rf *.ko Module.* *.mod.* modules.* *.o
insmod:
	insmod $(module_name).ko
rmmod:
	rmmod module_name
ir:
	insmod $(module_name).ko
	sleep 1
	rmmod module_name
