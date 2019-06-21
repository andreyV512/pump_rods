#include "StoredFiles.h"
#include <stdio.h>
#include <Windows.h>
#include <ATLComTime.h>
#include "templates/typelist.hpp"
#include "templates/sort.hpp"

namespace StoredFiles
{
	wchar_t *PathExec(wchar_t (&buf)[1024])
	{
		GetModuleFileName(0, buf, dimention_of(buf));
		PathRemoveFileSpec(buf);
		return buf;
	}

	wchar_t *PathUp(wchar_t (&buf)[1024], wchar_t *path)
	{
		GetModuleFileName(0, buf, dimention_of(buf));

		PathRemoveFileSpec(buf);
		PathRemoveFileSpec(buf);

		wsprintf(&buf[wcslen(buf)], L"\\%s", path);
		return buf;
	}

	wchar_t *PathFile(wchar_t(&buf)[1024], wchar_t *path, wchar_t *ext)
	{
		PathUp(buf, path);
		CreateDirectory(buf, NULL);

		COleDateTime tme = COleDateTime::GetCurrentTime();

		wsprintf(&buf[wcslen(buf)], L"\\%02d%02d%02d%02d%02d%02d.%s"
		, tme.GetYear()	- 2000
		, tme.GetMonth()
		, tme.GetDay()
		, tme.GetHour()
		, tme.GetMinute()
		, tme.GetSecond()
		, ext
		);

		return buf;
	}

	unsigned long long __compare__(wchar_t *txt, int extLen)
	{
		if(txt[0] >= '0' && txt[0] <= '9')
		{
			unsigned long long t = 0;
			for(int i = 0, len = wcslen(txt) - extLen; i < len; ++i)
			{
				t *= 10;
				t += txt[i];
			}
			return t;
		}
		return (unsigned long long)-1;
	}

#pragma warning(disable : 4996)

	void DeleteExtraFiles(wchar_t *dir, wchar_t *ext, int maxCount)
	{
		wchar_t path[1024];
		PathUp(path, dir);
		int pathLen = wcslen(path);
		wsprintf(&path[pathLen], L"\\*.%s", ext);
		++pathLen;

		WIN32_FIND_DATA ffd;
		HANDLE  hFind = FindFirstFile(path, &ffd);
		if(INVALID_HANDLE_VALUE == hFind) return;

		int countFiles = 0;
		int extLen = wcslen(ext);

		unsigned long long buf[5000];

		do
		{
			if (0 == (ffd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY))
			{
				buf[countFiles] = __compare__(ffd.cFileName, extLen);
				++countFiles;
				if(countFiles >= dimention_of(buf)) break;
			}
		}
		while (FindNextFile(hFind, &ffd) != 0);
		FindClose(hFind);

		if(countFiles > maxCount)
		{
			QuickSort(buf, countFiles);
			countFiles -= maxCount;

			HANDLE  hFind = FindFirstFile(path, &ffd);

			unsigned long long cmp = buf[countFiles];

			do
			{
				if(cmp > __compare__(ffd.cFileName, extLen))
				{
					wcscpy(&path[pathLen], ffd.cFileName); 
					DeleteFile(path);
				}
			}
			while (FindNextFile(hFind, &ffd) != 0);

			FindClose(hFind);
		}
	}
}