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
	execlp("ls", "ls", "-l", NULL);
	printf("process end...\n");
	return 0;
}
/*
带p的exec函数

这类函数有：execlp，execvp

具体说明：表示第一个参数无需给出具体的路径，只需给出函数名即可，
系统会在PATH环境变量中寻找所对应的程序，如果没找到的话返回－1。
*/