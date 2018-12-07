#include "CommonViewer.h"
#include "App/App.h"
#include "tools_debug\DebugMess.h"
#include "Graphics\Color.h"
#include "Graphics\FixedGridSeries.h"
#include "window_tool/MenuAPI.h"

using namespace Gdiplus;

CommonViewer::CommonViewer(Chart *c, const int &count_sensors)
	: backScreen(NULL)
	, count_sensors(count_sensors)
	, chart(c)
	, cursor(*chart)
	, currentX(0)
{}

CommonViewer::~CommonViewer()
{
	zprint("\n");
	delete backScreen;
}

void CommonViewer::operator()(TSize &l)
{
	if(l.resizing == SIZE_MINIMIZED || 0 == l.Width || 0 == l.Height) return;	
	
	if(NULL != backScreen)
	{
		if(backScreen->GetWidth() < l.Width || backScreen->GetHeight() < l.Height)
		{
			delete backScreen;
		    backScreen = new Bitmap(l.Width, l.Height);
		}
	}
	else if(l.Width > 0 && l.Height > 0)
	{
		backScreen = new Bitmap(l.Width, l.Height);
	}
	else
	{
		return;
	}
	chart->rect.right = l.Width;
	chart->rect.bottom = l.Height;
	

    Graphics g(backScreen);
	SolidBrush solidBrush(Color((ARGB)BACK_GROUND));
	g.FillRectangle(&solidBrush, 0, 0, 10, l.Height);   
	g.FillRectangle(&solidBrush, 0, 0, l.Width, 29);  
		
	chart->Draw(g);
}

void CommonViewer::operator()(TPaint &l)
{
	if(NULL == backScreen) return;
	PAINTSTRUCT p;
	HDC hdc = BeginPaint(l.hwnd, &p);
	{		
		Graphics g(hdc);		
		g.DrawCachedBitmap(&CachedBitmap(backScreen, &g), 0, 0);
		cursor.VerticalCursor(storedMouseMove, PaintGraphics(g));
	}
	EndPaint(l.hwnd, &p);
}

void CommonViewer::operator()(TMouseMove &l)
{
	if(mouseMove)
	{
		if(cursor.CrossCursor(l, HDCGraphics(l.hwnd, backScreen)))
		{
			storedMouseMove = l;
		}
	}
}

void CommonViewer::operator()(TLButtonDbClk &l)
{
  mouseMove = true;
}

void CommonViewer::operator()(TMouseWell &l)
{
	mouseMove = false;
		OffsetToPixel(
			*chart
			, storedMouseMove.x
			, storedMouseMove.y
			, l.delta / 120
			, true 
			);
		currentX -= l.delta / 120;
		//cursor.VerticalCursor(storedMouseMove, HDCGraphics(storedMouseMove.hwnd, backScreen));
		int currentY = 0;
		chart->CellCoord(storedMouseMove.x, storedMouseMove.y, currentX, currentY);
		cursor.CrossCursor(storedMouseMove, HDCGraphics(storedMouseMove.hwnd, backScreen));
}

void CommonViewer::operator()(TLButtonDown &l)
{
	mouseMove = false;
	storedMouseMove.x = l.x;
	storedMouseMove.y = l.y;
//	 
	int currentY = 0;
	chart->CoordCell(storedMouseMove.x, storedMouseMove.y, (int)currentX, currentY);
	chart->CellCoord(storedMouseMove.x, storedMouseMove.y, (int)currentX, currentY);
	cursor.CrossCursor(storedMouseMove, HDCGraphics(l.hwnd, backScreen)) ;
}

LRESULT CommonViewer::operator()(TCreate &l)
{
	storedMouseMove.hwnd = l.hwnd;
	currentX /*= currentY*/ = 0;

	chart->minAxesY = 0;
	chart->maxAxesY = 100;
	chart->minAxesX = 0;
	chart->maxAxesX = 1200;
	chart->rect.top = 17;
	mouseMove = true;
	label.fontHeight = 12;
	label.top = 0;

	return 0;
}

void CommonViewer::operator()(TCommand &m)
{
	EventDo(m);
}

