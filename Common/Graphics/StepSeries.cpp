#include "StepSeries.h"
#include "Graphics/Chart.h"

using namespace Gdiplus;

StepSeries::StepSeries(Chart &chart) : chart(chart), data(NULL), color(0xff0000ff) {}
//----------------------------------------------------------------------------------------------------------------------
void StepSeries::Draw()
{
  if(NULL != data)
  {
    Color col(color);
	Pen pen(col, 2);
	chart.g->SetClip(&Region(RectF(
		REAL(chart.rect.left + chart.offsetAxesLeft + 1)
		, REAL(chart.rect.top + chart.offsetAxesTop + 3)
		, REAL((chart.rect.right - chart.offsetAxesRight) - (chart.rect.left + chart.offsetAxesLeft) - 2)
		, REAL((chart.rect.bottom - chart.offsetAxesBottom) - (chart.rect.top + chart.offsetAxesTop) - 6)
		)),
       CombineModeReplace
     );
    double dY = (double) (chart.rect.bottom - chart.rect.top - chart.offsetAxesBottom - chart.offsetAxesTop) / (chart.maxAxesY - chart.minAxesY);

    double y = chart.rect.bottom - chart.offsetAxesBottom;
   
	int width = chart.rect.right - chart.rect.left - chart.offsetAxesRight - chart.offsetAxesLeft;
	double dX = (double)(width) / count;
	int x0 = chart.rect.left + chart.offsetAxesLeft;
	double x = x0;
	int last = 0;
	double minAxes = chart.minAxesY;
	for(int i = 0; i < count; ++i)
	{
		int tmp = int(y - (data[i] - minAxes) * dY);
		chart.g-> DrawLine(&pen, (int)x, tmp, int(x + dX), tmp);
		x += dX;
	}
	chart.g->SetClip(&Region());
  }
}
//-------------------------------------------------------------------------------------------------------------------
void StepSeries::SetData(double *d, int c, double min, double max)
{
	data = d;
	count = c;
	chart.minScaleX = min;
	chart.maxScaleX = max;
	chart.dScaleX = (max - min) / count;
}
//-------------------------------------------------------------------------------------------------------------


