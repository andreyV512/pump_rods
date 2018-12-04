#include "stdafx.h"
#include "DlgItems.h"
#include <windowsx.h>
#include <commctrl.h>
#include "templates/typelist.hpp"
 
//------------------------------------------------------------------------------------------------
HWND CheckItem(int dx, int dy, HWND h, wchar_t *buttonTxt, bool value)
{
	HWND hWnd = CreateWindow(L"button", buttonTxt
		,  WS_CHILD | WS_VISIBLE | BS_AUTOCHECKBOX | WS_TABSTOP
		, dx, dy, 300, 15, h, NULL, (HINSTANCE)::GetModuleHandle(NULL), NULL
		);
	Button_SetCheck(hWnd, value ? BST_CHECKED : BST_UNCHECKED);
	return hWnd;
}
//----------------------------------------------------------------------------------------------
HWND UpDownItem(int x, int y, HWND h, int min_val, int max_val, int value)
{
	HWND hh = CreateWindowEx(WS_EX_CLIENTEDGE,           //extended styles
                      L"edit",  //control 'class' name
                      0,                  //control caption
                      WS_CHILD|WS_VISIBLE| WS_TABSTOP,            //wnd style
                      x,            //position: left
                      y,             //position: top
                      40,           //width
                      20,          //height
                      h,            //parent window handle
                      //control's ID
                      0,
                      (HINSTANCE)::GetModuleHandle(NULL),              //instance
                      0);                 //user defined info
		
    CreateUpDownControl( WS_CHILD|WS_VISIBLE|UDS_ALIGNRIGHT|UDS_SETBUDDYINT|UDS_WRAP, x + 40, y, 70, 20
		, h, 0, (HINSTANCE)::GetModuleHandle(NULL), hh, min_val, max_val, value
		);
	return hh;
}
//----------------------------------------------------------------------------------------------
void CheckItem::Init(int x, int y, HWND h)
{
	hwnd = CreateWindow(L"button", txt
		,  WS_CHILD | WS_VISIBLE | BS_AUTOCHECKBOX | WS_TABSTOP
		, x, y, 300, 15, h, NULL, (HINSTANCE)::GetModuleHandle(NULL), NULL
		);
	Button_SetCheck(hwnd, value ? BST_CHECKED : BST_UNCHECKED);
}
//-----------------------------------------------------------------------------------
bool CheckItem::GetValue()
{
	return value = BST_CHECKED == Button_GetCheck(hwnd); 
}
//------------------------------------------------------------------------------------
void UpDownTimeItem::Init(int x, int y, HWND h)
{
	CTime v(value);
	hHour = UpDownItem(x, y, h, 23, 0, v.GetHour());//(value / 3600) % 24);
	hMin = UpDownItem(x + 50, y, h, 59, 0, v.GetMinute());//(value / 60) % 60);
	hSec = UpDownItem(x + 100, y, h, 59, 0, v.GetSecond());// % 60);

	CreateWindow(L"static", txt
		, WS_VISIBLE | WS_CHILD
		, x + 150, y + 3, 280, 20, h, 0, (HINSTANCE)::GetModuleHandle(NULL), NULL
		);
}
//---------------------------------------------------------------------------
unsigned UpDownTimeItem::GetValue()
{
	/*
	wchar_t buf[256];
	Edit_GetText(hHour, buf, dimention_of(buf));
	unsigned t = 3600 * (_wtoi(buf) % 24);
	Edit_GetText(hMin, buf, dimention_of(buf));
	t += 60 * (_wtoi(buf) % 60);
	Edit_GetText(hSec, buf, dimention_of(buf));
	t += _wtoi(buf) % 60;
    return value = t;
	*/
	return 0;
}
//---------------------------------------------------------------------------------------
void DataTimeItem::Init(HWND h, int x, int y, COleDateTime &)
{
	hDate = CreateWindowEx(0,                  //extended styles
		DATETIMEPICK_CLASS, //control 'class' name
		0,                  //control caption
		WS_CHILD|WS_VISIBLE|DTS_LONGDATEFORMAT| WS_TABSTOP,            //wnd style
		x,            //position: left
		y,             //position: top
		130,           //width
		20,          //height
		h,            //parent window handle
		0,
		(HINSTANCE)::GetModuleHandle(NULL),              //instance
		0); 
	SYSTEMTIME s = {};
	  long timezone;
 _get_timezone( &timezone );
	time_t val = value + timezone;
	CTime v(val);
	
	s.wYear = v.GetYear();
	s.wMonth = v.GetMonth();
	s.wDay = v.GetDay();

	if(DateTime_SetSystemtime(hDate, GDT_VALID, &s))
	{		
		hHour = UpDownItem(x + 140, y, h, 0, 23, v.GetHour());
		hMin  = UpDownItem(x + 190, y, h, 0, 59, v.GetMinute());
		hSec  = UpDownItem(x + 240, y, h, 0, 59, v.GetSecond());
	}
}
//.......................................................................................
time_t OleDateTimeTo_time_t(COleDateTime &t) 
{ 
    return time_t ((double) t * 86400) - 2209161600; 
}
void DataTimeItem::GetValue()
{
	SYSTEMTIME t;
	DateTime_GetSystemtime(hDate, &t);   
	wchar_t buf[128];
	Edit_GetText(hHour, buf, dimention_of(buf));
	t.wHour = _wtoi(buf);
	Edit_GetText(hMin, buf, dimention_of(buf));
	t.wMinute = _wtoi(buf);
	Edit_GetText(hSec, buf, dimention_of(buf));
	t.wSecond = _wtoi(buf);   
	_tzset();
	long timezone;	
	_get_timezone( &timezone );
	int daylight;
	_get_daylight(&daylight);
	value = CTime(t).GetTime() - timezone + 3600 * daylight;
}
//---------------------------------------------------------------------------------------
