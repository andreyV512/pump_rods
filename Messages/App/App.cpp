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
	HWND h = WindowTemplate(&mainWindow, L"События", r.left, r.top, r.right, r.bottom, IDI_LETTER);
	DWORD dwStyle = GetWindowLong(h, GWL_STYLE);
	dwStyle &= ~(WS_MAXIMIZEBOX);
	SetWindowLong(h, GWL_STYLE, dwStyle);
	ShowWindow(h, SW_SHOWNORMAL);
}

void App::Destroy()
{
}