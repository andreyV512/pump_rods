#include "ZipAll.h"
#include <Windows.h>
#include "Zip.h"
#include "StoredFiles.h"

#pragma warning(disable: 4996)

namespace Zip
{
	DWORD WINAPI  __ZipAll__(LPVOID)
	{
		wchar_t path[1024];
		StoredFiles::PathUp(path, L"Stored");
		int len = wcslen(path);
		wcscat(path, L"\\*.dat");
		++len;

		WIN32_FIND_DATA ffd;
		HANDLE  hFind = FindFirstFile(path, &ffd);
		if(INVALID_HANDLE_VALUE == hFind) return 0;	

		do
		{
			if (0 == (ffd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY))
			{
				wcscpy(&path[len], ffd.cFileName);
				Zip::ZipFile(path);
			}
		}
		while (FindNextFile(hFind, &ffd) != 0);
		FindClose(hFind);

		return 0;
	}

	void ZipAll()
	{
		HANDLE hMutex = CreateMutex(NULL, FALSE, L"__ZipAll__20190620");
		if(GetLastError() != ERROR_ALREADY_EXISTS)QueueUserWorkItem(__ZipAll__, NULL, WT_EXECUTEDEFAULT);
		CloseHandle(hMutex);
	}
}