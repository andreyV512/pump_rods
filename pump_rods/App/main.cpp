#include <windows.h>
#include <commctrl.h>
#include "Base/Base.hpp"
#include "App.h"
#include "window_tool/HookKey.h"
#include <gdiplus.h>
#include "tools_debug\DebugMess.h"

#if defined _M_IX86
#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='x86' publicKeyToken='6595b64144ccf1df' language='*'\"")
#elif defined _M_IA64
#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='ia64' publicKeyToken='6595b64144ccf1df' language='*'\"")
#elif defined _M_X64
#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='amd64' publicKeyToken='6595b64144ccf1df' language='*'\"")
#else
#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='*' publicKeyToken='6595b64144ccf1df' language='*'\"")
#endif

wchar_t *typeWindow = L"pump_rods_181114";

int APIENTRY wWinMain(HINSTANCE hInstance,
					   HINSTANCE hPrevInstance,
					   LPTSTR    lpCmdLine,
					   int       nCmdShow)
{
	dprint(__FUNCTION__"\n");
	CreateSemaphore(0, 0, 1, typeWindow);
	if(GetLastError() == ERROR_ALREADY_EXISTS)
	{
		HWND h = FindWindow(typeWindow, 0);
		SendMessage(h, WM_SYSCOMMAND, SC_RESTORE, 0);
		SetForegroundWindow(h);
		return 0;
	}
	INITCOMMONCONTROLSEX *iccx = new INITCOMMONCONTROLSEX;
	iccx->dwSize=sizeof(INITCOMMONCONTROLSEX);
	iccx->dwICC=0;
	InitCommonControlsEx(iccx);

	ULONG_PTR gdiplusToken; 
	Gdiplus::GdiplusStartupInput *gdiplusStartupInput = new Gdiplus::GdiplusStartupInput;    
	GdiplusStartup(&gdiplusToken, gdiplusStartupInput, NULL);
//--------------------------------------------------------------------
	Initialize initialize;

	//InitTcp initTcp;
	
	App::Init();

#ifdef THREAD_PRIORITY
	SetPriorityClass(GetCurrentProcess(), HIGH_PRIORITY_CLASS);
#endif

	//KeyHook keyHook;

	MSG msg;  

	//	RegisterHotKey(NULL, 1, MOD_ALT | MOD_CONTROL | MOD_NOREPEAT, VK_RETURN); 

	while (GetMessage(&msg, NULL, 0, 0))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);           
	}
	App::Destroy();
	Gdiplus::GdiplusShutdown(gdiplusToken);
	return (int) msg.wParam;
}
