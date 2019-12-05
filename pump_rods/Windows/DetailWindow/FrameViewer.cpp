#include "FrameViewer.h"

FrameViewer::FrameViewer()
	: CommonViewer(&tchart, 1) 
	, tchart(backScreen)
	, last(-1)
	, isBarGraph(true)
{
	tchart.items.get<BarSeries>().SetColorBarHandler(this, &FrameViewer::GetColorBar);
	tchart.items.get<LineSeries>().SetData(buffer, dimention_of(buffer));

	cursor.horizontalLine = false;
}

bool FrameViewer::GetColorBar(int zone_, double &data, unsigned &color)
{
	int zone = int((double)zone_ * dimention_of(buffer) / count);
	if(zone != last)
	{
		last = zone;
		data = buffer[zone];
		color = nominalColor;
		double delta = (tchart.maxAxesX - tchart.minAxesX) / dimention_of(buffer);

		int xzone = int(delta * zone + tchart.minAxesX);

		if(xzone < deathZoneFirst)color = deathZoneColor;
		else if(xzone > deathZoneSecond)color = deathZoneColor;
		else if(data > threshDefect) color = threshDefectColor;
		else if(data > threshSortDown) color = threshSortDownColor;
		if(tchart.maxAxesY < data) data = tchart.maxAxesY;
	}
	return zone < dimention_of(buffer);
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

	Graphics g(backScreen);
	SolidBrush solidBrush(Color((ARGB)BACK_GROUND));
	g.FillRectangle(&solidBrush, 0, 0, 10, l.Height);   
	g.FillRectangle(&solidBrush, 0, 0, l.Width, 29);
	if(isBarGraph)
	{
		tchart.DrawItems<chart_list0>(g);
	}
	else
	{
		tchart.DrawItems<chart_list1>(g);
	}
}

void CoordCell(Chart *chart, int mX, int &x, int delta)
{
	double left = chart->rect.left + chart->offsetAxesLeft;
	x = int(delta * (mX - left)/(chart->rect.right - chart->offsetAxesRight - left));
	if(x < 0) x = 0;
}

bool FrameViewer::DrawFrame(TMouseMove &l, VGraphics &g)
{
	if(0 == count) return false;
	int x;
	CoordCell(chart, l.x, x, dimention_of(buffer));
	double delta = (tchart.maxAxesX - tchart.minAxesX) / dimention_of(buffer);
	wsprintf(label.buffer, L"<ff>смещение %d %s"
		, int(delta * x + tchart.minAxesX)
		, Wchar_from<double, 2>(buffer[x])()
		);
	label.Draw(g());
	return true;
}


