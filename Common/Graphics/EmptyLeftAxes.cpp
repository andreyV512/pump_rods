#include "Graphics/Chart.h"
#include "EmptyLeftAxes.h"

using namespace Gdiplus;
EmptyLeftAxes::EmptyLeftAxes(Chart &chart) 
 : chart(chart)
 , leftOffset(25)
{}
//-----------------------------------------------------------------------------------
void EmptyLeftAxes::Draw()
{
	Font font(L"Arial", (REAL)chart.fontHeight, FontStyleBold);
	Color color(chart.colorAxes);
	Pen pen(color, 2);
	double height;

	int maxLen = 0;
	chart.offsetAxesLeft = leftOffset;

	chart.GetCountDigit(chart.minAxesY, chart.maxAxesY, height, font, maxLen);
	maxLen += 2;
	int x = leftOffset;
	int bottom = chart.rect.bottom - chart.offsetAxesBottom;
	chart.g->DrawLine(&pen, x, chart.rect.top + chart.offsetAxesTop, x, bottom);
	double deltaTick = 0;
	double deltaDigit = 0;
	double digit = 0;
	double minTick = 0;
	deltaDigit = 1;
	digit = chart.minAxesY;
	minTick = 0;
	deltaTick = (chart.rect.bottom - chart.rect.top - chart.offsetAxesBottom - chart.offsetAxesTop) / (chart.maxAxesY - chart.minAxesY);
	double offs = chart.offsetGridY = chart.rect.bottom - chart.offsetAxesBottom + minTick;
	chart.deltaTickY = deltaTick;
	chart.deltaDigitY = deltaDigit;
	chart.offsetGridX = leftOffset + deltaTick;
	chart.offsetAxesLeft = leftOffset;
}