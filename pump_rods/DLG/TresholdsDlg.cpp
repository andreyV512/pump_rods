#include "Dialogs.h"
#include "DlgTemplates\ParamDlgNew.h"

TEMPL_MIN_EQUAL_VALUE(Thresh<SortDown>, 0)
TEMPL_MIN_EQUAL_VALUE(Thresh<Defect>, 0)

TEMPL_MAX_EQUAL_VALUE(Thresh<SortDown>, 100)
TEMPL_MAX_EQUAL_VALUE(Thresh<Defect>, 100)

TEMPL_PARAM_TITLE(Thresh<SortDown>, L"Сорт")
TEMPL_PARAM_TITLE(Thresh<Defect>, L"Дефект")

typedef GROUP_BOX(DefectSig<Thresh<SortDown>>, DefectSig<Thresh<Defect>>) title_def;
typedef GROUP_BOX(StructSig<Thresh<SortDown>>, StructSig<Thresh<Defect>>) title_sort;
PARAM_TITLE(title_def, L"Дефектоскопия")
PARAM_TITLE(title_sort, L"Структура")

void TresholdsDlg::Do(HWND h)
{
	if(Dialog::Templ<ParametersBase, ThresholdsTable
		, TL::MkTlst<
		 title_def
		, title_sort
		>::Result
		, 300
	>(Singleton<ThresholdsTable>::Instance()).Do(h, L"Настройки порогов"))
	{
	}
}
