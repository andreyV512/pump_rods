#include "ItemIni.h"
#include <Windows.h>
#include "templates/typelist.hpp"
#pragma warning(disable: 4996)
namespace ItemIni
{
	void Set(wchar_t *section, wchar_t *nameParam, int value, wchar_t *fileName)
	{
		    wchar_t buf[32];
			_itow(value, buf, 10);
			WritePrivateProfileString(section, nameParam, buf, fileName);
	}
//----------------------------------------------------------------------------------------
	int  Get(wchar_t *section, wchar_t *nameParam, int value, wchar_t *fileName)
	{
		int val = GetPrivateProfileInt(section, nameParam, 0xBADC0DE, fileName);
		if(0xBADC0DE == val)
		{
			wchar_t buf[32];
			_itow(value, buf, 10);
			WritePrivateProfileString(section, nameParam, buf, fileName);
			val = value;
		}
		return val;
	}

	void Set(wchar_t *section, wchar_t *nameParam, wchar_t *value, wchar_t *fileName)
	{
		WritePrivateProfileString(section, nameParam, value, fileName);
	}

	wchar_t *Get(wchar_t *section, wchar_t *nameParam, wchar_t (&value)[128], wchar_t *fileName)
	{
		int ret = GetPrivateProfileString(section, nameParam, value, value, sizeof(value) / sizeof(wchar_t), fileName);
		if(!ret)
		{
			WritePrivateProfileString(section, nameParam, value, fileName);
		}
		return value;
	}
//----------------------------------------------------------------------------------------------
	void Set(wchar_t *section, wchar_t *nameParam, double value, wchar_t *fileName)
	{
		wchar_t buff[128];
		wsprintf(buff, L"%g", value);
		WritePrivateProfileString(section, nameParam, buff, fileName);
	}

	double Get(wchar_t *section, wchar_t *nameParam, double value, wchar_t *fileName)
	{
		wchar_t buff[128];
		int ret = GetPrivateProfileString(section, nameParam, L"", buff, sizeof(buff) / sizeof(wchar_t), fileName);
		if(!ret)
		{
			wsprintf(buff, L"%g", value);
			WritePrivateProfileString(section, nameParam, buff, fileName);
		}
		return _wtof(buff);
	}

	void GetPath(wchar_t (&path)[1024])
	{
		GetModuleFileName(0, path, dimention_of(path));
		int len = (int)wcslen(path);
		wcscpy(&path[len - 3], L"ini");
	}
}
