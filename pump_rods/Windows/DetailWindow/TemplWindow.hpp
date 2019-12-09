#pragma once
#include "window_tool/message.h"
#include "Windows\MainWindow\MainWindow.h"
#include "FrameViewer.h"
#include "TemplDlg.hpp"
#include "App/App.h"

template<template<class>class W>struct __main_window_viewer__;
template<>struct __main_window_viewer__<StructSig>
{
	typedef  MainWindow::StructureViewer Result;
};
template<>struct __main_window_viewer__<DefectSig>
{
	typedef  MainWindow::DefectoscopeViewer Result;
};

template<template<class>class W>class TemplWindow
{
public:
	typedef typename __main_window_viewer__<W>::Result TViewer;
	class Viewer: public TViewer
	{
	public:
		typedef TViewer Parent; 
		TemplWindow *owner;
		void operator()(TRButtonDown &){};
		bool Draw(TMouseMove &, VGraphics &);
	};

	typedef typename TL::MkTlst<
		Viewer
		, FrameViewer
	>::Result viewers_list;
	TL::Factory<viewers_list> viewers;
	HWND hWnd;
	int offs;
	LRESULT operator()(TCreate &);
	void operator()(TDestroy &);
	void operator()(TSize &);
	void operator()(TCommand &);
	void operator()(TMouseWell &);
	void operator()(TClose &);

	void ChangeFrame(int);
};

template<template<class>class W>bool TemplWindow<W>::Viewer::Draw(TMouseMove &l, VGraphics &g)
{
	if(count > 0)
	{
		Parent::Draw(l, g);
		int x;
		CoordCell(tchart, l.x, x, DataItem::output_buffer_size);
		W<DataItem::Buffer> &item = Singleton<W<DataItem::Buffer>>::Instance();
		owner->offs = int((double)x * item.secondOffset / DataItem::output_buffer_size);
		owner->ChangeFrame(owner->offs);	
	}
	return true;
}

template<template<class>class, class>struct __param_filtre__
{
	int operator()(){return 0;}
};

template<class P>struct __param_filtre__<DefectSig, P>
{
	typename DefectSig<P>::type_value operator()(){return Singleton<AnalogFilterTable>::Instance().items.get<DefectSig<P>>().value;}
};

template<template<class>class W>LRESULT TemplWindow<W>::operator()(TCreate &m)
{
	Menu<TemplDlg::Menu<W>::menu_list>().Init(m.hwnd);
	W<DataItem::Buffer> &item = Singleton<W<DataItem::Buffer>>::Instance();
	ColorTable::TItems &color = Singleton<ColorTable>::Instance().items;
	ViewerCountTable::TItems &viewerCount = Singleton<ViewerCountTable>::Instance().items;

	Viewer &viewer = viewers.get<Viewer>();
	viewer.owner = this;
	memmove(viewer.buffer, item.outputData, sizeof(viewer.buffer));
	memmove(viewer.status, item.status, sizeof(viewer.status));

	viewer. deathZoneFirst = item.deathZoneFirst;
	viewer.deathZoneSecond = item.deathZoneSecond;
	viewer.threshSortDown = item.threshSortDown; 
	viewer.threshDefect = item.threshDefect;
	viewer.result = item.result;
	viewer.tchart.maxAxesX = item.secondOffset;
	viewer.currentOffset = item.secondOffset;
	viewer.inputData = item.inputData;
	viewer.tchart.items.get<BottomAxesMeters>().maxBorder = Singleton<DeadAreaTable>::Instance().items.get<RodLenght>().value;	
	viewer.threshSortDownColor = color.get<Clr<SortDown>>().value;
	viewer.threshDefectColor = color.get<Clr<Defect>>().value;
	viewer.deathZoneColor = color.get<Clr<DeathZone>>().value;
	viewer.nominalColor = color.get<Clr<Nominal>>().value;
	viewer.cursor.SetMouseMoveHandler(&viewer, & DetailWindow<W>::Viewer::Draw);
	viewer.count = DataItem::output_buffer_size;
	viewer.chart->maxAxesY = Singleton<AxesGraphsTable>::Instance().items.get<W<Axes>>().value;
	viewer.structMinVal = item.structMinVal;

	FrameViewer &frame =  viewers.get<FrameViewer>();
	frame.count = Singleton<ViewerCountTable>::Instance().items.get<W<ViewerCount>>().value;
	frame.medianFiltreWidth = Singleton<MedianFiltreTable>::Instance().items.get<W<MedianFiltreWidth>>().value;
	frame.medianFiltreON =  Singleton<MedianFiltreTable>::Instance().items.get<W<MedianFiltreON>>().value;
	frame.cutoffFrequency = Singleton<AnalogFilterTable>::Instance().items.get<W<CutoffFrequency>>().value;
	frame.cutoffFrequencyON = Singleton<AnalogFilterTable>::Instance().items.get<W<CutoffFrequencyON>>().value;
	frame.cursor.SetMouseMoveHandler(&frame, &FrameViewer::DrawFrame); 
	frame.koef = Singleton<KoeffSignTable>::Instance().items.get<W<KoeffSign>>().value;
	frame.typeFiltre = __param_filtre__<W, TypeFiltre>()();
	frame.centerFrequency = __param_filtre__<W, CenterFrequency>()();
	frame.widthFrequency = __param_filtre__<W, WidthFrequency>()();
	frame.order = Singleton<AnalogFilterTable>::Instance().items.get<W<Order>>().value;

	frame.stopBandDb = Singleton<AnalogFilterTable>::Instance().items.get<W<StopBandDb>>().value;
	//frame.passBandRippleDb = Singleton<AnalogFilterTable>::Instance().items.get<W<PassBandRippleDb>>().value;

//	frame.stopBandDb = __param_filtre__<W, StopBandDb>()();
	frame.passBandRippleDb = __param_filtre__<W, PassBandRippleDb>()();

	TL::foreach<viewers_list, Common::__create_window__>()(&viewers, &m.hwnd);	
	return 0;
}

