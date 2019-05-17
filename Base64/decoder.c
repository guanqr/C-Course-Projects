#include<stdio.h>
#include<math.h>
#define MAX 2000
int binary(int num);//十进制转化成二进制的函数 
void devide(int a,int b,int c,int d);//字符中间部分的解码函数
void devidehead(int a,int b,int c,int d);//开头部分的解码函数
void devidetail(int a,int b,int c,int d);
void devidetail2(int a,int b,int c,int d);//两种不同结尾方式的解码函数 
int main()
{
	char base[64]={'A','B','C','D','E','F','G','H','I','J','K','L','M','N','O','P','Q',
	'R','S','T','U','V','W','X','Y','Z','a','b','c','d','e','f','g','h','i','j','k','l',
	'm','n','o','p','q','r','s','t','u','v','w','x','y','z','0','1','2','3','4','5','6',
	'7','8','9','+','/'	};//定义base64编码，数组下标对应相应的字符 
	char a[MAX];
	int i,j,n=0;
	int num[MAX],b[MAX];
	for(i=0;i<MAX;i++){
		scanf("%c",&a[i]);
		n++;
		if(a[i]=='\n') break;
	}
	n=n-1;
	n=n-10;
	for(i=0;i<n;i++){
		a[i]=a[i+10];//将迅雷标识头去掉
    }
    if(a[n-1]=='='){
    	n=n-4;
	}
	
    for(i=0;i<n;i++){
		for(j=0;j<64;j++){
			if(a[i]==base[j]){//将字符转为对应的数字,并转为二进制
				b[i]=binary(j);
			}
		}
	}
	int x,y,z,z0,z1=0;
	if(a[n+3]=='='){
		if(a[n+2]=='='){//考虑以两个等号结尾的特殊情况 
	        devidehead(b[0],b[1],b[2],b[3]);//去除开头多余字符 
	        for(i=4;i<n-4;i++,i++,i++,i++){
		        devide(b[i],b[i+1],b[i+2],b[i+3]);
        	}
        	devidetail2(b[n-4],b[n-3],b[n-2],b[n-1]);
		}
		else{
			devidehead(b[0],b[1],b[2],b[3]);//去除开头多余字符 
	        for(i=4;i<n;i++,i++,i++,i++){
		        devide(b[i],b[i+1],b[i+2],b[i+3]);
        	}
		}
	}
	else{
		devidehead(b[0],b[1],b[2],b[3]);//去除开头多余字符 
	    for(i=4;i<n-4;i++,i++,i++,i++){
		    devide(b[i],b[i+1],b[i+2],b[i+3]);
	    }
	    devidetail(b[n-4],b[n-3],b[n-2],b[n-1]);
	}	
}

int binary(int num)//定义函数，将十进制转化为二进制的函数
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
void devide(int a,int b,int c,int d)//定义函数，将要解码的中间部分拆分，四组二进制拆分成三组 
{
	char x,y,z;
	int i,e,f,g,e0,f0,g0,e1=0,g1=0,f1=0;
	g=c%100*1000000+d;
	f=b%10000*10000+c/100;
	e=a*100+b/10000;
	for(i=0;i<8;i++){
		e0=e%10;
		f0=f%10;
		g0=g%10;
		e1=e1+e0*pow(2,i);
		f1=f1+f0*pow(2,i);
		g1=g1+g0*pow(2,i);//二进制转化成十进制
		e=e/10;
		f=f/10;
		g=g/10;
	}
	printf("%c%c%c",e1,f1,g1);
}
void devidehead(int a,int b,int c,int d)//将要解码的开头部分拆分，删除额外字符，四组二进制拆分成三组 
{
	int i,g,g0,g1=0;
	g=c%100*1000000+d;
	for(i=0;i<8;i++){
		g0=g%10;
		g1=g1+g0*pow(2,i);//二进制转化成十进制 
		g=g/10;
	}
	printf("%c",g1);
}
void devidetail(int a,int b,int c,int d)//普通结尾方式，将要解码的结尾部分拆分，删除额外字符，四组二进制拆分成三组
{
	int i,e,e0,e1=0;
	e=a*100+b/10000;
	for(i=0;i<8;i++){
		e0=e%10;
		e1=e1+e0*pow(2,i);//二进制转化成十进制 
		e=e/10;
	}
	printf("%c",e1);
}
void devidetail2(int a,int b,int c,int d)//特殊结尾方式，带有两个个等号，将要解码的结尾部分拆分，删除额外字符，四组二进制拆分成三组
{
	int i,e,f,e0,f0,e1=0,f1=0;
	f=b%10000*10000+c/100;
	e=a*100+b/10000;
	for(i=0;i<8;i++){
		e0=e%10;
		f0=f%10;
		e1=e1+e0*pow(2,i);
		f1=f1+f0*pow(2,i);//二进制转化成十进制 
		e=e/10;
		f=f/10;
	}
	printf("%c%c",e1,f1);
}
