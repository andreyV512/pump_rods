#include "CommonWindow\CommonViewer.h"
#include "Graphics/Chart.hpp"
#include "Graphics/FixedGridSeries.h"
#include "Graphics/GridChart.h"
#include "DataItem/DataItem.h"
#include "Graphics/EmptyLeftAxes.h"
#include "Graphics/Borders.h"

class ResultViewer: public CommonViewer
{
public:
	typedef CommonViewer Parent;
	typedef ChartDraw<Chart, TL::MkTlst<
		EmptyLeftAxes
		, BottomAxesGrid
		, FixedGridSeries
		, VerticalFixedGrid	
	>::Result> TChart;
	TChart tchart;

	DataItem::ResultData &viewerData;

	ResultViewer();

	bool GetColorBar(unsigned sensor, int zone, double &data, unsigned &color);
	bool Draw(TMouseMove &l, VGraphics &g);
};