#include <math.h>
#include <limits>
#include "Graphics/Chart.h"
#include "tools_debug\DebugMess.h"
#include "Graphics\Color.h"
using namespace Gdiplus;

Chart::Chart(Gdiplus::Bitmap *&backScreen)
	: offsetAxesLeft(52)
	, offsetAxesBottom(20)
	, offsetAxesTop(0)
	, offsetAxesRight(10)
	, minAxesX(0)
	, maxAxesX(1)
	, minAxesY(0)
	, maxAxesY(1)
	, minAxesYRight(-1.0)
	, maxAxesYRight(1.0)
	, colorAxes(COLOR_AXES)
	, colorFontAxes(COLOR_AXES_FONT)
	, fontHeight(7)
	, colorGrid(COLOR_GRID)
	, backScreen(backScreen)
	, dScaleX(1.0)
{
	memset(&rect, 0, sizeof(rect));
}
Chart::~Chart()
{
}
//---------------------------------------------------------------------
#pragma warning(disable : 4996)
//--------------------------------------------------------------------------------------
void OffsetAxes(int offs, int length, double min, double max, double &digit, double &tick, double &minDigit, double &minTick)
{
	if(0 == length) length = 1;
	double dLength = (double)length / offs;
	double dValue = (max - min) / dLength;
	if(0.0 >= dValue) dValue = 1.0;
	double coeff = 1.0;
	if(max == min) dValue = 1.0;

	if(dValue > 1.0)
	{
		while(dValue > 10.0f)
		{
			dValue *= 0.1f;
			coeff *= 10.0f;
		}		
	}
	else
	{
		while(dValue < 1.0f)
		{
			dValue *= 10.0f;
			coeff *= 0.1f;
		}
	}
	double k[] = {1.0, 2.0, 5.0};
	digit = k[0] * coeff;
	tick =  offs * k[0] / dValue;
	for(int i = 1; i < dimention_of(k); ++i)
	{
		double t = offs * k[i] / dValue;
		if(t < offs)
		{
			digit = k[i] * coeff;
			tick =  t;
		}
	}
	double rem = fmod(min, digit);
	if(rem >= 0.0)
	{
		double x = rem / digit;
		if(x > 0.9) rem = 0.0;
		minTick = tick * (1 - (digit - rem) / digit);
	}
	else
	{
		minTick = tick * (digit + rem) / digit;		
	}
	minDigit = min - minTick * digit / tick;	
}
#pragma warning(disable : 4996)
int Chart::GetCountDigit(double min, double max, double &height, Font &font, int &len)
{
	StringFormat format;
	PointF origin;
	RectF rect;
	char buf[32];
	wchar_t wbuf[32];
	gcvt(min, 3, buf);
	len = (int)strlen(buf);
	mbstowcs(wbuf, buf, 32);
	g->MeasureString(wbuf, (int)wcslen(wbuf), &font, origin, &format, &rect);
	int mn = (int)rect.Width; 
	gcvt(max, 3, buf);
	int lenMax = (int)strlen(buf);
	mbstowcs(wbuf, buf, 32);
	g->MeasureString(wbuf, (int)wcslen(wbuf), &font, origin, &format, &rect);
	int mx = (int)rect.Width; 
	height = rect.Height;
	if(len < lenMax) len = lenMax;
	return mx > mn ? mx : mn;
}
void Chart::Draw(Gdiplus::Graphics &graph)
{
	g = &graph;
	graph.FillRectangle(&SolidBrush(Color((ARGB)BACK_GROUND)), rect.left, rect.top, rect.right, rect.bottom);
}
//------------------------------------------------------------------------------
LeftAxes::LeftAxes(Chart &chart) 
 : chart(chart)
