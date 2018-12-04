#pragma once

namespace ItemIni
{
	void Set(wchar_t *section, wchar_t *nameParam, int value, wchar_t *fileName);
	int  Get(wchar_t *section, wchar_t *nameParam, int value, wchar_t *fileName);

	void Set(wchar_t *section, wchar_t *nameParam, wchar_t *value, wchar_t *fileName);
	wchar_t *Get(wchar_t *section, wchar_t *nameParam, wchar_t (&value)[128], wchar_t *fileName);

	void Set(wchar_t *section, wchar_t *nameParam, double value, wchar_t *fileName);
	double  Get(wchar_t *section, wchar_t *nameParam, double value, wchar_t *fileName);

	void GetPath(wchar_t (&path)[1024]);
}