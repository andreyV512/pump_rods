#include "RunExecute.h"
#include <Windows.h>

bool RunExecute::operator()(wchar_t *path)
{
	PROCESS_INFORMATION pi;
	STARTUPINFO si = {};
	si.dwFlags = STARTF_USESHOWWINDOW;
	si.wShowWindow =  SW_SHOWNORMAL;
	si.cb = sizeof(STARTUPINFO);
	
	if(CreateProcess(NULL, path, NULL, NULL, FALSE, 0, NULL, NULL, &si, &pi))
	{
		CloseHandle(pi.hThread); 
		CloseHandle(pi.hProcess);
		return true;
	}
	return false;
}
