#include <stdio.h>
#include <time.h>

struct tcphdr{
	unsigned short sport;
	unsigned short dport;
	unsigned int seq;
	unsigned int ack_seq;
	unsigned short hlen:4,
				   reserved:6,
				   urg:1,
				   ack:1,
				   psh:1,
				   rst:1,
				   syn:1,
				   fin:1;
	unsigned short win_size;
	unsigned short check;
	unsigned short emergency_p;
				   
};

int main()
{
	int j=0;
	char a[20]={1,2,3,4,5,6,7,8,9,10, \
				11,12,13,14,15,16,17,18,19,20};
	
	struct tcphdr *tcphdr01={0};
	tcphdr01 = (struct tcphdr *)a;
	printf("sizeof:%d\n",sizeof(tcphdr01));
	
	printf("tcphdr01.sport[%d]\n",tcphdr01->sport);
	printf("tcphdr01.dport[%d]\n",tcphdr01->dport);
	printf("tcphdr01.seq[%d]\n",tcphdr01->seq);
	printf("tcphdr01.ack_seq[%d]\n",tcphdr01->ack_seq);
	printf("tcphdr01.hlen[%d]\n",tcphdr01->hlen);
	printf("tcphdr01.reserved[%d]\n",tcphdr01->reserved);
	printf("tcphdr01.urg[%d]\n",tcphdr01->urg);
	printf("tcphdr01.ack[%d]\n",tcphdr01->ack);
	printf("tcphdr01.psh[%d]\n",tcphdr01->psh);
	printf("tcphdr01.rst[%d]\n",tcphdr01->rst);
	printf("tcphdr01.syn[%d]\n",tcphdr01->syn);
	printf("tcphdr01.fin[%d]\n",tcphdr01->fin);
	printf("tcphdr01.win_size[%d]\n",tcphdr01->win_size);
	printf("tcphdr01.check[%d]\n",tcphdr01->check);
	printf("tcphdr01.emergency_p[%d]\n",tcphdr01->emergency_p);
	
	printf("*****************\n");
	
	while(1)
	{
	//	printf("add[%d] \r ",j);
	//	printf("add[%d] \r ",j);
		sleep(1);
	//	j++;
;
		
	}
}
