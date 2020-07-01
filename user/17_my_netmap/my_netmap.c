#include <stdio.h>
#include <poll.h>
 
#define NETMAP_WITH_LIBS
#include <net/netmap_user.h>
 
unsigned long pps = 0;
 
static void receive_packets(struct netmap_ring *ring)
{
    int i;
    char *buf;
 
    // 遍历所有的槽位
    while (!nm_ring_empty(ring)) {
        i   = ring->cur;
        buf = NETMAP_BUF(ring, ring->slot[i].buf_idx); // buf 就是收到的报文喽
        pps++;  // 统计收包个数
        ring->head = ring->cur = nm_ring_next(ring, i);  // 下一个槽位
    }
}
 
int main(void)
{
    struct nm_desc *d;
    struct pollfd fds;
    struct netmap_ring *ring;
    int i; 
    
    d = nm_open("netmap:ens33", NULL, 0, 0); // 注意格式，netmap:ehtX
 
    // d 的返回值我这里就不判断了
 
    fds.fd     = d->fd;
    fds.events = POLLIN;
	
	printf("zhaozonggang [%d]\n",d->fd);
	sleep(20);
    while (1) {
        if (poll(&fds, 1, -1) < 0) {
            perror("poll()");
            exit(1);
        }
 
        // 遍历所有的接收队列
		printf("*****\n");
        for (i = d->first_rx_ring; i <= d->last_rx_ring; i++) {
            ring = NETMAP_RXRING(d->nifp, i);
            receive_packets(ring);  // 处理 ring
        }
    }
}
