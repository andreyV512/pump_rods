#pragma once
#include <Windows.h>
#include "App/App.h"
#include "App\AppBase.h"
#include "DlgTemplates\ParamDlg.hpp"
#include "DlgTemplates\ParamDlgNew.h"
#include "MessageText\ListMess.hpp"

namespace TemplDlg
{	
	TEMPL_MIN_EQUAL_VALUE(MedianFiltreWidth, 3)
	TEMPL_MAX_EQUAL_VALUE(MedianFiltreWidth, 15)
	TEMPL_PARAM_TITLE(DefectSig<MedianFiltreWidth>, L"Ширина фильтра")
	TEMPL_PARAM_TITLE(DefectSig<MedianFiltreON>, L"Включение фильтра")

	template<class O, class P>struct __def_ok_btn__
	{
		void operator()(O &o)
		{
			o.value.value =  __data_from_widget__<O, typename TL::Inner<O>::Result::type_value>()(o);
		}
	};

	template<template<class>class W, class Def, class Frame>void Repaint(Def &def, Frame &frame)
	{
		L502ParametersTable::TItems &l502Param =  Singleton<L502ParametersTable>::Instance().items;
		W<DataItem::Buffer> &o = Singleton<W<DataItem::Buffer>>::Instance();
		Compute::Compute(
			o.inputData
			, o.currentOffset
			, frame.cutoffFrequency
			, frame.cutoffFrequencyON
			, frame.medianFiltreWidth
			, frame.medianFiltreON
			, def.buffer
			, DataItem::output_buffer_size
			, l502Param.get<W<ChannelSamplingRate>>().value
			);

		double adcRange =  100.0 / DataItem::ADC_RANGE(l502Param.get<W<RangeL502>>().value);
		for(int i = 0; i < DataItem::output_buffer_size; ++i)
		{
			def.buffer[i] *= adcRange * frame.koef;
		}

		for(int i = def.deathZoneFirst; i < def.deathZoneSecond; ++i)
		{
			double t = def.buffer[i];
			if(t > def.threshDefect)
			{
				def.status[i] = STATUS_ID(W<Defect>);
			}
			else if(t > def.threshSortDown)
			{
				def.status[i] = STATUS_ID(W<SortDown>);
			}
			else
			{
				def.status[i] = STATUS_ID(Nominal);
			}
		}
		for(int i = 0; i < def.deathZoneFirst; ++i)
		{
			def.status[i] = STATUS_ID(DeathZone);
		}
		for(int i = def.deathZoneSecond; i < dimention_of(def.status); ++i)
		{
			def.status[i] = STATUS_ID(DeathZone);
		}

		RepaintWindow(def.hWnd);
		RepaintWindow(frame.hWnd);
	}

struct DefOkBtn
{
	static const int width = 120;
	static const int height = 30;
	static const int ID = IDOK;
	wchar_t *Title(){return L"Применить";}
	template<class Owner>void BtnHandler(Owner &t, HWND h)
	{
		if(!TL::find<typename Owner::list, __test__>()(&t.items, &h))return;
		TL::foreach<typename Owner::list, __def_ok_btn__>()(t.items);
		EndDialog(h, TRUE);
	}
};

template<template<class>class W>struct MedianFiltre
{
	static void Do(HWND h)
	{
		typedef typename TemplWindow<W> Win;
		Win &e = *(Win *)GetWindowLongPtr(h, GWLP_USERDATA);
		FrameViewer &frame =  e.viewers.get<FrameViewer>();
		MedianFiltreTable par;
		par.items.get< W<MedianFiltreWidth>>().value = frame.medianFiltreWidth;
		par.items.get< W<MedianFiltreON>>().value = frame.medianFiltreON;
		if(Dialog::Templ<NullType, MedianFiltreTable
			, TL::MkTlst<
			W<MedianFiltreWidth>, W<MedianFiltreON>
			>::Result
			, 550
			, TL::MkTlst<DefOkBtn, CancelBtn>::Result
		>(par).Do(h, L"Настройки медианного фильтра"))
		{
			frame.medianFiltreWidth = par.items.get< W<MedianFiltreWidth>>().value;
			frame.medianFiltreON = par.items.get< W<MedianFiltreON>>().value;
	
			Repaint<W>(e.viewers.get<Win::Viewer>(), frame);
		}
	}
};

TEMPL_MIN_EQUAL_VALUE(CutoffFrequency, 0)
TEMPL_MAX_EQUAL_VALUE(CutoffFrequency, 4000)
TEMPL_PARAM_TITLE(CutoffFrequency, L"Частота отсечения фильтра")
TEMPL_PARAM_TITLE(CutoffFrequencyON, L"Включение фильтра")

template<template<class> class W>struct FilterDlg
{
	static void Do(HWND h)
	{
		typedef TemplWindow<W> Win;
		Win &e = *(Win *)GetWindowLongPtr(h, GWLP_USERDATA);
		FrameViewer &frame =  e.viewers.get<FrameViewer>();
		AnalogFilterTable par;
		par.items.get< W<CutoffFrequency>>().value = frame.cutoffFrequency;
		par.items.get< W<CutoffFrequencyON>>().value = frame.cutoffFrequencyON;
		if(Dialog::Templ<NullType, AnalogFilterTable
			, TL::MkTlst<
			W<CutoffFrequency>
			, W<CutoffFrequencyON>
			>::Result
			, 550
			, TL::MkTlst<DefOkBtn, CancelBtn>::Result
		>(par).Do(h, L"Настройки цифрового фильтра"))
		{
			frame.cutoffFrequency = par.items.get< W<CutoffFrequency>>().value;
			frame.cutoffFrequencyON = par.items.get< W<CutoffFrequencyON>>().value;

			Repaint<W>( e.viewers.get<Win::Viewer>(), frame);
		}
	}
};

TEMPL_MIN_EQUAL_VALUE(KoeffSign, 0.1)
TEMPL_MAX_EQUAL_VALUE( KoeffSign, 2.0)
TEMPL_PARAM_TITLE(KoeffSign, L"Коэффициент")

template<template<class>class W>struct CorrectionSensorDlg
{
	static void Do(HWND h)
	{
		typedef TemplWindow<W> Win;
		Win &e = *(Win *)GetWindowLongPtr(h, GWLP_USERDATA);
		FrameViewer &frame =  e.viewers.get<FrameViewer>();
		KoeffSignTable koef;
		koef.items.get<W<KoeffSign>>().value = frame.koef;
		if(Dialog::Templ<NullType, KoeffSignTable
			, TL::MkTlst<W<KoeffSign>>::Result
			, 550
			, TL::MkTlst<DefOkBtn, CancelBtn>::Result
			>(koef).Do(h, L"Корректировка датчика"))
		{
			frame.koef = koef.items.get<W<KoeffSign>>().value;
			Repaint<W>(e.viewers.get<Win::Viewer>(), frame);
		}
	}
};