// , minA(100)
// , maxA(100)
{}
//-----------------------------------------------------------------------------------
void LeftAxes::Draw()
{
	Font font(L"Arial", (REAL)chart.fontHeight, FontStyleBold);
	Color color(chart.colorAxes);
	Pen pen(color, 2);
	SolidBrush fontColor(chart.colorFontAxes);
	StringFormat format;
	format.SetAlignment(StringAlignmentNear);
	double height;
	
	int maxLen = 0;
	chart.offsetAxesLeft = 10 + chart.GetCountDigit(chart.minAxesY, chart.maxAxesY, height, font, maxLen);
	maxLen += 2;
	int x = chart.rect.left + chart.offsetAxesLeft;
	int bottom = chart.rect.bottom - chart.offsetAxesBottom;
	chart.g->DrawLine(&pen, x, chart.rect.top + chart.offsetAxesTop, x, bottom);
	char buf[32];
	wchar_t wbuf[32];
	PointF origin;
	RectF rect;	
	double deltaTick = 0;
	double deltaDigit = 0;
	double digit = 0;
	double minTick = 0;
	OffsetAxes(
		int(height * 2)
		, chart.rect.bottom - chart.rect.top - chart.offsetAxesBottom - chart.offsetAxesTop
		, chart.minAxesY
		, chart.maxAxesY
		, deltaDigit
		, deltaTick
		, digit
		, minTick
		);
	double offs = chart.offsetGridY = chart.rect.bottom - chart.offsetAxesBottom + minTick;
	chart.deltaTickY = deltaTick;
	chart.deltaDigitY = deltaDigit;
	while(bottom < offs)
	{
		offs -= deltaTick;
		digit += deltaDigit;
	}
	origin.X = (REAL)chart.rect.left;
	int len;
	while(offs > chart.rect.top + chart.offsetAxesTop)
	{
		chart.g->DrawLine(&pen, (REAL)x - 5, (REAL)offs, (REAL)x, (REAL)offs);
		gcvt(digit, 5, buf);
		mbstowcs(wbuf, buf, 32);
		len = (int)wcslen(wbuf);
		if(len <= maxLen)
		{
			chart.g->MeasureString(wbuf, len, &font, origin, &format, &rect);
			origin.Y = REAL(offs - deltaTick + deltaTick/2);
			chart.g->DrawString(wbuf, len, &font, origin, &fontColor);
		}
		offs -= deltaTick;
		digit += deltaDigit;
	}	
}
//----------------------------------------------------------------------------------------------
BottomAxes::BottomAxes(Chart &chart)
: chart(chart) 
, minA(100)
, maxA(100)
{
}
void BottomAxes::Draw()
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
		int(1.5 * chart.GetCountDigit(minA, maxA, height, font, maxLen))
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
	double offs = chart.offsetGridX = chart.rect.left + chart.offsetAxesLeft - minTick + deltaTick;
	deltaTickDigit = deltaTick / deltaDigit;
	offsMin = chart.rect.left + chart.offsetAxesLeft;
	offsMax =  chart.rect.right - chart.offsetAxesRight;
    y = chart.rect.bottom - chart.offsetAxesBottom;
    digit += deltaDigit;
	origin.Y = REAL(y + 5);
    while(offs < offsMax)
    {		
		chart.g->DrawLine(&pen, int(offs), y, int(offs), y + 7);
		//gcvt(digit, 5, buf);
		//mbstowcs(wbuf, buf, 32);
		_itow((int)digit, wbuf, 10);
		size_t len = wcslen(wbuf);
		chart.g->MeasureString(wbuf, (int)len, &font, origin, &format, &rect);
		origin.X = (REAL)offs;
		chart.g->DrawString(wbuf, (int)len, &font, origin, &fontColor);

        offs += deltaTick;
        digit += deltaDigit;
    }
	maxA = digit;
	chart.g->SetClip(&Region());
}
//-----------------------------------------------------------------------------------------------------------------
void BottomAxes::OffsetToPixel(WORD &offs, int delta)
{
	int offsX = offs;
	double t = offs - offsMin - deltaTickDigit * delta;
	t /= deltaTickDigit;
	double tt = (int)(t + 0.5);

	tt *= deltaTickDigit;
	offs = (WORD)tt + offsMin;
	if(offsMin >= offs){offs = offsMin + 3; return;}
	if(offsMax <= offs){offs = offsMax - 3; return;}
	if(offsX == offs) ++offs;
}
//-----------------------------------------------------------------------------------------------------------------
BottomAxesMeters::BottomAxesMeters(Chart &chart)//, int zoneInMM)
: chart(chart)
//, zoneInMM(zoneInMM)
//, minA(100)
//, maxA(100)
, minBorder(0), maxBorder(10)
{
}
void BottomAxesMeters::Draw()
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
	chart.g->DrawLine(&pen, chart.offsetAxesLeft, y, chart.rect.right - chart.offsetAxesRight, y);

   // char buf[32];
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
		int(2.0 * chart.GetCountDigit(chart.minAxesX, chart.maxAxesX, height, font, maxLen))
		, chart.rect.right - chart.rect.left - chart.offsetAxesLeft - chart.offsetAxesRight
		, minBorder
		, maxBorder
		, deltaDigit
		, deltaTick
		, digit
		, minTick
		);
