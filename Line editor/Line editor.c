#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<windows.h>
#define M 100	/*每行最多输入M个字符*/ 
#define N 10	/*每页最多显示N行*/ 
int cx = 4;/*光标横坐标*/
int cy = 0;/*光标纵坐标*/ 
int line=1;/*当前已创建的有效输入行数*/
int lineloc=1;/*打印的行数*/ 
int page=1;/*当前已创建的页数*/
int pageloc=1; 		/*页面停留的页数*/
typedef struct list
{
	char textline[M];
	struct list *next;
} list;

void SetColor(int color) /* 设置文本输出颜色 */ 
{
	HANDLE hOutput = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleTextAttribute(hOutput, color);
}			

void gotopos(int x, int y)	/* 设置光标位置 */ 
{
	COORD loc = {x, y};
	HANDLE hOutput = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleCursorPosition(hOutput,loc);
}

void erase(int x, int y, int w)/* 擦除文本 */ 
{
	int k;
	gotopos(x, y);
	for( k=0; k<w; k++ ) printf(" ");
}

list *textsto(char *filename,list *head) /*将文本text.txt读入链表*/ 
{
	list *p,*tail;
	FILE *fp;
	int i;
	
	p=(list*)malloc(sizeof(list));
	p->next=NULL;
	tail=head;
	
	if((fp=fopen(filename,"r+"))==NULL){
		gotopos(0,17);
		printf("Open failure!");
		gotopos(0,0);
		return head;
	}
	
	while(!feof(fp)){
		fgets(p->textline,M,fp);  /*按行读入链表*/ 
		if(head==NULL)
			head=p;
		else
			tail->next=p;
		tail=p;
		p=(list*)malloc(sizeof(list));
		p->next=NULL;
		if(ferror(fp)){
			gotopos(0,17);
			printf("Read failure!");
			gotopos(0,0);
			return head;
		}
	}
	
	fclose(fp);
	strcat(tail->textline,"\n");
	return head;
}

void textprint(list *head) /*打印文本*/ 
{
	list *p;
	
	system("CLS");
	gotopos(0,0);
	lineloc=1;
	
	gotopos(0,19);
	SetColor(4| FOREGROUND_INTENSITY );
	printf("-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-");
	gotopos(0,20);
	SetColor(6| FOREGROUND_INTENSITY );
	printf("help:\n");
	printf("up:↑ left:←  down:↓ right:→\n");
	printf("last page:a    next page:s\n");
	printf("delete the pointed line:dd  or <x>dd to delete x lines  \n");
	printf("line move forward:l or <x>l to move x lines \n");
	printf("line move afterward:L or <x>L to move x lines  \n");
	printf("add chars before:i   after:f  correct chars:c  \n");
	printf("insert a line before:O  after:o  \n");
	printf("enter cmd: :  \n-->1.exchange two lines:m <line1> <line2>  2.open a file:o <filename>\n");
	printf("3.save to a file:w <filename>\n");
	SetColor(15| FOREGROUND_INTENSITY );
	gotopos(0,0);
	
	if(head==NULL)
		return;
	for(p=head;p!=NULL;p=p->next){
		SetColor(11| FOREGROUND_INTENSITY );
		printf("%d | ",lineloc++);
		SetColor(15);
		printf("%s",p->textline);
		if(lineloc==10){
			p=p->next;
			SetColor(11| FOREGROUND_INTENSITY );
			printf("%d| ",lineloc++);
			SetColor(15);
			printf("%s",p->textline);
			while(p->next!=NULL){   /*确认总行数*/
				lineloc++;
				if(p->next!=NULL)
					p=p->next;
				else
					;
			}
			lineloc--;
			break;
		}
	}
	gotopos(cx,cy);
	return;
}


list* start() /*开始时读入和打印文本*/
{
	list *head=(list *)malloc(sizeof(list));
	char filename[100];
	
	system("CLS");
	printf("Enter the filename(like a.txt):");
	scanf("%s",&filename);
	
	head=NULL;
	head=textsto(filename,head);
	textprint(head);
	line=lineloc;
	page=(line-1)/10 +1;
	
	gotopos(0,18);
	SetColor(49| FOREGROUND_INTENSITY|120 );
	printf("                                     Page:%d/%d",pageloc,page);
	SetColor(15| FOREGROUND_INTENSITY);
	gotopos(cx,cy);
	
	return head;
}

