#pragma once
#include "CommonWindow/CommonViewer.h"
#include "Graphics/Chart.hpp"
#include "Graphics/GridChart.h"
#include "Graphics/FixedGridSeries.h"
#include "Graphics/BarSeries.h"
#include "App\AppBase.h"
#include "Graphics\Borders.h"

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

	int count;
	//int lengthTube;
	int currentOffset;
	int deathZoneFirst, deathZoneSecond;
	double threshSortDown, threshDefect;
	int threshSortDownColor, threshDefectColor, deathZoneColor, nominalColor;
	int result;
	double *inputData;
	double buffer[2000];

	ViewerData();

	bool GetColorBar(int zone, double &data_, unsigned &color);

	void operator()(TRButtonDown &);
	void operator()(TSize &);

	void operator()(TMouseWell &);

	void CoordCell(int mX, int &x, int delta);
	wchar_t *Mess(double val, int offs);
};
