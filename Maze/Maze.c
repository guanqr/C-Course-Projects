#include<stdio.h>
#include <windows.h>
int map[10][10]={
	{1,1,1,1,1,1,1,1,1,1},
	{1,0,0,1,0,0,0,1,0,1},
	{1,0,0,1,0,0,0,1,0,1},
	{1,0,0,0,0,1,1,0,0,1},
	{1,0,1,1,1,0,0,0,0,1},
	{1,0,0,0,1,0,0,0,0,1},
	{1,0,1,0,0,0,1,0,0,1},
	{1,0,1,1,1,0,1,1,0,1},
	{1,1,0,0,0,0,0,0,0,1},
	{1,1,1,1,1,1,1,1,1,1}
};	/*表示迷宫，其中1表示围墙，0表示可以走的路*/ 
int location[10][10]={};	/*表示是否走过某点,2表示经过，0表示未经过*/ 
int search(int starti,int startj);	/*寻路过程*/ 

int main()
{

	int starti=1,startj=1;	/*表示横向与纵向初始位置*/
	int i,j;
	
	SetConsoleOutputCP(437);	/*为了显示拓展的ascii字符，将出口的代码页更改显示为英文*/ 
	
	/*判断出口是否存在和打印路径*/ 
	if(search(starti,startj)==0)
		printf("Path doesn't exist!");
	else{
		for(i=0;i<=9;i++){
			for(j=0;j<=9;j++){
				if(map[i][j]==1)
					printf("%c",178);
				else if(location[i][j]==0)
					printf("%c",255);
				else
					printf("%c",254);
			}
			printf("\n");
		}
	}
	
} 


int search(int starti,int startj)
{
	int endi=8,endj=8;	/*表示终点位置*/  
	int goal=0;	/*表示是否到达终点*/
	
	if(starti==endi&&startj==endj)
		goal=1;	/*表示到达终点*/ 
		
	location[starti][startj]=2;	/*标记经过的位置*/ 
	
	
	if(goal==0&&map[starti][startj+1]==0&&location[starti][startj+1]==0)
		if(search(starti,startj+1)==1)
			return 1;
	if(goal==0&&map[starti+1][startj]==0&&location[starti+1][startj]==0)
		if(search(starti+1,startj)==1)
			return 1;
	if(goal==0&&map[starti][startj-1]==0&&location[starti][startj-1]==0)
		if(search(starti,startj-1)==1)
			return 1;
	if(goal==0&&map[starti-1][startj]==0&&location[starti-1][startj]==0) 
		if(search(starti-1,startj)==1)
			return 1;
	/*按照右下左上的顺序探路*/ 
				
	if(goal==0)
		location[starti][startj]=0;	/*当四面都走不通时，退回分支位置*/ 
		
	return goal;
	
}







