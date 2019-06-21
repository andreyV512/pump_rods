#include "Dialogs.h"
#include "DlgTemplates\ParamDlgNew.h"

MIN_EQUAL_VALUE(CountStoredFiles, 10)
MAX_EQUAL_VALUE(CountStoredFiles, 5000)
PARAM_TITLE(CountStoredFiles, L"Количество сохраняемых файлов")
PARAM_TITLE(CountStoredFilesOn, L"Включение сохранения файлов")

void StoredFilesDlg::Do(HWND h)
{
	if(Dialog::Templ<ParametersBase, CountStoredFilesTable
	>(Singleton<CountStoredFilesTable>::Instance()).Do(h, L"Сохранение файлов измерений"))
	{
	}
}

