#include "TopLabelViewer.h"
#include "tools_debug\DebugMess.h"
#include "window_tool\Emptywindow.h"
#include "Graphics\Color.h"
//------------------------------------------------------------------------------------------------------
using namespace Gdiplus;

TopLabelViewer::TopLabelViewer()
	: backScreen(NULL)
{
	label.fontHeight = 16;
	label.top = 0;	
}
TopLabelViewer::~TopLabelViewer(){delete backScreen;}
//----------------------------------------------------------------------------------------------------
LRESULT TopLabelViewer::operator()(TSize &l)
{
	if(l.resizing == SIZE_MINIMIZED) return 0;	
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
		return 0;
	}
    Graphics g(backScreen);
	g.FillRectangle(&SolidBrush(Color((ARGB)BACK_GROUND)), 0, 0, l.Width, l.Height);   
	label.Draw(g);
	return 0;
}
//----------------------------------------------------------------------------------------------------
LRESULT TopLabelViewer::operator()(TPaint &l)
{
	PAINTSTRUCT p;
	HDC hdc = BeginPaint(l.hwnd, &p);
	{		
		Graphics g(hdc);		
		g.DrawCachedBitmap(&CachedBitmap(backScreen, &g), 0, 0);
	}
	EndPaint(l.hwnd, &p);
	return 0;
}
//---------------------------------------------------------------------------------------------------
LRESULT TopLabelViewer::operator()(TUser &l)
{
	label = (wchar_t *)l.data;
	Graphics g(backScreen);
	label.Draw(g);
	RepaintWindow(hWnd);
	return 0;
}
//---------------------------------------------------------------------------------------------------
void TopLabelViewer::SetMessage(wchar_t *text)
{
	SendMessage(hWnd, WM_USER, 0, (LPARAM)text);
}
//----------------------------------------------------------------------------------------------------
void TopLabelViewer::operator()(TDestroy &m)
{
	SetWindowLongPtr(m.hwnd, GWLP_USERDATA, NULL);
}
//------------------------------------------------------------------------------------------------

