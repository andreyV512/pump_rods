#include "Dialogs.h"
#include "DlgTemplates\ParamDlgNew.h"

namespace
{
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
}

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

