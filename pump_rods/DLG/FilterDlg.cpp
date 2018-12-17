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

