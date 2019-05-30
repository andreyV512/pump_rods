#include <Windows.h>
#include "Dlg/Dialogs.h"
#include "window_tool\Emptywindow.h"
#include "window_tool\MenuAPI.h"
#include "App\AppBase.h"
#include "DlgTemplates\ParamDlg.hpp"
#include "window_tool\Emptywindow.h"
#include "window_tool\WindowsPosition.h"
#include "DlgTemplates\ParamDlgNew.h"
#include "window_tool\TEvent.h"
#include "window_tool\GroupBox.h"
#include "App/App.h"

class TestIOWindow;	
namespace TestIOPortsN
{
	template<class T>struct OutStat;
	template<class T>struct InpStat;

#define INPUT_STAT_TYPES_0\
	iCU\
	, iKM2_DC \
	, iKM3_AC \
	, iCycle  \
	, iP1     \
	, iP2     \
	, iCOPT   \
	, iControl
	

#define OUTPUT_STAT_TYPES_0\
	oDC_ON1   \
	, oAC_ON  \
	, oDC_ON2 \
	, oWork   \
	, oStart  \
	, oToShift\
	, oC1     \
	, oC2    

#define INPUT_WHAT(n)  DEFINE_PARAM_WAPPER(InpStat, n, bool, false)
	FOR_EACH(INPUT_WHAT, INPUT_STAT_TYPES_0)
#undef INPUT_WHAT

#define DLG_SUB_ITEMS(n)template<>struct DlgSubItems<InpStat<n> , bool>: DlgItemsRadio<InpStat<n>>{};
		FOR_EACH(DLG_SUB_ITEMS, INPUT_STAT_TYPES_0)
#undef DLG_SUB_ITEMS

#define INPUT_WHAT(n)  DEFINE_PARAM_WAPPER(OutStat, n, bool, false)
	FOR_EACH(INPUT_WHAT, OUTPUT_STAT_TYPES_0)
#undef INPUT_WHAT

#undef INPUT_STAT_TYPES_0
#undef OUTPUT_STAT_TYPES_0

		PARAM_TITLE(InpStat<iCU     >, L"Сигнал ЦЕПИ УПРАВЛЕНИЯ")
		PARAM_TITLE(InpStat<iKM2_DC >, L"Пускатель (КМ2 DC)")
		PARAM_TITLE(InpStat<iKM3_AC >, L"Пускатель (КМ3 AC)")
		PARAM_TITLE(InpStat<iCycle  >, L"Сигнал \"ЦИКЛ\"")
		PARAM_TITLE(InpStat<iP1     >, L"Сигнал транспортной системы (П1)")
		PARAM_TITLE(InpStat<iP2     >, L"Сигнал транспортной системы (П2)")
		PARAM_TITLE(InpStat<iCOPT   >, L"Сигнал \"СОРТ\"")
		PARAM_TITLE(InpStat<iControl>, L"Сигнал \"КОНТРОЛЬ\"")

		typedef GROUP_BOX(
		InpStat<iCU     >
		, InpStat<iKM2_DC >
		, InpStat<iKM3_AC >
		, InpStat<iCycle  >
		, InpStat<iP1     >
		, InpStat<iP2     >
		, InpStat<iCOPT   >
		, InpStat<iControl>
		) GBInput;
	PARAM_TITLE(GBInput , L"Вход")

	typedef TL::MkTlst<
		GBInput
	>::Result __input_list_0__;

		typedef GROUP_BOX(
		OutStat<oDC_ON1 >
		, OutStat<oAC_ON  >
		, OutStat<oDC_ON2 >
		, OutStat<oWork   >
		, OutStat<oStart  >
		, OutStat<oToShift>
		, OutStat<oC1     >
		, OutStat<oC2 	>
		) GBOutput;
		PARAM_TITLE(GBOutput, L"Выход")

