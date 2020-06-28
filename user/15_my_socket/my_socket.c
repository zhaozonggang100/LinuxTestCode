#include <stdio.h>
#include <sys/poll.h>
#include <time.h>

#define NETMAP_WITH_LIBS
#include <net/netmap_user.h>

#pragma(1)

//应用层 你好
#define PORT_IP 0x0800
#define PORT_UDP 17



//数据链路层 [eth][ip][udphdr]你好  [内核]
#define ETH_LENGTH 6
struct ethhdr{
	//目的地址(6)+源地址(6)+类型(2) 数据填充 CRC校验(2) = 16字节
	unsigned char dest[ETH_LENGTH];
	unsigned char source[ETH_LENGTH];
	unsigned short protocal;
};//sizeof = 6+6+2

//网络层 [ip][udphdr]你好           [内核]
struct iphdr{
	unsigned char version:4,
				  hdr_leng:4;
	unsigned char tos;//8位的服务类型
	unsigned short totlen;//总长度
	unsigned short id;
	unsigned short flag:3,
				   offset:13;
	unsigned char ttl;//生命周期
	unsigned char proto;
	unsigned short check;
	unsigned int sip;
	unsigned int dip;
};//sizeof = 1+1+2+2+2+1+1+2(12) + 8 = 20

//传输层(UDP) [udphdr]你好               [内核]
struct udphdr{
	//UDP端口号(2) UDP目的端口(2) UDP长度(2) UDP效验(2)
	unsigned short sport;
	unsigned short dport;
	unsigned short len;
	unsigned short check;
		
};//sizeof 2+2+2+2

struct udppkt{
	struct ethhdr eh;
	struct iphdr ip;
	struct udphdr udp;
	unsigned char body[0];
};//sizeof 14+20+8=42(需要是4的整数倍 44)


int main()
{
	struct ethhdr ethhdr01;
	struct iphdr iphdr01;
	struct udphdr udphdr01;
	struct udppkt udppkt01;
	printf("ethhdr01:%d\n",sizeof(ethhdr01));
	printf("iphdr01:%d\n",sizeof(iphdr01));
	printf("udphdr01:%d\n",sizeof(udphdr01));
	printf("udppkt01:%d\n",sizeof(udppkt01));
	sleep(1);
	struct nm_desc *nmr = (struct nm_desc *)nm_open("netmap:ens33",NULL ,0 ,NULL);
	if(nmr == NULL)
	{ 
		return -1;
	}
	
	struct pollfd pfd = {0};
	pfd.fd = nmr->fd;
	pfd.events = POLLIN;
	int ret;
	
	while(1)
	{
		ret = poll(&pfd, 1, -1);
		
		if(ret < 0) continue;
		printf("***OK***\n");
		if(pfd.revents & POLLIN)
		{
			struct nm_pkthdr h;
			unsigned char *stream = nm_nextpkt(nmr ,&h);//read
			struct ethhdr *eh = ( struct ethhdr *)stream ;
			if(ntohs(eh->protocal) == PORT_IP)
			{
				struct udppkt *udp = (struct udppkt *)stream;
				if(udp->ip.proto == PORT_UDP)
				{
					int udp_length = ntohs(udp->udp.len);
					udp->body[udp_length - 8] = '\0';
					printf("udp-->> %s\n",udp->body);
				}
			}
			
		}
	}
	
	return 0;
}