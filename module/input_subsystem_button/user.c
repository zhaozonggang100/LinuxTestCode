//https://www.cnblogs.com/zengjfgit/p/5222398.html
#include <stdio.h>
#include <linux/input.h>
#include <fcntl.h>
#include <sys/time.h>
#include <unistd.h>
#include <string.h>


//按键模拟，按键包含按下和松开两个环节
void simulate_key(int fd, int kval)
{
    struct input_event event;
    gettimeofday(&event.time, 0);//获取时间精确到微妙
    
    //按下kval键
    event.type = EV_KEY;
    event.value = 1;
    event.code = kval;
    write(fd, &event, sizeof(event));
        
    //同步，也就是把它报告给系统
    event.type = EV_SYN;
    event.value = 0;
    event.code = SYN_REPORT;
    write(fd, &event, sizeof(event));
    
    memset(&event, 0, sizeof(event));
    gettimeofday(&event.time, 0);
        
    //松开kval键
    event.type = EV_KEY;
    event.value = 0;
    event.code = kval;
    write(fd, &event, sizeof(event));
        
    //同步，也就是把它报告给系统
    event.type = EV_SYN;
    event.value = 0;
    event.code = SYN_REPORT;
    write(fd, &event, sizeof(event));
    }




int main(int argc, char **argv)
{
    int fd_mouse = -1;
    int fd_kbd = -1;
    int i = 0;
	fd_kbd = open("/dev/input/event6", O_RDWR);
	if(fd_kbd <= 0)
	{
		printf("Can not open keyboard input file\n");
		return -1;
	}
	for (i = 0; i < 50; i++) {
		printf("模拟按下键盘A键 \n");
		sleep(4);
        simulate_key(fd_kbd, 700);    //模拟按下键盘A键
		
	}
	close(fd_kbd);
}