list* findynode(int y,list *head)	/*查找第y行*/
{
	list *p;
	int i=1;
	
	p=head;
	while(p!=NULL&&i<y){
		p=p->next;
		i++;
	} 
	if(i==y)
		return p;
	else{
			gotopos(0,17);
			printf("Delete failure!");
			gotopos(cx,cy);
			return NULL;
	}
} 

void pageprint(list* head){   /*真正的打印函数！解决了一些Bug和特殊情况*/
	list *p;
	int endofpage; 
	
	system("CLS");
	gotopos(0,18);
	SetColor(49| FOREGROUND_INTENSITY|120 );
	printf("                                     Page:%d/%d",pageloc,page);
	gotopos(0,19);
	SetColor(4| FOREGROUND_INTENSITY );
	printf("-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-");
	gotopos(0,20);
	SetColor(6| FOREGROUND_INTENSITY );
	printf("help:\n");
	printf("up:↑ left:←  down:↓ right:→\n");
	printf("last page:a    next page:s\n");
	printf("delete the pointed line:dd  or <x>dd to delete x lines  \n");
	printf("line move forward:p or <x>p to move x lines \n");
	printf("line move afterward:P or <x>P to move x lines  \n");
	printf("add chars before:i   after:f  correct chars:c  \n");
	printf("insert a line before:O  after:o  \n");
	printf("enter cmd: :  \n-->1.exchange two lines:m <line1> <line2>  2.open a file:o <filename>\n");
	printf("3.save to a file:w <filename>\n");
	SetColor(15| FOREGROUND_INTENSITY );
	gotopos(0,0);
	
	lineloc=(pageloc-1)*10+1;
	endofpage=(pageloc-1)*10+10;
	p=findynode((pageloc-1)*10+1,head);
	
	if(p==NULL)
		return;
	for(p;p!=NULL;p=p->next){
		if(lineloc>line||lineloc>endofpage)
			break;
			
		SetColor(11| FOREGROUND_INTENSITY );
		if(lineloc==10)		/*为了显示的对称美观*/
			printf("%d| ",lineloc++ );
		else
			printf("%d | ",lineloc++ );
		SetColor(15);
		printf("%s",p->textline);
	} 
	
	gotopos(cx,cy);
	return;
}

list* deleteline(int y,list *head)	/*删除第y行,对应输入命令为dd*/ 
{
	list *p,*s;
	p=s=head;
	
	if(head==NULL){
		gotopos(0,17);
		printf("No line exist!");
		gotopos(cx,cy);
		return NULL;
	}
	if(y==1){
		s=head;
		head=head->next;
		free(s);
	}
	else{
		if(y<=line&&y>0)
			p=findynode(y-1,head);
		else{
			gotopos(0,17);
			printf("Delete failure!");
			gotopos(cx,cy);
		}
		s=p->next;
		p->next=s->next;
		free(s);
	}
	line--;
	page=(line-1)/10 +1;
	
	pageprint(head); 
	
	return head;
}

list* addline(int y,list* head)	/*在第y行前后插入一空行，对应输入命令为o（前）或O（后）*/
{
	list *p,*s,*c; 
	p=s=head;
/**/ 
	if(y>line)
		return head;
	if(head==NULL){
		p=(list*)malloc(sizeof(list));
		p->next=NULL;
		head->next=p;
	} 
	else if(y==0){
		p=(list*)malloc(sizeof(list));
		strcpy(p->textline,"\n");
		p->next=head;
		head=p;
	}
	else{
		p=findynode(y,head);
		s=(list*)malloc(sizeof(list));
		strcpy(s->textline,"\n");
		s->next=p->next;
		p->next=s;
	}
	line++;
	page=(line-1)/10 +1;
	
	pageprint(head);
	
	return head;
}

