#pragma once
#include <windows.h>
#include <ATLComTime.h>
struct CheckItem
{
	int x, y;
	wchar_t *txt;
	HWND hwnd;
	bool value;
    void Init(int, int, HWND);
	bool GetValue();
};
HWND UpDownItem(int x, int y, HWND h, int min_val, int max_val, int value);
struct UpDownTimeItem
{
	int x, y;
	wchar_t *txt;
	HWND hHour, hMin, hSec;
	time_t value;
	void Init(int, int, HWND);
	unsigned GetValue();	
};
time_t OleDateTimeTo_time_t(COleDateTime &t);
struct DataTimeItem
{
	HWND hDate, hHour, hMin, hSec;
	time_t value;
	wchar_t *txt;
	void Init(HWND h, int x, int y, COleDateTime &);
	void GetValue();	
};