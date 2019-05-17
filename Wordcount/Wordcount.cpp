#include<stdio.h> 
#include<string.h>
#include<stdlib.h>
#include<time.h>
#include<conio.h>
#include<windows.h>
#define MaxWord 20   //单词最长20个字母 
#define N 10000  //结构数组最多储存N个单词 
struct Word_Item;
int count=0;//用于记录总单词数
int count2=0;//用于记录不重复的单词数 
struct Trans_Item  {
    struct Word_Item * transWord; // 转移单词
    int  transFreq;                             // 转移概率
    struct Trans_Item * next;           // 下一个转移单词项
};
typedef struct Trans_Item TransItem;
struct Word_Item 
{
    char word[MaxWord+1]; // 单词
    float  freq;                         // 词频
    struct Word_Item * next;  //  指针
    TransItem * trans;             // 转移概率列表
};
typedef struct Word_Item WordItem;
WordItem *list;

WordItem *head = NULL;
int sum;     
WordItem *SearchWord(char *current); 
void WordCount(char *current);
void Exchage_word(char *current);
WordItem *BubbleSort(WordItem *pNode);
void PrintResult();
void Release();

WordItem * countWord(char w[]);

int IsLetter(char c);
char ToLower(char c);
int hash(char word[ ]);
WordItem * findWord(char w[ ]);
WordItem * findCreateNode(WordItem *head, char w[]);
WordItem *sort(WordItem a[],int low,int high);
TransItem *findcreateNode(TransItem *head,char w[]);

 
int main(void)
{
	begin:
	int i,n;

	list = (WordItem*)malloc(sizeof(WordItem) * N);

	for(i=0;i<N;i++){
		strcpy(list[i].word,"\0");
		list[i].freq=0;
		list[i].next=NULL;
		list[i].trans=NULL;
	}

	int choice;
	
	printf("Enter novel you want to analyze:\n");
	printf("1.Gone with the wind\n");
	printf("2.The Twilight Saga\n");
	FILE *fp;
	scanf("%d",&choice);
	
	if(choice==1)
	    fp = fopen("Gone with the wind.txt", "r");
	else
	    fp = fopen("The Twilight Saga.txt", "r");
	int func;

	printf("Please choice the function :\n");
	printf("1.All word frequency\n");
	printf("2.Followed word frequency\n");
	printf("3.Writingrobot\n");
	scanf("%d",&func);
	
	if(func==1){
		char temp[30];
		while( EOF != (fscanf(fp,"%s",temp)) ) 
    {  
        Exchage_word(temp);
        WordCount(temp);  
    }  
       fclose(fp); //关闭文件 
	   head=BubbleSort(head);
       PrintResult(); //输出统计结果 
       Release();
		
	}
	else if(func==2){
		char buf[1024], w[30], *p, *s;
	char last_word[20] = "";
	while (fscanf(fp, "%s", buf) > 0) // 从文件中读取一个字符串
	{
		strlwr(buf);
		for (p = buf; *p; )
		{
			
			while (*p && !IsLetter(*p))
				p++; // 过滤非字母
			for (s = w; s < &w[sizeof(w)] && IsLetter(*p); s++, p++)
				*s = ToLower(*p);  // 转换为小写，保存到单词中
			*s = '\0'; // 字符串结束符
			
			if (strcmp(w, "") != 0)	{
				WordItem *word;
				word = countWord(w);// w 加入hash表
				if (strcmp(last_word, "") != 0)
				{
					WordItem *LastWord = findWord(last_word);
					TransItem *ptr = LastWord->trans;
					if (ptr == NULL)
					{
						LastWord->trans = (TransItem*)malloc(sizeof(TransItem));			
						LastWord->trans->transWord = word;
						LastWord->trans->transFreq = 1;
						LastWord->trans->next = NULL;
					}
					else {
						int max_length = 0;
						int flg = 1; //记录是否在转移表里(默认不在)
						while (1)
						{
							if (ptr->next == NULL) {
								TransItem *p = (TransItem*)malloc(sizeof(TransItem));						
								p->transFreq = 1;
								p->transWord = word;
								p->next = NULL;
								ptr->next = p;
								break;										
							}
							if (strcmp(ptr->transWord->word, word->word) == 0){
								ptr->transFreq++;
								flg = 0;
								break;
							}
							max_length++;
							ptr = ptr->next;
						}
					}
				}
				strcpy(last_word, w);
			}
		}
	}
	fclose(fp);
//以下为转移概率部分 
    FILE *fpt;
    fpt = fopen("Followed word frequency.txt","w");

	for (int i = 0;i<N;i++)
	{
		if (strcmp(list[i].word, "") != 0)
		{
			fprintf(fpt,"[%s] \n", list[i].word);
			printf("[%s] \n", list[i].word);
			TransItem *trans = list[i].trans;
			while (trans != NULL)
			{
				fprintf(fpt,"%-15s  %d\n",trans->transWord->word, (trans->transFreq));
				printf("%-15s  %d\n",trans->transWord->word, (trans->transFreq));
				trans = trans->next;
			}
			printf("\n");
			fprintf(fpt,"\n");
			WordItem *p = list[i].next;
			while (p!=NULL)
			{
				if (strcmp(p->word, "") != 0)
				{
					fprintf(fpt,"[%s] \n", p->word);
					printf("[%s] \n", p->word);
					TransItem *transTemp = p->trans;
					while (transTemp != NULL)
					{
						fprintf(fpt,"%-15s  %d\n", transTemp->transWord->word, (transTemp->transFreq));
						printf("%-15s  %d\n", transTemp->transWord->word, (transTemp->transFreq));
						transTemp = transTemp->next;
					}
					fprintf(fpt,"\n");
					printf("\n");
				}
				p = p->next;
			}
		}
	}
	fclose(fpt);
	}
  else if(func==3){
		
	
	char buf[1024], w[30], *p, *s;
	char last_word[20] = "";
	while (fscanf(fp, "%s", buf) > 0) // 从文件中读取一个字符串
	{
		strlwr(buf);
		for (p = buf; *p; )
		{
			
			while (*p && !IsLetter(*p))
				p++; // 过滤非字母
			for (s = w; s < &w[sizeof(w)] && IsLetter(*p); s++, p++)
				*s = ToLower(*p);  // 转换为小写，保存到单词中
			*s = '\0'; // 字符串结束符
			
			if (strcmp(w, "") != 0)	{
				WordItem *word;
				word = countWord(w);// w 加入hash表
				if (strcmp(last_word, "") != 0)
				{
					WordItem *LastWord = findWord(last_word);
					TransItem *ptr = LastWord->trans;
					if (ptr == NULL)
					{
						LastWord->trans = (TransItem*)malloc(sizeof(TransItem));			
						LastWord->trans->transWord = word;
						LastWord->trans->transFreq = 1;
						LastWord->trans->next = NULL;
					}
					else {
						int max_length = 0;
						int flg = 1; //记录是否在转移表里(默认不在)
						while (1)
						{
							if (ptr->next == NULL) {
								TransItem *p = (TransItem*)malloc(sizeof(TransItem));						
								p->transFreq = 1;
								p->transWord = word;
								p->next = NULL;
								ptr->next = p;
								break;										
							}
							if (strcmp(ptr->transWord->word,word->word) == 0){
								ptr->transFreq++;
								flg = 0;
								break;
							}
							max_length++;
							ptr = ptr->next;
						}
					}
				}
				strcpy(last_word, w);
			}
		}
	}
	fclose(fp);
	
	char *start_word = (char*)malloc(sizeof(char) * 20);
	printf("Give it a word to start an article:\n");
	scanf("%s", start_word);
	printf("Enter the number of words in this article:\n");
	scanf("%d",&n);
	
	FILE *fpt;
    fpt = fopen("writingrobot.txt","w");//打开文档，写入


	fprintf(fpt,"%s ", start_word);
	for (int i = 0;i<n;i++)
	{
		WordItem *word = findWord(start_word);
		TransItem *trans_word = word->trans;
		if (trans_word !=NULL)
		{
			// 俄罗斯轮盘发选择转移矩阵中的单词
			TransItem *next_word = trans_word;
			int num = 0;
			int sum = 0;
			while (next_word!=NULL)
			{
				num++;
				sum += next_word->transFreq;
				next_word = next_word->next;
			}
			double seed = rand() % 100 / (double)101;
			double s = 0.0;
			for (int i = 0;i<num;i++)
			{
				s += 1.0 * trans_word->transFreq / sum;
				if (s > seed)
					break;
				trans_word = trans_word->next;
			}
			int max_feq = -1;

			fprintf(fpt,"%s ", trans_word->transWord->word);
			printf("%s ", trans_word->transWord->word);
			start_word = trans_word->transWord->word;
		}
		else {
			// 没有转移矩阵 随机挑选其他的
			srand((int)time(0));
			int pos = rand() % N;
			while (strcmp(list[pos].word,"") == 0)
			{
				pos = rand() % N;
			}
			start_word = list[pos].word;
		}

	}
	fclose(fpt);
  }goto begin;
}

