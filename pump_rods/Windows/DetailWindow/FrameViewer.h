﻿#pragma once
#include "CommonWindow\CommonViewer.h"
#include "Graphics/Chart.hpp"
#include "Graphics/GridChart.h"
#include "App\AppBase.h"
#include "Graphics/FixedGridSeries.h"
#include "Graphics/BarSeries.h"
#include "Graphics\Borders.h"



class FrameViewer: public CommonViewer
{
	int last;
public:
	typedef CommonViewer Parent;
	template<class T>struct Border: HBorder{Border(Chart &c): HBorder(c){}};
	template<class T>struct BorderDown: HBorder
	{
		BorderDown(Chart &c): HBorder(c){}
		void Draw();
		//{
		//	//HBorder::value = -value;
		//	value = (*(TChart *)&chart).items.get<Border<T>>().value;
		//	HBorder::Draw();
		//}
	};

	typedef TL::MkTlst<
		NoOffsetLeftAxes
		, BottomAxes
		, BarSeries
		, Grid	
		, Border<SortDown>
		, Border<Defect>
	>::Result chart_list0;

	typedef TL::MkTlst<
		NoOffsetLeftAxes
		, BottomAxes
		, LineSeries
		, Grid	
		, Border<SortDown>
		, Border<Defect>
		, BorderDown<SortDown>
		, BorderDown<Defect>
	>::Result chart_list1;

	typedef ChartDraw<Chart, TL::MkTlst<
		NoOffsetLeftAxes
		, BottomAxes
		, BarSeries
		, LineSeries
		, Grid	
		, Border<SortDown>
		, Border<Defect>
		, BorderDown<SortDown>
		, BorderDown<Defect>
	>::Result> TChart;	
	TChart tchart;

	

	double buffer[1024];
	double status[1024];

	int nominalColor	 ;
	int deathZoneFirst	 ;
	int deathZoneColor	 ;
	int deathZoneSecond	 ;
	int threshDefect	 ;	
	int threshSortDown;
	int threshDefectColor;
	int threshSortDownColor;
	int count;
	int medianFiltreWidth;
	int cutoffFrequency;
	bool cutoffFrequencyON;
	bool medianFiltreON;
	double delta;
	double koef;
	bool isBarGraph;

	FrameViewer();

	bool GetColorBar(int , double &, unsigned &);
	void operator()(TMouseWell &);
	void operator()(TSize &);
	bool DrawFrame(TMouseMove &, VGraphics &);
};

template<class T>void FrameViewer::BorderDown<T>::Draw()
{
	Border<T>  &b = (*(TChart *)&chart).items.get<Border<T>>();
	value = -b.value;
	color = b.color;
	HBorder::Draw();
}


