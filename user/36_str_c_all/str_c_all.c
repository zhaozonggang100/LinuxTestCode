#include <stdio.h>
#include <string.h>

int main()
{
	
#if 0

    char s[] = "192.168.0.26";  
    char *delim = ".";  
    char *p;  
    printf("%s ", strtok(s, delim));  
 
    while((p = strtok(NULL, delim)))    
        printf("%s ", p); 
	printf("\n");
	
#elif 1
	
	char str1[15];
	char str2[15];
	int ret;
 
 
	strcpy(str1, "abcdef");
	strcpy(str2, "ABCDEF");
 
	ret = strcmp(str1, str2);
 
	if(ret < 0)
	{
		printf("str1 小于 str2");
	}
	else if(ret > 0) 
	{
		printf("str1 大于 str2");
	}
	else 
	{
		printf("str1 等于 str2");
	}
	
#endif
	
	return 0;
}