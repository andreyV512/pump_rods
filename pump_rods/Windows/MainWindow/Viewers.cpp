#include "Viewers.h"
#include "templates/templates.hpp"
#include "App/AppBase.h"
#include "window_tool/MenuAPI.h"
#include "DataItem/DataItem.h"

using namespace Gdiplus;

ViewerData::ViewerData()
	: CommonViewer(&tchart, 1) 
    , tchart(backScreen)
	, count(0)
	, currentOffset(1000)
{
	chart = &tchart;

	tchart.items.get<BarSeriesNoFixed>().SetColorBarHandler(this, &ViewerData::GetColorBar);
	cursor.horizontalLine = false;
}

void ViewerData::operator()(TSize &l)
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

	ThresholdsTable::TItems &ax = Singleton<ThresholdsTable>::Instance().items;
	tchart.items.get<BarSeriesNoFixed>().dataLength = count;
	tchart.items.get<Border<SortDown>>().color  = threshSortDownColor;
	tchart.items.get<Border<Defect>>().color  = threshDefectColor;
	tchart.items.get<Border<SortDown>>().value  = threshSortDown;
	tchart.items.get<Border<Defect>>().value  = threshDefect;
    Graphics g(backScreen);
	SolidBrush solidBrush(Color((ARGB)BACK_GROUND));
	g.FillRectangle(&solidBrush, 0, 0, 10, l.Height);   
	g.FillRectangle(&solidBrush, 0, 0, l.Width, 29);  
		
	chart->Draw(g);
}

bool ViewerData::GetColorBar(int zone, double &data, unsigned &color)
{
	data = buffer[zone];
	color = nominalColor;
	if(zone < deathZoneFirst) color = deathZoneColor;
	else if(zone > deathZoneSecond)color = deathZoneColor;
	else if(data > threshDefect) color = threshDefectColor;
	else if(data > threshSortDown) color = threshSortDownColor;
	return zone < count;
}

//void Chart_CoordCell(Chart &c, int mX, int &x, int delta)
// {
//	double left = c.rect.left + c.offsetAxesLeft;
//	x = int(delta * (mX - left)/(c.rect.right - c.offsetAxesRight - left));
//	if(x < 0) x = 0;
// }

//bool ViewerData::Draw(TMouseMove &l, VGraphics &g)
//{
//	int x;
//	Chart_CoordCell(*chart, l.x, x, count);
//	unsigned color = 0xff00ff00;
//
//	wsprintf(label.buffer, L"<ff>толщины зона <ff00>%d <%6x>%s %s"
//		, x
//		, color
//		, Wchar_from<double, 2>(buffer[x])()
//		, L"buf__0k"
//		);
//	label.Draw(g());
//	return true;
//}

//CONTEXT_MENU(ThickWindow)
void ViewerData::operator()(TRButtonDown &l)
{
	//if(Singleton<OnTheJobTable>::Instance().items.get<OnTheJob<Thick>>().value)
	//PopupMenu<ContextMenuThickWindow::items_list>::Do(l.hwnd, l.hwnd);
}

void ViewerData::operator()(TMouseWell &l)
{
	if(0 == l.delta) return;
	int offsMin = chart->rect.left + chart->offsetAxesLeft;
	storedMouseMove.x += l.delta < 0 ? 1: -1;
	if(offsMin >= storedMouseMove.x){storedMouseMove.x = offsMin;}
	else
	{
		int offsMax = chart->rect.right - chart->offsetAxesRight;
		if(offsMax <= storedMouseMove.x)storedMouseMove.x = storedMouseMove.x;
	}
	cursor.VerticalCursor(storedMouseMove, HDCGraphics(storedMouseMove.hwnd, backScreen));
}

void ViewerData::CoordCell(int mX, int &x, int delta)
{
	double left = chart->rect.left + chart->offsetAxesLeft;
	x = int(delta * (mX - left)/(chart->rect.right - chart->offsetAxesRight - left));
	if(x < 0) x = 0;
}

wchar_t *ViewerData::Mess(double val, int offs)
{
	if(offs < deathZoneFirst || offs > deathZoneSecond) return L"Мёртвая зона";
	if(val > threshDefect) return L"Брак";
	if(val > threshSortDown) return L"Сорт";
	return L"Норма";
}
