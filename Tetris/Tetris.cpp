/**@author 赵宸泽 
  *@version 1.99 build614
  *@name Tetris 
  *@project ZJU_XJC_C_DJ Project3
  *@group G14
  *@brief 一个俄罗斯方块小游戏 
  */ 
#include "graphics.h"
#include <time.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <locale.h>


#define DefaultFont 20,0,"微软雅黑"
#define MarkFont 29,0,"微软雅黑"
#define TITLE "G14 Aploium Tetris v1.99 build614"
#define RAWFPS 120
#define FTP_SALT "TtTPVg9rZUkh4t3m"
//#define IS_DEBUG


struct ScoreData{
		int mark,no;
		char name[50];
		char date[25];
	};

const int g_width = 400;
const int g_height = 520;

/*记录7种形状及其4种变化的表*/
static int g_trs_map[8][4][4][4];
/*变化数目表*/
static int g_map_mod[] = {1, 4, 4, 4, 2, 2, 2, 1, 0};
static float bk_HSL_H = 77.0;
	int nfps = RAWFPS;
    int m_base_x, m_base_y, m_base_w, m_base_h;
    int m_droptime;
    int m_curtime;
    int m_cursubtime;
    int m_movxtime;
    int m_curxtime;

    int m_gamepool_w, m_gamepool_h;
    int m_gamepool[30][30]; //从1为起始下标，0用于边界碰撞检测
    int m_ctl_x, m_ctl_y, m_ctl_t, m_ctl_s; //当前控制块属性
    float m_ctl_dx, m_ctl_dy;
    int m_next1_s, m_next2_s;
    int m_forbid_down;
    int m_colormap[10];

    int m_pause;
    int m_state; //游戏主状态
    int m_gray_y;
    int m_over_st;
    int m_Keys[8];
    int m_KeyFlag[8];
    int m_KeyState[8];
	int m_socre=0;

    PIMAGE m_pcb;
        int Trs_map[8][4][4][4] = /*格子形式存储基本方块*/
    {
        {{{0}}},{{
            {0,0,0,0},{1,1,1,0},{0,1,0,0},
        },{
            {0,1,0,0},{1,1,0,0},{0,1,0,0},
        },{
            {0,1,0,0},{1,1,1,0},
        },{
            {0,1,0,0},{0,1,1,0},{0,1,0,0},
        },},{{
            {2,2,0,0},{0,2,0,0},{0,2,0,0},
        },{
            {0,0,2,0},{2,2,2,0},
        },{
            {0,2,0,0},{0,2,0,0},{0,2,2,0},
        },{
            {0,0,0,0},{2,2,2,0},{2,0,0,0},
        },},{{
            {0,3,3,0},{0,3,0,0},{0,3,0,0},
        },{
            {0,0,0,0},{3,3,3,0},{0,0,3,0},
        },{
            {0,3,0,0},{0,3,0,0},{3,3,0,0},
        },{
            {3,0,0,0},{3,3,3,0},
        },},{{
            {4,4,0,0},{0,4,4,0},
        },{
            {0,0,4,0},{0,4,4,0},{0,4,0,0},
        },},{{
            {0,5,5,0},{5,5,0,0},
        },{
            {0,5,0,0},{0,5,5,0},{0,0,5,0},
        },},{{
            {0,0,0,0},{6,6,6,6},
        },{
            {0,0,6,0},{0,0,6,0},{0,0,6,0},{0,0,6,0},
        },},{{
            {0,0,0,0},{0,7,7,0},{0,7,7,0},
        },},
    };
    
    enum {/*状态表*/
        ST_START, /*游戏重新开始*/
        ST_NEXT,  /*准备下一个方块*/
        ST_NORMAL,/*玩家控制阶段*/
        ST_OVER   /*游戏结束*/
    };

