#include "StructDlg.h"
#include "tools_debug/DebugMess.h"
#include "App/AppBase.h"
#include "DlgTemplates\ParamDlgNew.h"
#include "StructWindow.h"
#include "Compute\Compute.h"
#include "window_tool\EmptyWindow.h"
#include "MessageText\ListMess.hpp"

namespace Structure
{
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
		StructWindow &e = *(StructWindow *)GetWindowLongPtr(h, GWLP_USERDATA);
		FrameViewer &frame =  e.viewers.get<FrameViewer>();
		MedianFiltreTable par;
		par.items.get< StructSig<MedianFiltreWidth>>().value = frame.medianFiltreWidth;
		par.items.get< StructSig<MedianFiltreON>>().value = frame.medianFiltreON;
		if(Dialog::Templ<NullType, MedianFiltreTable
			, TL::MkTlst<
			StructSig<MedianFiltreWidth>, StructSig<MedianFiltreON>
			>::Result
			, 550
			, TL::MkTlst<DefOkBtn, CancelBtn>::Result
		>(par).Do(h, L"Настройки медианного фильтра"))
		{
			frame.medianFiltreWidth = par.items.get< StructSig<MedianFiltreWidth>>().value;
			frame.medianFiltreON = par.items.get< StructSig<MedianFiltreON>>().value;

			Repaint<StructSig>(e.viewers.get<StructWindow::Str>(), frame);
		}
	}

	MIN_EQUAL_VALUE(StructSig<CutoffFrequency>, 0)
		MAX_EQUAL_VALUE(StructSig<CutoffFrequency>, 4000)
		PARAM_TITLE(StructSig<CutoffFrequency>, L"Частота отсечения фильтра")
		PARAM_TITLE(StructSig<CutoffFrequencyON>, L"Включение фильтра")

		void FilterDlg::Do(HWND h)
	{
		StructWindow &e = *(StructWindow *)GetWindowLongPtr(h, GWLP_USERDATA);
		FrameViewer &frame =  e.viewers.get<FrameViewer>();
		AnalogFilterTable par;
		par.items.get< StructSig<CutoffFrequency>>().value = frame.cutoffFrequency;
		par.items.get< StructSig<CutoffFrequencyON>>().value = frame.cutoffFrequencyON;
		if(Dialog::Templ<NullType, AnalogFilterTable
			, TL::MkTlst<
			StructSig<CutoffFrequency>
			, StructSig<CutoffFrequencyON>
			>::Result
			, 550
			, TL::MkTlst<DefOkBtn, CancelBtn>::Result
		>(par).Do(h, L"Настройки цифрового фильтра"))
		{
			frame.cutoffFrequency = par.items.get< StructSig<CutoffFrequency>>().value;
			frame.cutoffFrequencyON = par.items.get< StructSig<CutoffFrequencyON>>().value;

			Repaint<StructSig>( e.viewers.get<StructWindow::Str>(), frame);
		}
	}

	MIN_EQUAL_VALUE( StructSig<KoeffSign>, 0.1)
		MAX_EQUAL_VALUE( StructSig<KoeffSign>, 2.0)
		PARAM_TITLE( StructSig<KoeffSign>, L"Коэффициент")

		void CorrectionSensorDlg::Do(HWND h)
	{
		StructWindow &e = *(StructWindow *)GetWindowLongPtr(h, GWLP_USERDATA);
		FrameViewer &frame =  e.viewers.get<FrameViewer>();
		KoeffSignTable koef;
		koef.items.get<StructSig<KoeffSign>>().value = frame.koef;
		if(Dialog::Templ<NullType, KoeffSignTable
			, TL::MkTlst<StructSig<KoeffSign>>::Result
			, 550
			, TL::MkTlst<DefOkBtn, CancelBtn>::Result
			>(koef).Do(h, L"Корректировка датчика"))
		{
			StructWindow &e = *(StructWindow *)GetWindowLongPtr(h, GWLP_USERDATA);
			FrameViewer &frame =  e.viewers.get<FrameViewer>();
			frame.koef = koef.items.get<StructSig<KoeffSign>>().value;

			Repaint<StructSig>(e.viewers.get<StructWindow::Str>(), frame);
		}
	}

	template<class Table, class Item, class Param, class Type, Type Param::*>struct __test_change_param__;

	template<>struct __test_change_param__<KoeffSignTable, StructSig<KoeffSign>, FrameViewer, double, &FrameViewer::koef>{};

	template<>struct __test_change_param__<AnalogFilterTable, StructSig<CutoffFrequency>, FrameViewer, int, &FrameViewer::cutoffFrequency>{};
	template<>struct __test_change_param__<AnalogFilterTable, StructSig<CutoffFrequencyON>, FrameViewer, bool, &FrameViewer::cutoffFrequencyON>{};

	template<>struct __test_change_param__<MedianFiltreTable, StructSig<MedianFiltreWidth>, FrameViewer, int, &FrameViewer::medianFiltreWidth>{};
	template<>struct __test_change_param__<MedianFiltreTable, StructSig<MedianFiltreON>, FrameViewer, bool, &FrameViewer::medianFiltreON>{};

	typedef TL::MkTlst<
		__test_change_param__<KoeffSignTable   , StructSig<KoeffSign>        , FrameViewer, double, &FrameViewer::koef>
		, __test_change_param__<AnalogFilterTable, StructSig<CutoffFrequency>  , FrameViewer, int,    &FrameViewer::cutoffFrequency>
		, __test_change_param__<AnalogFilterTable, StructSig<CutoffFrequencyON>, FrameViewer, bool,   &FrameViewer::cutoffFrequencyON>
		, __test_change_param__<MedianFiltreTable, StructSig<MedianFiltreWidth>, FrameViewer, int,    &FrameViewer::medianFiltreWidth>
		, __test_change_param__<MedianFiltreTable, StructSig<MedianFiltreON>   , FrameViewer, bool,   &FrameViewer::medianFiltreON>
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
				UpdateWhere<Table>(Singleton<Table>::Instance(), p.base).ID(id).Execute();
			}
		}
	};


	bool TestChangeParam(HWND h)
	{
		StructWindow &e = *(StructWindow *)GetWindowLongPtr(h, GWLP_USERDATA);
		FrameViewer &frame =  e.viewers.get<FrameViewer>();

		return TL::find<__param_list__, __test_param__>()(frame);
	}

	void  StoreParam(HWND h)
	{
		if(TypesizePasswordDlg().Do(h))
		{
			StructWindow &e = *(StructWindow *)GetWindowLongPtr(h, GWLP_USERDATA);
			FrameViewer &frame =  e.viewers.get<FrameViewer>();
			CBase base(ParametersBase().name());
			if(base.IsOpen())
			{
				TL::foreach<__param_list__, __store_param__>()(__store_param_data__(frame, base));
			}
		}
	}

	MIN_EQUAL_VALUE(StructSig<ViewerCount>, 100)
	MAX_EQUAL_VALUE(StructSig<ViewerCount>, 30000)
	PARAM_TITLE(StructSig<ViewerCount>, L"Ширина")

	void FrameWidthViewDlg::Do(HWND h)
	{
		if(Dialog::Templ<ParametersBase, ViewerCountTable
			, TL::MkTlst<
			StructSig<ViewerCount>
			>::Result
		>(Singleton<ViewerCountTable>::Instance()).Do(h, L"Ширина кадра"))
		{
			StructWindow &e = *(StructWindow *)GetWindowLongPtr(h, GWLP_USERDATA);
			FrameViewer &frame =  e.viewers.get<FrameViewer>();
			frame.count = Singleton<ViewerCountTable>::Instance().items.get<StructSig<ViewerCount>>().value;
		}
	}
}

