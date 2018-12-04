#include "GridChart.h"
#include "tools_debug\DebugMess.h"

using namespace Gdiplus;
//------------------------------------------------------------------------
LeftAxesGrid::LeftAxesGrid(Chart &chart) 
 : chart(chart)
 , minA(100)
 , maxA(100)
{}
//-----------------------------------------------------------------------
#pragma warning(disable : 4996)
void LeftAxesGrid::Draw()
{
	Font font(L"Arial", (REAL)chart.fontHeight, FontStyleBold);
	Color color(chart.colorAxes);
	Pen pen(color, 2);
	SolidBrush fontColor(chart.colorFontAxes);
	StringFormat format;
	format.SetAlignment(StringAlignmentNear);
	double height;
	double minAA = ((int)minA * 1000) / 1000;
	double maxAA = ((int)maxA * 1000) / 1000;
	int maxLen = 0;
	chart.offsetAxesLeft = 7 + chart.GetCountDigit(minAA, maxAA, height, font, maxLen);
	int x = chart.rect.left + chart.offsetAxesLeft;
	int bottom = chart.rect.bottom - chart.offsetAxesBottom;
	chart.g->DrawLine(&pen, x, chart.rect.top + chart.offsetAxesTop, x, bottom);
 //   char buf[32];
	wchar_t wbuf[32];
	PointF origin;
	RectF rect;	
	double deltaTick = 0;
	double deltaDigit = 0;
	double digit = 0;
	double minTick = 0;
	OffsetAxes(
		int(height * 2.5)
		, chart.rect.bottom - chart.rect.top - chart.offsetAxesBottom - chart.offsetAxesTop
		, chart.minAxesY
		, chart.maxAxesY
		, deltaDigit
		, deltaTick
		, digit
		, minTick
		);
	minA = digit;
	double offs = chart.offsetGridY = chart.rect.bottom - chart.offsetAxesBottom + minTick;	
	chart.deltaDigitY = deltaDigit;
	chart.deltaTickY = deltaTick;
	while(bottom < offs)
	{
		offs -= deltaTick;
		digit += deltaDigit;
	}
	origin.X = (REAL)chart.rect.left;
	int len;
	digit += deltaDigit;
	int int_d = 0;
	while(offs > chart.rect.top + chart.offsetAxesTop + deltaTick / 2)
	{
		chart.g->DrawLine(&pen, (REAL)x - 5, (REAL)offs, (REAL)x, (REAL)offs);
		_itow((int)digit, wbuf, 10);
		len = (int)wcslen(wbuf);
		chart.g->MeasureString(wbuf, len, &font, origin, &format, &rect);
		origin.Y = REAL(offs  - deltaTick);
		chart.g->DrawString(wbuf, len, &font, origin, &fontColor);
		offs -= deltaTick;
		digit += deltaDigit;
	}	
	maxA = digit;
}
//----------------------------------------------------------------------------------------------
BottomAxesGrid::BottomAxesGrid(Chart &chart)
: chart(chart) 
, minA(100)
, maxA(100)
{
}
void BottomAxesGrid::Draw()
{
	int y = chart.rect.bottom - chart.offsetAxesBottom;
	chart.g->SetClip(&Region(RectF(
		REAL(chart.rect.left + chart.offsetAxesLeft - 3)
		, REAL(y - 3)
		, REAL((chart.rect.right - chart.offsetAxesRight) - (chart.rect.left + chart.offsetAxesLeft) + 3)
		, REAL(chart.rect.bottom)
		)),
       CombineModeReplace
     );	
	Font font(L"Arial", (REAL)chart.fontHeight, FontStyleBold);
	Color color(chart.colorAxes);
	Pen pen(color, 2);
	SolidBrush fontColor(chart.colorFontAxes);
	StringFormat format;
	format.SetAlignment(StringAlignmentCenter);
	double height;	
   
	chart.g->DrawLine(&pen, chart.rect.left + chart.offsetAxesLeft, y, chart.rect.right - chart.offsetAxesRight, y);

	wchar_t wbuf[32];
	PointF origin;
	RectF rect;	
    double length = chart.rect.right - chart.rect.left - chart.offsetAxesLeft - chart.offsetAxesRight;
    double deltaTick = 0;
	double deltaDigit = 0;
    double digit = 0;
    double minTick = 0;
    int maxLen = 0;
	OffsetAxes(
		int(5.0 * chart.GetCountDigit(minA, maxA, height, font, maxLen))
		, chart.rect.right - chart.rect.left - chart.offsetAxesLeft - chart.offsetAxesRight
		, chart.minAxesX
		, chart.maxAxesX
		, deltaDigit
		, deltaTick
		, digit
		, minTick
		);
	minA = digit;
    chart.deltaTickX = deltaTick;
	chart.deltaDigitX = deltaDigit;
	double offs = chart.offsetAxesLeft;
    y = chart.rect.bottom - chart.offsetAxesBottom;
    digit += deltaDigit;
	origin.Y = REAL(y + 2);
	chart.offsetGridX = chart.rect.left + chart.offsetAxesLeft - minTick + deltaTick;
    while(offs < chart.rect.right - chart.offsetAxesRight)
    {		
		chart.g->DrawLine(&pen, int(offs), y, int(offs), y + 7);
		_itow((int)digit, wbuf, 10);
		size_t len = wcslen(wbuf);
		chart.g->MeasureString(wbuf, (int)len, &font, origin, &format, &rect);
		origin.X = (REAL)(offs + (deltaTick - rect.Width) / 2);
		chart.g->DrawString(wbuf, (int)len, &font, origin, &fontColor);

        offs += deltaTick;
        digit += deltaDigit;
    }
	//maxA = digit;
	chart.g->SetClip(&Region());
}
//--------------------------------------------------------------------------
void GridGrid::Draw()
{
	Color color(chart.colorGrid);
	Pen pen(color, 1);
    double offs = chart.offsetGridX;
    int y_b = chart.rect.bottom - chart.offsetAxesBottom - 3;
    int y_t = chart.rect.top + chart.offsetAxesTop + 3;
	double dx = chart.deltaTickX / chart.deltaDigitX;
    while(offs < chart.rect.right - chart.offsetAxesRight)
    {
		chart.g->DrawLine(&pen, (int)offs, y_b, (int)offs, y_t);
        offs += dx;
    }
    offs = chart.offsetGridY;
    y_b = chart.rect.left + chart.offsetAxesLeft + 3;
    y_t = chart.rect.right - chart.offsetAxesRight - 3;
	int bottom = chart.rect.bottom - chart.offsetAxesBottom;
	double dy = chart.deltaTickY / chart.deltaDigitY;
    while(offs > chart.rect.top + chart.offsetAxesTop + chart.deltaTickY * 0.2)
    {
		chart.g->DrawLine(&pen, y_b, (int)offs, y_t, (int)offs);
        offs -= dy;
    }
}
//----------------------------------------------------------
GridSeries::GridSeries(Chart &chart) 
	: chart(chart)
	, sensorCount(10)
{
	SetColorCellHandler(this, &GridSeries::GetColorCellDefault);
}
//----------------------------------------------------------
void GridSeries::Draw()
{
	chart.g->SetClip(&Region(RectF(
		REAL(chart.rect.left + chart.offsetAxesLeft + 3)
		, REAL(chart.rect.top + chart.offsetAxesTop + 3)
		, REAL((chart.rect.right - chart.offsetAxesRight) - (chart.rect.left + chart.offsetAxesLeft) - 6)
		, REAL((chart.rect.bottom - chart.offsetAxesBottom) - (chart.rect.top + chart.offsetAxesTop) - 6)
		)),
       CombineModeReplace
     );	
	double dx = chart.deltaTickX / chart.deltaDigitX;
	double dy = chart.deltaTickY / chart.deltaDigitY;
	double left = chart.rect.left + chart.offsetAxesLeft;
	double bottom = chart.rect.bottom - chart.offsetAxesBottom;
	double x;
	double y = bottom - dy;
	unsigned color;
	double data;
	for(unsigned i = 0; i < sensorCount; ++i)
	{
		x = left;
		for(unsigned j  = 0; j < 60 && (obj->*SetColorBar)(i, j, data, color); ++j)
		{
			chart.g->FillRectangle(&SolidBrush(color), (REAL)x, (REAL)y, (REAL)dx, (REAL)dy);
			x += dx;
		}
		y -= dy;
	}
	chart.g->SetClip(&Region());
}
//----------------------------------------------------------
bool GridSeries::GetColorCellDefault(unsigned sensor, unsigned zone, double &data, unsigned &color)
{
	data = 10.0 / 10 * sensor;
	color = 0xffff0000;
	if(data > 4.0) {color = 0xff00ff00; return zone < 33;}
	if(data > 2.0) color = 0xffffff00;
	return zone < 33;
}
//----------------------------------------------------------------------------