	template<class Table, class Item, class Param, class Type, Type Param::*>struct __test_change_param__;

	template<>struct __test_change_param__<KoeffSignTable   , StructSig<KoeffSign>, FrameViewer, double, &FrameViewer::koef>{};
	template<>struct __test_change_param__<AnalogFilterTable, StructSig<CutoffFrequency>, FrameViewer, int, &FrameViewer::cutoffFrequency>{};
	template<>struct __test_change_param__<AnalogFilterTable, StructSig<CutoffFrequencyON>, FrameViewer, bool, &FrameViewer::cutoffFrequencyON>{};
	template<>struct __test_change_param__<MedianFiltreTable, StructSig<MedianFiltreWidth>, FrameViewer, int, &FrameViewer::medianFiltreWidth>{};
	template<>struct __test_change_param__<MedianFiltreTable, StructSig<MedianFiltreON>, FrameViewer, bool, &FrameViewer::medianFiltreON>{};

	template<template<class> class W>struct __param_list__
	{
		typedef typename TL::MkTlst<
			__test_change_param__<KoeffSignTable     , W<KoeffSign>        , FrameViewer, double, &FrameViewer::koef>
			, __test_change_param__<AnalogFilterTable, W<CutoffFrequency>  , FrameViewer, int,    &FrameViewer::cutoffFrequency>
			, __test_change_param__<AnalogFilterTable, W<CutoffFrequencyON>, FrameViewer, bool,   &FrameViewer::cutoffFrequencyON>
			, __test_change_param__<MedianFiltreTable, W<MedianFiltreWidth>, FrameViewer, int,    &FrameViewer::medianFiltreWidth>
			, __test_change_param__<MedianFiltreTable, W<MedianFiltreON>   , FrameViewer, bool,   &FrameViewer::medianFiltreON>
		>::Result Result;
	};

	template<class O, class P>struct __test_param__;
	template<class Table, class Item, class Param, class Type, Type Param::*X, class P>struct __test_param__<__test_change_param__<Table, Item, Param, Type, X>, P>
	{
		bool operator()(P &p)
		{
			return Singleton<Table>::Instance().items.get<Item>().value == p.*X;
		}
	};

	struct __store_param_data__
	{
		FrameViewer &frame;
		CBase &base;
		__store_param_data__(FrameViewer &frame, CBase &base)
			: frame(frame)
			, base(base)
		{}
	};
	template<class O, class P>struct __store_param__;
	template<class Table, class Item, class Param, class Type, Type Param::*X, class P>struct __store_param__<__test_change_param__<Table, Item, Param, Type, X>, P>
	{
		void operator()(P &p)
		{	
			Type &param = Singleton<Table>::Instance().items.get<Item>().value;
			if(param != p.frame.*X)
			{
				int id = CurrentId<ID<Table> >();
				param = p.frame.*X;
				Update<Table>(p.base).set<Item>(param).Where().ID(id).Execute();
			}
		}
	};