WordItem * countWord(char w[]) {
	int h = hash(w) % N;
	if (h<0){
		h = 0;
	}
	if (list[h].word[0] == '\0') {
		strcpy(list[h].word, w);
		list[h].freq = 1;
		return list + h;
	}
	else if (strcmp(list[h].word, w) == 0) {
		list[h].freq++;
		return list + h;
	}
	else {
		WordItem *head = list + h;
		WordItem * p = head->next;
		while (1) {
			if (strcmp(head->word, w) == 0) {
				head->freq++;
				return head;
			}
			if (head->next)
				head = head->next;
			else break;
		}
		p = (WordItem*)malloc(sizeof(WordItem));
		strcpy(p->word, w);
		p->freq = 1;
		p->next = NULL;
		p->trans = NULL;
		head->next = p;
		return p;
	}
}

//判断是否为字母 
int IsLetter(char c)
{
	if((c<='z'&&c>='a')||(c<='Z'&&c>='A')) 
		return 1;
	else 
		return 0;
}
//将大写字母转化为小写，若本为小写字母则不变
char ToLower(char c) {
	return c;
}
// 计算单词的哈希值（散列值）
int hash(char word[ ])  
{  
    int hash = 0, ch;  
    while ( ch = *word++ )  
         // 也可以乘以31、131、1313...  
         hash = hash * 31 + ch; 
     return hash;  
} 
//计算单词所在的节点 
WordItem * findWord(char w[ ])
{
      int h = hash(w) % N;
      if (h < 0)
		  h = 0;
      if(list[h].word [0] == '\0' ) {
          strcpy(list[h].word, w);
          count2++;
          return list + h;
      }
      return findCreateNode(list+h, w);
}
WordItem * findCreateNode(WordItem *head, char w[ ])
{
     WordItem * p = head->next;
     while ( 1 ) {
          if( strcmp(head->word, w)==0 )
               return head;
         if( head->next ) 
               head = head->next;
         else break;
     }
     p = (WordItem*)malloc(sizeof(WordItem));
     strcpy(p->word, w);
     p->freq = 0;
     p->next = NULL;
     head->next = p;
     count2++;
     p->trans = NULL;
     return p;
}
//找到trans中w所在节点，若不存在，则创建
TransItem *findcreateNode(TransItem *head,char w[])
{
	TransItem *p=head;
	while ( 1 ) {
    	if(strcmp(head->transWord->word, w)==0)
    		return head;
        if( head->next ) 
            head = head->next;
        else break;
     }
     p = (TransItem*)malloc(sizeof(TransItem));
     strcpy(p->transWord->word, w);
     p->transFreq = 0;
     p->next = NULL;
     head->next = p;
    
     return p;
}

