#include "stdafx.h"
#include "Button.h"
#include <Windows.h>
#include "tools_debug\DebugMess.h"

 

Button::Button()
	: obj(NULL)
	, ptr(NULL)
{}

void Button::Do(TCommand &l)
{
	if(obj&&ptr) (obj->*ptr)();
}

void Button::Create(HWND h, wchar_t *txt)
{
	hWnd = CreateWindow(L"button", txt
				, WS_VISIBLE | WS_CHILD | WS_TABSTOP
				, 0, 0, 0, 0, h, NULL, (HINSTANCE)::GetModuleHandle(NULL), NULL
				);
	SetWindowLongPtr(hWnd, GWLP_USERDATA, (DWORD)this);
}

void Button::Size(int x, int y)
{
	  MoveWindow(hWnd, x, y, 200, 25, TRUE);
}
