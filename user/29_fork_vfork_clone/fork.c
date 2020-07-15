#include <stdio.h>
#include <unistd.h>
#include <time.h>

char *data = {"10"};

void child()
{
     printf("son: %s \n", data); 
     printf("son: %p \n", data); 
     //data = "20";
     printf("son: %s \n", data); 
     printf("son: %p \n", data);
	sleep(50);
}

int main(void)
{
	
     if (0 == fork()) {
		sleep(3); 
        child();
     } else {
		 while(1);
          printf("father: %s \n", data); 
          printf("father: %p \n", data); 
          data = "30";
          printf("father: %s \n", data);  
          printf("father: %p \n", data); 
     }
     return 0;
}