		PARAM_TITLE(OutStat<oDC_ON1 >, L"Сигнал (DC ON1)")
		PARAM_TITLE(OutStat<oAC_ON  >, L"Сигнал (AC ON)")
		PARAM_TITLE(OutStat<oDC_ON2 >, L"Сигнал (DC ON2)")
		PARAM_TITLE(OutStat<oWork   >, L"Сигнал \"РАБОТА\"")
		PARAM_TITLE(OutStat<oStart  >, L"Сигнал \"ПУСК\"")
		PARAM_TITLE(OutStat<oToShift>, L"Сигнал \"ПЕРЕКЛАДКА\"")
		PARAM_TITLE(OutStat<oC1     >, L"Сигнал (С1)")
		PARAM_TITLE(OutStat<oC2 	>, L"Сигнал (С2)")

		typedef TL::MkTlst<
		GBOutput
		>::Result __output_list_0__;

	template<class T>struct __data_io__
	{
		T &items;
		unsigned msk;
		unsigned &value;
		__data_io__(T &items, unsigned msk, unsigned &value): items(items), msk(msk), value(value){}
	};

	template<class O, class P>struct __i__
	{
		bool operator()(O &o, P &p)
		{
			if(p.msk)
			{
				if(p.msk & o.value)
				{
					p.msk &= ~o.value;
					HWND h = p.items.get<Dialog::DlgItem2<InpStat<O>,NullType>>().hWnd;
					unsigned t = (o.value & p.value) ? BST_CHECKED : BST_UNCHECKED;
					Button_SetCheck(h, t);
				}
				return true;
			}
			return false;
		}
	};	
	template<class O, class P>struct __o__
	{
		bool operator()(O &o, P &p)
		{
			if(p.msk)
			{
				if(p.msk & o.value)
				{
					p.msk &= ~o.value;
					HWND h = p.items.get<Dialog::DlgItem2<OutStat<O>,NullType>>().hWnd;
					bool b = 0 != (o.value & p.value);
					unsigned t = (b) ? BST_CHECKED : BST_UNCHECKED;
					Button_SetCheck(h, t);
					if(b)
					{
						//p.value |= o.value;
						device1730.WriteOutput(o.value);
					}
					else
					{
						//p.value &= ~o.value;
						device1730.WriteOutput(0, o.value);
					}
				}
				return true;
			}
			return false;
		}
	};	
}									   

class TestIOWindow					   
{
	template<class O, class P>struct init_check_box
	{
		void operator()(O &o, P &p)
		{
		   SetWindowLongPtr(o.hWnd, GWLP_USERDATA, (LONG)&p.get<CheckBox<O>>());
		}
	};
	template<class T>class CheckBox: public TEvent
	{
		TestIOWindow *owner;
	public:
		CheckBox(TestIOWindow *owner): owner(owner){}
		void Do(TCommand &l)
		{
			typedef TL::Inner<TL::Inner<T>::Result>::Result Tbit;
			unsigned bit = Singleton<OutputBitTable>::Instance().items.get<Tbit>().value;
			bool b = BST_CHECKED == Button_GetCheck(l.hControl);
			if(b)
			{
				device1730.WriteOutput(bit);
			}
			else
			{
				device1730.WriteOutput(0, bit);
			}
		}
	};

	unsigned lastBitsInput, lastBitsOutput;

	typedef TL::TypeToTypeLstParam1<TestIOPortsN::__input_list_0__, Dialog::DlgItem2, NullType>::Result original_list_input0;
	typedef TL::TypeToTypeLstParam1<TestIOPortsN::__output_list_0__, Dialog::DlgItem2, NullType>::Result original_list_output0;
public:
	typedef TL::TypeToTypeLstParam1<Dialog::__del_group_box__<TestIOPortsN::__input_list_0__>::Result, Dialog::DlgItem2, NullType>::Result list_input0;
	typedef TL::TypeToTypeLstParam1<Dialog::__del_group_box__<TestIOPortsN::__output_list_0__>::Result, Dialog::DlgItem2, NullType>::Result list_output0;
private:
	typedef TL::TypeToTypeLst<list_output0, CheckBox>::Result list_check_box;
	TL::Factory<list_check_box> items_check_box;
	int idTimer;
public:
	HWND hWnd;
	bool lastIsRun;
	TL::Factory<list_input0> items_input0;
	TL::Factory<list_output0> items_output0;
	TestIOWindow();
	LRESULT operator()(TCreate &l);
	void operator()(TDestroy &);
	void operator()(TCommand &);
	void operator()(TTimer &);
};

