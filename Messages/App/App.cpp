#include "StdAfx.h"
#include "App.h"
#include "Windows\MainWindow.h"
#include "window_tool\EmptyWindow.h"
#include "tools_debug/DebugMess.h"
#include "Log\LogBuffer.h"
#include "window_tool\WindowsPosition.h"

namespace{
	MainWindow mainWindow;
}

App::App()
{
	RECT r;
	WindowPosition::Get<MainWindow>(r);
	WindowClass<MainWindow> t;
	MyRegisterClass(Viewer<MainWindow>::Proc, t(),  CS_HREDRAW | CS_VREDRAW | CS_DBLCLKS
		, IDI_LETTER, IDI_LETTER, NULL);

	HWND h = CreateWindowEx(WS_EX_CLIENTEDGE | WS_EX_TOPMOST | WS_EX_LAYERED, t(), L"События", WS_OVERLAPPEDWINDOW, r.left, r.top, r.right, r.bottom, NULL, NULL, (HINSTANCE)::GetModuleHandle(NULL), &mainWindow); 

	DWORD dwStyle = GetWindowLong(h, GWL_STYLE);
	dwStyle &= ~(WS_MAXIMIZEBOX);
	SetWindowLong(h, GWL_STYLE, dwStyle);

	ShowWindow(h, SW_SHOWNORMAL);
	SetLayeredWindowAttributes (h, 0, (255*70)/100, LWA_ALPHA);
}

void App::Destroy()
{
}