void initgr();/*图形界面初始化*/
void BkColorSwift();/*控制背景渐变*/
int InitWelcome();/*主菜单初始化，会多次调用*/
int DotWithin(int x,int y,int n,int bz[][4]);/*判断点是否在区域内，用于button*/
void InitGame(int w, int h, int bw, int bh);/*游戏主参数初始化*/
int deal();/*处理游戏各类进程*/
bool isCrash();/*是否失败*/
void merge();/*deal下属分功能模块*/
void update();/*逻辑更新，处理键盘输入*/
void drawedge(int x, int y, int w, int h, int color, int bdark );/*方块绘图用*/
void drawtile(int x, int y, int w, int h, int d, int color);/*方块绘图用*/
void drawframe(int x, int y, int w, int h, int d );/*方块绘图用*/
void draw44(int bx, int by, int mat[4][4],float dx, float dy, int nc, int deep);/*方块绘图用*/
void render();/*游戏进行时更新画面并处理buttons*/
int GameOverProcess();/*处理GameOver*/
int QqsortScoreCmp(const void *a,const void *b);/*快排用*/
void DisplayScoreList(ScoreData	*sdata,int start,int total);/*显示排行榜*/
int ShowGlobalHighScore(char *seekname);/*菜单项，读取并显示网络排行榜*/
void AboutAndThanks();/*About*/
void EmptyMsgQueue();/*清空鼠标和键盘消息队列*/
int dealbit(int a, float dt);
int getchangcolor(int Color, float t);

/*初始化全局数据及图形显示*/
void initgr() {
//	setinitmode()
	setinitmode(0);
    initgraph(g_width, g_height);
    setcaption(TITLE);
    setfont(12, 6, "宋体");
    memcpy(g_trs_map, Trs_map, sizeof(Trs_map));
    setbkmode(TRANSPARENT);
}

void BkColorSwift(){
		bk_HSL_H = bk_HSL_H + 0.06;
		setbkcolor(hsl2rgb(bk_HSL_H,0.5,0.5));
}

int InitWelcome(){
		
		int bz[5][4]={/*ButtonZone*/
			{80,92,273,66},
			{10,200,400,80},
			{10,270,400,65},
			{10,340,400,73},
			{130,430,160,70}
		};
  static PIMAGE menu_tetris = newimage(bz[0][2],bz[0][3]),
				menu_start = newimage(bz[1][2],bz[1][3]),
				menu_score = newimage(bz[2][2],bz[2][3]),
				menu_about = newimage(bz[3][2],bz[3][3]),
				menu_exit = newimage(bz[4][2],bz[4][3]),				
				egg_avatar = newimage(128,128);
		static int firstrunflag = 1;
		int currentslect = 1,mousex,mousey,clickflag=0;
		mouse_msg ch;
		
		
		
		if(firstrunflag)
		{
			firstrunflag = 0;
			getimage(menu_tetris,"res\\Tetris.png");/*读取菜单图片*/
			getimage(menu_start,"res\\StartNewGame.png");
			getimage(menu_score,"res\\ViewGlobalHighSocre.png");
			getimage(menu_about,"res\\AboutAndThanks.png");
			getimage(menu_exit,"res\\Exit.png");
			getimage(egg_avatar,"res\\avatar.png");	
		}

		
		while(1)
		{
	
			if(mousemsg() && (ch=getmouse()).is_left() && (ch.is_up() || ch.is_down())  )
			{
				currentslect = DotWithin(ch.x,ch.y,5,bz);
				if(currentslect==4) exit(0);
				EmptyMsgQueue(); /*清空鼠标消息队列*/
				return currentslect;
			}
			else
			{
				mousepos(&mousex,&mousey);
				currentslect = DotWithin(mousex,mousey,5,bz);
			}
			
			BkColorSwift();
			
			cleardevice();
			if(currentslect>0)
				drawframe(bz[currentslect][0],
						bz[currentslect][1],
						bz[currentslect][2] - 20,
						bz[currentslect][3] - 20,
						hsl2rgb(bk_HSL_H+60,0.5,0.5)
						);
			if(currentslect==0)
				putimage(136,50,egg_avatar,SRCCOPY);
			else
				putimage(bz[0][0],bz[0][1],menu_tetris,MERGEPAINT);
			
			
			putimage(bz[1][0],bz[1][1],menu_start,SRCPAINT);
			putimage(bz[2][0],bz[2][1],menu_score,SRCPAINT);
			putimage(bz[3][0],bz[3][1],menu_about,SRCPAINT);
			putimage(bz[4][0],bz[4][1],menu_exit,SRCPAINT);

			delay_fps(nfps);
		}
	}

