#include "stdafx.h"
#include "Zip.h"
#include <stdio.h>
#include <Shlwapi.h>
#include "templates/typelist.hpp"

#include "tools_debug\DebugMess.h"

#pragma warning(disable: 4996)

namespace Zip
{
	wchar_t *ProgrammDir::Get()
	{
		GetModuleFileName(0, path, dimention_of(path));
		PathRemoveFileSpec(path);
		return path;
	}

	wchar_t name[] = L"Parallel hatch in zip space";
	wchar_t eventName[] = L"zip event 2015 12 16 08 29";

	struct TZipData
	{
		wchar_t data[16][16];
	};
	struct Test
	{
		HANDLE hMapFile, hMutex;
		TZipData *map;
		Test()
		{
			hMutex = CreateMutex(NULL, FALSE, eventName);
			hMapFile = CreateFileMapping(
				INVALID_HANDLE_VALUE,    // use paging file
				NULL,                    // default security 
				PAGE_READWRITE,          // read/write access
				0,                       // max. object size 
				sizeof(TZipData),                // buffer size  
				name);                 // name of mapping object

			int res = GetLastError();
			map = (TZipData *) MapViewOfFile(hMapFile,   // handle to map object
				FILE_MAP_ALL_ACCESS, // read/write permission
				0,                   
				0,                   
				sizeof(TZipData));           

			if(NULL == map) 
			{ 
				return;
			}
			if(0 == res)
			{
				ZeroMemory(map, sizeof(TZipData));
			}
		}

		void Destroy(void)
		{
			UnmapViewOfFile(map);
			CloseHandle(hMapFile);
			CloseHandle(hMutex);
		}
	public:
		static Test &Inst(){static Test x; return x;}
	};

	bool TestName(wchar_t *n)
	{
		Test &t = Test::Inst();
		WaitForSingleObject(t.hMutex, INFINITE);
		bool res = false;
		for(int i = 0; i < dimention_of(t.map->data); ++i){
			res = 0 == wcsncmp(n, t.map->data[i], 12);
			if(res) break;
		}
		ReleaseMutex(t.hMutex);
		return res;
	}
	bool SetName(wchar_t *n)
	{
		Test &t = Test::Inst();
		WaitForSingleObject(t.hMutex, INFINITE);
		for(int i = 0; i < dimention_of(t.map->data); ++i)
		{
			if(0 == wcsncmp(n, t.map->data[i], 12))
			{
				ReleaseMutex(t.hMutex);
				return false;
			}
		}
		for(int i = 0; i < dimention_of(t.map->data); ++i)
		{
			if(0 == *t.map->data[i])
			{
				wcsncpy(t.map->data[i], n, 12);
				t.map->data[i][12] = 0;
				printf("%S\n", t.map->data[i]);
				break;
			}
		}
		ReleaseMutex(t.hMutex);
		return true;
	}

	bool UnsetName(wchar_t *n)
	{
		Test &t = Test::Inst();
		WaitForSingleObject(t.hMutex, INFINITE);
		bool res = false;
		for(int i = 0; i < dimention_of(t.map->data); ++i)
		{
			res = 0 == wcsncmp(n, t.map->data[i], 12);
			t.map->data[i][0] = 0;
			if(res) break;
		}
		ReleaseMutex(t.hMutex);
		return res;
	}

	void ZipFile(wchar_t *n)
	{
		PROCESS_INFORMATION pi;
		STARTUPINFO si = {};
		si.dwFlags = STARTF_USESHOWWINDOW;
		si.wShowWindow =  SW_HIDE;
		si.cb = sizeof(STARTUPINFO);
		ProgrammDir pd;
		wchar_t path[1024];
		wchar_t *pdPath = pd.Get();
		wsprintf(path, L"%s\\bzip2 -z -f %s\\..\\Stored\\%s", pdPath, pdPath, n);
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
	void UnZipFile(wchar_t *n)
	{
		PROCESS_INFORMATION pi;
		STARTUPINFO si = {};
		si.cb = sizeof(STARTUPINFO);
		si.dwFlags = STARTF_USESHOWWINDOW;
		si.wShowWindow =  SW_HIDE;
		ProgrammDir pd;
		wchar_t *pdPath = pd.Get();
		wchar_t path[1024];
		wsprintf(path, L"%s\\bzip2 -d -k %s\\..\\Stored\\%s.bz2", pdPath, pdPath, n);
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
	bool UnZipFile2(wchar_t *n)
	{
		STARTUPINFO si = {};
		PROCESS_INFORMATION pi;
		si.cb = sizeof(STARTUPINFO);
		si.dwFlags = STARTF_USESHOWWINDOW;
		si.wShowWindow =  SW_HIDE;
		ProgrammDir pd;
		wchar_t *pdPath = pd.Get();
		wchar_t path[1024];
		wsprintf(path, L"%s\\bzip2 -d -k %s", pdPath, n);
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
		dprint("%S\n", path);
		if(!b) dprint("run not unzip\n");
		if(b)
		{
			if(b) dprint("wait while unzip file\n");
			unsigned res = WaitForSingleObjectEx(pi.hProcess, INFINITE, FALSE); 
            dprint("unzip file ok  ret WaitForSingleObject %x  %d\n", res, res);
			CloseHandle(pi.hThread); 
			CloseHandle(pi.hProcess);
		}
		return 0 != b;
	}
	void ZipFile2(wchar_t *n)
	{
		STARTUPINFO si = {};
		PROCESS_INFORMATION pi;
		si.cb = sizeof(STARTUPINFO);
		si.dwFlags = STARTF_USESHOWWINDOW;
		si.wShowWindow =  SW_HIDE;
		ProgrammDir pd;
		wchar_t *pdPath = pd.Get();
		wchar_t path[1024];
		wsprintf(path, L"%s\\bzip2 -z -f %s", pdPath, n);
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
}