#include <stdio.h>
#include <time.h>
#include <sys/types.h>
#include <fcntl.h>
#include <unistd.h>

int main()
{
	int fd ;
	fd = open("/dev/Drgaon01",O_RDONLY);
	if(fd < 0 ){
		printf( "open fild !\n");
	}
	close(fd);
	return 0;
}