int DotWithin(int x,int y,int n,int bz[][4]){
	n--;
	for( ;n>=0;n--){
		if(x>=bz[n][0] && x<=bz[n][0]+bz[n][2]
		&& y>=bz[n][1] && y<=bz[n][1]+bz[n][3])
			return n;
	}
	return -1;
}
    
    

   void InitGame(int w, int h, int bw, int bh) {
        int colormap[10] = {0, 0xA00000, 0xA05000, 0xA0A000, 0xC000,
            0x00A0A0, 0x4040C0, 0xA000A0, 0x808080, 0xFFFFFF};
        memcpy(m_colormap, colormap, sizeof(m_colormap));

        int Keys[8] = {VK_F2,VK_LEFT,VK_RIGHT,VK_DOWN,VK_UP,VK_NUMPAD0,VK_SPACE};
        memcpy(m_Keys, Keys, sizeof(Keys));

        memset(m_KeyState, 0, sizeof(m_KeyState));
        m_gamepool_w = w;
        m_gamepool_h = h;
        m_base_w = bw;
        m_base_h = bh;
        m_socre = 0;
		m_pause = 0;
		nfps = RAWFPS;
		
        randomize();
        m_ctl_t = -1;
        m_pcb = newimage();
        for (int i=0; i<10; ++i) {
            drawtile(bw * i, 0, bw, bh, 5, colormap[i]);
        }
        getimage(m_pcb, 0, 0, bw*10, bh);
        m_state = ST_START;
    }
    /*状态转换处理*/
    int deal () {
        int nRet = 0;
        if ( m_state == ST_START ) { //初始化 
            m_next1_s = random(7) + 1;
            m_next2_s = random(7) + 1;
            m_pause = 0;
            memset(m_gamepool, 255, sizeof(m_gamepool));
            for (int y = 1; y <= m_gamepool_h; ++y) {
                for (int x = 1; x <= m_gamepool_w; ++x)
                    m_gamepool[y][x] = 0;
            }
            m_forbid_down = 0;
            m_ctl_t = -1;
            nRet = 1;
            m_state = ST_NEXT;
        } else if ( m_state == ST_NEXT ) {
            m_ctl_x = (m_gamepool_w - 4) / 2 + 1;
            m_ctl_y = 1;
            m_ctl_t = 0;
            m_ctl_s = m_next1_s;
            m_ctl_dy = 0;
            m_next1_s = m_next2_s;
            m_next2_s = random(7) + 1;
            m_curtime = m_droptime;
            m_curxtime = 0;
            nRet = 1;
            if ( isCrash() ) {
                m_gray_y = m_gamepool_h * 2;
                m_over_st = 0;
                m_state = ST_OVER;
            } else {
                m_state = ST_NORMAL;
            }
        } else if (m_state == ST_NORMAL) {
            /*处理自由下落*/
            int i, j;
            if ( m_KeyState[3] == 0 || m_forbid_down) {
                --m_curtime, m_cursubtime = 1;
            }
            if ( m_curxtime ) {
                if (m_curxtime<0)
                    m_curxtime++;
                else
                    m_curxtime--;
            }
            /*按键处理*/
            for (i = 1, j = 1; i<=2; ++i, j-=2) {
                for ( ; m_KeyFlag[i] > 0; --m_KeyFlag[i]) {
                    m_ctl_x -= j;
                    if ( isCrash() )
                        m_ctl_x += j;
                    else
                        m_curxtime = m_movxtime * j;
                }
            }
            m_ctl_dx = float(double(m_curxtime) / m_movxtime); //处理x方向平滑
            for (i = 4, j = 1; i<=5; ++i, j-=2) {
                for (int t ; m_KeyFlag[i] > 0; --m_KeyFlag[i]) {
                    m_ctl_t=((t=m_ctl_t)+g_map_mod[m_ctl_s]+j)%g_map_mod[m_ctl_s];
                    if ( isCrash() ) m_ctl_t = t;
                }
            }
            if ( m_forbid_down == 0 && (m_KeyState[3] ) ) {
                    m_curtime -= m_cursubtime++;
            }
            if (m_curtime<0) {
                ++m_ctl_y;
                if ( isCrash() ) {
                    --m_ctl_y;
                    merge();
                    m_ctl_dy = 0; m_ctl_dx = 0; m_ctl_t = -1;
                    if ( m_KeyState[3] )
                        m_forbid_down = 1;
                    m_state = ST_NEXT;
                } else {
                    m_curtime += m_droptime;
                }
            }
            if (m_state == ST_NORMAL) {
                m_ctl_dy = float(double(m_curtime) / m_droptime);//处理y方向平滑
            }
        } else if (m_state == ST_OVER) {
            if ( m_gray_y>0 && (m_gray_y % 2) == 0)
                for (int x = 1; x <= m_gamepool_w; ++x)
                    if ( m_gamepool[m_gray_y>>1][x] )
                        m_gamepool[m_gray_y>>1][x] = 8;
            m_gray_y--;
            ++m_over_st;
         //   if ( m_KeyFlag[0] > 0 )
        //        m_state = ST_START;
        }
        memset(m_KeyFlag, 0, sizeof(m_KeyFlag));
        return nRet;
    }
    /*碰撞检测*/
    bool isCrash() {
        for (int y=0; y<4; ++y) {
            for (int x=0; x<4; ++x)
                if ( g_trs_map[m_ctl_s][m_ctl_t][y][x] ) {
                    if ( m_ctl_y + y < 0 || m_ctl_x + x < 0
                        || m_gamepool[m_ctl_y + y][m_ctl_x + x])
                        return true;
                }
        }
        return false;
    }
    void merge() {
        int y, x, cy = m_gamepool_h;
        int accu;
        accu = 0;
        /*合并处理*/
        for (y=0; y<4; ++y) {
            for (x=0; x<4; ++x)
                if ( g_trs_map[m_ctl_s][m_ctl_t][y][x] )
                    m_gamepool[m_ctl_y + y][m_ctl_x + x]
                        = g_trs_map[m_ctl_s][m_ctl_t][y][x];
        }
        /*消行计算*/
        for (y = m_gamepool_h; y >= 1; --y) {
            for (x = 1; x <= m_gamepool_w; ++x) {
                if ( m_gamepool[y][x] == 0 )
                    break;
            }
            if ( x <= m_gamepool_w ) {
                if ( cy != y ) {
                    for (x = 1; x <= m_gamepool_w; ++x)
                        m_gamepool[cy][x] = m_gamepool[y][x];
                }
                --cy;
                
            }
             
        }
        for (y = cy; y >= 1; --y) {
            for (x = 1; x <= m_gamepool_w; ++x)
                m_gamepool[y][x] = 0;
            accu = !accu ? 1 : 2*accu + 1;
        }
        if(accu) m_socre += (int)(100.0*accu*nfps/RAWFPS);
    }
    /*逻辑更新主函数*/
    void update() {
    	if(m_pause) return; 
        key_msg key;
        int k = kbmsg();
        while ( k ) {
            key = getkey();
            for (int i=0; i<8; ++i) {
                if (key.key == m_Keys[i]) {
                    if (key.msg == key_msg_down) {
                        m_KeyFlag[i]++;
                        m_KeyState[i] = 1;
                    } else if (key.msg == key_msg_up) {
                        m_KeyFlag[i] = 0;
                        m_KeyState[i] = 0;
                        if ( i == 3 )
                            m_forbid_down = 0;
                    }
                }
            }
            k = kbmsg();
        }
        
        
        
        while ( deal() );
    }
    void drawedge(int x, int y, int w, int h, int color, int bdark = 1) {
        setcolor(getchangcolor(color, 1.5f));
        line(x, y+h, x, y);
        lineto(x+w, y);
        if ( bdark )
            setcolor(getchangcolor(color, 0.7f));
        lineto(x+w, y+h);
        lineto(x, y+h);
    }
    void drawtile(int x, int y, int w, int h, int d, int color) {
        w--, h--;
        setfillcolor(color);
        bar(x+1, y+1, x+w, y+h);
        drawedge(x, y, w, h, color);
        drawedge(x+1, y+1, w-2, h-2, color);
    }
    void drawframe(int x, int y, int w, int h, int d = 0) {
        int coll[] = {0x400040, 0x600060, 0xA000A0, 0xFF00FF,
            0xA000A0, 0x600060, 0x400040};
        setfillcolor(!d ? 0x010101 : d);
        bar(x, y, x + w--, y + h--);
        for (int i=0; i<7; ++i) {
            --x, --y, w += 2, h += 2;
            drawedge(x, y, w, h, coll[i], 0);
        }
    }
    void draw44(int bx, int by, int mat[4][4],
        float dx=0, float dy=0, int nc=0, int deep=5) {
        for (int y = 3; y >= 0; --y) {
            for (int x = 0, c; x < 4; ++x) {
                if ( c = mat[y][x] ) {
                    if ( nc ) c = nc;
                    drawtile(int(bx + (x + dx) * m_base_w + 1000.5) - 1000,
                            int(by + (y - dy) * m_base_h + 1000.5) - 1000,
                            m_base_w, m_base_h, deep,
                            m_colormap[c]);
                }
            }
        }
    }
    /*图形更新主函数*/
    void render() {
        int x, y, c, bx, by ,currentslect;
        int mousex,mousey; 
		mouse_msg ch;
		int bz[4][4] = {
						{m_base_x-3, m_base_y + 18*m_base_h, 4*m_base_w +5, m_base_h},/*返回菜单*/
						{m_base_x-3, m_base_y + 16*m_base_h, 4*m_base_w +5, m_base_h},/*暂停继续*/
						{m_base_x-3, m_base_y + 14*m_base_h, 2*m_base_w -5  , m_base_h},/*1.2x 加速*/
						{m_base_x*3+15, m_base_y + 14*m_base_h, 2*m_base_w   , m_base_h} /*0.8x 减速*/				
						};
		
        
        cleardevice();
        
        /*画按钮并处理按按钮的事件，按钮如果不是第一个画的话会不断闪烁，原因未知*/
        mousepos(&mousex,&mousey);
        currentslect = DotWithin(mousex,mousey,4,bz);
        if( mousemsg() 
			&& ( ch = getmouse()).is_left()  
			&& ch.is_up() )
			{
				switch(currentslect){
					case 0: m_state = ST_OVER; break;
					case 1: m_pause = !m_pause; break;
					case 2: if(!m_pause) nfps = 1.2 * nfps;break;
					case 3: if(!m_pause) nfps = 0.833333 * nfps;break;
				}
				
				
			}

		/*画按钮们*/
  		drawframe(bz[0][0],bz[0][1],bz[0][2],bz[0][3],
		  	currentslect==0 ? hsl2rgb(bk_HSL_H+60,0.5,0.5) : getbkcolor());
        outtextxy(bz[0][0]+3 , bz[0][1],"终止并返回菜单");
        
  		drawframe(bz[1][0],bz[1][1],bz[1][2],bz[1][3],
		  	currentslect==1 ? hsl2rgb(bk_HSL_H+60,0.5,0.5) : getbkcolor());
        outtextxy(bz[1][0]+3 , bz[1][1],m_pause?"继续游戏":"暂停游戏"); 
        
        if(!m_pause){
        	drawframe(bz[2][0],bz[2][1],bz[2][2],bz[2][3],
			  	currentslect==2 ? hsl2rgb(bk_HSL_H+60,0.5,0.5) : getbkcolor());
	        outtextxy(bz[2][0]+3 , bz[2][1],"加速");
	        
	        drawframe(bz[3][0],bz[3][1],bz[3][2],bz[3][3],
			  	currentslect==3 ? hsl2rgb(bk_HSL_H+60,0.5,0.5) : getbkcolor());
	        outtextxy(bz[3][0]+3 , bz[2][1],"减速");
	    	
        }
	   	    
        xyprintf(bz[2][0]+3 , bz[2][1] -30,"当前速度 %.3fx",(float)nfps/RAWFPS);
        
        
        /*画背景框*/
        drawframe(  m_base_x + 5 * m_base_w,
                    m_base_y,
                    m_gamepool_w * m_base_w,
                    m_gamepool_h * m_base_h);
        drawframe(m_base_x, m_base_y,              4*m_base_w, 4*m_base_h);
        drawframe(m_base_x, m_base_y + 5*m_base_h, 4*m_base_w, 4*m_base_h);
        

        /*画主游戏池*/
        bx = m_base_x + 4 * m_base_w;
        by = m_base_y - 1 * m_base_h;
        for (y = m_gamepool_h; y >= 1; --y) {
            for (x = 1; x <= m_gamepool_w; ++x) {
                if ( c = m_gamepool[y][x] )
                        putimage(bx + x * m_base_w, by + y * m_base_h,
                            m_base_w, m_base_h, m_pcb,
                            c * m_base_w, 0);
            }
        }
        /*画控制块*/
        if ( m_ctl_t >=0 ) {
            bx = m_base_x + (m_ctl_x + 4) * m_base_w;
            by = m_base_y + (m_ctl_y - 1) * m_base_h;
            draw44(bx, by, g_trs_map[m_ctl_s][m_ctl_t], m_ctl_dx, m_ctl_dy);
        }
        /*画下一块和下二块*/
        bx = m_base_x;
        by = m_base_y;
        draw44(bx, by, g_trs_map[m_next1_s][0]);
        bx = m_base_x;
        by = m_base_y + 5 * m_base_h;
        draw44(bx, by, g_trs_map[m_next2_s][0], 0, 0, 8);
        

        
        /*画当前分数*/
        setfont(MarkFont);
        setcolor(WHITE);
        xyprintf(148,25,"分数 %d",m_socre);
        setfont(DefaultFont);
        

	//	EmptyMsgQueue();/*清空鼠标消息队列*/
        setcolor(0xFFFFFF);
    }
    int dealbit(int a, float dt) {
        a = int(a * dt);
        if ( a>255 )    a = 255;
        else if ( a<0 ) a = 0;
        return a;
    }
     int getchangcolor(int Color, float t) {
        int r = EGEGET_R(Color), g = EGEGET_G(Color), b = EGEGET_B(Color);
        r = dealbit(r, t);
        g = dealbit(g, t);
        b = dealbit(b, t);
        return EGERGB(r, g, b);
    }



