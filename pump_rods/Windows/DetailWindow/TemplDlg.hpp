﻿#pragma once
#include <Windows.h>
#include "App/App.h"
#include "App\AppBase.h"
#include "DlgTemplates\ParamDlg.hpp"
#include "DlgTemplates\ParamDlgNew.h"
#include "MessageText\ListMess.hpp"
#include "FrameViewer.h"
#include "window_tool/MenuAPI.h"
#include "Compute/Compute.h"
#include "window_tool/Emptywindow.h"

namespace TemplDlg
{	
	template<class>struct __is_acselerator__{static const int value = 768;};
	template<>struct __is_acselerator__<bool>{static const int value = 0;};

	template<class T>struct __get_val__
	{
		void operator()(HWND h, T &t)
		{
			wchar_t buf[128];
			GetWindowText(h, buf, dimention_of(buf));
			T x = Wchar_to<T>()(buf);
			if(x > 0) t = x;
		}
	};

	template<>struct __get_val__<bool>
	{
		void operator()(HWND h, bool &t)
		{
			t = BST_CHECKED == Button_GetCheck(h);
		}
	};

	#define FILTER_ITEMS(type, param)\
template<template<class>class W>struct Dialog::NoButton<W<type>>{};\
template<template<class>class W, class P>struct __command__<Dialog::NoButton<W<type>>, P>\
{\
	typedef Dialog::NoButton<W<type>> O;\
	bool operator()(O *o, P *p)\
	{\
		typedef typename TL::Inner<O>::Result TVal;\
		if(TemplDlg::__is_acselerator__<TVal::type_value>::value == p->e.isAcselerator)\
		{\
			HWND h = p->owner.items.get<Dialog::DlgItem2<TVal, P::Owner>>().hWnd;\
			if(p->e.hControl == h)\
			{\
				typedef TemplWindow<W> Win;\
				HWND hWnd = GetWindow(p->e.hwnd, GW_OWNER);\
				Win &e = *(Win *)GetWindowLongPtr(hWnd, GWLP_USERDATA);\
				FrameViewer &frame =  e.viewers.get<FrameViewer>();\
				Win::Viewer &viewer = e.viewers.get<Win::Viewer>();\
				TemplDlg::__get_val__<TVal::type_value>()(h, frame.param);\
				TemplDlg::Repaint<W>(viewer, frame);\
				return false;\
			}\
		}\
		return true;\
	}\
};
FILTER_ITEMS(CutoffFrequency, cutoffFrequency)
FILTER_ITEMS(CenterFrequency, centerFrequency)
FILTER_ITEMS(WidthFrequency, widthFrequency)
FILTER_ITEMS(Order, order)
FILTER_ITEMS(StopBandDb, stopBandDb)
FILTER_ITEMS(PassBandRippleDb, passBandRippleDb)
FILTER_ITEMS(CutoffFrequencyON, cutoffFrequencyON)


FILTER_ITEMS(MedianFiltreWidth, medianFiltreWidth)
FILTER_ITEMS(MedianFiltreON, medianFiltreON)

FILTER_ITEMS(Thresh<SortDown>, threshSortDown)
FILTER_ITEMS(Thresh<Defect>, threshDefect)

FILTER_ITEMS(KoeffSign, koef)
#undef FILTER_ITEMS

	TEMPL_MIN_EQUAL_VALUE(MedianFiltreWidth, 3)
	TEMPL_MAX_EQUAL_VALUE(MedianFiltreWidth, 15)
	TEMPL_PARAM_TITLE(DefectSig<MedianFiltreWidth>, L"Ширина фильтра")
	TEMPL_PARAM_TITLE(DefectSig<MedianFiltreON>, L"Включение фильтра")

