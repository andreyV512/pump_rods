#include "Dialogs.h"
#include "window_tool\RunExecute.h"
#include <Shlwapi.h>

#pragma warning( disable : 4996 )
void MessageDlg::Do(HWND )
{
	wchar_t path[1024];
	GetModuleFileName(0, path, dimention_of(path));
	PathRemoveFileSpec(path);
	wcscat(path, L"\\Messages.exe");
	RunExecute()(path);
}