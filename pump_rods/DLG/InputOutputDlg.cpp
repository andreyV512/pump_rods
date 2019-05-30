#include "Dialogs.h"
#include "DlgTemplates\ParamDlgNew.h"

#define Param(n, txt)\
MIN_EQUAL_VALUE(n, 1)\
MAX_EQUAL_VALUE(n, 32)\
PARAM_TITLE(n, txt)\
template<>struct DlgSubItems<n, unsigned>: UpDownSubItem<n>{};


template<class O, class P>struct __unshift__
{
	void operator()(O &o, P &p)
	{
		unsigned val = p.get<O>().value;
		unsigned offs = 1;
		while(val >>= 1) ++offs;
		o.value = offs;
	}
};

Param(iCU     , L"Сигнал ЦЕПИ УПРАВЛЕНИЯ")
Param(iKM2_DC , L"Пускатель (КМ2 DC)")
Param(iKM3_AC , L"Пускатель (КМ3 AC)")
Param(iCycle  , L"Сигнал \"ЦИКЛ\"")
Param(iP1     , L"Сигнал транспортной системы (П1)")
Param(iP2     , L"Сигнал транспортной системы (П2)")
Param(iCOPT   , L"Сигнал \"СОРТ\"")
Param(iControl, L"Сигнал \"КОНТРОЛЬ\"")

Param(oDC_ON1 , L"Сигнал (DC ON1)")
Param(oAC_ON  , L"Сигнал (AC ON)")
Param(oDC_ON2 , L"Сигнал (DC ON2)")
Param(oWork   , L"Сигнал \"РАБОТА\"")
Param(oStart  , L"Сигнал \"ПУСК\"")
Param(oToShift, L"Сигнал \"ПЕРЕКЛАДКА\"")
Param(oC1     , L"Сигнал (С1)")
Param(oC2 	  , L"Сигнал (С2)")
#undef Param

typedef Dialog::Templ<ParametersBase, InputBitTable, InputBitTable::items_list, 350> TInputBitDlg;
template<class P>struct __data_from_widget__<Dialog::DlgItem2<P, TInputBitDlg>, unsigned>
{
    unsigned operator()(Dialog::DlgItem2<P, TInputBitDlg> &o)
	{
		wchar_t buf[128];
		GetWindowText(o.hWnd, buf, dimention_of(buf));
		return 1 << (Wchar_to<unsigned>()(buf) - 1);
	}
};

void InputBitDlg::Do(HWND h)
{
	InputBitTable par;
	TL::foreach<InputBitTable::items_list, __unshift__>()(par.items, Singleton<InputBitTable>::Instance().items);
	if(TInputBitDlg(par).Do(h, L"Смещение входных портов"))
	{
	}
}

typedef Dialog::Templ<ParametersBase, OutputBitTable, OutputBitTable::items_list, 300> TOutputBitDlg;
template<class P>struct __data_from_widget__<Dialog::DlgItem2<P, TOutputBitDlg>, unsigned>
{
    unsigned operator()(Dialog::DlgItem2<P, TOutputBitDlg> &o)
	{
		wchar_t buf[128];
		GetWindowText(o.hWnd, buf, dimention_of(buf));
		return 1 << (Wchar_to<unsigned>()(buf) - 1);
	}
};

void OutputBitDlg::Do(HWND h)
{
	OutputBitTable par;
	TL::foreach<OutputBitTable::items_list, __unshift__>()(par.items, Singleton<OutputBitTable>::Instance().items);
	if(TOutputBitDlg(par).Do(h, L"Смещение выходных портов"))
	{
	}
}

DO_NOT_CHECK(NamePlate1730)
PARAM_TITLE(NamePlate1730, L"Дескриптор")

void DiscriptorBitDlg::Do(HWND h)
{
	if(Dialog::Templ<ParametersBase, NamePlate1730ParametersTable
	>(Singleton<NamePlate1730ParametersTable>::Instance()).Do(h, L"Дескриптор платы"))
	{
	}
}