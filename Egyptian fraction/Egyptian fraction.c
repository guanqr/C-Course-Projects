#include <stdio.h>
int main(){
	long long int a,b,c;/*输出的数值很可能会溢出*/
	long long int m,n;
	int k;
	scanf("%ld/%ld",&m,&n);/*输入要分解的分数*/ 
	a=m;
	b=n;
	k=0;
	printf("(1) %ld/%ld = ",a,b);/*第一种分解方法*/
	while((a%2==0)&&(b%2==0)){
		a=a/2;
		b=b/2;
	}/*首先进行约分*/
	while(1){/*执行循环语句*/
		if(b%a!=0){/*判断分子是否能化为1，若不能，则执行该条语句*/
			c=b/a+1;
			printf("1/%ld + ",c);
			k++;/*记录埃及分数的个数，为第二种分解方法做判断*/
		}
		else if(b%a==0){
			c=b/a;
			printf("1/%ld\n",c);
			k++;
		    break;/*终止循环*/
		}
		a=a*c-b;
		b=b*c;/*将分数减去分离的埃及分数，作为下一轮循环的分数*/
		if((a==3)&&(b%2==0)){/*为减少算法复杂度，若分子为3，分母为偶数，则可直接分离出两个埃及分数*/
		    printf("1/%ld + 1/%ld\n",b/2,b);
		    k=k+2;
			break;
		}
	}
		a=m;/*第二种分解方法*/
	    b=n;
	    int i,j,x=1;
	    printf("(2) %ld/%ld = ",a,b);
	    while((a%2==0)&&(b%2==0)){
		    a=a/2;
	    	b=b/2;
    	}
    	while(1){
		    if(b%a!=0){
		    	x++;/*记录非终止条件下输出的埃及分数的个数*/ 
		    	c=b/a+1;
		    	if(x==5){/*对第一种分解方法中第4个埃及分数进行再次分解*/
		    		i=(c+1);
		    		j=c*(c+1);
		    		printf("1/%ld + 1/%ld + ",i,j);
				}
				else{
					printf("1/%ld + ",c);
				}
	    	}
	    	else if(b%a==0){
			    c=b/a;
			    if(k<=4){/*当分数分解后的埃及分数小于等于4个，则进行下面的分解*/
			    	i=(c+1);
			        j=c*(c+1);
		    	    printf("1/%ld + 1/%ld\n",i,j);
		            break;
				}
				else{
					printf("1/%ld\n",c);
		            break;
				}
	    	}
	    	a=a*c-b;
	    	b=b*c;/*在此省去第一种分解方法中的部分算法，以便分解出不同的分数*/
    	}
} 

