#include "Borders.h"
//#include "App/AppBase.h"
using namespace Gdiplus;
//------------------------------------------------------------------
VBorder::VBorder(Chart &c) : chart(c), color(0xffffff00),  widthPen(2), value(0.0), dashStyle(DashStyleSolid)
{}
//-----------------------------------------------------------------
void VBorder::Draw()
{
	Color col(color);
	Pen pen(col, (Gdiplus::REAL)widthPen);
	pen.SetDashStyle((Gdiplus::DashStyle)dashStyle);
	double dX = (double)(chart.rect.right - chart.rect.left - chart.offsetAxesLeft - chart.offsetAxesRight) / (chart.maxAxesX - chart.minAxesX);

	double x =  chart.rect.left + chart.offsetAxesLeft + (value - chart.minAxesX - 0.5) * dX;

	int y_b = chart.rect.bottom - chart.offsetAxesBottom - 3;
	int y_t = chart.rect.top + chart.offsetAxesTop + 3;
	chart.g->DrawLine(&pen, (int)x, y_b, (int)x, y_t);
}
//------------------------------------------------------------------------
 void VBorder::SetCoordinate(POINTS &point)
 {
	 int min = chart.rect.left + chart.offsetAxesLeft;
	 int max = chart.rect.right - chart.offsetAxesRight;
	 if(point.x > min && point.x < max)
	 {
		value = (chart.maxAxesX - chart.minAxesX) * (point.x - min) / (max - min);
	 }
 }
//--------------------------------------------------------------------
HBorder::HBorder(Chart &c) : chart(c), color(0xffffff00), widthPen(2), value(0.0), dashStyle(DashStyleDash){}
//---------------------------------------------------------------------
void HBorder::Draw()
{
	Color col(color);
	Pen pen(col, (Gdiplus::REAL)widthPen);
	pen.SetDashStyle((Gdiplus::DashStyle)dashStyle);
	double dY = (double) (chart.rect.bottom - chart.rect.top - chart.offsetAxesBottom - chart.offsetAxesTop) / (chart.maxAxesY - chart.minAxesY);
    double y = chart.rect.bottom - chart.offsetAxesBottom - (value - chart.minAxesY) * dY;

	int y_b = chart.rect.left + chart.offsetAxesLeft + 3;
    int y_t = chart.rect.right - chart.offsetAxesRight - 3;

	chart.g->DrawLine(&pen, y_b, (int)y, y_t, (int)y);
}