	template<template<class>class W>bool TestChangeParam(HWND h)
	{
		typedef TemplWindow<W> Win;
		Win &e = *(Win *)GetWindowLongPtr(h, GWLP_USERDATA);
		FrameViewer &frame =  e.viewers.get<FrameViewer>();

		return TL::find<typename __param_list__<W>::Result, __test_param__>()(frame);
	}

	template<template<class>class W>void  StoreParam(HWND h)
	{
		if(TypesizePasswordDlg().Do(h))
		{
			typedef TemplWindow<W> Win;
		Win &e = *(Win *)GetWindowLongPtr(h, GWLP_USERDATA);
			FrameViewer &frame =  e.viewers.get<FrameViewer>();
			CBase base(ParametersBase().name());
			if(base.IsOpen())
			{
				TL::foreach<typename __param_list__<W>::Result, __store_param__>()(__store_param_data__(frame, base));
			}
		}
	}

	TEMPL_MIN_EQUAL_VALUE(ViewerCount, 100)
	TEMPL_MAX_EQUAL_VALUE(ViewerCount, 30000)
	TEMPL_PARAM_TITLE(ViewerCount, L"Ширина")

	template<template<class> class W>struct FrameWidthViewDlg
	{
		static void Do(HWND h)
		{
			if(Dialog::Templ<ParametersBase, ViewerCountTable
				, TL::MkTlst<
				W<ViewerCount>
				>::Result
			>(Singleton<ViewerCountTable>::Instance()).Do(h, L"Ширина кадра"))
			{
				typedef TemplWindow<W> Win;
				Win &e = *(Win *)GetWindowLongPtr(h, GWLP_USERDATA);
				FrameViewer &frame =  e.viewers.get<FrameViewer>();
				frame.count = Singleton<ViewerCountTable>::Instance().items.get<W<ViewerCount>>().value;
			}
		}
	};


	struct MainFile{};
	MENU_TEXT(L"Файл", TopMenu<MainFile>)

	struct MainExit
	{
		static void Do(HWND h)
		{
			TClose c = {h, WM_CLOSE, 0, 0};
			SendMessage(MESSAGE(c));
		}
	};

	MENU_ITEM(L"Выход", MainExit)

	template<>struct TopMenu<MainFile>
	{
		typedef TL::MkTlst<
			MenuItem<MainExit>
		>::Result list;
	};

	#define TEMPL_MENU_TEXT(txt, item)template<template<class>class W, template<class>class Item>struct NameMenu<Item<W<item>>>{wchar_t *operator()(HWND){return txt;}};

	#define TEMPL_MENU_ITEM(txt, item)\
		template<template<class>class W, template<class>class Item>struct NameMenu<Item<item<W>>>{wchar_t *operator()(HWND){return txt;}};\
		template<template<class>class W, template<class>class Item>struct Event<Item<item<W>> >:item<W>{};

	struct TypeSize{};
	TEMPL_MENU_TEXT(L"Типоразмер", TypeSize)

	TEMPL_MENU_ITEM(L"Настройки цифрового фильтра", TemplDlg::FilterDlg)
	TEMPL_MENU_ITEM(L"Медианный фильтр", TemplDlg::MedianFiltre)
	TEMPL_MENU_ITEM(L"Корректировка датчика", TemplDlg::CorrectionSensorDlg)
	//
	template<template<class >class W>struct TopMenu<W<TypeSize>>
	{
		typedef typename TL::MkTlst<
			 MenuItem<TemplDlg::MedianFiltre<W>>
			 , MenuItem<TemplDlg::FilterDlg<W>>
			 , Separator<0>
			 , MenuItem<TemplDlg::CorrectionSensorDlg<W>>
		>::Result list;
	};
	//
	struct Options{};
	TEMPL_MENU_TEXT(L"Настройки", Options)
	
	TEMPL_MENU_ITEM(L"Ширина кадра", TemplDlg::FrameWidthViewDlg)
	
	template<template<class>class W>struct GraphView{
		static void Do(HWND h)
		{			
			typedef TemplWindow<W> Win;
			Win &w = *(Win *)GetWindowLongPtr(h, GWLP_USERDATA);
			FrameViewer &f = w.viewers.get<FrameViewer>();
			f.isBarGraph ^= true;
			w.ChangeFrame(w.offs);
		}
	};
	TEMPL_MENU_ITEM(L"Вид графика", TemplDlg::GraphView)
	
	template<template<class >class W>struct TopMenu<W<Options>>
	{
		typedef typename TL::MkTlst<
			MenuItem<TemplDlg::FrameWidthViewDlg<W>>
			, Separator<0>
			, MenuItem<TemplDlg::GraphView<W>>
		>::Result list;
	};

	template<template<class>class W>struct Menu
	{
		typedef typename TL::MkTlst<
			TopMenu<MainFile>
			, typename TopMenu<W<TypeSize>>
			, typename TopMenu<W<Options>>
		>::Result menu_list;	
	};

}