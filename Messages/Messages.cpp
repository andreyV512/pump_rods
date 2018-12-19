// Thickness.cpp : Defines the entry point for the application.
//

#include "stdafx.h"
#include "App/App.h"
#include "Windows\MainWindow.h"
#include "window_tool\Emptywindow.h"

// Global Variables:
HINSTANCE hInstance;								// current instance
wchar_t *typeApplication = L"Messagess 181219 11:27";

int APIENTRY _tWinMain(HINSTANCE hInstance,
                     HINSTANCE hPrevInstance,
                     LPTSTR    lpCmdLine,
                     int       nCmdShow)
{
	CreateSemaphore(0, 0, 1, typeApplication);
	if(GetLastError() == ERROR_ALREADY_EXISTS)
	{
		HWND h = FindWindow(WindowClass<MainWindow>()(), 0);
		SendMessage(h, WM_SYSCOMMAND, SC_RESTORE, 0);
		SetForegroundWindow(h);
		return 0;
	}

	::hInstance = hInstance;
	INITCOMMONCONTROLSEX iccx;
	iccx.dwSize=sizeof(INITCOMMONCONTROLSEX);
	iccx.dwICC=0;
	InitCommonControlsEx(&iccx);
	
	App app;

	MSG msg;
	// Main message loop:
	while (GetMessage(&msg, NULL, 0, 0))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
	return (int) msg.wParam;
}


