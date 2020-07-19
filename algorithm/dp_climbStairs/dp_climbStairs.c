#include <stdio.h>
#include <string.h>
#include <malloc.h>

int climbStairs(int n){
    
    if(n == 1) return 1;
    if(n == 2) return 2;

    int i;
    int array[ n+1 ];
    array[1] = 1;
    array[2] = 2;
	//可以使用 记录子事件发生的情况 纯递归 重复计算 浪费事件
    for(i=3 ;i <=n ;i++ )
    {
        array[i] = array[i-1] + array[i-2];
    }
        
    return array[n];
}
int main()
{
	climbStairs(10);
	return 0;
}