int QqsortScoreCmp(const void *a,const void *b){
	int temp; 
	temp = ((ScoreData *)b)->mark - ((ScoreData *)a)->mark; /*优先按照分数降序*/
	if(temp) 
		return temp;
	else /*否则按照时间由降序*/
		return strcmp(((ScoreData *)b)->date , ((ScoreData *)a)->date);
}

int ShowGlobalHighScore(char *seekname = NULL){
	FILE *fp;
	int bz[5][4]={ /*按钮区域*/
		{15,450,70,25}, /*上一页*/
		{100,450,70,25}, /*下一页*/
		{185,450,70,25}, /*返回第一页*/
		{270,450,100,25}, /*按名查找*/
		{143,487,100,25} /*返回菜单*/
	};
	int trycount=2500,n,i=0,j,currentpage=0;
	int mousex,mousey,currentslect,lastselect=-2,searchFlag=0;
	mouse_msg ch;
	ScoreData *sdata,*ssearch;
	char cmdbuff[255];
	
	setfont(30,0,"宋体");
	setcolor(BLACK);
	sprintf(cmdbuff,"start TetrisNet.exe 0 %s",FTP_SALT);
#ifdef IS_DEBUG
	printf("%s\n",cmdbuff);
#endif
	system(cmdbuff);
	while(trycount--){
		cleardevice();
		outtextxy(25,trycount/6.25,"国外服务器连接缓慢需要十几秒");
		outtextxy(25,400-trycount/6.25,"正在从服务器读取数据");
		delay_fps(120);
		BkColorSwift();
		if(trycount % 50 != 0) continue;
		if(fp = fopen("doing.txt","r"))
			{
				fclose(fp);
			}
		else break;
	}
	fclose(fp);
	if(!trycount) return 1;
#ifdef IS_DEBUG
	printf("trycount = %d\n",trycount);
#endif

	
	if((fp = fopen("ScoreOnline.txt","r"))==NULL)
		return 1; 
	cleardevice();
	fscanf(fp,"%d",&n);
	if(n<=0) return 1;
#ifdef IS_DEBUG
	printf("n = %d\n",n);
//	system("pause");
#endif
	
	sdata = (ScoreData *)malloc(sizeof(ScoreData)*(n+10));
	memset((void *)sdata,0,n+10);
	
	while(!feof(fp)){  /*读取记录文件*/
		fscanf(fp,"%d %s %s",&(sdata[i].mark),&(sdata[i].name),&(sdata[i].date));
//#ifdef IS_DEBUG
//	printf("%d  %s %s",(sdata[i].mark),(sdata[i]).name,(sdata[i]).date);
//	system("pause");
//#endif
		i++;
	}
	system("start TetrisNet.exe 1");
	setfont(20,0,"宋体");
//	xyprintf(5,500,"数据读取成功，共 %d 条记录",i);
//	delay_fps(5);
	
	fclose(fp);
	qsort((void *)sdata,i,sizeof(ScoreData),&QqsortScoreCmp);
	for(j=0;j<i;j++)
		sdata[j].no = j+1;

	EmptyMsgQueue();
	while(1){/*注意:这是一个硕大的while*/
		mousepos(&mousex,&mousey);
		currentslect = DotWithin(mousex,mousey,5,bz);
	 	if( seekname || mousemsg() 
			&& ( ch = getmouse()).is_left()  
			&& ch.is_up() )/*若鼠标左键单击*/
				{
			 		lastselect = -2;/*强制重绘*/
					switch(currentslect){
						case 0: if(currentpage>=10)  currentpage -= 10;  break;
						case 1: if(currentpage<i-10)  currentpage += 10;  break;
						case 2: currentpage = 0; searchFlag = 0;break;
						case 3: if(!seekname) /*若有外部查询则跳过输入*/
									inputbox_getline("请输入您的姓名","可输入全部或部分，区分大小写，回车确认。\n(只显示前10个结果)",
										cmdbuff,20);
								searchFlag = 0;
								ssearch = (ScoreData *)malloc(sizeof(ScoreData)*20);/*多分配10个防止溢出*/
								for(j=0;j<i && searchFlag<=10;j++){ /*搜索该名字*/
									if(strstr(sdata[j].name,cmdbuff))
										{
											memcpy(ssearch+searchFlag,sdata+j,sizeof(ScoreData));
											searchFlag++;
											#ifdef IS_DEBUG
											printf("No.%d  %d   %s   %s\n",sdata[i].no,sdata[i].mark,sdata[i].name,sdata[i].date);
											#endif	
										}
								}
							/*searchFlag=
							 *		   -1: 查询结果显示期间。
							 *				 此时hover按钮不刷新(也没有特效)
					 		 *				 仅在点击按钮后刷新 
							 *			0: 程序正常运行 
							 *		   >0: 表示搜索到的结果数，也用于标记刷新一次
							 **/
							if(searchFlag == 0)
							{
								 xyprintf(57,400,"搜索""%s""没有发现结果:(",cmdbuff);
								 delay_ms(2000);
							}
							seekname = NULL;
							break;
						case 4:return 0;
				}/*EndOfSwitch*/
				EmptyMsgQueue();
				
			}
			
		/*---------绘制菜单-------*/ 
		if(currentslect != lastselect && searchFlag!=-1)/*若鼠标hover状态没有发生改变则不重绘*/
		{
			cleardevice();
			lastselect = currentslect; 
			#ifdef IS_DEBUG
			printf("currentpage = %d\n",currentslect);
			#endif
			/*画按钮们*/
			if(currentpage>=10 && !searchFlag) /*首页与查询页不呈现上一页*/
			{
				drawframe(bz[0][0],bz[0][1],bz[0][2],bz[0][3],
				  	currentslect==0 ? hsl2rgb(bk_HSL_H+60,0.5,0.5) : getbkcolor());
		        outtextxy(bz[0][0]+3 , bz[0][1],"上一页");
			}
	
	        if(currentpage<i-10 && !searchFlag){ /*末页与查询页不呈现下一页*/
				drawframe(bz[1][0],bz[1][1],bz[1][2],bz[1][3],
				  	currentslect==1 ? hsl2rgb(bk_HSL_H+60,0.5,0.5) : getbkcolor());
		        outtextxy(bz[1][0]+3 , bz[1][1],"下一页");
	        }
	        
			drawframe(bz[2][0],bz[2][1],bz[2][2],bz[2][3],
	 			currentslect==2 ? hsl2rgb(bk_HSL_H+60,0.5,0.5) : getbkcolor());
	      	outtextxy(bz[2][0]+3 , bz[2][1],searchFlag?"回排行榜":"回第一页");
	      	
	      	if(!searchFlag){ /*查询页不显示*/
			drawframe(bz[3][0],bz[3][1],bz[3][2],bz[3][3],
	 			currentslect==3 ? hsl2rgb(bk_HSL_H+60,0.5,0.5) : getbkcolor());
	      	outtextxy(bz[3][0]+3 , bz[3][1],"按名字查找");
	      	
	      	xyprintf(57,400,"一共有 %d 个记录:)",i);
	      	}
			
			drawframe(bz[4][0],bz[4][1],bz[4][2],bz[4][3],
	 			currentslect==4 ? hsl2rgb(bk_HSL_H+60,0.5,0.5) : getbkcolor());
	      	outtextxy(bz[4][0]+3 , bz[4][1],"返回首页");
			
			if(!searchFlag)
				DisplayScoreList(sdata,currentpage,n);
			else
			{
				DisplayScoreList(ssearch,0,searchFlag);
				searchFlag = -1;/*此后暂时禁用hover重绘*/
				free(ssearch);
			}
		}
		/*背景渐变与延时*/
		BkColorSwift();
		delay_fps(nfps);
	}
	
}



