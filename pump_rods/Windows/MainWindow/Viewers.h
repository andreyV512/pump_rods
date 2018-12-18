#pragma once
#include "CommonWindow/CommonViewer.h"
#include "Graphics/Chart.hpp"
#include "Graphics/GridChart.h"
#include "Graphics/FixedGridSeries.h"
#include "Graphics/BarSeries.h"
#include "App\AppBase.h"
#include "Graphics\Borders.h"
#include "DataItem\DataItem.h"

class ViewerData: public CommonViewer
{
public:
	template<class T>struct Border: public HBorder{Border(Chart &c): HBorder(c){}};
public:
	typedef CommonViewer Parent;
	typedef ChartDraw<Chart, TL::MkTlst<
		NoOffsetLeftAxes
		, BottomAxesMeters
		, BarSeriesNoFixed
		, FixedGrid		
		, Border<SortDown>
		, Border<Defect>
	>::Result> TChart;
	TChart tchart;

	int currentOffset;
	int deathZoneFirst, deathZoneSecond;
	double threshSortDown, threshDefect;
	int threshSortDownColor, threshDefectColor, deathZoneColor, nominalColor;
	int result;
	double *inputData;
	int count;
	double buffer[DataItem::output_buffer_size];
	char status[DataItem::output_buffer_size];

	ViewerData();

	bool GetColorBar(int zone, double &data_, unsigned &color);

	//void operator()(TRButtonDown &);
	void operator()(TSize &);
};
