#include "GroupBox.h"

WNDPROC OldGroupBoxProc;
LRESULT CALLBACK GroupBoxProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch(uMsg)
	{
	case WM_COMMAND:
		SendMessage(GetParent(hWnd), uMsg, wParam, lParam);
		break;
	}
	return CallWindowProc(OldGroupBoxProc, hWnd, uMsg, wParam, lParam);
}

HWND GroupBoxWindow(wchar_t *title, int x, int top, int width, int height, HWND owner)
{
	HWND h = CreateWindow(L"button", title
		, WS_VISIBLE | WS_CHILD	| BS_GROUPBOX
		, x, top, width, height, owner, 0, (HINSTANCE)::GetModuleHandle(NULL), NULL
		);

	OldGroupBoxProc = (WNDPROC)GetWindowLongPtr(h, GWLP_WNDPROC);
	SetWindowLongPtr(h, GWLP_WNDPROC, (LONG)GroupBoxProc);
	return h;
}