#include <stdio.h>  
#include <unistd.h>  
#include <sys/mman.h>  
#include <sys/types.h>  
#include <fcntl.h>  
#include <stdlib.h>  
#define LEN (10*4096)  
int main(void)  
{  
    int fd,loop;  
    char *vadr;  
  
    if ((fd = open("/dev/Dragon", O_RDWR)) < 0) {  
        return 0;  
    }  
    vadr = mmap(0, LEN, PROT_READ, MAP_PRIVATE | MAP_LOCKED, fd, 0);    
	printf("%p \n",vadr);
	
    for(loop=0;loop<2;loop++){
        printf("[%-10s----%lx]\n",vadr+4096*loop,vadr+4096*loop);
    }
	
    while(1)
    {
        sleep(1);
    }
     
}  