/*画排行榜*/
void DisplayScoreList(ScoreData	*sdata,int start,int total){
	int x,y,i;

	setfont(23,0,"微软雅黑");
	outtextxy(20,20,"         分数        姓名            日期");
	for(i=start;i<start+10 && i<total;i++){
		xyprintf(20,60+(i-start)*35,"No.%d %4d %9s%22s",sdata[i].no,sdata[i].mark,sdata[i].name,sdata[i].date);
#ifdef IS_DEBUG
	printf("No.%d  %d   %s   %s\n",sdata[i].no,sdata[i].mark,sdata[i].name,sdata[i].date);
#endif
	}
}

int GameOverProcess(){
	
	int trycount=1600;	
	if(m_socre == 0) return 0;
	cleardevice();
	char *name = (char *)malloc(50*sizeof(char));
	char *cmdbuff = (char *)malloc(255*sizeof(char));
	int i;
	do{
		inputbox_getline("请输入您的大名!","您的分数会被记录并上传到服务器端\n不超过20个汉字\n不允许有空格、Tab和感叹号\n\n按回车确认",
		name,20);
	}while(strchr(name,' ') || strchr(name,'\t') || strchr(name,'\n') || strchr(name,'!') );
	EmptyMsgQueue();
	FILE *fp;
	time_t tme=time(NULL);
	fp = fopen("score.txt","a+"); 
	fprintf(fp,"%d %s %s\n",m_socre,name,ctime(&tme));
	fclose(fp);
	
	sprintf(cmdbuff,"start TetrisNet.exe %d %s %s",m_socre,name,FTP_SALT);
	system(cmdbuff);
#ifdef IS_DEBUG
	printf("%s\n",cmdbuff);
#endif
	setfont(34,0,"宋体");
	while(trycount--){
		cleardevice();
		outtextxy(30,trycount/4,"正在连接并上传分数记录");
		delay_fps(120);
		BkColorSwift();
	}
	EmptyMsgQueue();
	ShowGlobalHighScore();
	free(name);
	free(cmdbuff);
	return 1;
}