void leftpage(list *head)	/*向左翻页*/
{
	pageloc--;
	pageprint(head);
	
	return;
}
void rightpage(list *head) /*向右翻页*/ 
{
	pageloc++;
	pageprint(head);
	
	return;
}

list* backwordline(int y,list *head)	/*将第y行向后移动一行，对应输入命令为l或L*/
{
	list *p,*s,*q;
	p=s=head;
	
	if(y>line)
		return head;
	if(y==0){
		return head;
	}
	else if(y==1){
		p=head->next;
		head->next=p->next;
		p->next=head;
		head=p;
		pageprint(head);
	}
	else if(y==line){
		head=addline(y-1,head);
		pageprint(head);
		return head;
	}
	else{         //倒数第二行存在BUG 
		p=findynode(y-1,head);
		s=p->next;
		q=s->next;
		s->next=q->next;
		p->next=q;
		q->next=s;
		pageprint(head);
	}
	
	return head;
}

list* movelines(list *head,int x1,int x2)	//将第x1行和x2行交换，对应命令为m x1 x2 
{
	list *tail1,*tail2,*p1,*p2;
	int change;
	
	if(x1>x2){	//使x1<x2，写得清楚点 
		change=x1;
		x1=x2;
		x2=change;
	}
	
	if(x2-x1==1){
		head=backwordline(x1,head);
		pageprint(head);
	}
/**/ 
	else if(x1==1){
		p1=head;
		tail2=findynode(x2-1,head);
		p2=findynode(x2,head);
		tail2->next=p2->next;
		p2->next=p1->next;
		head=p2;
		p1->next=tail2->next;
		tail2->next=p1;
		
		pageprint(head);
	}
	else{
		tail1=findynode(x1-1,head);
		tail2=findynode(x2-1,head);
		p1=findynode(x1,head);
		p2=findynode(x2,head);
		tail2->next=p2->next;
		tail1->next=p1->next;
		p2->next=tail1->next;
		tail1->next=p2;
		p1->next=tail2->next;
		tail2->next=p1;
		pageprint(head);
	}
	
	
	return head;
} 

list *addtext(int x,int y,list *head)	//开始输入文本，并插入到当前位置的内容之后，直到按ESC结束,对应命令为f或i 
{
	list *p;
	char ch;
	int i=0;
	
	while((ch=getch())!=27){  
		p=findynode(y,head);
		for(i;p->textline[i]!='\0';i++);
			i++;
		for(i;i>x;i--)
			p->textline[i]=p->textline[i-1];
		p->textline[x]=ch;
		pageprint(head);
		x++;
	}
	return head;
}

list *changetext(int x,int y,list *head)	//开始输入并修改对应的文本(只能修改已存在的文本)，直到按ESC结束,对应命令为c 
{
	list *p,*s;
	char ch;
	int i=0;
	
	while((ch=getch())!=27){
		p=findynode(y,head);
		if(p->textline[x]!='\n'){
			p->textline[x]=ch;
			pageprint(head);
			x++;
		} 
	}
	return head;
}

void textsave(list *p)		//保存文本文件 
{
	char filename[100];
	FILE *fp;
	
	erase(0,16,20);
	erase(0,17,20);
	gotopos(0,16);
	printf("Enter the filename you want to save to(like a.txt):\n");
	scanf("%s",&filename);
	
	if((fp=fopen(filename,"w+"))==NULL){
		gotopos(0,17);
		printf("Open failure!");
		gotopos(0,0);
		return;
	}
	while(p){
		fwrite(p->textline,strlen(p->textline),1,fp);
		p=p->next;
	}
	
	fclose(fp);
	return;
}

