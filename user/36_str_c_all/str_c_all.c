#include <stdio.h>
#include <string.h>
#include <math.h>

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
	
#elif 0
	
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

#elif 0
	//strcpy复制了一个"\0"
	char src[40];
	char dest[100];
  
	memset(dest, '\0', sizeof(dest));
	strcpy(src, "This is runoob.com");
	strcpy(dest, src);
 
	printf("最终的目标字符串： %s\n", dest);

#elif 0

	//strncpy复制了n个字符 但是没有添加'\0'
	char src[40];
	char dest[12];
  
	memset(dest, '\0', sizeof(dest));
	strcpy(src, "This is runoob.com");
	strncpy(dest, src, 10);
 
	printf("最终的目标字符串： %s\n", dest);

#elif 0

	//sprinf自动追加'\0' 
	char str[80];
	sprintf(str, "Pi 的值 = %f", M_PI);
	puts(str);
   
#elif 0 
	
	//snprintf 读取n个字符串追加'\0' 
    char buffer[50]; 
    char* s = "runoobcom"; 
    // 读取字符串并存储在 buffer 中
    int j = snprintf(buffer, 6, "%s\n", s); 
    // 输出 buffer及字符数
    printf("string:\n%s\ncharacter count = %d\n", buffer, j); 
 
#elif 0
	
	//memcpy没有自动加 '\0' 
	const char src[50] = "http://www.runoob.com";
	char dest[50];
 
	memcpy(dest, src, strlen(src)+1);
	printf("dest = %s\n", dest);

#elif 0
	
	//字符串合并
	char src[50], dest[50];
 
	strcpy(src,  "This is source");
	strcpy(dest, "This is destination");
 
	strcat(dest, src);
	printf("最终的目标字符串： |%s|\n", dest);

#elif 0
	//拼接指定个数的字符 自动追加'\0'
	char src[50], dest[50];

	strcpy(src,  "This is source");
	strcpy(dest, "This is destination");

	strncat(dest, src, 9);

	printf("最终的目标字符串： |%s| \n", dest);


#elif 0
	int val;
	char str[20];
   
	strcpy(str, "98993489");
	val = atoi(str);
	printf("字符串值 = %s, 整型值 = %d\n", str, val);

	strcpy(str, "runoob.com");
	val = atoi(str);
	printf("字符串值 = %s, 整型值 = %d\n", str, val);
	
	
#elif 0
	
	//strlen求字符串长度 见到'\0'结束！
	char str[50];
	int len;

	strcpy(str, "This \0 is runoob.com");

	len = strlen(str);
	printf("|%s| 的长度是 |%d|\n", str, len);
	
#elif 0
	//返回出现 待查找字符串第一次出现 的位置
	const char str[] = "http://www.runoob.com";
	const char ch = '.';
	char *ret;

	ret = strchr(str, ch);

	printf("|%c| 之后的字符串是 - |%s|\n", ch, ret);
   
   
#elif 1
	
	//该函数返回 str1 中第一个匹配字符串 str2 中字符的字符数
	const char str1[] = "abcde2fghi3jk4l";
	const char str2[] = "234";
	char *ret;
 
	ret = strpbrk(str1, str2);
	if(ret) 
	{
		printf("第一个匹配的字符是： %c\n", *ret);
	}
	else 
	{
		printf("未找到字符");
	}
   
#endif


	
	return 0;
}