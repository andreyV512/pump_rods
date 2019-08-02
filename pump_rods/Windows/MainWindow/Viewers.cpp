#include "Viewers.h"
#include "templates/templates.hpp"
#include "App/AppBase.h"
#include "window_tool/MenuAPI.h"
#include "MessageText/StatusMessages.h"
#include "MessageText/ListMess.hpp"
#include "DLG/Dialogs.h"

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
	tchart.items.get<BarSeriesNoFixed>().dataLength = DataItem::output_buffer_size;
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
	color = StatusColor()(status[zone]);
	if(status[zone] == STATUS_ID(DeathZone) || data > chart->maxAxesY) data = chart->maxAxesY;
	return zone < count;
}

LRESULT ViewerData::operator()(TCreate &l)
{
	DragAcceptFiles(l.hwnd, TRUE);
	return(*(Parent *)this)(l);
}

void ViewerData::operator()(TDropFiles &l)
{
	wchar_t path[1024];
	DragQueryFile(l.hDrop,0, path, dimention_of(path));
	LoadDataFromFile(l.hwnd, path);
	DragFinish(l.hDrop);
}