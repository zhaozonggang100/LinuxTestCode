#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<netinet/in.h>
#include<string.h>
#include<sys/socket.h>
#include<arpa/inet.h>
#include<sys/types.h>
#include<sys/epoll.h>

#define MAX 128

int startUp(int port)
{
	int sock=socket(AF_INET,SOCK_STREAM,0);
	if(sock<0)
	{
		perror("socket");
		exit(2);
	}
	int opt=1;
	setsockopt(sock,SOL_SOCKET,SO_REUSEADDR,&opt,sizeof(opt));
	
	struct sockaddr_in local;
	local.sin_family=AF_INET;
	local.sin_addr.s_addr=htonl(INADDR_ANY);
	local.sin_port=htons(port);
	if(bind(sock,(struct sockaddr*)&local,sizeof(local))<0)
	{
		perror("bind");
		exit(3);
	}
	if(listen(sock,5)<0)
	{
		perror("listen");
		exit(4);
	}
		return sock;
	}
	
void serviceIO(struct epoll_event*res,int num,int epfd,int listen_sock)
{
	int i=0;
	struct epoll_event ev;
	for(i=0;i<num;i++)
	{
		//read & write
		int fd=res[i].data.fd;
		if(res[i].events&EPOLLIN)
		{
			//listen_sock&nomal
			if(fd==listen_sock)
			{
				struct sockaddr_in client;
				socklen_t len=sizeof(client);
	
				int new_sock=accept(fd,(struct sockaddr*)&client,&len);
				if(new_sock<0)
				{
					perror("accept");
					continue;
				}
				printf("get new client [%s:%d]\n",inet_ntoa(client.sin_addr),\
							ntohs(client.sin_port));
				ev.events=EPOLLIN;
				ev.data.fd=new_sock;
				epoll_ctl(epfd,EPOLL_CTL_ADD,new_sock,&ev);
			}
			else
			{
				char buf[1024];
				ssize_t s=read(fd,buf,sizeof(buf)-1);
				if(s>0)
				{
					buf[s]=0;
					printf("client:> %s\n",buf);
	
					ev.events=EPOLLOUT;
					ev.data.fd=fd;
					epoll_ctl(epfd,EPOLL_CTL_MOD,fd,&ev);
	
				}
				else if(s==0)
				{
					printf("client quit!\n");
					close(fd);
					epoll_ctl(epfd,EPOLL_CTL_DEL,fd,NULL);
				}
				else
				{
					perror("read");
					close(fd);
					epoll_ctl(epfd,EPOLL_CTL_DEL,fd,NULL);
				}
			}
		}
	if(res[i].events & EPOLLOUT)
	{
		const char*msg="http/1.0 200 OK\r\n\r\n<html><h1>epoll server,hello!</h1></html> ";
		write(fd,msg,strlen(msg));
		close(fd);
		epoll_ctl(epfd,EPOLL_CTL_DEL,fd,NULL);
	}
}
}
//./epoll_server 8080
int main(int argc,char*argv[])
{
	if(argc!=2)
	{
		printf("Usage:%s[port]\n",argv[0]);
		return 1;
	}
	//listen_sock
	int listen_sock=startUp(atoi(argv[1]));
	int epfd=epoll_create(256);
	if(epfd<0)
	{
		perror("epoll_create");
		return 5;
	}
	
	struct epoll_event ev;
	ev.events=EPOLLIN;
	ev.data.fd=listen_sock;
	epoll_ctl(epfd,EPOLL_CTL_ADD,listen_sock,&ev);
	int timeout=-1;
	struct epoll_event res[MAX];
	int num=0;
	for(;;)
	{
		switch(num=(epoll_wait(epfd,res,MAX,timeout)))
		{
			case -1:
				perror("epoll_wait");
				break;
			case 0:
				printf("timeout\n");
				break;
			default:
				serviceIO(res,num,epfd,listen_sock);
				break;
		}
	}
}