//对数组进行排序
WordItem *sort(WordItem a[],int low,int high){
	int s = low, t = high ;
	 WordItem pivot = a[s];
     if (low >= high) return a;
     while ( s < t ) {
          while (t > s && a[ t ].freq <= pivot.freq )  -- t;
          a[ s ] = a[ t ];  //将比pivot的词频大的移到前面
          while ( s < t && a[ s ].freq >= pivot.freq) ++ s;
          a[ t ] = a[ s ]; //将比pivot的词频小的移到后面
      }
     a[ s ] = pivot; //将词频保存回到数组
     sort(a, low, s - 1); //递归
     sort(a, s + 1, high); //递归
}


WordItem * SearchWord(char *current)//储存并查找单词所在节点  
{  
    if(head==NULL) //统计第一个单词 
    {  
        head = new WordItem;  
        strcpy(head->word, current);  
        head->freq= 0;  
        head->next = NULL;  
        return head;  
    }  
  
      
    WordItem *ptr1 = head;  
    WordItem *ptr2 = NULL;  
    while( (NULL != ptr1) && (0 != strcmp(ptr1->word, current)) )  
    {  
        ptr2 = ptr1;  
        ptr1 = ptr1->next;  
    }  
  
    if(ptr1 == NULL) //该单词不存在  
    {  
        ptr1 = new WordItem;  
        strcpy(ptr1->word, current);  
        ptr1->freq = 0;  
        ptr1->next = NULL;  
        ptr2->next = ptr1;  
    }  
    return ptr1;  
}


