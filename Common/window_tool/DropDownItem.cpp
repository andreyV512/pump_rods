#include "stdafx.h"
#include "DropDownItem.h"
#include <WindowsX.h>
#include "window_tool/message.h"
#include "templates/typelist.hpp"

extern HINSTANCE hInstance;

DropDown::DropDown()
	: ptr(NULL)
	, obj(NULL)
	, current(0)
{}

void DropDown::Do(TCommand &m)
{
	if(1 == m.isAcselerator)
	{
		wchar_t buf[128];
		GetWindowText(m.hControl, buf, dimention_of(buf));
		current = ComboBox_FindStringExact(m.hControl, 0, buf);
		if(obj&&ptr) (obj->*ptr)(current);
		SetFocus(m.hwnd);
	}
}

void DropDown::Size(int x, int y, int width)
{	
	MoveWindow(hWnd, x, y, width, 525, TRUE);
}

void DropDown::Init(HWND h, int fontSize, const wchar_t **items, int count)
{
	hWnd = CreateWindow(L"COMBOBOX", NULL
		, WS_CHILD | WS_VISIBLE | CBS_DROPDOWNLIST|CBS_AUTOHSCROLL |WS_VSCROLL 
		, 0, 0, 0, 0, h, 0, (HINSTANCE)::GetModuleHandle(NULL), 0
		);
	SetWindowLongPtr(hWnd, GWLP_USERDATA, (LONG)this);
	HFONT font = CreateFont(40,0,0,90,900,TRUE,0,0,ANSI_CHARSET,0,0,0,0, L"Times New Roman"); 
	SendMessage(hWnd, WM_SETFONT, (WPARAM)font, (LPARAM)0);
	for(int i = 0; i < count; ++i)
	{
		ComboBox_AddString(hWnd, items[i]);
	}
	ComboBox_SetCurSel(hWnd, current);
}