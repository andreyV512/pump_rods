#include "FrameViewer.h"

FrameViewer::FrameViewer()
	: CommonViewer(&tchart, 1) 
    , tchart(backScreen)
{
	tchart.items.get<BarSeriesNoFixed>().SetColorBarHandler(this, &FrameViewer::GetColorBar);
	cursor.horizontalLine = false;
}

bool FrameViewer::GetColorBar(int zone, double &data, unsigned &color)
{
	data = buffer[zone];
	color = nominalColor;
	if(zone < deathZoneFirst) color = deathZoneColor;
	else if(zone > deathZoneSecond)color = deathZoneColor;
	else if(data > threshDefect) color = threshDefectColor;
	else if(data > threshSortDown) color = deathZoneColor;
	return zone < count;
}

void FrameViewer::operator()(TMouseWell &l)
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

void FrameViewer::operator()(TSize &l)
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
	//tchart.items.get<BarSeriesNoFixed>().dataLength = count;
	//tchart.items.get<Border<SortDown>>().color  = threshSortDownColor;
	//tchart.items.get<Border<Defect>>().color  = threshDefectColor;
	//tchart.items.get<Border<SortDown>>().value  = threshSortDown;
	//tchart.items.get<Border<Defect>>().value  = threshDefect;
    Graphics g(backScreen);
	SolidBrush solidBrush(Color((ARGB)BACK_GROUND));
	g.FillRectangle(&solidBrush, 0, 0, 10, l.Height);   
	g.FillRectangle(&solidBrush, 0, 0, l.Width, 29);
	chart->Draw(g);
}