void AboutAndThanks(){
	
	cleardevice();
	setfont(35,0,"微软雅黑");
	drawframe(10,10,g_width-20,g_height-20,getbkcolor());
	outtextxy(20,20,"本程序由Aploium编写");
	outtextxy(20,80,"Logo的TETRIS使用了");
	outtextxy(20,110,"   dungeon字体");
	outtextxy(20,140,"菜单使用了Celestial字体");
	outtextxy(20,170,"InputBox部分的代码来源于网络");
	outtextxy(20,200,"网络使用了AHK语言,由自己编写");
	outtextxy(20,230,"   存储数据的服务器是我自己的");
	outtextxy(20,260,"本程序遵循GPLv2 license");
	outtextxy(20,290,"感谢您的使用");
	outtextxy(20,330,"欢迎访问我的个人网站:");
	setfont(45,0,"微软雅黑"); 
	outtextxy(50,390,"www.aploium.com");
	setfont(DefaultFont);
	EmptyMsgQueue();
	while(1){
		BkColorSwift();
		if(kbhit())  return;
		delay_fps(nfps);
	}
	
}

void EmptyMsgQueue(){
	while(mousemsg()) getmouse();
	//while(kbhit()) getch();
	while(kbmsg()) getkey();
}

//#ifdef IS_DEBUG
//int main(){
//#endif
#ifndef IS_DEBUG
int WinMain(){
#endif    
    int over_Flag=60;
    initgr();
    
   // fps ui_fps;
    setlocale(LC_ALL,"");
    m_base_x = 20;
    m_base_y = 20;
    m_droptime = nfps / 2;
    m_movxtime = 10;
    setrendermode(RENDER_MANUAL);
    InitGame(10, 20, 24, 24);
    while(1){
    	switch(InitWelcome()){
	    	case 1:    
				EmptyMsgQueue(); 
				for ( ; is_run(); delay_fps(nfps)) {

							
					        update();
					        render();
					        if(m_state == ST_OVER)	
						        {
						        	nfps = RAWFPS; 
						        	for(over_Flag=120;over_Flag>0;over_Flag--)
						        	{/*GameOver时方块灰化*/
						        	update();
						        	render();
									delay_fps(nfps/2);        							
	        						}
						        	//delay_ms(1000);
						        	
									GameOverProcess(); 
	        						InitGame(10, 20, 24, 24);
	        						EmptyMsgQueue();
									break;	
	        					}
								//else
							//		over_Flag++;	        
					    }
			    break;
 			case 2: ShowGlobalHighScore();EmptyMsgQueue();break;
 			case 3: AboutAndThanks();EmptyMsgQueue();break; 
	    }

    }
    

    

    return 0;
}