template<template<class>class>struct diff_templ
{
	template<class O>void operator()(O &o, double(&arr)[1024])
	{
		for(int i = 0; i < dimention_of(arr); ++i)
		{
			if(arr[i] < 0) arr[i] =-arr[i];
		}
	}
};

template<>struct diff_templ<StructSig>
{
	template<class O>void operator()(O &o, double(&arr)[1024])
	{
		for(int i = 0; i < dimention_of(arr); ++i)
		{
			if(arr[i] < 0) arr[i] =-arr[i];
			arr[i] -= o.structMinVal;
			//if(arr[i] < 0) arr[i] = 0;
			if(arr[i] < 0) arr[i] = -arr[i];
		}
	}
};

template<template<class>class>struct __get_ampl__
{
	template<class O>double *operator()(O &o)
	{
		return o.inputData;
	}
};

template<>struct __get_ampl__<StructSig>
{
	template<class O>double *operator()(O &o)
	{
		return o.inputDataX;
	}
};

template<template<class>class W>void TemplWindow<W>::ChangeFrame(int offsetDef)
{
	Viewer &viewer = viewers.get<Viewer>();
	W<DataItem::Buffer> &item = Singleton<W<DataItem::Buffer>>::Instance();
	FrameViewer &frame =  viewers.get<FrameViewer>();

	frame.delta = (double)frame.count / dimention_of(frame.buffer);

	static const int tbuf_size = 3 * dimention_of(frame.buffer);
	double tbuf[tbuf_size];

	int offs_b = tbuf_size - dimention_of(frame.buffer);
	int offs = int(offsetDef - offs_b * frame.delta);
	int frameWidth = 3 * frame.count;

	typedef typename WapperFiltre<W>::Result WFiltre;
	WFiltre aFiltre;
	Compute::Filtre analog;
	if(frame.cutoffFrequencyON)
	{
		Compute::InitFiltre()(aFiltre
			, frame.order
			, frame.stopBandDb
			, frame.passBandRippleDb
			, Singleton<L502ParametersTable>::Instance().items.get<W<ChannelSamplingRate>>().value
			, frame.cutoffFrequency
			, frame.centerFrequency
			, frame.widthFrequency
			, frame.typeFiltre
			);
		analog.Init<WFiltre>(&aFiltre, &WFiltre::operator());
	}

	MedianFiltre mFiltre;
	Compute::Filtre median;
	if(frame.medianFiltreON && frame.medianFiltreWidth > 2)
	{
		mFiltre.InitWidth(frame.medianFiltreWidth);
		median.Init(&mFiltre, &MedianFiltre::operator());
	}

	double *d = frame.isBarGraph ? __get_ampl__<W>()(item):item.inputData;

	Compute::ComputeFrame(
		&d[item.firstOffset], offs
		, frameWidth
		, tbuf
		, tbuf_size
		, analog
		, median
		);

	memmove(frame.buffer, &tbuf[offs_b], sizeof(frame.buffer));

	if(frame.isBarGraph)
	{
		frame.tchart.minAxesY = 0;
		frame.tchart.maxAxesY = Singleton<AxesGraphsTable>::Instance().items.get<W<Axes>>().value;
	}
	else
	{
		//frame.tchart.minAxesY = -100;
		//frame.tchart.maxAxesY = 100;
		frame.tchart.minAxesY = -Singleton<AxesGraphsTable>::Instance().items.get<W<Axes>>().value;
		frame.tchart.maxAxesY = Singleton<AxesGraphsTable>::Instance().items.get<W<Axes>>().value;
	}

	frame.tchart.minAxesX = offsetDef;
	frame.tchart.maxAxesX = frame.tchart.minAxesX + frame.count;

	double adcRange =  100.0 / DataItem::ADC_RANGE(Singleton<L502ParametersTable>::Instance().items.get<W<RangeL502>>().value);

	for(int i = 0; i < dimention_of(frame.buffer); ++i)
	{
		frame.buffer[i] *= adcRange * frame.koef;
	}

	if(frame.isBarGraph)
	{
		diff_templ<W>()(viewer, frame.buffer);
	}

	frame.delta = (double)frame.count / dimention_of(frame.buffer);

	frame.nominalColor	= viewer.nominalColor;

	DeadAreaTable::TItems &dead = Singleton<DeadAreaTable>::Instance().items;
	int rodLength = dead.get<RodLenght>().value;

////	item.deathZoneFirst =  int((double )item.deathZoneFirst * DataItem::output_buffer_size / rodLength);
	//item.deathZoneSecond =  DataItem::output_buffer_size - int((double )item.deathZoneSecond * DataItem::output_buffer_size / rodLength);

	frame.deathZoneFirst	= 0;
	frame.deathZoneSecond	= item.currentOffset;

	frame.deathZoneColor	= viewer.deathZoneColor	; 	
	frame.threshDefect	 	= viewer.threshDefect	 	;
	frame.threshDefectColor	= viewer.threshDefectColor	;
	frame.threshSortDownColor = viewer.threshSortDownColor;
	frame.threshSortDown   	= viewer.threshSortDown   	;

	ThresholdsTable::TItems &tresh = Singleton<ThresholdsTable>::Instance().items;

	frame.tchart.items.get<FrameViewer::Border<SortDown>>().value = frame.threshSortDown;
	frame.tchart.items.get<FrameViewer::Border<Defect>>().value = frame.threshDefect;
	frame.tchart.items.get<FrameViewer::Border<SortDown>>().color  = frame.threshSortDownColor;
	frame.tchart.items.get<FrameViewer::Border<Defect>>().color  = frame.threshDefectColor;

	frame.tchart.items.get<FrameViewer::BorderDown<SortDown>>().value = -frame.threshSortDown;
	frame.tchart.items.get<FrameViewer::BorderDown<Defect>>().value = -frame.threshDefect;
	frame.tchart.items.get<FrameViewer::BorderDown<SortDown>>().color  = frame.threshSortDownColor;
	frame.tchart.items.get<FrameViewer::BorderDown<Defect>>().color  = frame.threshDefectColor;

	RepaintWindow(frame.hWnd);
}

