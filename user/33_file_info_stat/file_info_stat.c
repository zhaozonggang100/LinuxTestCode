#include <sys/stat.h> 
#include <unistd.h> 
#include <stdio.h> 
/*
需要传入一个文件描述符
int fstat(int filedes, struct stat *buf);
处理实体文件
int stat(const char *path, struct stat *buf);
如果是软连接 直接返回软连接的信息，不会处理连接的文件
int lstat(const char *path, struct stat *buf);
*/
int main() { 
    struct stat buf; 
    stat("/etc/hosts", &buf); 
    printf("/etc/hosts file size = %d\n", buf.st_size); 
} 
