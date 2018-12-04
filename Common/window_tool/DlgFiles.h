#pragma once
#include <windows.h>
#include <Commdlg.h>
class SaveData
{
public:
	OPENFILENAME ofn;
public:
	wchar_t sFile[256];
public:
  SaveData(HWND, wchar_t *title = NULL);
  bool operator()();
};
//--------------------------------------------------------
class OpenData
{
public:
	OPENFILENAME ofn;
	wchar_t sFile[256];
	OpenData(HWND, wchar_t *title = NULL);
	bool operator()();
};
class ColorDialog
{
public:
	CHOOSECOLOR color;
	COLORREF crCustColors[16];
public:
	COLORREF Result;
	ColorDialog(HWND, wchar_t * = NULL);
	bool operator()();
};
