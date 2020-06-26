#include <unistd.h>
#include <sys/syscall.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>


static int mysyscall(const char *msg)
{
    size_t len = 0;
    int ret = 0;
	printf("dragon:%s\n",msg);
    if (!msg) {
        errno = EINVAL;
        return -1;
    }
    
    len = strlen(msg);
	printf("dragon-len:%d\n",len);
    if (!len) {
        errno = EINVAL;
        return -1;
    }

    ret = syscall(223, msg, len);
	printf("dragon-ret:%d\n",ret);
    if (ret != 0) {
        errno = - ret;
        return -1;
    }
    return 0; 
}

int main()
{
    if (mysyscall("Test System call by userspace"))
        perror("dragon syscall error");
    
    return 0;
}
