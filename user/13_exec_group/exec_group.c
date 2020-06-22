#include<stdio.h>
#include<unistd.h>
int main()
{
        char *filename[]={"./bp",NULL};//BP是c文件编译链接后产生的可执行文件，目的是打印一条语句
        char *envp[]={0,NULL};//如果依赖于新环境变量，这里才需要改
        execve("./bp",filename,envp);//字符串是文件的路径需要具体到该运行文件名，直到文件夹貌似识别不了，或者命令的路径
        /*注意，把下面的三个注释遇到去掉，后面的ls命令也无法执行，因为*execve执行后，相当于结束了当前的程序，切换到execve里面的那个程序*去了*/
//      char *filename1[]={"ls","-al","/root",NULL};//命令和参数，记得要加NULL
//      char *envp1[]={0,NULL};
//      execve("/bin/ls",filename1,envp1);
		while(1)
		{
			;
		}
	
        return 0;
}
