#pragma once
#include "CommonWindow\CommonViewer.h"
#include "Graphics/Chart.hpp"
#include "Graphics/GridChart.h"
#include "App\AppBase.h"
#include "Graphics/FixedGridSeries.h"
#include "Graphics/BarSeries.h"
#include "Graphics\Borders.h"

class FrameViewer: public CommonViewer
{
public:
	typedef CommonViewer Parent;
	template<class T>struct Border: public HBorder{Border(Chart &c): HBorder(c){}};
	typedef ChartDraw<Chart, TL::MkTlst<
		NoOffsetLeftAxes
		, BottomAxes//Meters
		, BarSeriesNoFixed
		, Grid		
		, Border<SortDown>
		, Border<Defect>
	>::Result> TChart;
	TChart tchart;

	double buffer[2 * 1024];
	int nominalColor	 ;
	int deathZoneFirst	 ;
	int deathZoneColor	 ;
	int deathZoneSecond	 ;
	double threshDefect	 ;
	int threshDefectColor;
	int threshSortDownColor;
	double threshSortDown   ;
	int count;
	int medianFiltreLength;
	int cutoffFrequency;
	double delta;

	FrameViewer();

	bool GetColorBar(int , double &, unsigned &);
	void operator()(TMouseWell &);
	void operator()(TSize &);
};


