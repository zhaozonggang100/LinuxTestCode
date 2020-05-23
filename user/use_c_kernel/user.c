#include <stdio.h>
#include <time.h>
#include <sys/types.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <malloc.h>
#include <sys/ioctl.h>
#define TDD_TRON (('M'<<0)|0x01)
int main()
{
	int fd ;
	int write_size;
	char *buf_w = "nihao";
	char *buf_r = (char *)malloc(10);
	int buf_size = strlen(buf_w);
	fd = open("/dev/Dragon",O_RDWR);
	if(fd < 0 ){
		printf( "open fild !\n");
	}

	read(fd,buf_r,buf_size);//
	printf("buf_r %s \n",buf_r);
	
	
	write(fd,buf_w,buf_size);
	printf("wriite_size= %d  buf_size= %d \n",write_size,buf_size);
	
	ioctl(fd,TDD_TRON);
	
	close(fd);
	return 0;
}
