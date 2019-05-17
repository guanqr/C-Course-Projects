#include<stdio.h>
#include<math.h>
#define MAX 2000 
int binary(int num);//十进制转化成二进制的函数 
void devide(int a,int b,int c);//定义函数将三组数拆分成四组数 
int main()
{
	char base[64]={'A','B','C','D','E','F','G','H','I','J','K','L','M','N','O','P','Q',
	'R','S','T','U','V','W','X','Y','Z','a','b','c','d','e','f','g','h','i','j','k','l',
	'm','n','o','p','q','r','s','t','u','v','w','x','y','z','0','1','2','3','4','5','6',
	'7','8','9','+','/'	};//定义base64编码，数组下标对应相应的字符 
	char a[MAX];
	int b[MAX];
	int i,n=2,k=0;
    int m=0;
    a[0]='A';
    a[1]='A';//字符串开头添加AA字符 
	for(i=2;i<=MAX;i++){
		scanf("%c",&a[i]);
		n++;
		if(a[i]=='\n') break; 
	}
	n=n-1;
	a[i]='Z';
	n++;
	a[i+1]='Z';//字符串结尾添加ZZ字符 
	n++;
	printf("thunder://");//打印迅雷标识头 
	if(n%3==0){//考虑字符个数恰能分成三个一组的情况 
		for(i=0;i<n;i++){
    	    b[i]=binary(a[i]);//十进制转化成二进制 
	    }
	    for(i=0;i<n;i++,i++,i++){
		    devide(b[i],b[i+1],b[i+2]);//将三个一组的二进制数拆分成四组 
    	}
	}
	else if(n%3==1){//考虑将字符个数分成三个一组，剩余单独一个字符的情况 
		for(i=0;i<n;i++){
    	    b[i]=binary(a[i]);//十进制转化成二进制 
	    }
	    for(i=0;i<n-1;i++,i++,i++){
		    devide(b[i],b[i+1],b[i+2]);//将三个一组的二进制数拆分成四组
    	}
    	int x0,y0,x1,y1,x2=0,y2=0;
    	y0=(b[n-1]%100)*10000;
    	x0=b[n-1]/100;
    	for(i=0;i<8;i++){
    		y1=y0%10;
    		y2=y2+y1*pow(2,i);
    		y0=y0/10;
    		x1=x0%10;
    		x2=x2+x1*pow(2,i);
    		x0=x0/10;
    	}//将结尾剩余字符的二进制数转化成十进制 
    	printf("%c%c==",base[x2],base[y2]);//输出结尾部分，空余部分由等号补位 
	}
	else if(n%3==2){//考虑将字符个数分成三个一组，剩余两个字符的情况 
		for(i=0;i<n;i++){
    	    b[i]=binary(a[i]);//十进制转化成二进制 
	    }
	    for(i=0;i<n-2;i++,i++,i++){
		    devide(b[i],b[i+1],b[i+2]);//将三个一组的二进制数拆分成四组
    	}
    	int x0,y0,z0,x1,y1,z1,x2=0,y2=0,z2=0;
    	z0=(b[n-1]%10000)*100;
		y0=(b[n-2]%100)*10000+b[n-1]/10000;
    	x0=b[n-2]/100;
    	for(i=0;i<8;i++){
    		z1=z0%10;
    		z2=z2+z1*pow(2,i);
    		z0=z0/10;
    		y1=y0%10;
    		y2=y2+y1*pow(2,i);
    		y0=y0/10;
    		x1=x0%10;
    		x2=x2+x1*pow(2,i);
    		x0=x0/10;
    	}//将结尾剩余字符的二进制数转化成十进制 
    	printf("%c%c%c=",base[x2],base[y2],base[z2]);//输出结尾部分，空余部分由等号补位 
	}
 } 
 
int binary(int num)//定义函数，将十进制转化成二进制 
{
    int a[8]={0};
    int i=-1,sum=0;
    while(num!=0&&i<=8){
        i++;
        a[i]=num%2;
        num/=2;
    }
    for(i=7;i>=0;i--){
    	sum=sum*10+a[i];
    }
    return sum;
}
 
void devide(int a,int b,int c)//定义函数，将三组二进制数拆分为四组，并转化成十进制的数 
{
	char base[64]={'A','B','C','D','E','F','G','H','I','J','K','L','M','N','O','P','Q',
	'R','S','T','U','V','W','X','Y','Z','a','b','c','d','e','f','g','h','i','j','k','l',
	'm','n','o','p','q','r','s','t','u','v','w','x','y','z','0','1','2','3','4','5','6',
	'7','8','9','+','/'	};//定义base64编码，数组下标对应相应的字符
	int i,d,e,f,g;
	int d1=0,e1=0,f1=0,g1=0;
	int d0,e0,f0,g0;
	g=c%1000000;
	c=c/1000000;
	f=b%10000*100+c;
	b=b/10000;
	e=a%100*10000+b;
	a=a/100;
	d=a;//将二进制数拆分组合 
	for(i=0;i<8;i++){
		d0=d%10;
		e0=e%10;
		f0=f%10;
		g0=g%10;
		d1=d1+d0*pow(2,i);
		e1=e1+e0*pow(2,i);
		f1=f1+f0*pow(2,i);
		g1=g1+g0*pow(2,i);//二进制转化成十进制 
		d=d/10;
		e=e/10;
		f=f/10;
		g=g/10;
	}
    printf("%c%c%c%c",base[d1],base[e1],base[f1],base[g1]);
}

