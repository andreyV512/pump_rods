#include "Dialogs.h"
#include "DlgTemplates\ParamDlgNew.h"

MIN_EQUAL_VALUE(DefectSig<Axes>, 1)
MAX_EQUAL_VALUE(DefectSig<Axes>, 100)
PARAM_TITLE(DefectSig<Axes>, L"Максимальное значение оси Y дефектоскопа")

MIN_EQUAL_VALUE(StructSig<Axes>, 1)
MAX_EQUAL_VALUE(StructSig<Axes>, 100)
PARAM_TITLE(StructSig<Axes>, L"Максимальное значение оси Y структуры")

void AxesGraphsDlg::Do(HWND h)
{
	if(Dialog::Templ<ParametersBase, AxesGraphsTable>(
		Singleton<AxesGraphsTable>::Instance()
		).Do(h, L"Оси"))
	{
	}
}
