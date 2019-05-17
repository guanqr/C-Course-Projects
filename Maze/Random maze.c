#include<stdio.h>
#include<windows.h>
#include<time.h>	 
int location[60][60];	/*表示是否走过某点,2表示经过，0表示未经过*/ 
int map[60][60];     /*表示迷宫*/
int search(int starti,int startj,int n);   /*寻路过程*/ 
void maze_generator(int n);    /*随机生成迷宫*/
int steps=1;    /*记录步数*/
void printfsteps(int n);   /*打印路径*/ 
int main(void)
{
	srand((unsigned int)time(NULL));
	int n,c; 
	int i,j;
	int count=1;
	printf("请输入迷宫半径（最大60）：\n");
	scanf("%d",&n);
	maze_generator(n);
	SetConsoleOutputCP(437);
	for(i=0;i<n;i++){
		for(j=0;j<n;j++){
			if(map[i][j]==1)
				printf("%c",178);
			else
				printf("%c",255);
		}
		printf("\n");
		
	}
	printf("*****************\n");
	if(search(1,1,n)==0)
		printf("Path doesn't exist!");
	else{
		for(i=0;i<=n;i++){
			for(j=0;j<=n;j++){
				if(map[i][j]==1)
					printf("%c",178);
				else if(location[i][j]==0)
					printf("%c",255);
				else
					printf("%c",254);
			}
			printf("\n");
		} 
		printf("(1,1)") ;
	}
	printfsteps(n);
	return 0;
}
void maze_generator(int n)
{
	srand((unsigned int)time(NULL));
	int i,j;
	for(i=0;i<n;i++)
		for(j=0;j<n;j++){
			if(i==0||j==0||i==n-1||j==n-1)
				map[i][j]=1;
			else if((i==1&&j==1)||(i==n-2&&j==n-2))
				map[i][j]=0;
			else{
				if(rand()%4==0)
					map[i][j]=1;
				else map[i][j]=0;
			}
		}
}	
int search(int starti,int startj,int n)
{
	int endi=n-2,endj=n-2;	/*表示终点位置*/  
	int goal=0;	/*表示是否到达终点*/
	
	if(starti==endi&&startj==endj)
		goal=1;	/*表示到达终点*/ 
		
	location[starti][startj]=steps+1;	/*标记经过的位置*/ 
	steps++;
	
	if(goal==0&&map[starti][startj+1]==0&&location[starti][startj+1]==0)
		if(search(starti,startj+1,n)==1)
			return 1;
	if(goal==0&&map[starti+1][startj]==0&&location[starti+1][startj]==0)
		if(search(starti+1,startj,n)==1)
			return 1;
	if(goal==0&&map[starti][startj-1]==0&&location[starti][startj-1]==0)
		if(search(starti,startj-1,n)==1)
			return 1;
	if(goal==0&&map[starti-1][startj]==0&&location[starti-1][startj]==0) 
		if(search(starti-1,startj,n)==1)
			return 1;
	/*按照右下左上的顺序探路*/ 
				
	if(goal==0)
		location[starti][startj]=0;	/*当四面都走不通时，退回分支位置*/ 
		
	return goal;
	
}
void printfsteps(int n){
	int i,j,c=1,count=1;
	while(c<=steps){
		for(i=0;i<n;i++)
			for(j=0;j<n;j++){
				if(location[i][j]==c){
					printf("==>(%d,%d)",i,j);
					count++;
					if(count==6){
						printf("\n");
						count=0;
					}
				}
					
			}
		c++;
	}
	
}
