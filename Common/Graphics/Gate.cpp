#include "Gate.h"
#include "Graphics/Chart.h"
#include "tools_debug\DebugMess.h"

using namespace Gdiplus;
Gate::Gate(Chart &c)
	: chart(c)
	, color(0xff0000ff)
{
}
void Gate::Draw()
{
	Color col(color);
	Pen pen(col, 2);
	chart.g->SetClip(&Region(RectF(
		REAL(chart.rect.left + chart.offsetAxesLeft + 3)
		, REAL(chart.rect.top + chart.offsetAxesTop + 3)
		, REAL((chart.rect.right - chart.offsetAxesRight) - (chart.rect.left + chart.offsetAxesLeft) - 6)
		, REAL((chart.rect.bottom - chart.offsetAxesBottom) - (chart.rect.top + chart.offsetAxesTop) - 6)
		)),
       CombineModeReplace
     );
	double dX = (double)(chart.rect.right - chart.rect.left - chart.offsetAxesLeft - chart.offsetAxesRight) / (chart.maxAxesX - chart.minAxesX);

	double xx =  chart.rect.left + chart.offsetAxesLeft + (x - chart.minAxesX) * dX;

	double width__ = width * dX;

	double dY = (double) (chart.rect.bottom - chart.rect.top - chart.offsetAxesBottom - chart.offsetAxesTop) / (chart.maxAxesY - chart.minAxesY);
    double yy = chart.rect.bottom - chart.offsetAxesBottom - (y - chart.minAxesY) * dY;

	chart.g->DrawLine(&pen, (int)xx, (int)yy, (int)(xx + width__), (int)yy);
	chart.g->SetClip(&Region());
}
