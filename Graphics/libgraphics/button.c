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


UIState gUIState = { 0, 0, 0, 0 };

static bool inBox(double x0, double y0, double x1, double x2, double y1, double y2)
{
	return (x0 >= x1 && x0 <= x2 && y0 >= y1 && y0 <= y2);
}

static void DrawBox(double x, double y, double width, double height, char label[])
{
    MovePen(x, y);
    DrawLine(0, height);
    DrawLine(width, 0);
    DrawLine(0, -height);
    DrawLine(-width, 0);
	if( label && strlen(label)>0 ) {
		double tw = TextStringWidth(label);
		MovePen(x+(width-tw)/2,y+height/2-3);
		DrawTextString(label);
	}
}



int button(int id, double x, double y, double w, double h,char label[])
{
	static double SinkMagnitude = 5;
	double sinkx = 0, sinky = 0;

	if (inBox(gUIState.mousex, gUIState.mousey, x, x + w, y, y + h)) {
		SetPenColor("Red");
		if (gUIState.buttondown) {
			gUIState.clickeditem = id;
			sinkx =   SinkMagnitude;
			sinky = - SinkMagnitude;
		}
	}
	else
		SetPenColor("Blue");

	DrawBox(x+sinkx, y+sinky, w, h, label);

	if( gUIState.clickeditem==id && // must be clicked before
		!gUIState.buttondown) // but now mouse button is up
		return 1; 

	return 0;
}
