//execle程序
 /*
l (list)                           命令行参数列表
p (path)                       搜素file时使用path变量
v (vector)                    使用命令行参数数组
e (environment)       使用环境变量数组,不使用进程原有的环境变量，设置新加载程序运行的环境变量

*/
#include <stdio.h>
#include <unistd.h>
 
int main()
{
	printf("process begin...\n");
	char *envp[] = {"AA=11", "BB=22", NULL};
	execle("./env", "env", NULL, envp);
	printf("process end...\n");
	return 0;
}
/*
//execle程序
 
#include <stdio.h>
#include <unistd.h>
 
int main()
{
	printf("process begin...\n");
	char *envp[] = {"AA=11", "BB=22", NULL};
	execle("./env", "env", NULL, envp);
	printf("process end...\n");
	return 0;
}
*/