	template<class O, class P>struct __set_param_ok_btn__
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
			analog.Init<WFiltre>(&aFiltre, &WFiltre::Simple);
		}

		MedianFiltre mFiltre;
		Compute::Filtre median;
		if(frame.medianFiltreON && frame.medianFiltreWidth > 2)
		{
			mFiltre.InitWidth(frame.medianFiltreWidth);
			median.Init(&mFiltre, &MedianFiltre::operator());
		}

		Compute::ComputeFrame(
			Compute::get_ampl<W>()(o)
			, o.firstOffset
			, o.secondOffset
			, def.buffer
			, DataItem::output_buffer_size
			, analog
			, median
			);

		double adcRange =  100.0 / DataItem::ADC_RANGE(l502Param.get<W<RangeL502>>().value);
		for(int i = 0; i < DataItem::output_buffer_size; ++i)
		{
			if(def.buffer[i] < 0) def.buffer[i] =-def.buffer[i];
			def.buffer[i] *= adcRange * frame.koef;
		}

	    Compute::diff<W>()(def, def.buffer);
		for(int i = 0; i < DataItem::output_buffer_size; ++i)
		{
			double t = def.buffer[i];
			if(t > frame.threshDefect)
			{
				def.status[i] = STATUS_ID(W<Defect>);
			}
			else if(t > frame.threshSortDown)
			{
				def.status[i] = STATUS_ID(W<SortDown>);
			}
			else
			{
				def.status[i] = STATUS_ID(Nominal);
			}
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
		TL::foreach<typename Owner::list, __set_param_ok_btn__>()(t.items);
		EndDialog(h, TRUE);
	}
};

template<template<class>class W>struct MedianFiltreDlg
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
			, TL::MkTlst<DefOkBtn, CancelBtn
			, Dialog::NoButton<W<MedianFiltreWidth>>
			, Dialog::NoButton<W<MedianFiltreON>>
			>::Result
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

TEMPL_MIN_EQUAL_VALUE(Order, 0)
TEMPL_MAX_EQUAL_VALUE(Order, 5)
TEMPL_PARAM_TITLE(Order, L"Порядок фильтра")

TEMPL_MIN_EQUAL_VALUE(StopBandDb, 5)
TEMPL_MAX_EQUAL_VALUE(StopBandDb, 100)
TEMPL_PARAM_TITLE(StopBandDb, L"Затухание в полосе подавления Db")

template<>struct FillComboboxList<DefectSig<TypeFiltre>>			 
{		
	void operator()(HWND h, DefectSig<TypeFiltre> &t)			 
	{							
		static const wchar_t *typeFiltreNames[] ={L"Низкочастотный фильтр", L"Полосовой фильтр"};
		for(int i = 0; i < dimention_of(typeFiltreNames); ++i)	
		{													 
			ComboBox_AddString(h, typeFiltreNames[i]);			 
		}													 
	}														
};															 
template<>struct CurrentValue<DefectSig<TypeFiltre>>				 
{															 
	void operator()(HWND h, DefectSig<TypeFiltre> &t)			 
	{		
		ComboBox_SetCurSel(h, t.value);
	}
};
template<>struct DlgSubItems<DefectSig<TypeFiltre>, int>: ComboBoxSubItem<DefectSig<TypeFiltre>>{};

DO_NOT_CHECK(DefectSig<TypeFiltre>)

template<template<class> class W>struct FilterDlg
{
	static void Do(HWND h)
	{
		typedef TemplWindow<W> Win;
		Win &e = *(Win *)GetWindowLongPtr(h, GWLP_USERDATA);
		FrameViewer &frame =  e.viewers.get<FrameViewer>();
		AnalogFilterTable par;
		par.items.get< W<CutoffFrequency>>().value = frame.cutoffFrequency;

		par.items.get< W<Order>>().value = frame.order;

		par.items.get< W<CutoffFrequencyON>>().value = frame.cutoffFrequencyON;
		if(Dialog::Templ<NullType, AnalogFilterTable
			, TL::MkTlst<
			W<CutoffFrequency>
			, W<Order>
			, W<StopBandDb>
			, W<CutoffFrequencyON>
			>::Result
			, 550
			, TL::MkTlst<DefOkBtn, CancelBtn
			, Dialog::NoButton<W<CutoffFrequency>>
			, Dialog::NoButton<W<CutoffFrequencyON>>
			, Dialog::NoButton<W<Order>>
			, Dialog::NoButton<W<StopBandDb>>
			>::Result
			>(par).Do(h, L"Настройки цифрового фильтра"))
		{
			frame.cutoffFrequency = par.items.get< W<CutoffFrequency>>().value;
			frame.cutoffFrequencyON = par.items.get< W<CutoffFrequencyON>>().value;
			frame.order = par.items.get< W<Order>>().value;
		}
		else
		{
			AnalogFilterTable::TItems &t = Singleton<AnalogFilterTable>::Instance().items;
			frame.cutoffFrequency = t.get< W<CutoffFrequency>>().value;
			frame.cutoffFrequencyON = t.get< W<CutoffFrequencyON>>().value;
			frame.order = t.get< W<Order>>().value;
		}
		Repaint<W>( e.viewers.get<Win::Viewer>(), frame);
	}
};
////-------------------------------------------------------------------
template<>struct FilterDlg<DefectSig>
{
	static void Do(HWND h);	
};
//---------------------------------------------------------------
TEMPL_MIN_EQUAL_VALUE(Thresh<SortDown>, 0)
TEMPL_MIN_EQUAL_VALUE(Thresh<Defect>, 0)
TEMPL_MAX_EQUAL_VALUE(Thresh<SortDown>, 100)
TEMPL_MAX_EQUAL_VALUE(Thresh<Defect>, 100)

