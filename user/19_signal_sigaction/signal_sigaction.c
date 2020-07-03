#include <stdio.h>
#include <signal.h>
#include <time.h>
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
printf("pid = %d \n",getpid());
printf("ok1 [%p]\n",handle);
printf("ok1 [%p]\n",handler);

sleep(1);

	signal(2 , handle);
	sleep(1);
	
	signal(3 , handle);
	sleep(1);
	
	signal(4 , handler);
	sleep(1);
	
	signal(6 , handle);
	sleep(1);
	
	signal(9 , handle);
	sleep(1);


	signal(32 , handle);
	sleep(1);	
	
	signal(33 , handler);
	sleep(1);
	
	signal(40, handle);
	sleep(1);
	
printf("ok [%p]\n",handle);
//int sigaction(int signum, const struct sigaction *act,struct sigaction *oldact);
	
	/*
	struct sigaction act ,oact;
	act.sa_handler = handler ;
	sigemptyset(&act.sa_mask);
	act.sa_flags = 0;
	
	sigaction(2,&act ,&oact);//给2信号分配一个新的处理函数
	*/
	
	while(1);
	return 0;
}