template<template<class>class W>void TemplWindow<W>::operator()(TDestroy &m)
{
	delete this;
	SetWindowLongPtr(m.hwnd, GWLP_USERDATA, 0);
}

template<template<class>class W>void TemplWindow<W>::operator()(TSize &m)
{
	static const int height = 200;

	{
		Viewer &viewer = viewers.get<Viewer>();
		TSize size = {viewer.hWnd, WM_SIZE, 0, m.Width, height};
		SendMessage(MESSAGE(size));
		MoveWindow(viewer.hWnd , 0, 0, size.Width, size.Height, true);
	}
	{
		FrameViewer &frame = viewers.get<FrameViewer>();
		TSize size = {frame.hWnd, WM_SIZE, height, m.Width, m.Height - height};
		SendMessage(MESSAGE(size));
		MoveWindow(frame.hWnd , 0, height, size.Width, size.Height, true);
	}
}

template<template<class>class W>void TemplWindow<W>::operator()(TCommand &m)
{
	EventDo(m);
}

template<template<class>class W>void TemplWindow<W>::operator()(TMouseWell &l)
{
	TL::find<viewers_list, Common::__in_rect__>()(
		&viewers
		, &Common::__event_data__<TMouseWell, TemplWindow<W>>(*this, l)
		);
}

template<template<class>class W>void TemplWindow<W>::operator()(TClose &l)
{
	if(!TemplDlg::TestChangeParam<W>(l.hwnd) && IDYES == MessageBox(l.hwnd, L"Сохранить?", L"Параметры были изменены", MB_ICONQUESTION | MB_YESNO))
	{
		TemplDlg::StoreParam<W>(l.hwnd);
		Compute::Recalculation();
	}
	DestroyWindow(l.hwnd);
}