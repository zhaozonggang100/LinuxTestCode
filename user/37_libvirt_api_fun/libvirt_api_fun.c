#include <stdio.h>
#include <stdlib.h>

#include <libvirt/libvirt.h>
#include <libvirt/libvirt-qemu.h>
#include <libvirt/virterror.h>


int main()
{
	virConnectPtr conn;
	unsigned long ver;
	char *hostname;
	conn = virConnectOpen("qemu:///system");
	
	//conn = virConnectOpenReadOnly(NULL);
	
	if(conn == NULL)
	{
		printf("conn failed\n");
	}
	printf("Dragon: %d \n",virConnectIsSecure(conn));
	/*
	获取虚拟机软件版本 virConnectGetVersion
	*/
	virConnectGetVersion(conn,&ver);
	printf("Dragon: %lu \n",ver);
	virConnectClose(conn);
	/*
	获取主机的名字
	*/
	hostname = virConnectGetHostname(conn);
	printf("hostname: %s \n",hostname);
	return 0;
}
