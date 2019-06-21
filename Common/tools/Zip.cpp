#include "Zip.h"
#include <stdio.h>
#include <Shlwapi.h>
#include "templates/typelist.hpp"
#include "StoredFiles.h"
#include "tools_debug/DebugMess.h"

#pragma warning(disable: 4996)

namespace Zip
{
	void ZipFile(wchar_t *fileName)
	{
		wchar_t path[1024];
		StoredFiles::PathExec(path);
		int length = wcslen(path);
		wsprintf(&path[length], L"\\bzip2 -z -f %s", fileName);

		PROCESS_INFORMATION pi;
		STARTUPINFO si = {};
		si.dwFlags = STARTF_USESHOWWINDOW;
		si.wShowWindow =  SW_HIDE;
		si.cb = sizeof(STARTUPINFO); 

		BOOL b = CreateProcess(
			NULL
			, path
			, NULL
			, NULL
			, FALSE
			, CREATE_NEW_CONSOLE
			, NULL
			, NULL
			, &si
			, &pi
			);

		if(b)
		{
			WaitForSingleObject(pi.hProcess, INFINITE); 

			CloseHandle(pi.hThread); 
			CloseHandle(pi.hProcess);
		}
	}
	void UnZipFile(wchar_t *nameFile)
	{
		wchar_t path[1024];
		StoredFiles::PathExec(path);
		int length = wcslen(path);

		PROCESS_INFORMATION pi;
		STARTUPINFO si = {};
		si.cb = sizeof(STARTUPINFO);
		si.dwFlags = STARTF_USESHOWWINDOW;
		si.wShowWindow =  SW_HIDE;

		wsprintf(&path[length], L"\\bzip2 -d  %s", nameFile);
		BOOL b = CreateProcess(
			NULL
			, path
			, NULL
			, NULL
			, FALSE
			, 0
			, NULL
			, NULL
			, &si
			, &pi
			);
		if(b)
		{
			WaitForSingleObject(pi.hProcess, INFINITE); 

			CloseHandle(pi.hThread); 
			CloseHandle(pi.hProcess);
		}
	}
}