#include<iostream>
using namespace std;
void DFS(int step);
int n;
int a[1000] ;//把全排列结果存到这个数组里 
int book[1000];        // 标记 用来记录第I个数是否已经被使用  如果被使用 book[i]=1 否则 为0 
 

 
//step 是第几次搜索结果  N是搜索边界 
void DFS(int step){
	if(step==n+1){ //循环变量从1开始 所以边界是第N+1次搜索	
		for(int i=1;i<=n;i++){
			cout<<a[i];
		}
		cout<<endl;//结果换行 
		return ;
	}else{
		//查找可以使用的元素 
		for(int i=1;i<=n;i++){ //从0开始遍历 
			
			if(book[i]==0){//判断元素是否被使用过 
				a[step]=i;  //元素没被使用过 存放数组中 
				book[i]=1;  //被标记已经被使用 
				DFS(step+1); //向前搜索一次
				book[i]=0; //搜索完成撤回之前的标记 
			}
		} 
		return ; //在遍历完成后再返回到 Book[i]=0 从而实现撤销操作并且回到上一步 
	}
}

int main(void){
	cin>>n; //N个数的全排列

	DFS(1); 
	system("pause");
	return 0;
} 
