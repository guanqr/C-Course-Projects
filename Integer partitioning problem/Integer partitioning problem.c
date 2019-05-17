#include <stdio.h>
int n;
void integerdivision(int x,int k,int p,int *m)  
 {
    
    int i;
    if(x>n) return;  
    if(x==n)
    {
        printf("%d = ",n);
		for(i=0;i<k-1;i++)
            printf("%d+",m[i]);
        printf("%d\n",m[i]);
    }
    else
    {
        for(i=p;i>0;i--)
        {
            if(i<=p)  
            {
                m[k]=i;
                x=x+i;
                integerdivision(x,k+1,i,m);
                x=x-i;
            }
        }
    }
 }

int main()
{
	int i;
    scanf("%d",&n);/*输入n*/ 
    int m[n];
    for(i=0;i<n;i++) m[i]=0;
    integerdivision(0,0,n-1,m);/*调用函数求划分形式并打印*/
    return 0;
}