//	minA = digit;
    chart.deltaTickX = deltaTick;
	chart.deltaDigitX = deltaDigit;
	double offs = chart.offsetAxesLeft;
	deltaTickDigit = deltaTick / deltaDigit;
	offsMin = chart.rect.left + chart.offsetAxesLeft;
	offsMax =  chart.rect.right - chart.offsetAxesRight;
    y = chart.rect.bottom - chart.offsetAxesBottom;
    digit += deltaDigit;
	origin.Y = REAL(y + 5);
    while(offs < offsMax)
    {		
		chart.g->DrawLine(&pen, int(offs), y, int(offs), y + 7);
		//gcvt(digit, 5, buf);
		//mbstowcs(wbuf, buf, 32);
		//wsprintf(wbuf, L"%d", (int)digit);
		_itow((int)digit, wbuf, 10);
		size_t len = wcslen(wbuf);
		chart.g->MeasureString(wbuf, (int)len, &font, origin, &format, &rect);
		origin.X = REAL(offs + (deltaTick - rect.Width) / 2);
		chart.g->DrawString(wbuf, (int)len, &font, origin, &fontColor);

        offs += deltaTick;
        digit += deltaDigit;
    }
	//maxA = digit;
	chart.g->SetClip(&Region());
}
//--------------------------------------------------------
void CoordCell(Chart &chart, int mX, int &x, int delta)
{
	double left = chart.rect.left + chart.offsetAxesLeft;
	x = int(delta * (mX - left)/(chart.rect.right - chart.offsetAxesRight - left));
	if(x < 0) x = 0;
}
//-----------------------------------------------------------------------------------------------------------------
//void BottomAxesMeters::OffsetToPixel(WORD &offs, int delta)
//{
//	double deltaTickDigit_ = deltaTickDigit / zoneInMM;
//	int offsX = offs;
//	double t = offs - offsMin - deltaTickDigit_ * delta;
//	t /= deltaTickDigit_;
//	double tt = (int)(t + 0.5);
//
//	tt *= deltaTickDigit_;
//	offs = (WORD)tt + offsMin;
//	if(offsMin >= offs){offs = offsMin + 3; return;}
//	if(offsMax <= offs){offs = offsMax - 3; return;}
//	if(offsX == offs) ++offs;
//}
//-----------------------------------------------------------------------------------------------------------------
//------------------------------------------------------------------------------------------------------------------
BottomAxesInt::BottomAxesInt(Chart &chart)
: chart(chart) 
, minA(100)
, maxA(100)
{
}
void BottomAxesInt::Draw()
{
	Font font(L"Arial", (REAL)chart.fontHeight, FontStyleBold);
	Color color(chart.colorAxes);
	Pen pen(color, 2);
	SolidBrush fontColor(chart.colorFontAxes);
	StringFormat format;
	format.SetAlignment(StringAlignmentCenter);
	double height;	
    int y = chart.rect.bottom - chart.offsetAxesBottom;
	chart.g->DrawLine(&pen, chart.rect.left + chart.offsetAxesLeft, y, chart.rect.right - chart.offsetAxesRight, y);

    char buf[32];
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
		int(3.5 * chart.GetCountDigit(minA, maxA, height, font, maxLen))
		, chart.rect.right - chart.rect.left - chart.offsetAxesLeft - chart.offsetAxesRight
		, chart.minAxesX
		, chart.maxAxesX
		, deltaDigit
		, deltaTick
		, digit
		, minTick
		);
	minA = digit;
	if(deltaDigit < 1)
	{
		deltaTick /= deltaDigit;
		deltaDigit = 1;
	}
	chart.deltaTickX = deltaTick;
	chart.deltaDigitX = deltaDigit;
	double offs = chart.offsetGridX = chart.rect.left + chart.offsetAxesLeft - minTick + deltaTick;

	deltaTickDigit = deltaTick / deltaDigit;
	offsMin = chart.rect.left + chart.offsetAxesLeft;
	offsMax =  chart.rect.right - chart.offsetAxesRight;

    y = chart.rect.bottom - chart.offsetAxesBottom;
    digit += deltaDigit;
	origin.Y = REAL(y + 5);
    while(offs < chart.rect.right - chart.offsetAxesRight)
    {		
		chart.g->DrawLine(&pen, int(offs), y, int(offs), y + 7);
		gcvt(digit, 5, buf);
		mbstowcs(wbuf, buf, 32);
		size_t len = wcslen(wbuf);
		chart.g->MeasureString(wbuf, (int)len, &font, origin, &format, &rect);
		origin.X = (REAL)offs;
		chart.g->DrawString(wbuf, (int)len, &font, origin, &fontColor);

        offs += deltaTick;
        digit += deltaDigit;
    }
	maxA = digit;
}
//----------------------------------------------------------------------------------------------------------------
void BottomAxesInt::OffsetToPixel(WORD &offs, int delta)
{
	int offsX = offs;
	double t = offs - offsMin - deltaTickDigit * delta;
	t /= deltaTickDigit;
	double tt = (int)(t + 0.5);

	tt *= deltaTickDigit;
	offs = (WORD)tt + offsMin;
	if(offsMin >= offs){offs = offsMin + 3; return;}
	if(offsMax <= offs){offs = offsMax - 3; return;}
	if(offsX == offs) ++offs;
}
//----------------------------------------------------------------------------------------------------------------
void Grid::Draw()
{
	chart.g->SetClip(&Region(RectF(
		REAL(chart.rect.left + chart.offsetAxesLeft + 3)
		, REAL(chart.rect.top + chart.offsetAxesTop + 3)
		, REAL((chart.rect.right - chart.offsetAxesRight) - (chart.rect.left + chart.offsetAxesLeft) - 6)
		, REAL((chart.rect.bottom - chart.offsetAxesBottom) - (chart.rect.top + chart.offsetAxesTop) - 6)
		)),
       CombineModeReplace
     );
	Color color(chart.colorGrid);
	Pen pen(color, 1);
	pen.SetDashStyle(DashStyleDash);
    double offs = chart.offsetGridX;
    int y_b = chart.rect.bottom - chart.offsetAxesBottom - 3;
    int y_t = chart.rect.top + chart.offsetAxesTop + 3;
    while(offs < chart.rect.right - chart.offsetAxesRight - *deltaY * 0.2)
    {
		chart.g->DrawLine(&pen, (int)offs, y_b, (int)offs, y_t);
        offs += chart.deltaTickX;
    }
    offs = chart.offsetGridY;
    y_b = chart.rect.left + chart.offsetAxesLeft + 3;
    y_t = chart.rect.right - chart.offsetAxesRight - 3;
	int bottom = chart.rect.bottom - chart.offsetAxesBottom;
	while(bottom <= offs) offs -= *deltaY;
    while(offs > chart.rect.top + chart.offsetAxesTop + *deltaY * 0.2)
    {
		chart.g->DrawLine(&pen, y_b, (int)offs, y_t, (int)offs);
        offs -= *deltaY;
    }
	chart.g->SetClip(&Region());
}
//---------------------------------------------------------------------------------------------------------------------
LineSeries::LineSeries(Chart &chart) : chart(chart), data(NULL), color(0xff0000ff) {}
//----------------------------------------------------------------------------------------------------------------------
void LineSeries::Draw()
{
  if(NULL != data)
  {	
	  Color col(color);
		Pen pen(col, 2);
		chart.g->SetClip(&Region(RectF(
			REAL(chart.rect.left + chart.offsetAxesLeft + 1)
			, REAL(chart.rect.top + chart.offsetAxesTop + 1)
			, REAL((chart.rect.right - chart.offsetAxesRight) - (chart.rect.left + chart.offsetAxesLeft) - 2)
			, REAL((chart.rect.bottom - chart.offsetAxesBottom) - (chart.rect.top + chart.offsetAxesTop) - 2)
			)),
			CombineModeReplace
			);
		double dY = (double) (chart.rect.bottom - chart.rect.top - chart.offsetAxesBottom - chart.offsetAxesTop) / (chart.maxAxesY - chart.minAxesY);

		double yOffs = chart.rect.bottom - chart.offsetAxesBottom;

		int width = chart.rect.right - chart.rect.left - chart.offsetAxesRight - chart.offsetAxesLeft;
		double dX = (double)width / (count - 1);
		int x0 = chart.rect.left + chart.offsetAxesLeft;
		double minY = chart.minAxesY;
		int y0 = int(yOffs - (data[0] - minY) * dY);
		double x = x0;
		int y = y0;
		for(int i = 1; i < count; ++i)
		{
			x += dX;
			y = int(yOffs - (data[i] - minY) * dY);
			if(x0 != int(x) || y0 != y)
			{
				chart.g->DrawLine(&pen, x0, y0, (int)x, y);
				x0 = int(x);
				y0 = y;
			}
		}
		chart.g->SetClip(&Region());
  }
}
//-------------------------------------------------------------------------------------------------------------------
void LineSeries::SetData(double *d, int c, double min, double max)
{
	data = d;
	count = c;
	chart.minScaleX = min;
	chart.maxScaleX = max;
	chart.dScaleX = (max - min) / count;
}
//------------------------------------------------------------------------------
void LineSeries::SetData(double *d, int c)
{
	data = d;
	count = c;	
}
//-------------------------------------------------------------------------------------------------------------
Cursor::Cursor(Chart &chart) 
: chart(chart)
, oMove(NULL), ptrMove(NULL)
, horizontalLine(true)
{}	

