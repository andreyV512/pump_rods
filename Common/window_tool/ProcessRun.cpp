#include "stdafx.h"
#include "ProcessRun.h"
#include <tlhelp32.h>
//---------------------------------------------------------------------------
bool IsProcessRun( wchar_t *processName )
{
	HANDLE hSnapshot = CreateToolhelp32Snapshot( TH32CS_SNAPPROCESS, 0 );

	int len = (int)wcslen(processName);
	wchar_t *pn = &processName[len - 1];
	for(int i = 0; i < len && ('\\' != *pn && '/' != *pn); --i, --pn);
	++pn;

	PROCESSENTRY32 pe;
	pe.dwSize = sizeof( PROCESSENTRY32 );
	Process32First( hSnapshot, &pe );

	while(true) {
		if( 0 == wcscmp( pe.szExeFile, pn )) return true;
		if( !Process32Next( hSnapshot, &pe ) ) return false;
	}
	return false;
}
#pragma warning(disable : 4996)
bool ProcessRun(wchar_t *processName, wchar_t *path)
{
	STARTUPINFO si = { sizeof(si) };
	PROCESS_INFORMATION pi;	
	wchar_t buf[1024];
	wcscpy(buf, processName);
	int len = (int)wcslen(buf);
	buf[len] = ' ';
	wcscpy(&buf[len + 1], path);
	return 0 != CreateProcess(
		NULL, buf, NULL, NULL, FALSE,
		NORMAL_PRIORITY_CLASS, NULL, NULL, &si, &pi
		);
	return false;
}