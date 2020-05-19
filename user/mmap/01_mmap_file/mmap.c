/*
通过MMAP函数修改文件
*/
#include <sys/mman.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <error.h>
 
#define BUF_SIZE 100
 
int main(int argc, char **argv)
{
    int fd, nread, i;
    struct stat sb;//struct stat这个结构体是用来描述一个linux系统文件系统中的文件属性的结构
    char *mapped, buf[BUF_SIZE];
 
    for (i = 0; i < BUF_SIZE; i++) {
        buf[i] = '#';
    }
 
    /* 打开文件 */
    if ((fd = open(argv[1], O_RDWR)) < 0) {
        perror("open");
    }
 
    /* 获取文件的属性 */
	/*
	struct stat {

        mode_t     st_mode;       //文件对应的模式，文件，目录等
        ino_t      st_ino;       //inode节点号
        dev_t      st_dev;        //设备号码
        dev_t      st_rdev;       //特殊设备号码
        nlink_t    st_nlink;      //文件的连接数
        uid_t      st_uid;        //文件所有者
        gid_t      st_gid;        //文件所有者对应的组
        off_t      st_size;       //普通文件，对应的文件字节数
        time_t     st_atime;      //文件最后被访问的时间
        time_t     st_mtime;      //文件内容最后被修改的时间
        time_t     st_ctime;      //文件状态改变时间
        blksize_t st_blksize;    //文件内容对应的块大小
        blkcnt_t   st_blocks;     //伟建内容对应的块数量
		
      };
	*/
    if ((fstat(fd, &sb)) == -1) { ////通过文件描述符获取文件对应的属性。fdp为文件描述符
        perror("fstat");
    }
 
    /* 将文件映射至进程的地址空间 */
	/*
	    NULL                     进程地址的启始地址，NULL由系统自动分配
		sb.st_size               映射到进程空间的大小
		PROT_READ | PROT_WRITE   内存映射区的读写属性（读写权限）读权限必须有
		                         open权限大于mmap指定权限
		参数prot：映射区域的保护方式。可以为以下几种方式的组合：
		PROT_EXEC 映射区域可被执行
		PROT_READ 映射区域可被读取
		PROT_WRITE 映射区域可被写入
		PROT_NONE 映射区域不能存取
		
		参数flags：影响映射区域的各种特性。在调用mmap()时必须要指定MAP_SHARED 或MAP_PRIVATE。
		MAP_FIXED 如果参数start所指的地址无法成功建立映射时，则放弃映射，不对地址做修正。通常不鼓励用此旗标。
		MAP_SHARED对映射区域的写入数据会复制回文件内，而且允许其他映射该文件的进程共享。
		MAP_PRIVATE 对映射区域的写入操作会产生一个映射文件的复制，即私人的“写入时复制”（copy on write）对此区域作的任何修改都不会写回原来的文件内容。
		MAP_ANONYMOUS建立匿名映射。此时会忽略参数fd，不涉及文件，而且映射区域无法和其他进程共享。
		MAP_DENYWRITE只允许对映射区域的写入操作，其他对文件直接写入的操作将会被拒绝。
		MAP_LOCKED 将映射区域锁定住，这表示该区域不会被置换（swap）
		
		
		
		
		
		MAP_SHARED               设置内存映射的属性（私有映射-MAP_PRIVATE 与共享映射-MAP_SHARED）
				1）MAP_PRIVATE 指明对映射区数据的修改不会影响 真正的文件。(数据不会同步到磁盘)
				2）MAP_SHARED 指明对映射区数据的修改，多个共享该映射区的进程都可以看见，(数据会同步到磁盘)
		fd                       打开文件的句柄
		0                        打开文件的偏移量（必须指定4K整倍数4096整数倍）
	*/
	printf("sb.st_size:%ld\n",sb.st_size);
    if ((mapped = (char *)mmap(NULL, sb.st_size, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0)) == (void *)-1) {
        perror("mmap");
    }
 
    /* 映射完后, 关闭文件也可以操纵内存 */
    close(fd);
	printf("%s:\n",argv[1]);
    printf("%s\n", mapped);
	printf("address:%p\n", mapped);
    /* 修改一个字符,同步到磁盘文件 */
	/*
	int msync(void *addr, size_t length, int flags)
	length 映射区的长度（ps:无论填写什么都会刷新文本值）
	flags 取值为MS_ASYNC（异步）时，调用会立即返回，不等到更新的完成；
    flags 取值为MS_SYNC（同步）时，调用会等到更新完成之后返回；
	*/
    mapped[31] = '1';
	mapped[32] = '2';
	mapped[33] = '3';
    if ((msync((void *)mapped, sb.st_size, MS_SYNC)) == -1) {
        perror("msync");
    }
    printf("###########################\n");
	printf("%s\n", mapped);
    /* 释放存储映射区 */
    if ((munmap((void *)mapped, sb.st_size)) == -1) {
        perror("munmap");
    }
 
    return 0;
}
