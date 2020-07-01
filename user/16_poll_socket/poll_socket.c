//https://blog.csdn.net/weixin_43825537/article/details/90211331
#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <unistd.h>
 
#include <sys/socket.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <poll.h>
 
#define NFDS 100//fds数组的大小 是不是监控100个连接？
 
// 创建一个用于监听的socket  
int CreateSocket()
{
	int listenfd = socket(AF_INET, SOCK_STREAM, 0);
	assert(-1 != listenfd);
 
	struct sockaddr_in ser;
	memset(&ser, 0, sizeof(ser));
	ser.sin_family = AF_INET;
	ser.sin_port = htons(6000);//端口
	ser.sin_addr.s_addr = inet_addr("192.168.145.178");//IP
 
	int res =  bind(listenfd, (struct sockaddr*)&ser, sizeof(ser));
	assert(-1 != res);
 
	listen(listenfd, 5);
 
	return listenfd;
}
 
// 初始化fds结构体数组
void InitFds(struct pollfd *fds)
{
	int i = 0;
	for(; i < NFDS; ++i)
	{
		fds[i].fd = -1;///* 文件描述符 */
		fds[i].events = 0;//等待的时间
		fds[i].revents = 0;//实际发生的事件
	}
}
 
// 向fds结构体数组中插入一个文件描述符
void InsertFd(struct pollfd *fds, int fd, int flag)//此处flag是为了判断是文件描述符c，还是listenfd，来设置events
{
	int i = 0;
	for(; i < NFDS; ++i)
	{
		if(fds[i].fd == -1)
		{
			fds[i].fd = fd;
			fds[i].events |= POLLIN; 
			if(flag)
			{
				fds[i].events |= POLLRDHUP;
			}
 
			break;
		}
	}
}
 
// 从fds结构体数组中删除一个文件描述符
void DeleteFd(struct pollfd *fds, int fd)
{
	int i = 0;
	for(; i < NFDS; ++i)
	{
		if(fds[i].fd == fd)
		{
			fds[i].fd = -1;
			fds[i].events = 0; 
			break;
		}
	}
}
 
// 获取一个已完成三次握手的连接
void GetClientLink(int fd, struct pollfd *fds)
{
	struct sockaddr_in cli;
	socklen_t len = sizeof(cli); 
	int c = accept(fd, (struct sockaddr*)&cli, &len);
	assert(c != -1);
 
	printf("one client link success\n");
 
	InsertFd(fds, c, 1);
}
 
// 断开一个用户连接
void UnlinkClient(int fd, struct pollfd *fds)
{
	close(fd);
	DeleteFd(fds, fd);
	printf("one client unlink\n");
}
 
// 处理客户端发送来的数据
void DealClientData(int fd, struct pollfd *fds)
{
	char  buff[128] = {0};
 
	int n = recv(fd, buff, 127, 0);
	if(n <= 0)
	{
		UnlinkClient(fd, fds);
		return;
	}
 
	printf("%s\n", buff);
 
	send(fd, "ok", 2, 0);
}
 
// poll返回后，处理就绪的文件描述符
void DealFinishFd(struct pollfd *fds, int listenfd)
{
	int i = 0;
	for(; i < NFDS; ++i)
	{
		if(fds[i].fd == -1)
		{
			continue;
		}
 
		int fd = fds[i].fd;
		/*
		判断是不是这个结构体 
		*/
		if(fd == listenfd && fds[i].revents & POLLIN)
		{
			//zhaozonggang_GetClientLink[3][1][1] 
			printf("zhaozonggang_GetClientLink[%d][%d][%d] \n",listenfd, fds[i].revents ,POLLIN);
			GetClientLink(fd, fds);
			//获取连接
		}
		else if(fds[i].revents & POLLRDHUP)
		{
			//zhaozonggang_UnlinkClient[3][8193][8192] 
			printf("zhaozonggang_UnlinkClient[%d][%d][%d] \n",listenfd, fds[i].revents ,POLLRDHUP);
			UnlinkClient(fd, fds);
			//断开连接
		}
		else if(fds[i].revents & POLLIN)
		{
			printf("zhaozonggang_DealClientData[%d][%d][%d] \n",listenfd, fds[i].revents ,POLLIN);
			DealClientData(fd, fds);
			//处理客户端数据
		}
	}
}
 
int main()
{
	int listenfd = CreateSocket();//创建一个服务端
 
	struct pollfd *fds = (struct pollfd*)malloc(sizeof(struct pollfd) * NFDS);
	//malloc一个fds结构体数组
	assert(NULL != fds);//如果程序正确继续执行，否则报错停止
 
	InitFds(fds);
	//初始化fds结构体数组
 
	InsertFd(fds, listenfd, 0);
	//插入文件描述符listenfd
 
 
	while(1)
	{
		/*
		（1）fds：指向元素类型为struct poll_fd类型的首元素
		（2）nfds：fds结构体数组的大小
		（3）timeout：表示poll函数超时的时间限制
		*/
		int n = poll(fds, NFDS, -1);//阻塞等待有事件发生 在执行
		//printf("zhaozonggang [%d]\n",n);
		if(n <= 0)
		{
			printf("poll error\n");
			continue;
		}
 
		DealFinishFd(fds, listenfd);
		//处理就绪的文件描述符
	}
 
	free(fds);
}