void WordCount(char *current)//单词统计  
{  
    WordItem *ptr = NULL;  
    ptr = SearchWord(current);  
    if(NULL == ptr)  
    {  
        return;  
    }  
    else  
    {  
        ptr->freq++;  
    }  
}  

void Exchage_word(char *current) //转换单词的大小写，去掉标点符号 
{  
    int i=0,j;  
    while(current[i]!='\0'){  
        if(current[i]>='A' && current[i]<='Z'){   //大写转小写  
            current[i]=current[i]+32;  
        }  
        if(!((current[i]>='A' && current[i]<='Z')  //去标点符号  
            || (current[i]>='a' && current[i]<='z')  
            || current[i]=='-'   
            || current[i]=='\'')){  
                for(j=i;current[j]!='\0';j++){  
                    current[j]=current[j+1];  
                }  
                 
                i--;  
        }  
        i++;  
    }  
    return;  
} 

WordItem *BubbleSort(WordItem *pNode)//按顺序排序 
{  
  
    int nodecount;//用来控制次数  
     WordItem *ptr;
     sum=0;
	 for(ptr=head;ptr!=NULL;ptr=ptr->next){
		sum=sum+ptr->freq;
		nodecount++;
		
	}
	
	WordItem *pMove;  
    pMove = pNode;  
  
      
    while (nodecount > 1) {  
        while (pMove->next != NULL) {  
            if (pMove->freq < pMove->next->freq) {  
                int temp;   
                temp = pMove->freq;  
                pMove->freq = pMove->next->freq;  
                pMove->next->freq = temp;
                char *temp2;
                strcpy(temp2,pMove->word);
                strcpy(pMove->word,pMove->next->word);
                strcpy(pMove->next->word,temp2);
            }  
            pMove = pMove->next;  
        }  
        nodecount--;  
          
        pMove = pNode;  
    }  
  
     
  
    return pNode;  
} 

void PrintResult()//输出结果  
{  
    int i=1; 
    
	FILE* fp1=fopen("all_word_frequency.txt", "w");  
     
    if(head ==NULL)  
    {  
        printf("No Word!!\n");  
    }  
    else  
    {  
        WordItem *ptr1 = head;
  
        while(ptr1 != NULL)  
        {      
            fprintf(fp1,"%6d %-20s%lf\n",i,ptr1->word,(double)ptr1->freq/sum);  
            printf("%6d %-20s%lf\n",i,ptr1->word,(double)ptr1->freq/sum);  
  
            i++;
  
            ptr1 = ptr1->next;  
        }  
         fclose(fp1);  
          
    }  
} 
void Release()  
{  
    if(head=NULL)  
    {  
        return;  
    }  
    WordItem *ptr = head;  
    while(ptr != NULL)  
    {  
        head = ptr->next;  
        delete ptr;  
        ptr = head;  
    }  
}  