bool Cursor::VerticalCursor(TMouseMove &l, VGraphics &g)
{
	double xx, xy;
	if(chart.AxesValues(l.x, l.y, xx, xy)	)
	{		
		Color col(COLOR_WHITE);
		Pen pen(col, 1);	
		int top = chart.rect.top + chart.offsetAxesTop + 3;
		int bottom = chart.rect.bottom - chart.offsetAxesBottom - 3;
      
        if(oMove&&ptrMove)(oMove->*ptrMove)(l, g);

		g().DrawLine(&pen, l.x, top, l.x, bottom);
		return true;
	}
	return false;
}
void Cursor::VerticalCursorNoTest(TMouseMove &l, VGraphics &g)
{
	Color col(COLOR_WHITE);
	Pen pen(col, 1);	
	int top = chart.rect.top + chart.offsetAxesTop + 3;
	int bottom = chart.rect.bottom - chart.offsetAxesBottom - 3;

	int left = chart.rect.left + chart.offsetAxesLeft;
	int right = chart.rect.right - chart.offsetAxesRight;

	if(l.x < left) l.x = right;
	else if(l.x > right) l.x = left;

	if(oMove&&ptrMove)(oMove->*ptrMove)(l, g);

	g().DrawLine(&pen, l.x, top, l.x, bottom);
}
bool Cursor::CrossCursor(TMouseMove &l, VGraphics &g)
{
	double xx, xy;
	if(chart.AxesValues(l.x, l.y, xx, xy)	)
	{		
		Color col(COLOR_WHITE);
		Pen pen(col, 1);	
		int left = chart.rect.left + chart.offsetAxesLeft + 3;
		int right = chart.rect.right - chart.offsetAxesRight - 3;
		int top = chart.rect.top + chart.offsetAxesTop + 3;
		int bottom = chart.rect.bottom - chart.offsetAxesBottom - 3;
      
        if(oMove&&ptrMove)(oMove->*ptrMove)(l, g);

		g().DrawLine(&pen, l.x, top, l.x, bottom);
		if(horizontalLine)g().DrawLine(&pen, left, l.y, right, l.y);
		return true;
	}
	return false;
}

