#include "CommonWindow\CommonViewer.h"
#include "Graphics/Chart.hpp"
#include "Graphics/FixedGridSeries.h"
#include "Graphics/GridChart.h"
#include "DataItem/DataItem.h"
#include "Graphics/EmptyLeftAxes.h"
#include "Graphics/BarSeries.h"

class ResultViewer: public CommonViewer
{
public:
	typedef CommonViewer Parent;
	typedef ChartDraw<Chart, TL::MkTlst<
		EmptyLeftAxes
		, BottomAxesMeters
		, BarSeriesNoFixed
		, VerticalFixedGrid		
	>::Result> TChart;
	TChart tchart;

	int count;
	DataItem::ResultData &viewerData;

	ResultViewer();

	bool GetColorBar(int zone, double &data, unsigned &color);
	bool Draw(TMouseMove &l, VGraphics &g);
};