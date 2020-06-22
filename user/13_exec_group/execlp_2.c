#include <stdio.h>
#include <unistd.h>
int main()
{
	if(fork()==0)
	{
		if(execlp("/usr/bin/env","env",NULL)<0)
		{
			perror("execlp error!");
			return -1 ;
		}
	}
return 0 ;
}
/*
由执行结果看出，execlp函数使执行码重生时继承了Shell进程的所有环境变量，其他三个不以e结尾的函数同理。
*/