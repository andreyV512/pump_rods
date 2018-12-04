#include "stdafx.h"
#include "ZipAll.h"
#include "Zip.h"
#include <Windows.h>

#pragma warning(disable: 4996)

namespace Zip
{
	DWORD WINAPI  __ZipAll__(LPVOID)
	{
		HANDLE hMutex = CreateMutex(NULL, FALSE, L"__ZipAll__201609061048");
	    if(GetLastError() == ERROR_ALREADY_EXISTS) return 0;
		WIN32_FIND_DATA fd; 
		ProgrammDir pd;
		wchar_t *path = pd.Get();
		int len = (int)wcslen(path);
		wcscat(path, L"\\..\\Stored\\*.dat");
		HANDLE hFindFile = FindFirstFile(path, &fd); 
		PROCESS_INFORMATION pi = {};
		if(INVALID_HANDLE_VALUE != hFindFile)
		{
			do
			{
				if(SetName(fd.cFileName))
				{
					ZipFile(fd.cFileName);					
					UnsetName(fd.cFileName);
				}
			}
			while(FindNextFile(hFindFile, &fd)); 
			FindClose(hFindFile); 
		}
		CloseHandle(hMutex);
		return 0;
	}

	void ZipAll()
	{
		QueueUserWorkItem(__ZipAll__, NULL, WT_EXECUTEDEFAULT);
	}
}