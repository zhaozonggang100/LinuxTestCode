/* exec函数族的语法 */
//https://blog.csdn.net/amoscykl/article/details/80354052
#include<stdio.h>
#include<unistd.h>
#include<stdlib.h>
#include<string.h>
#include<errno.h>
 
 
int main(int argc, char *argv[])
{
	/* 字符串指针数组传递参数,使用包含v的exec函数参数 */
	char *arg[] = {"ls","-a",NULL};
	char *arg1[] = {"env",NULL};		//只用于execve函数
 
	char *envp[] = {"NAME=amoscykl","EMAIL=xxxx@xx.com","PATH=/tmp",NULL};
	char **ptr;				//指向环境表
 
	// 打印出环境表
	printf("自定义环境表\n");
 
	for (ptr = envp; *ptr != 0; ptr++)
		printf("%s \n",*ptr);
 
	printf("\n");
 
	sleep(2);
 
	/* 子进程调用execl函数 */
	if (fork() == 0)
	{
		//child1
		printf("1-----execl-----\n");
		if (execl("/bin/ls","ls","-a",NULL) == -1)
		{
			perror("execl error!");
			exit(1);
		}
	}
 
	sleep(2);
 
	/* 子进程调用execv函数 */
	if (fork() == 0)
	{
		//child2
		printf("2-----execv-----\n");
		if (execv("/bin/ls",arg) == -1)
		{
			perror("execv error!");
			exit(1);
		}
	}
 
	sleep(2);
 
	/* 子进程调用execlp函数 */
	if (fork() == 0)
	{
		//child3
		printf("3-----execlp-----\n");
		if (execlp("ls","ls","-a",NULL) == -1)
		{
			perror("execlp error!");
			exit(1);
		}
	}
 
	sleep(2);
 
	/* 子进程调用execvp函数 */
	if (fork() == 0)
	{
		//child4
		printf("4-----execvp-----\n");
		if (execvp("ls",arg) == -1)
		{
			perror("execvp error!");
			exit(1);
		}
	}
 
	sleep(2);
 
	/* 子进程调用execle函数 */
	if (fork() == 0)
	{
		//child5
		printf("5-----execle-----\n");
		if (execle("/usr/bin/env","env",NULL,envp) == -1)		//使用自定义的环境表,并打印出自定义环境变量
		{
			perror("execle error!");
			exit(1);
		}
	}
 
	sleep(2);
 
	/* 子进程调用execve函数 */
	if (fork() == 0)
	{
		//child6
		printf("6-----execve-----\n");
		if (execve("/usr/bin/env",arg1,envp) == -1)				//使用自定义的环境表，并打印出自定义环境变量
		{
			perror("execve error!");
			exit(1);
		}
	}
 
	sleep(2);
 
	printf("over!\n");
	return 0;
}