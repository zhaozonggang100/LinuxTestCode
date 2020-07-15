#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>
#include<sys/socket.h>
#include<sys/select.h>
#include<netinet/in.h>
#include<arpa/inet.h>

#define INIT_DATA -1
#define MAX_FD 1024
 
static void InitArray(int fdArray[],int size)
{
    int i=0;
    for(;i<size;i++)
        fdArray[i]=INIT_DATA;
}
 
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
	//local 
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
	
	static int AddtoArray(int fd,int array[],int num)
	{
		int i=0;
		for(i=0;i<num;i++)
		{
			if(array[i]==INIT_DATA)
			{
				array[i]=fd;
				return i;
			}
		}
		return -1;
	}
	
static int AddtoFdset(int array[],int num,fd_set*rfds)
{
	int i=0;
	int max_fd=INIT_DATA;
	for(;i<num;i++)
	{
		if(array[i]>=0)
		{
			FD_SET(array[i],rfds);
			if(max_fd<array[i])
			max_fd=array[i];
		}
	}
	return max_fd;
	}
	
static void serverice_select(int array[],int num,fd_set *rfds)
{
	int i=0;
	for(;i<num;i++)
	{
		if(array[i]>INIT_DATA)
		{//listen_sock & normal
			int fd=array[i];
			if(i==0&&FD_ISSET(array[i],rfds))
			{
				//listen_sock ready
				struct sockaddr_in client;
				socklen_t len=sizeof(client);
				int sock=accept(fd,(struct sockaddr*)&client,&len);
				if(sock<0)
				{
					perror("accept");
					continue;
				}
				printf("get a connect[%s:%d]\n",inet_ntoa(client.sin_addr),\
				ntohs(client.sin_port));
				int ret=AddtoArray(sock,array,num);
				if(ret==-1)//array full 
					close(sock);
			}
			else if(i!=0&&FD_ISSET(array[i],rfds))
			{
				//normal fd ready
				printf("read:)\n");
				char buf[1024];
				ssize_t s=read(fd,buf,sizeof(buf)-1);
				if(s<0)
				{
					perror("read");
					close(fd);
					array[i]=INIT_DATA;
				}
				else if(s==0)
				{
					perror("read the end\n");
					close(fd);
					array[i]=INIT_DATA;
					printf("client quit\n");
				}
				else
				{
				printf("read  success!\n");
				buf[s]=0;
				printf("client:> %s\n",buf);
				}
			}
				else
				{
					//array[i] exsit not ready
				}
			}
		}
	}
	// ./server port
int main(int argc,char*argv[])
{
	if(argc!=2)
	{
		printf("Usage:%s[port]\n",argv[0]);
		return 1;
	}
	//tcp---select
	int listen_sock=startUp(atoi(argv[1]));
	int fdArray[MAX_FD];
	int size=sizeof(fdArray)/sizeof(fdArray[0]);
	InitArray(fdArray,size);
	AddtoArray(listen_sock,fdArray,size);
	
	fd_set read_fds;
	for(;;)
	{
		FD_ZERO(&read_fds);//清零
		int max_fd=AddtoFdset(fdArray,size,&read_fds);
		struct timeval timeout={0};
		//ret 所有符合 监控 文件描述符个数
		int ret=select(max_fd+1,&read_fds,NULL,NULL,NULL);
		
		switch(ret)
		{
		case -1:
			{
				perror("select");
				break;
			}
		case 0:
			{
				perror("Time_Out");
				break;
			}
		default:
			{
				serverice_select(fdArray,size,&read_fds);
				break;
			}
		}
	}
}