void Cursor::Draw()
{
	int xx, xy;
	if(chart.ValuesAxes(x0, y0, xx, xy))
	{
        Color col(COLOR_BLUE);
		Pen pen(col, 1);	
		int left = chart.rect.left + chart.offsetAxesLeft + 3;
		int right = chart.rect.right - chart.offsetAxesRight - 3;
		int top = chart.rect.top + chart.offsetAxesTop + 3;
		int bottom = chart.rect.bottom - chart.offsetAxesBottom - 3;
		chart.g->DrawLine(&pen, xx, top, xx, bottom);
		chart.g->DrawLine(&pen, left, xy, right, xy);
	}
	if(chart.ValuesAxes(x1, y1, xx, xy))
	{
        Color col(COLOR_RED);
		Pen pen(col, 1);	
		int left = chart.rect.left + chart.offsetAxesLeft + 3;
		int right = chart.rect.right - chart.offsetAxesRight - 3;
		int top = chart.rect.top + chart.offsetAxesTop + 3;
		int bottom = chart.rect.bottom - chart.offsetAxesBottom - 3;
		chart.g->DrawLine(&pen, xx, top, xx, bottom);
		chart.g->DrawLine(&pen, left, xy, right, xy);
	}
}
//-------------------------------------------------------------------------------------------
bool Chart::AxesValues(int x, int y, double &dx, double &dy)
{	
	if(
         x >= rect.left + offsetAxesLeft
		 && x < rect.right - offsetAxesRight
		 && y >= rect.top + offsetAxesTop
		 && y < rect.bottom - offsetAxesBottom
		)
	{
		double dY = (maxAxesY - minAxesY) / (rect.bottom - rect.top - offsetAxesBottom - offsetAxesTop);
		dy = (rect.bottom - offsetAxesBottom - y) * dY + minAxesY;
		dy = floor(dy);
		double dX = (maxAxesX - minAxesX) / (rect.right - rect.left - offsetAxesLeft - offsetAxesRight);
		dx = (x - rect.left - offsetAxesLeft) * dX + minAxesX + dX;// / 2;
		return true;
	}
	return false;
}
int Chart::BetweenLeftRight(int x)
{
	return  x < rect.left + offsetAxesLeft ? -1 : x > rect.right - offsetAxesRight ? 1 : 0;
}
 bool Chart::ValuesAxes(double dx, double dy, int &x, int &y)
 {
	 dx -= minAxesX;
     double dX = (maxAxesX - minAxesX) / (rect.right - rect.left - offsetAxesLeft - offsetAxesRight);
	 dx /= dX;
	 x = int(dx + rect.left + offsetAxesLeft);
	 dy -= minAxesY;
     double dY = (maxAxesY - minAxesY) / (rect.bottom - rect.top - offsetAxesBottom - offsetAxesTop);
	 dy /= dY;
	 y = int(rect.bottom - offsetAxesBottom - dy);

     return  x > rect.left + offsetAxesLeft && x < rect.right - offsetAxesRight 
		  && y > rect.top + offsetAxesTop && y < rect.bottom - offsetAxesBottom;
 }
 //----------------------------------------------------------------------------------------------
 void Chart::Restore(HWND hwnd)
 {
	 maxAxesY = maxScaleY;
	 minAxesY = minScaleY;

	 maxAxesX = maxScaleX;
	 minAxesX = minScaleX;
	 Graphics g(backScreen);
	 Draw(g);
	 InvalidateRect(hwnd, NULL, true);
 }
 //------------------------------------------------------------------------------
 void Chart::OffsetToPixelHorizontal(WORD &offsX, int delta)
 {
	 double dX = (double)(rect.right - rect.left - offsetAxesLeft - offsetAxesRight) / (maxAxesX - minAxesX);
	 int offsMin = rect.left + offsetAxesLeft;
	 double t = offsX - offsMin - dX * delta;
	 t /= dX;
	 double tt = (int)(t + 0.1);
	 tt *= dX;
	 tt += dX / 2;
	 offsX = (WORD)tt + offsMin;
	 if(offsMin >= offsX){offsX = offsMin + 3; return;}
	 int offsMax = rect.right - offsetAxesRight;
	 if(offsMax <= offsX)offsX = offsMax - 3;
 }
 void Chart::OffsetToPixelVertical(WORD &offsY, int delta)
 {
	double dY = (double)(rect.bottom - rect.top - offsetAxesTop - offsetAxesBottom) / (maxAxesY - minAxesY);
	int offsMin = rect.top + offsetAxesTop;
	double t = offsY - offsMin + dY * delta;
	t /= dY;
	double tt = (int)(t + 0.1);
	tt *= dY;
	tt += dY / 2;
	offsY = (WORD)tt + offsMin;
	if(offsMin >= offsY){offsY = offsMin + 3; return;}
	int offsMax = rect.bottom - offsetAxesBottom;
	if(offsMax <= offsY)offsY = WORD(offsMax - dY / 2);
}
 //--------------------------------------------------------------------
 void Chart::CoordCell(int mX, int mY, int &x, int &y)
 {
	double left = rect.left + offsetAxesLeft;
	double bottom = rect.bottom - offsetAxesBottom;
	x = int((mX - left) / deltaTickX * deltaDigitX);
	y = int((bottom - mY) / deltaTickY * deltaDigitY);
	if(x < 0) x = 0;
	if(y < 0) y = 0;
 }
 //---------------------------------------------------------------------------------------------
 void Chart::CellCoord(WORD &mX, WORD &mY, int x, int y)
 {
	 double left = rect.left + offsetAxesLeft;
	 double bottom = rect.bottom - offsetAxesBottom;
	 double dX = deltaTickX / deltaDigitX;
	 double dY = deltaTickY / deltaDigitY;
	 mX = int(deltaTickX * x / deltaDigitX + left + 0.5 * dX);
	 mY = int(bottom - deltaTickY * y / deltaDigitY - 0.5 * dY);
 }
 //---------------------------------------------------------------------------------------
