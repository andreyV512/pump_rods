#include "DefectDlg.h"
#include "tools_debug/DebugMess.h"
#include "App/AppBase.h"
#include "DlgTemplates\ParamDlgNew.h"
#include "DefectWindow.h"
#include "Compute\Compute.h"
#include "window_tool\EmptyWindow.h"
#include "MessageText\ListMess.hpp"

namespace Defectoscope
{
	MIN_EQUAL_VALUE(DefectSig<MedianFiltreWidth>, 3)
		MAX_EQUAL_VALUE(DefectSig<MedianFiltreWidth>, 15)
		PARAM_TITLE(DefectSig<MedianFiltreWidth>, L"Ширина фильтра")
		PARAM_TITLE(DefectSig<MedianFiltreON>, L"Включение фильтра")

		MIN_EQUAL_VALUE(StructSig<MedianFiltreWidth>, 3)
		MAX_EQUAL_VALUE(StructSig<MedianFiltreWidth>, 15)
		PARAM_TITLE(StructSig<MedianFiltreWidth>, L"Ширина фильтра")
		PARAM_TITLE(StructSig<MedianFiltreON>, L"Включение фильтра")

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
	void MedianFiltre::Do(HWND h)
	{
		DefectWindow &e = *(DefectWindow *)GetWindowLongPtr(h, GWLP_USERDATA);
		FrameViewer &frame =  e.viewers.get<FrameViewer>();
		MedianFiltreTable par;
		par.items.get< DefectSig<MedianFiltreWidth>>().value = frame.medianFiltreWidth;
		par.items.get< DefectSig<MedianFiltreON>>().value = frame.medianFiltreON;
		if(Dialog::Templ<NullType, MedianFiltreTable
			, TL::MkTlst<
			DefectSig<MedianFiltreWidth>, DefectSig<MedianFiltreON>
			>::Result
			, 550
			, TL::MkTlst<DefOkBtn, CancelBtn>::Result
		>(par).Do(h, L"Настройки медианного фильтра"))
		{
			frame.medianFiltreWidth = par.items.get< DefectSig<MedianFiltreWidth>>().value;
			frame.medianFiltreON = par.items.get< DefectSig<MedianFiltreON>>().value;

			Repaint<DefectSig>(e.viewers.get<DefectWindow::Def>(), frame);
		}
	}

	MIN_EQUAL_VALUE(DefectSig<CutoffFrequency>, 0)
		MAX_EQUAL_VALUE(DefectSig<CutoffFrequency>, 4000)
		PARAM_TITLE(DefectSig<CutoffFrequency>, L"Частота отсечения фильтра")
		PARAM_TITLE(DefectSig<CutoffFrequencyON>, L"Включение фильтра")

		MIN_EQUAL_VALUE(StructSig<CutoffFrequency>, 0)
		MAX_EQUAL_VALUE(StructSig<CutoffFrequency>, 4000)
		PARAM_TITLE(StructSig<CutoffFrequency>, L"Частота отсечения фильтра")
		PARAM_TITLE(StructSig<CutoffFrequencyON>, L"Включение фильтра")

		void FilterDlg::Do(HWND h)
	{
		DefectWindow &e = *(DefectWindow *)GetWindowLongPtr(h, GWLP_USERDATA);
		FrameViewer &frame =  e.viewers.get<FrameViewer>();
		AnalogFilterTable par;
		par.items.get< DefectSig<CutoffFrequency>>().value = frame.cutoffFrequency;
		par.items.get< DefectSig<CutoffFrequencyON>>().value = frame.cutoffFrequencyON;
		if(Dialog::Templ<NullType, AnalogFilterTable
			, TL::MkTlst<
			DefectSig<CutoffFrequency>
			, DefectSig<CutoffFrequencyON>
			>::Result
			, 550
			, TL::MkTlst<DefOkBtn, CancelBtn>::Result
		>(par).Do(h, L"Настройки цифрового фильтра"))
		{
			frame.cutoffFrequency = par.items.get< DefectSig<CutoffFrequency>>().value;
			frame.cutoffFrequencyON = par.items.get< DefectSig<CutoffFrequencyON>>().value;

			Repaint<DefectSig>( e.viewers.get<DefectWindow::Def>(), frame);
		}
	}

	MIN_EQUAL_VALUE( DefectSig<KoeffSign>, 0.1)
	MAX_EQUAL_VALUE( DefectSig<KoeffSign>, 2.0)
	PARAM_TITLE( DefectSig<KoeffSign>, L"Коэффициент")

