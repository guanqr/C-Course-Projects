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

#define HouseHeight 3.2
#define HouseWidth 2.8
#define AtticHeight 2.8

#define DoorWidth 0.7
#define DoorHeight 2.5 
#define WindowWidth 0.6
#define WindowHeight 0.6


void DrawHouse(double x, double y);
void DrawOutline(double x, double y);
void DrawWindows(double x, double y);
void DrawDoor(double x, double y);
void DrawBox(double x, double y, double width, double height);
void DrawTriangle(double x, double y, double base, double height);
void DrawChimney(double x,double y);

void Main()
{
	double cx,cy;
	
	InitGraphics();
	SetPenSize(3); 
	cx=GetWindowWidth()/2;
	cy=GetWindowHeight()/2;
	DrawHouse(cx-HouseWidth/2,cy-(HouseHeight+AtticHeight)/2);
}

void DrawHouse(double x, double y)
{
	DrawOutline(x,y);
	DrawDoor(x+HouseWidth/2,y);
	DrawWindows(x+WindowWidth,y+DoorHeight-WindowHeight);
	DrawChimney(x,y);
}

void DrawOutline(double x,double y)
{
	DrawBox(x,y,HouseWidth,HouseHeight);
	DrawTriangle(x,y+HouseHeight,HouseWidth,AtticHeight);
}

void DrawDoor(double x, double y)
{
	DrawBox(x,y,DoorWidth,DoorHeight);
}

void DrawWindows(double x, double y)
{
	DrawBox(x,y,WindowWidth,WindowHeight);
}

void DrawChimney(double x,double y)
{
	MovePen(x+0.3,y+HouseHeight+0.6/HouseWidth*AtticHeight);
	DrawLine(0.0,1.2);
	MovePen(x+0.6,y+HouseHeight+1.2/HouseWidth*AtticHeight);
	DrawLine(0.0,1.2-0.6/HouseWidth*AtticHeight);
	DrawBox(x+0.2,y+HouseHeight+0.6/HouseWidth*AtticHeight+1.2,0.5,0.3);
}

void DrawBox(double x,double y,double width, double height)
{
	MovePen(x,y);
	DrawLine(0,height);
	DrawLine(width,0);
	DrawLine(0,-height);
	DrawLine(-width,0);
}

void DrawTriangle(double x,double y,double base,double height)
{
	MovePen(x,y);
	DrawLine(base,0);
	DrawLine(-base/2,height);
	DrawLine(-base/2,-height);
}