int main()
{
	list *head;
	int ch;  
	int i,k; /*进行一些重复操作时用于计数*/ 
	int x1,x2;	/*进行行移动时所用变量*/
	
	SetConsoleOutputCP(936);
	head=start();	/*得到存储完毕后的链表头*/
	
	gotopos(0,19);
	SetColor(4| FOREGROUND_INTENSITY );
	printf("-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-");
	SetColor(15| FOREGROUND_INTENSITY );
	gotopos(cx,cy);
	
	while((ch=getch())!='q'){
		switch(ch){
			case 72: //上
				if( cy>0 ) gotopos(cx, --cy);
				break;
			case 80: //下
				if( (pageloc<page&&cy<9)||(pageloc==page&&cy<line-10*(page-1)-1)) 
					gotopos(cx, ++cy);
				break;
			case 75: //左
				if( cx>4 ) gotopos(--cx, cy);
				break;
			case 77: //右
				if( cx<=M+4 ) gotopos(++cx, cy);
				break;
			case 'd':	//删除当前行 
				if((ch=getch())=='d'&&(pageloc<page&&cy<9)||(pageloc==page&&cy<line-10*(page-1)-1))
					head=deleteline(cy+1+10*(pageloc-1),head);
				break;
			case 'L':	//将当前行往后移动一行 
				head=backwordline(cy+1+10*(pageloc-1),head);
				break;
			case 'l':	//将当前行往前移动一行 
				head=backwordline(cy+10*(pageloc-1),head);
				break;
			case 'i':	//开始输入文本，并插入到当前位置的内容之前，直到按ESC结束
				head=addtext(cx-4,cy+1+10*(pageloc-1),head);
				break;
			case 'f':	//开始输入文本，并插入到当前位置的内容之后，直到按ESC结束
				head=addtext(cx-3,cy+1+10*(pageloc-1),head);
				break;
			case 'c':	//开始输入并修改对应的文本，直到按ESC结束 
				head=changetext(cx-4,cy+1+10*(pageloc-1),head);
				break;
			case 49:case 50:case 51:case 52:case 53:case 54:case 55:case 56:case 57:	//表示1-9这几个数字
				k=ch-48; 	//读入1-9的数字
				ch=getch();
				switch(ch){
					case 'd':	//删除从当前行开始的k行 
						if((ch=getch())=='d'&&cy<line-10*(page-1))
							for(i=1;i<=k;i++){
								head=deleteline(cy+1+10*(pageloc-1),head);
							}
						break;
					case 'L':	//将当前行向后移k行 
						for(i=0;i<k;i++)
							head=backwordline(cy+1+10*(pageloc-1)+i,head);
						break;
					case 'l':	//将当前行向前移k行       //移动最后一行时也存在bug 
						for(i=0;i<k;i++)
							head=backwordline(cy+10*(pageloc-1)-i,head); 
						break;
				}
				break;
			case 'o':	/*在当前行的后面插入一个新行*/ 
				head=addline(cy+1+10*(pageloc-1),head);
				break;
			case 'O':	/*在当前行的前面插入一个新行*/
				head=addline(cy+10*(pageloc-1),head);
				break;
			case 'a':	/*向左翻页*/
				if(pageloc>1)
					leftpage(head);
				break;
			case 's':	/*向右翻页*/
				if(pageloc<page)
					rightpage(head);
				break;  				
			case ':':	/*命令行模式*/
				gotopos(0,16);
				printf("Enter command:");
				gotopos(0,17); 
				while((ch=getche())!='q'){
					switch(ch){
						case 'm':	/*将两行换位*/ 
							printf(" ");
							scanf("%d%d",&x1,&x2);
							if(x1==x2)
								;
							else if(x1<=line&&x2<=line&&x1>0&&x2>0)
								movelines(head,x1,x2);
							else
								printf("Selection error!");
							gotopos(0,16);
							printf("Enter command:");
							erase(0,17,20);
							gotopos(0,17);
							break;
						case 'o':	/*打开指定文件*/ 
							head=start();
							gotopos(0,16);
							printf("Enter command:");
							erase(0,17,20);
							gotopos(0,17);
							break;
						case 'w':	/*将已有的信息存入指定文件*/ 
							textsave(head);
							erase(0,16,100);
							gotopos(0,16);
							printf("Enter command:");
							erase(0,17,20);
							gotopos(0,17);
							break;
						case '\b':	/*清除命令*/ 
							erase(0,17,20);
							gotopos(0,17);
							break;
					}
				}
				erase(0,16,20);
				erase(0,17,20);
				gotopos(cx,cy);
				break;
		}
	}
	gotopos(0,13);

	return 0;
}