		void CorrectionSensorDlg::Do(HWND h)
	{
		DefectWindow &e = *(DefectWindow *)GetWindowLongPtr(h, GWLP_USERDATA);
		FrameViewer &frame =  e.viewers.get<FrameViewer>();
		KoeffSignTable koef;
		koef.items.get<DefectSig<KoeffSign>>().value = frame.koef;
		if(Dialog::Templ<NullType, KoeffSignTable
			, TL::MkTlst<DefectSig<KoeffSign>>::Result
			, 550
			, TL::MkTlst<DefOkBtn, CancelBtn>::Result
			>(koef).Do(h, L"Корректировка датчика"))
		{
			DefectWindow &e = *(DefectWindow *)GetWindowLongPtr(h, GWLP_USERDATA);
			FrameViewer &frame =  e.viewers.get<FrameViewer>();
			frame.koef = koef.items.get<DefectSig<KoeffSign>>().value;

			Repaint<DefectSig>(e.viewers.get<DefectWindow::Def>(), frame);
		}
	}

	template<class Table, class Item, class Param, class Type, Type Param::*>struct __test_change_param__;

	template<>struct __test_change_param__<KoeffSignTable, DefectSig<KoeffSign>, FrameViewer, double, &FrameViewer::koef>{};

	template<>struct __test_change_param__<AnalogFilterTable, DefectSig<CutoffFrequency>, FrameViewer, int, &FrameViewer::cutoffFrequency>{};
	template<>struct __test_change_param__<AnalogFilterTable, DefectSig<CutoffFrequencyON>, FrameViewer, bool, &FrameViewer::cutoffFrequencyON>{};

	template<>struct __test_change_param__<MedianFiltreTable, DefectSig<MedianFiltreWidth>, FrameViewer, int, &FrameViewer::medianFiltreWidth>{};
	template<>struct __test_change_param__<MedianFiltreTable, DefectSig<MedianFiltreON>, FrameViewer, bool, &FrameViewer::medianFiltreON>{};

	typedef TL::MkTlst<
		__test_change_param__<KoeffSignTable   , DefectSig<KoeffSign>        , FrameViewer, double, &FrameViewer::koef>
		, __test_change_param__<AnalogFilterTable, DefectSig<CutoffFrequency>  , FrameViewer, int,    &FrameViewer::cutoffFrequency>
		, __test_change_param__<AnalogFilterTable, DefectSig<CutoffFrequencyON>, FrameViewer, bool,   &FrameViewer::cutoffFrequencyON>
		, __test_change_param__<MedianFiltreTable, DefectSig<MedianFiltreWidth>, FrameViewer, int,    &FrameViewer::medianFiltreWidth>
		, __test_change_param__<MedianFiltreTable, DefectSig<MedianFiltreON>   , FrameViewer, bool,   &FrameViewer::medianFiltreON>
	>::Result __param_list__;

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


	bool TestChangeParam(HWND h)
	{
		DefectWindow &e = *(DefectWindow *)GetWindowLongPtr(h, GWLP_USERDATA);
		FrameViewer &frame =  e.viewers.get<FrameViewer>();

		return TL::find<__param_list__, __test_param__>()(frame);
	}

	void  StoreParam(HWND h)
	{
		if(TypesizePasswordDlg().Do(h))
		{
			DefectWindow &e = *(DefectWindow *)GetWindowLongPtr(h, GWLP_USERDATA);
			FrameViewer &frame =  e.viewers.get<FrameViewer>();
			CBase base(ParametersBase().name());
			if(base.IsOpen())
			{
				TL::foreach<__param_list__, __store_param__>()(__store_param_data__(frame, base));
			}
		}
	}

	MIN_EQUAL_VALUE(DefectSig<ViewerCount>, 100)
	MAX_EQUAL_VALUE(DefectSig<ViewerCount>, 30000)
	PARAM_TITLE(DefectSig<ViewerCount>, L"Ширина")

	void FrameWidthViewDlg::Do(HWND h)
	{
		if(Dialog::Templ<ParametersBase, ViewerCountTable
			, TL::MkTlst<
			DefectSig<ViewerCount>
			>::Result
		>(Singleton<ViewerCountTable>::Instance()).Do(h, L"Ширина кадра"))
		{
			DefectWindow &e = *(DefectWindow *)GetWindowLongPtr(h, GWLP_USERDATA);
			FrameViewer &frame =  e.viewers.get<FrameViewer>();
			frame.count = Singleton<ViewerCountTable>::Instance().items.get<DefectSig<ViewerCount>>().value;
		}
	}
}

