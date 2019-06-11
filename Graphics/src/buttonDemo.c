#include "graphics.h"
#include "extgraph.h"
#include "genlib.h"
#include "simpio.h"
#include "conio.h"
#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>

#include <windows.h>
#include <olectl.h>
#include <mmsystem.h>
#include <wingdi.h>
#include <ole2.h>
#include <ocidl.h>
#include <winuser.h>

#include "button.h"
/*显示函数*/
void display();
/*鼠标消息回调函数*/
void MouseEventProcess(int x, int y, int button, int event);

void Main() /*仅初始化执行一次*/
{
	SetWindowSize(800, 600);
    InitGraphics();        	
	
	registerMouseEvent(MouseEventProcess);/*注册鼠标消息回调函数*/

	display();
}

void MouseEventProcess(int x, int y, int button, int event)
{
	/*擦除旧的*/
     SetEraseMode(TRUE);
	 display();

	 gUIState.mousex = ScaleXInches(x);/*pixels --> inches*/
	 gUIState.mousey = ScaleYInches(y);/*pixels --> inches*/

	 switch (event) {
	 case BUTTON_DOWN:
		 gUIState.buttondown = 1;
		 break;
	 case BUTTON_UP:
		 gUIState.buttondown = 0;
		 break;
	 }

	 /*画上新的*/
     SetEraseMode(FALSE);
	 display();
}


void display()
{
	static int show_more = 0;
	int x = 10, y = 500;
	int w = 70, h = 20;

	button(GenUIID(0), x,      y, w, h, "OK");
	button(GenUIID(0), x += 100, y, w, h, "Cancel");

	if (button(GenUIID(0), x += 100, y, w, h, "Quit"))
		exit(-1);

	if (button(GenUIID(0), x = 40, y -= 50, 120, h, "Show More Buttons"))
		show_more = 1;
	
	if (button(GenUIID(0), x = 180, y, 120, h, "Show Less Buttons"))
		show_more = 0;

	if( show_more ) {
		button(GenUIID(0), x = 10, y -= 50, w, h, "Click Me 1");
		button(GenUIID(0), x + 100, y, w, h, "Click Me 2");
		button(GenUIID(0), x + 200, y, w, h, "Click Me 3");
		button(GenUIID(0), x, y -= 50, w, h, "Click Me 4");
		button(GenUIID(0), x + 100, y, w, h, "Click Me 5");
		button(GenUIID(0), x + 200, y, w, h, "Click Me 6");

		if (button(GenUIID(0), 60, y -= 50, 150, h, "Click Me to Quit Demo"))
			exit(-1);
	}
}
