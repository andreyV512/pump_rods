#include "Dialogs.h"
#include "DlgTemplates\ParamDlgNew.h"

MIN_EQUAL_VALUE(DefectSig<MedianFiltreWidth>, 3)
MAX_EQUAL_VALUE(DefectSig<MedianFiltreWidth>, 15)
PARAM_TITLE(DefectSig<MedianFiltreWidth>, L"Ширина фильтра")
PARAM_TITLE(DefectSig<MedianFiltreON>, L"Включение фильтра")

MIN_EQUAL_VALUE(StructSig<MedianFiltreWidth>, 3)
MAX_EQUAL_VALUE(StructSig<MedianFiltreWidth>, 15)
PARAM_TITLE(StructSig<MedianFiltreWidth>, L"Ширина фильтра")
PARAM_TITLE(StructSig<MedianFiltreON>, L"Включение фильтра")

PARAM_TITLE(GROUP_BOX(DefectSig<MedianFiltreWidth>, DefectSig<MedianFiltreON>), L"Дефектоскопия")
PARAM_TITLE(GROUP_BOX(StructSig<MedianFiltreWidth>, StructSig<MedianFiltreON>), L"Структура")

//template<class T>struct RadioItems
//{
//	HWND Init(HWND h, int &x, int &width, int &dy, T &t)
//	{
//		HWND hWnd = CreateWindow(L"button", ParamTitle<T>()()
//				,  WS_CHILD | WS_VISIBLE | BS_AUTORADIOBUTTON 
//				, x, dy, dlg_width, 15, h, NULL, (HINSTANCE)::GetModuleHandle(NULL), NULL
//				);
//		Button_SetCheck(hWnd, t.value ? BST_CHECKED : BST_UNCHECKED);
//		dy += 20;
//		if(width < dlg_width) width = dlg_width;
//		return hWnd;
//	}
//};
//
//template<class T>struct RadioItemsGroup
//{
//	HWND Init(HWND h, int &x, int &width, int &dy, T &t)
//	{
//		HWND hWnd = CreateWindow(L"button", ParamTitle<T>()()
//				,  WS_CHILD | WS_VISIBLE | BS_AUTORADIOBUTTON | WS_GROUP
//				, x, dy, dlg_width, 15, h, NULL, (HINSTANCE)::GetModuleHandle(NULL), NULL
//				);
//		Button_SetCheck(hWnd, t.value ? BST_CHECKED : BST_UNCHECKED);
//		dy += 20;
//		if(width < dlg_width) width = dlg_width;
//		return hWnd;
//	}
//};

void MedianFilterDlg::Do(HWND h)
{
	if(Dialog::Templ<ParametersBase, MedianFiltreTable
		, TL::MkTlst<
		  GROUP_BOX(DefectSig<MedianFiltreWidth>, DefectSig<MedianFiltreON>)
		, GROUP_BOX(StructSig<MedianFiltreWidth>, StructSig<MedianFiltreON>)
		>::Result
	>(Singleton<MedianFiltreTable>::Instance()).Do(h, L"Настройки медианного фильтра"))
	{
	}
}

