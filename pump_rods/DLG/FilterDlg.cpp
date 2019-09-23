#include "Dialogs.h"
#include "DlgTemplates\ParamDlgNew.h"

MIN_EQUAL_VALUE(DefectSig<CutoffFrequency>, 0)
MAX_EQUAL_VALUE(DefectSig<CutoffFrequency>, 4000)
PARAM_TITLE(DefectSig<CutoffFrequency>, L"Средняя частота фильтра")

MIN_EQUAL_VALUE(DefectSig<WidthFrequency>, 0)
MAX_EQUAL_VALUE(DefectSig<WidthFrequency>, 1000)
PARAM_TITLE(DefectSig<WidthFrequency>, L"Ширина пропускания фильтра")

PARAM_TITLE(DefectSig<CutoffFrequencyON>, L"Включение фильтра")

MIN_EQUAL_VALUE(StructSig<CutoffFrequency>, 0)
MAX_EQUAL_VALUE(StructSig<CutoffFrequency>, 4000)
PARAM_TITLE(StructSig<CutoffFrequency>, L"Частота отсечения фильтра")
PARAM_TITLE(StructSig<CutoffFrequencyON>, L"Включение фильтра")

typedef GROUP_BOX(DefectSig<CutoffFrequency>, DefectSig<WidthFrequency>, DefectSig<CutoffFrequencyON>) __def_param__;
PARAM_TITLE(__def_param__, L"Дефектоскопия")
	typedef GROUP_BOX(StructSig<CutoffFrequency>, StructSig<CutoffFrequencyON>) __str_param__;
PARAM_TITLE(__str_param__, L"Структура")

void FilterDlg::Do(HWND h)
{
	if(Dialog::Templ<ParametersBase, AnalogFilterTable
		, TL::MkTlst<
	      __def_param__
		, __str_param__
		>::Result
	>(Singleton<AnalogFilterTable>::Instance()).Do(h, L"Настройки цифрового фильтра"))
	{
	}
}

