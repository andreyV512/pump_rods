#include "Dialogs.h"
#include "DlgTemplates\ParamDlgNew.h"

MIN_EQUAL_VALUE(DefectSig<CutoffFrequency>, 0)
MAX_EQUAL_VALUE(DefectSig<CutoffFrequency>, 4000)
PARAM_TITLE(DefectSig<CutoffFrequency>, L"Частота отсечения фильтра")
PARAM_TITLE(DefectSig<CutoffFrequencyON>, L"Включение фильтра")

MIN_EQUAL_VALUE(StructSig<CutoffFrequency>, 0)
MAX_EQUAL_VALUE(StructSig<CutoffFrequency>, 4000)
PARAM_TITLE(StructSig<CutoffFrequency>, L"Частота отсечения фильтра")
PARAM_TITLE(StructSig<CutoffFrequencyON>, L"Включение фильтра")

PARAM_TITLE(GROUP_BOX(DefectSig<CutoffFrequency>, DefectSig<CutoffFrequencyON>), L"Дефектоскопия")
PARAM_TITLE(GROUP_BOX(StructSig<CutoffFrequency>, StructSig<CutoffFrequencyON>), L"Структура")

template<class T>struct RadioItems
{
	HWND Init(HWND h, int &x, int &width, int &dy, T &t)
	{
		HWND hWnd = CreateWindow(L"button", ParamTitle<T>()()
				,  WS_CHILD | WS_VISIBLE | BS_AUTORADIOBUTTON 
				, x, dy, dlg_width, 15, h, NULL, (HINSTANCE)::GetModuleHandle(NULL), NULL
				);
		Button_SetCheck(hWnd, t.value ? BST_CHECKED : BST_UNCHECKED);
		dy += 20;
		if(width < dlg_width) width = dlg_width;
		return hWnd;
	}
};

template<class T>struct RadioItemsGroup
{
	HWND Init(HWND h, int &x, int &width, int &dy, T &t)
	{
		HWND hWnd = CreateWindow(L"button", ParamTitle<T>()()
				,  WS_CHILD | WS_VISIBLE | BS_AUTORADIOBUTTON | WS_GROUP
				, x, dy, dlg_width, 15, h, NULL, (HINSTANCE)::GetModuleHandle(NULL), NULL
				);
		Button_SetCheck(hWnd, t.value ? BST_CHECKED : BST_UNCHECKED);
		dy += 20;
		if(width < dlg_width) width = dlg_width;
		return hWnd;
	}
};

void FilterDlg::Do(HWND h)
{
	if(Dialog::Templ<ParametersBase, AnalogFilterTable
		, TL::MkTlst<
		  GROUP_BOX(DefectSig<CutoffFrequency>, DefectSig<CutoffFrequencyON>)
		, GROUP_BOX(StructSig<CutoffFrequency>, StructSig<CutoffFrequencyON>)
		>::Result
	>(Singleton<AnalogFilterTable>::Instance()).Do(h, L"Настройки цифрового фильтра"))
	{
	}
}

