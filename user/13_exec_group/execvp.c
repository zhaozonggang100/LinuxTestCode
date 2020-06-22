#include <stdio.h>
#include <unistd.h>
 /*
l (list)                           命令行参数列表
p (path)                       搜素file时使用path变量
v (vector)                    使用命令行参数数组
e (environment)       使用环境变量数组,不使用进程原有的环境变量，设置新加载程序运行的环境变量

*/
int main()
{
	printf("process begin...\n");
	char *argv[] = {"ls", "-a", "-l", NULL};
	execvp("ls", argv);
	printf("process end...\n");
	return 0;
}
/*
带v的exec函数

这类函数有：execv，execvp

具体说明：表示命令所需的参数以char *arg[]形式给出且arg最后一个元素必须是NULL
*/