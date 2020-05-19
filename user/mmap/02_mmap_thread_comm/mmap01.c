#include <sys/mman.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
 
#define BUF_SIZE 100
 
int main(int argc, char** argv)
{
    char    *p_map;
 
    /* 匿名映射,创建一块内存供父子进程通信 */
	/*
	参数fd：要映射到内存中的文件描述符。如果使用匿名内存映射时，
	即flags中设置了MAP_ANONYMOUS，fd设为-1。有些系统不支持匿名内存映射，
	则可以使用fopen打开/dev/zero文件，然后对该文件进行映射，可以同样达到匿名内存映射的效果
	*/
    p_map = (char *)mmap(NULL, BUF_SIZE, PROT_READ | PROT_WRITE,MAP_SHARED | MAP_ANONYMOUS, -1, 0);
    printf("add_mmap:%p \n",p_map);
    if(fork() == 0) //孩子进程
	{
		while(1)
		{
			sleep(5);
			printf("add_mmap:%p \n",p_map);
			if(p_map[0] == 1)
			{
				printf("child\n");
				p_map[0] = 2;
			}

		}
         //实际上，进程终止时，会自动解除映射。
        exit(0);
    }

	while(1)//父进程
	{
		sleep(2);
		p_map[0] = 1;
		printf("p: %d\n",p_map[0]);
	}
    munmap(p_map, BUF_SIZE);
 
    return 0;
}
