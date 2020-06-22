#include <unistd.h>
#include <stdio.h>
int main()
{
	/*命令参数列表，必须以 NULL 结尾*/
	char *envp[]={"PATH=/tmp","USER=sun",NULL};
	if(fork()==0)
	{
		/*调用 execle 函数，注意这里也要指出 env 的完整路径*/
		if(execle("/usr/bin/env","env",NULL,envp)<0)
		{
			perror("execle error!");
			return -1 ;
		}
	}
	return 0 ;
}
/*
可见，使用execle和execve可以自己向执行进程传递环境变量，但不会继承Shell进程的环境变量，
而其他四个exec函数则继承Shell进程的所有环境变量。
*/