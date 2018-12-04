#pragma once
#include <windows.h>
#include <windowsx.h>
 
template<class T>struct DlgItems;

struct TEdit;

template<>struct DlgItems<TEdit>
{
	static const int DY = 25;
	HWND hWnd;
	void Init(int &dy, HWND h, wchar_t *editTxt, wchar_t *labelTxt)
	{
		hWnd = CreateWindow(L"edit", editTxt
		, WS_BORDER | WS_VISIBLE | WS_CHILD | ES_LEFT | WS_TABSTOP
			, 10, dy, 280, 20, h, 0, (HINSTANCE)::GetModuleHandle(NULL), NULL
			);
		CreateWindow(L"static", labelTxt
		, WS_VISIBLE | WS_CHILD
			, 280 + 20, dy + 3, 280, 20, h, 0, (HINSTANCE)::GetModuleHandle(NULL), NULL
			);
		dy += DY;
	}
};

struct TButton;
template<>struct DlgItems<TButton>
{
	HWND hWnd;
	template<class O>void Init(O *o, int &dx, int dy, HWND h, wchar_t *buttonTxt, int tabStop = BS_DEFPUSHBUTTON)
	{
		WORD id = (WORD)&O::_0::Do; 
		hWnd = CreateWindow(L"button", buttonTxt
				, WS_VISIBLE | WS_CHILD | WS_TABSTOP | tabStop
				, dx, dy, 90, 25, h, (HMENU)id, (HINSTANCE)::GetModuleHandle(NULL), NULL
				);
		SetWindowLongPtr(hWnd, GWLP_USERDATA, (DWORD)&O::_0::Do);
		dx += 100;
	}
};

struct TCheck;
template<>struct DlgItems<TCheck>
{
	HWND hWnd;
	void Init(int dx, int &dy, HWND h, wchar_t *buttonTxt, bool value)
	{
		hWnd = CreateWindow(L"button", buttonTxt
				,  WS_CHILD | WS_VISIBLE | BS_AUTOCHECKBOX | WS_TABSTOP
				, dx, dy, 300, 15, h, NULL, (HINSTANCE)::GetModuleHandle(NULL), NULL
				);
		Button_SetCheck(hWnd, value ? BST_CHECKED : BST_UNCHECKED);
		dy += 20;
	}
};
//struct Dialog
//{
//	template<class list>void Do()
//	{
//	}
//};