#include "Dialogs.h"
#include "DlgTemplates\ParamDlgNew.h"

namespace
{
	MIN_EQUAL_VALUE( DefectSig<KoeffSign>, 0.1)
	MAX_EQUAL_VALUE( DefectSig<KoeffSign>, 2.0)
	PARAM_TITLE( DefectSig<KoeffSign>, L"Коэффициент дефектоскоп")

	MIN_EQUAL_VALUE( StructSig<KoeffSign>, 0.1)
	MAX_EQUAL_VALUE( StructSig<KoeffSign>, 2.0)
	PARAM_TITLE( StructSig<KoeffSign>, L"Коэффициент структура")
}

void KoefDlg::Do(HWND h)
{
	if(Dialog::Templ<ParametersBase, KoeffSignTable>(
		Singleton<KoeffSignTable>::Instance()
		).Do(h, L"Корректировка датчиков"))
	{
	}
}