TestIOWindow::TestIOWindow()
	: items_check_box(this)
{}

LRESULT TestIOWindow::operator()(TCreate &l)
{
	lastBitsInput  = ~device1730.Read();
	lastBitsOutput = ~device1730.ReadOutput();
	lastIsRun = !App::IsRun();

	int width = 280;
	int height = 15;
	int xOffs = 5;

	HWND h = GroupBoxWindow(L"", 3, 5, 565, 208, l.hwnd);

	TL::foreach<original_list_input0, Dialog::__init__X>()(Dialog::__table_data__X<TL::Factory<list_input0>>(h, xOffs, width, height, items_input0));
	width = 280;
	height = 15;
	xOffs = 290;
	TL::foreach<original_list_output0, Dialog::__init__X>()(Dialog::__table_data__X<TL::Factory<list_output0>>(h, xOffs, width, height, items_output0));
	TL::foreach<list_output0, init_check_box>()(items_output0, items_check_box);

	SetTimer(l.hwnd, idTimer, 300, NULL);
	return 0;
}

void TestIOWindow::operator()(TDestroy &l)
{	
	KillTimer(l.hwnd, idTimer);
	WindowPosition::Set<TestIOWindow>(l.hwnd);
	SetWindowLongPtr(l.hwnd, GWLP_USERDATA, NULL);
	delete this;
}

void TestIOWindow::operator()(TCommand &l)
{
	EventDo(l);
}

namespace
{
	template<class O, class P>struct __enable__
	{
		void operator()(O &o, P &p)
		{
			EnableWindow(o.hWnd, p);
		}
	};
}

void TestIOWindow::operator()(TTimer &)
{
	bool isRun = App::IsRun();
	if(lastIsRun != isRun)
	{
		TL::foreach<TestIOWindow::list_output0, __enable__>()(items_output0, isRun);
		lastIsRun = isRun;
	}
	unsigned test_testIOWindow = device1730.Read();
	TestIOPortsN::__data_io__<TL::Factory<list_input0>> idata(items_input0, test_testIOWindow ^ lastBitsInput, test_testIOWindow);
	TL::find<InputBitTable::items_list, TestIOPortsN::__i__>()(Singleton<InputBitTable>::Instance().items, idata);
	lastBitsInput = test_testIOWindow;  
	test_testIOWindow = device1730.ReadOutput();
	TestIOPortsN::__data_io__<TL::Factory<list_output0>> odata(items_output0, test_testIOWindow ^ lastBitsOutput, test_testIOWindow);
	TL::find<OutputBitTable::items_list, TestIOPortsN::__o__>()(Singleton<OutputBitTable>::Instance().items, odata);
	lastBitsOutput = test_testIOWindow; 	
}

void TestIOPorts::Do(HWND)
{
	HWND h = FindWindow(WindowClass<TestIOWindow>()(), 0);
	if(NULL != h)
	{
		SendMessage(h, WM_SYSCOMMAND, SC_RESTORE, 0);
		SetForegroundWindow(h);
	}
	else
	{
		RECT r;
		WindowPosition::Get<TestIOWindow>(r);
		const int width = 590;
		const int height = 270;
		h = WindowTemplate(
			new TestIOWindow
			, L"Порты ввода-вывода"
			, r.left, r.top
			, width, height
			, IDI_settings
			, (HBRUSH)COLOR_WINDOW
			);

		DWORD dwStyle = GetWindowLongPtr(h, GWL_STYLE);
		dwStyle &= ~(WS_MAXIMIZEBOX | WS_MINIMIZEBOX|WS_THICKFRAME);
		SetWindowLongPtr(h, GWL_STYLE, dwStyle);
		ShowWindow(h, SW_SHOWNORMAL);

		SetWindowPos(h, HWND_TOPMOST, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);
	}
}

void TestIOPortWindowEnable(bool b)
{
	HWND h = FindWindow(WindowClass<TestIOWindow>()(), 0);
	if(NULL != h)
	{
		TestIOWindow &o = *(TestIOWindow *)GetWindowLongPtr(h, GWLP_USERDATA);
		TL::foreach<TestIOWindow::list_output0, __enable__>()(o.items_output0, b);
	}
}