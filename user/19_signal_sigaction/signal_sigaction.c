#include <stdio.h>
#include <signal.h>

void handle(int signo)
{
	printf("zhaozongagng_signal signal [%d] \n",signo);
}

void handler(int signo)
{
	printf("zhaozongagng_sigaction signal [%d] \n",signo);
}


int main()
{

//sighandler_t signal(int signum, sighandler_t handler);
	signal(2 , handle);
	
	

//int sigaction(int signum, const struct sigaction *act,struct sigaction *oldact);
	struct sigaction act ,oact;
	act.sa_handler = handler ;
	sigemptyset(&act.sa_mask);
	act.sa_flags = 0;
	
	sigaction(2,&act ,&oact);//给2信号分配一个新的处理函数
	
	while(1);
	return 0;
}
