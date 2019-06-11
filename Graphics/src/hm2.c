#include "graphics.h"
#include "extgraph.h"
#include "genlib.h"
#include "simpio.h"
#include "random.h"
#include "strlib.h"
#include "conio.h"
#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
#include <windows.h>
#include <olectl.h>
#include <stdio.h>
#include <mmsystem.h>
#include <wingdi.h>
#include <ole2.h>
#include <ocidl.h>
#include <winuser.h>
#include "linkedlist.h"
#include <math.h>


double arc;
double ach;
double pi=3.1415926;
void forward(double x,double y,double distance);

void paint(double cx,double cy);

void Main()
{
	double cx,cy;
	InitGraphics();

	SetPenColor("GREEN");
	SetPenSize(3); 

    cx = GetWindowWidth() / 2;
    cy = GetWindowHeight() / 2;

	ach=0;
	paint(cx,cy);
	ach=20;
	paint(cx,cy);
	ach=40;
	paint(cx,cy);
	ach=60;
	paint(cx,cy);
	ach=80;
	paint(cx,cy);
	ach=100;
	paint(cx,cy);
	ach=120;
	paint(cx,cy);
	ach=140;
	paint(cx,cy);
	ach=160;
	paint(cx,cy);
	ach=180;
	paint(cx,cy);
	ach=200;
	paint(cx,cy);
	ach=220;
	paint(cx,cy);
	ach=240;
	paint(cx,cy);
	ach=260;
	paint(cx,cy);
	ach=280;
	paint(cx,cy);
	ach=300;
	paint(cx,cy);
	ach=320;
	paint(cx,cy);
	ach=340;
	paint(cx,cy);
	ach=360;
	paint(cx,cy);

}

void forward(double x,double y,double distance)
{
	MovePen(x,y);
	DrawLine(distance*cos(arc*pi/180),distance*sin(arc*pi/180));
 }



void paint(double cx,double cy)
{
	double x1,y1;
	arc=ach+30;
	forward(cx,cy,1.0);
	x1=cx; y1=cy;
	x1=x1+1.0*cos(arc*pi/180);
	y1=y1+1.0*sin(arc*pi/180);
	arc=ach+90;
	forward(x1,y1,1.0);
	x1=x1+1.0*cos(arc*pi/180);
	y1=y1+1.0*sin(arc*pi/180);
	arc=ach+150;
	forward(x1,y1,1.0);
	x1=x1+1.0*cos(arc*pi/180);
	y1=y1+1.0*sin(arc*pi/180);
	arc=ach+210;
	forward(x1,y1,1.0);
	x1=x1+1.0*cos(arc*pi/180);
	y1=y1+1.0*sin(arc*pi/180);
	arc=ach+270;
	forward(x1,y1,1.0);
	x1=x1+1.0*cos(arc*pi/180);
	y1=y1+1.0*sin(arc*pi/180);
	arc=ach+330;
	forward(x1,y1,1.0);
	x1=x1+1.0*cos(arc*pi/180);
	y1=y1+1.0*sin(arc*pi/180);
	arc=ach+90;
	forward(x1,y1,1.0);
	x1=x1+1.0*cos(arc*pi/180);
	y1=y1+1.0*sin(arc*pi/180);
	arc=ach+30;
	forward(x1,y1,1.0);
	arc=ach+150;
	forward(x1,y1,1.0);
	x1=cx;
	y1=cy;
}
