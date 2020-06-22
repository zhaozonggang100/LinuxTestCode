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
	execl("/bin/ls", "ls", NULL);
	printf("process end...\n");
	return 0;
}
/*
带l的exec函数

这类函数有：execl，execlp，execle

具体说明：表示后边的参数以可变参数的形式给出且都以一个空指针结束。
这里特别要说明的是，程序名也是参数，所以第一个参数就是程序名。
*/