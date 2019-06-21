#pragma once
#include <windows.h>
#include "templates/typelist.hpp"
//-----------------------------------------------------------------
class CopyFactory
{
template<class O, class P>struct loc
{
	void operator ()(O *o, P *p)
	{
		o->value = p->get<O>().value;
	}
};
public:
	template<class List> void operator()(TL::Factory<List> &to, TL::Factory<List> &from)
	{
		TL::foreach<List, loc>()(&to, &from);
	}
};
//-------------------------------------------------------------------
#define DLG(n)struct n{static void Do(HWND);};

DLG(FilterDlg)
DLG(MedianFilterDlg)
DLG(AnalogPlatelDlg)
DLG(InputBitDlg)
DLG(OutputBitDlg)
DLG(DiscriptorBitDlg)
DLG(DeathZonesDlg)
DLG(ColorItemsDlg)
DLG(AddTypeSizeDlg)
DLG(DelTypeSizeDlg)
DLG(TestIOPorts)
DLG(AboutWindowDlg)
DLG(KoefDlg)
DLG(MessageDlg)
DLG(StoreDlg)
DLG(LoadDlg)
DLG(TresholdsDlg)
DLG(AxesGraphsDlg)
DLG(StoredFilesDlg)
#undef DLG

void AutoStoredData();
