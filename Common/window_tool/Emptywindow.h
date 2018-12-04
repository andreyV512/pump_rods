#pragma once
#include <typeinfo>
#include <windows.h>
#include "window_tool/message.h"
#include "resource.h"
#include "templates/WindowsEventTemplate.hpp"
#include "App/App.h"
//---------------------------------------------------------------------------------
ATOM MyRegisterClass(WNDPROC WndProc
	, const wchar_t *windowClassName
	, int style = CS_HREDRAW | CS_VREDRAW
	, int idIcon = 0
	, int idSmIcon = 0
	, HBRUSH backGround = NULL					 
	);

HWND MyCreateWindow(const wchar_t *windowClass, const wchar_t *title = NULL
	, void *data = NULL
	, int style = WS_OVERLAPPEDWINDOW
	, HWND hParent = NULL
	, int x = CW_USEDEFAULT
	, int y = CW_USEDEFAULT
	, int width = CW_USEDEFAULT
	, int height = CW_USEDEFAULT
	);

HWND CreateChildWindow(HWND hParent, WNDPROC WndProc, wchar_t *classStr, void *data);
HWND CreateChildWindowBackground(HWND hParent, WNDPROC WndProc, wchar_t *classStr, void *data);

#define CREATE_CHILD_WINDOW(hParent, type) o##type.hWnd = CreateChildWindow(hParent, (WNDPROC)&Viewer<type>::Proc, L###type, &o##type);

#pragma warning(disable :  4996)
template<class T>struct WindowClass
{
	wchar_t buf[512];
	WindowClass()
	{
		mbstowcs(buf, &(typeid(T).name())[6], dimention_of(buf));
		wcscat(buf, App::Salt());
	}
	wchar_t *operator()()const
	{
		return (wchar_t *)buf;
	}
};
template<class T>HWND WindowTemplate(
	T *self
	, wchar_t *title	
	, int x = CW_USEDEFAULT
	, int y = CW_USEDEFAULT
	, int width = CW_USEDEFAULT
	, int height = CW_USEDEFAULT
	, int styleS = IDI_UPLOAD
	, HBRUSH backGround = NULL	
	)
{
	WindowClass<T> t;
	MyRegisterClass(Viewer<T>::Proc, t(),  CS_HREDRAW | CS_VREDRAW | CS_DBLCLKS, styleS, styleS, backGround);
	HWND hWnd = MyCreateWindow(t(), title, self, WS_OVERLAPPEDWINDOW, NULL, x, y, width, height);
	return hWnd;
}

void RepaintWindow(HWND h);
void SizeWindow(HWND h);

template<class T>void CreateChildWindow(HWND hParent, T *data)
{
	WindowClass<T> cl;
	wchar_t *classStr =  cl();
	MyRegisterClass((WNDPROC)&Viewer<T>::Proc, classStr, CS_DBLCLKS);
	data->hWnd = CreateWindowEx(WS_EX_CLIENTEDGE, classStr, L"", WS_CHILD | WS_VISIBLE, 0, 0, 0, 0, hParent, NULL, (HINSTANCE)::GetModuleHandle(NULL), (void *)data);  
}