TEMPL_PARAM_TITLE(Thresh<SortDown>, L"Сорт")
TEMPL_PARAM_TITLE(Thresh<Defect>, L"Дефект")

struct TreshOkBtn
{
	static const int width = 120;
	static const int height = 30;
	static const int ID = IDOK;
	wchar_t *Title(){return L"Применить";}
	template<class Owner>void BtnHandler(Owner &t, HWND h)
	{
		if(!TL::find<typename Owner::list, __test__>()(&t.items, &h))return;
		TL::foreach<typename Owner::list, __set_param_ok_btn__>()(t.items);
		EndDialog(h, TRUE);
	}
};

template<template<class> class W>struct TreshDlg
{
	static void Do(HWND h)
	{
		typedef TemplWindow<W> Win;
		Win &e = *(Win *)GetWindowLongPtr(h, GWLP_USERDATA);
		FrameViewer &frame =  e.viewers.get<FrameViewer>();
		Win::Viewer &viewer = e.viewers.get<Win::Viewer>();
		ThresholdsTable par;
		par.items.get< W<Thresh<SortDown>>>().value = frame.threshSortDown;
		par.items.get< W<Thresh<Defect>>>().value = frame.threshDefect;
		if(Dialog::Templ<NullType, ThresholdsTable
			, TL::MkTlst<
			W<Thresh<SortDown>>
			, W<Thresh<Defect>>
			>::Result
			, 350
			, TL::MkTlst<TreshOkBtn, CancelBtn
			, Dialog::NoButton<W<Thresh<SortDown>>>
			, Dialog::NoButton<W<Thresh<Defect>>>
			>::Result
		>(par).Do(h, L"Настройки порогов"))
		{
			viewer.threshSortDown =  frame.threshSortDown = par.items.get< W<Thresh<SortDown>>>().value;
			viewer.threshDefect = frame.threshDefect = par.items.get< W<Thresh<Defect>>>().value;

			Repaint<W>( e.viewers.get<Win::Viewer>(), frame);
		}
	}
};
//---------------------------------------------------------------
TEMPL_MIN_EQUAL_VALUE(KoeffSign, 0.1)
TEMPL_MAX_EQUAL_VALUE( KoeffSign, 10.0)
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
			, TL::MkTlst<DefOkBtn, CancelBtn
			, Dialog::NoButton<W<KoeffSign>>
			>::Result
			>(koef).Do(h, L"Корректировка датчика"))
		{
			frame.koef = koef.items.get<W<KoeffSign>>().value;
			Repaint<W>(e.viewers.get<Win::Viewer>(), frame);
		}
	}
};

	template<class Table, class Item, class Param, class Type, Type Param::*>struct __test_change_param__;

	template<template<class> class W>struct __param_list__
	{
		typedef typename TL::MkTlst<
			__test_change_param__<KoeffSignTable     , W<KoeffSign>        , FrameViewer, double, &FrameViewer::koef>
			, __test_change_param__<AnalogFilterTable, W<CutoffFrequency>  , FrameViewer, int,    &FrameViewer::cutoffFrequency>
			, __test_change_param__<AnalogFilterTable, W<CutoffFrequencyON>, FrameViewer, bool,   &FrameViewer::cutoffFrequencyON>
			, __test_change_param__<AnalogFilterTable, W<Order>            , FrameViewer, int ,   &FrameViewer::order>
			, __test_change_param__<MedianFiltreTable, W<MedianFiltreWidth>, FrameViewer, int,    &FrameViewer::medianFiltreWidth>
			, __test_change_param__<MedianFiltreTable, W<MedianFiltreON>   , FrameViewer, bool,   &FrameViewer::medianFiltreON>
			, __test_change_param__<ThresholdsTable, W<Thresh<SortDown>>   , FrameViewer, double,    &FrameViewer::threshSortDown>
			, __test_change_param__<ThresholdsTable, W<Thresh<Defect>>   , FrameViewer  , double,    &FrameViewer::threshDefect>
		>::Result Result;
	};

	template<>struct __param_list__<DefectSig>
	{
		typedef  TL::MkTlst<
			__test_change_param__<KoeffSignTable     , DefectSig<KoeffSign>        , FrameViewer, double, &FrameViewer::koef>
			, __test_change_param__<AnalogFilterTable, DefectSig<CutoffFrequency>  , FrameViewer, int,    &FrameViewer::cutoffFrequency>
			, __test_change_param__<AnalogFilterTable, DefectSig<CutoffFrequencyON>, FrameViewer, bool,   &FrameViewer::cutoffFrequencyON>
			, __test_change_param__<AnalogFilterTable, DefectSig<TypeFiltre>, FrameViewer, int,   &FrameViewer::typeFiltre>
			, __test_change_param__<AnalogFilterTable, DefectSig<WidthFrequency>  , FrameViewer, int,    &FrameViewer::widthFrequency>
			, __test_change_param__<AnalogFilterTable, DefectSig<CenterFrequency>, FrameViewer, int,   &FrameViewer::centerFrequency>
			, __test_change_param__<AnalogFilterTable, DefectSig<Order>, FrameViewer, int,   &FrameViewer::order>
			, __test_change_param__<AnalogFilterTable, DefectSig<StopBandDb>, FrameViewer, double,   &FrameViewer::stopBandDb>
			, __test_change_param__<AnalogFilterTable, DefectSig<PassBandRippleDb>, FrameViewer, double,   &FrameViewer::passBandRippleDb>

			, __test_change_param__<MedianFiltreTable, DefectSig<MedianFiltreWidth>, FrameViewer, int,    &FrameViewer::medianFiltreWidth>
			, __test_change_param__<MedianFiltreTable, DefectSig<MedianFiltreON>   , FrameViewer, bool,   &FrameViewer::medianFiltreON>
			, __test_change_param__<ThresholdsTable  , DefectSig<Thresh<SortDown>>   , FrameViewer, double,    &FrameViewer::threshSortDown>
			, __test_change_param__<ThresholdsTable  , DefectSig<Thresh<Defect>>   , FrameViewer  , double,    &FrameViewer::threshDefect>
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
				param = p.frame.*X;
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
	TEMPL_MAX_EQUAL_VALUE(ViewerCount, 100000)
	TEMPL_PARAM_TITLE(ViewerCount, L"Ширина")

	template<>struct TestPassword<ParametersBase, ViewerCountTable>
	{
		bool operator()(HWND h)
		{
			return true;
		}
	};

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
	TEMPL_MENU_ITEM(L"Медианный фильтр", TemplDlg::MedianFiltreDlg)
	TEMPL_MENU_ITEM(L"Корректировка датчика", TemplDlg::CorrectionSensorDlg)
	TEMPL_MENU_ITEM(L"Пороги отбраковки", TemplDlg::TreshDlg)
	//
	template<template<class >class W>struct TopMenu<W<TypeSize>>
	{
		typedef typename TL::MkTlst<
			 MenuItem<TemplDlg::MedianFiltreDlg<W>>
			 , MenuItem<TemplDlg::FilterDlg<W>>
			 , MenuItem<TemplDlg::TreshDlg<W>>
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