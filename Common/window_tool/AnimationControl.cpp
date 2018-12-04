#include "stdafx.h"
#include <Windows.h>
#include <CommCtrl.h>
#include <WinUser.h>
#include <WindowsX.h>
#include "AnimationControl.h"
#include "resource.h"
#include "tools_debug\DebugMess.h"

namespace AnimationWindow
{
	static const int IDC_ANIMATION	= 990;
	HWND hDlg;
	//LONG exchange = 0;
	static const LONG start = 0x22222;
	static const LONG stop = 0x55555;
	void OnClose(HWND hWnd)
	{
		EndDialog(hWnd, TRUE);
	}
	BOOL OnInitAnimationDialog(HWND hWnd, HWND hWndFocus, LPARAM lParam)
	{
		// Create the animation control.
		hDlg = hWnd;
		static const int width = 280;
		static const int height = 110;
		RECT rc = { 0, 0, width, height };
		HWND hAnimate = CreateWindowEx(0, ANIMATE_CLASS, 0, 
			ACS_TIMER | ACS_AUTOPLAY | ACS_TRANSPARENT | WS_CHILD | WS_VISIBLE, 
			rc.left, rc.top, rc.right, rc.bottom, 
			hWnd, reinterpret_cast<HMENU>(IDC_ANIMATION), (HINSTANCE)::GetModuleHandle(NULL), 0);
		if (hAnimate == NULL)
		{
			return FALSE;
		}

		// Open the AVI clip and display its first frame in the animation control.
		if (0 == SendMessage(hAnimate, ACM_OPEN, static_cast<WPARAM>(0), 
			reinterpret_cast<LPARAM>(MAKEINTRESOURCE(IDR_UPLOAD_AVI))))
		{
			return FALSE;
		}

		// Plays the AVI clip in the animation control.
		if (0 == SendMessage(hAnimate, ACM_PLAY, static_cast<WPARAM>(-1), 
			MAKELONG(/*from frame*/0, /*to frame*/-1)))
		{
			return FALSE;
		}
		HWND h = GetParent(hWnd);

		int x = 230;
		int y = 130;

		if(NULL != h)
		{
			RECT r;
			GetWindowRect(h, &r);

			x = (r.left + r.right - width) / 2;
			y = (r.top + r.bottom - height) / 2;
		}
		MoveWindow(hWnd, x, y, width, height, FALSE);
		//long t = InterlockedCompareExchange(&exchange, start, 0);
		//if(stop == t)
		//{
		//	SendMessage(hDlg, WM_CLOSE, 0, 0);
		//}
		return TRUE;
	}
	INT_PTR CALLBACK AnimationDlgProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
	{
		switch (message)
		{
			// Handle the WM_INITDIALOG message in OnInitAnimationDialog
			HANDLE_MSG (hWnd, WM_INITDIALOG, OnInitAnimationDialog);

			// Handle the WM_CLOSE message in OnClose
			HANDLE_MSG (hWnd, WM_CLOSE, OnClose);

		default:
			return FALSE;	// Let system deal with msg
		}
		return 0;
	}

	char *p;

	DWORD WINAPI ThreadProc(_In_ LPVOID lpParameter)
	{
	    DialogBoxIndirectParam((HINSTANCE)::GetModuleHandle(NULL), (DLGTEMPLATE *)p, (HWND)lpParameter, AnimationDlgProc, NULL);
		LocalFree((HLOCAL)p);
		return 0;
	}

	//void Prepare()
	//{
	//	//exchange = 0;
	//}
#pragma warning(disable : 4996)
	void Init(HWND h, wchar_t *title)
	{
		p = (char *)LocalAlloc(LPTR, 2048);
		DLGTEMPLATE &d = *(DLGTEMPLATE *)p;
		d.style = DS_MODALFRAME | WS_POPUP | WS_CAPTION | WS_VISIBLE;
		d.dwExtendedStyle = 0;
		d.cdit = 0;
		wchar_t *c = (wchar_t *)&p[sizeof(DLGTEMPLATE) + 4];
		wcscpy(c, title);
		QueueUserWorkItem(ThreadProc, h, WT_EXECUTEDEFAULT);
	}

	void Destroy()
	{
		//long t = InterlockedCompareExchange(&exchange, stop, 0);
		//if(start == t)
		//{
	    	SendMessage(hDlg, WM_CLOSE, 0, 0);
		//}
	}
}
