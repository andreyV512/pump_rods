#include "window_tool/HookKey.h"

#include <windows.h>
#include <winuser.h>
#include <stdio.h>
#include "tools_debug\DebugMess.h"

//#include "Automat/AppKeyHandler.h"
#include "window_tool/message.h"

HHOOK hKeyHook;
HWND hWnd;
void dump(unsigned){}
void(*__proc)(unsigned) = &dump;

void UserMessage(void *value)
{
//	AppKeyHandler::KeyPressed((unsigned)value);
	(*__proc)((unsigned)value);
}
 
__declspec(dllexport) LRESULT CALLBACK KeyEvent (

  int nCode,      // The hook code
  WPARAM wParam,  // The window message (WM_KEYUP, WM_KEYDOWN, etc.)
  KBDLLHOOKSTRUCT *hooked   // A pointer to a struct with information about the pressed key

) {
    if  ((nCode == HC_ACTION) &&       // HC_ACTION means we may process this event
        ((wParam == WM_SYSKEYDOWN) ||  // Only react if either a system key ...
        (wParam == WM_KEYDOWN)))       // ... or a normal key have been pressed.
    {
		//PostMessage(hWnd, WM_GET_SCAN_CODE, 0, hooked->vkCode);
		PostMessage(hWnd, WM_USER, (WPARAM)UserMessage, hooked->vkCode);
    }
    return CallNextHookEx(hKeyHook, nCode, wParam, (LPARAM)hooked);
}
void MsgLoop()
{
	MSG msg;
	while (GetMessage(&msg,NULL,0,0)) 
	{
		TranslateMessage( &msg );
		DispatchMessage( &msg );
	}
}
DWORD WINAPI KeyLogger(LPVOID lpParameter)
{
    hKeyHook = SetWindowsHookEx (  // install the hook:
        WH_KEYBOARD_LL,            // as a low level keyboard hook
        (HOOKPROC) KeyEvent,       // with the KeyEvent function from this executable
        (HINSTANCE)::GetModuleHandle(NULL),                      // and the module handle to our own executable
        NULL                       // and finally, the hook should monitor all threads.
    );
    MsgLoop();
    UnhookWindowsHookEx(hKeyHook);
    return 0;
}
#ifdef HOOK_KEY_DLL
extern "C"__declspec(dllexport) void __stdcall SetHWND(HWND h)
#else
void SetHWND(HWND h)
#endif
{
	hWnd = h;
}
#ifdef HOOK_KEY_DLL
extern "C"__declspec(dllexport) void __stdcall StartKeyHook(HWND h)
#else
void StartKeyHook(HWND h, void(*proc)(unsigned))
#endif
{
	__proc = proc;
	//SetHWND(h);
	hWnd = h;
	CloseHandle(CreateThread(NULL,NULL,(LPTHREAD_START_ROUTINE)KeyLogger, NULL, NULL, NULL));
}

HHOOK hHook;

__declspec(dllexport) LRESULT CALLBACK __KeyEvent__ (

  int nCode,      // The hook code
  WPARAM wParam,  // The window message (WM_KEYUP, WM_KEYDOWN, etc.)
  KBDLLHOOKSTRUCT *hooked   // A pointer to a struct with information about the pressed key

) {
    if  ((nCode == HC_ACTION) &&       // HC_ACTION means we may process this event
        ((wParam == WM_SYSKEYDOWN) ||  // Only react if either a system key ...
        (wParam == WM_KEYDOWN)))       // ... or a normal key have been pressed.
    {
		//AppKeyHandler::KeyPressed( hooked->vkCode);
    }
    return CallNextHookEx(hKeyHook, nCode,wParam,(LPARAM)hooked);
}

//KeyHook::KeyHook()
//{
//	 hHook = SetWindowsHookEx (  // install the hook:
//        WH_KEYBOARD_LL,            // as a low level keyboard hook
//        (HOOKPROC) __KeyEvent__,       // with the KeyEvent function from this executable
//        (HINSTANCE)::GetModuleHandle(NULL),                      // and the module handle to our own executable
//        NULL                       // and finally, the hook should monitor all threads.
//    );   
//}
//
//KeyHook::~KeyHook()
//{
//	 UnhookWindowsHookEx(hHook);
//}

