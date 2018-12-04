#include "stdafx.h"
#include "EditDlg.h"
#include "templates/typelist.hpp"
#include <commctrl.h>
extern HINSTANCE hInstance;
#pragma warning(disable : 4996)
EditDlg::EditDlg(HWND hWnd
		, wchar_t *title, wchar_t *data, bool(*ok)(HWND, wchar_t *)
		, bool(*cancel)(HWND, wchar_t *))
		: OkBtn(ok)
	    , CancelBtn(cancel)
{
	wcscpy(buf, data);
	char *p = (char *)LocalAlloc(LPTR, 2000);
	DLGTEMPLATE &d = *(DLGTEMPLATE *)p;
	d.style = DS_MODALFRAME | WS_POPUP | WS_CAPTION;
	d.dwExtendedStyle = 0;
	d.cdit = 0;
	wchar_t *c = (wchar_t *)&p[sizeof(DLGTEMPLATE) + 4];
    wcscpy(c, title);
	
	DialogBoxIndirectParam((HINSTANCE)::GetModuleHandle(NULL), &d, hWnd, (DLGPROC)Proc, (LPARAM)this);
    LocalFree((HLOCAL)p);
}
LRESULT CALLBACK EditDlg::Proc(HWND h, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch(msg)
	{
	case WM_COMMAND:
		switch(wParam)
		{
		case IDOK:
			{
				EditDlg *e = (EditDlg *)GetWindowLongPtr(h, GWLP_USERDATA);
				GetWindowText(e->hEdit, e->buf, dimention_of(e->buf));
				if((*e->OkBtn)(h, e->buf))EndDialog(h, TRUE);
			}
			return TRUE;
		case IDCANCEL: 
			{
				EditDlg *e = (EditDlg *)GetWindowLongPtr(h, GWLP_USERDATA);
				GetWindowText(e->hEdit, e->buf, dimention_of(e->buf));
				if((*e->CancelBtn)(h, e->buf))EndDialog(h, TRUE);
			}
			return TRUE;
		}
		break;
	case WM_INITDIALOG:
		{
			HWND hParent = GetParent(h);
			SetWindowLongPtr(h, GWLP_USERDATA, lParam);
			EditDlg *e = (EditDlg *)lParam;			
			e->hEdit = CreateWindowEx(WS_EX_CLIENTEDGE, L"edit", e->buf
				, WS_BORDER | WS_VISIBLE | WS_CHILD | ES_LEFT | WS_TABSTOP
				,10, 20, 400, 25, h, 0, (HINSTANCE)::GetModuleHandle(NULL), NULL
				);
			static const int offs = 100;			
			CreateWindow(L"button", L"Применить"
				, WS_VISIBLE | WS_CHILD | WS_TABSTOP
				,offs, 50, 110, 30, h, (HMENU)IDOK, (HINSTANCE)::GetModuleHandle(NULL), NULL
				);
			CreateWindow(L"button", L"Отмена"
				, WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON | WS_TABSTOP
				,offs + 120, 50, 110, 30, h, (HMENU)IDCANCEL, (HINSTANCE)::GetModuleHandle(NULL), NULL
				);
			RECT r;
			GetWindowRect(hParent, &r);
			static const int width = 430;
			static const int height = 125;
			int x = r.left +(r.right - r.left - width) / 2;
			int y = r.top +(r.bottom - r.top - height) / 2;
			MoveWindow(h, x, y, width, height, FALSE);
		}
		return TRUE;
	}
	return FALSE;
}
//-------------------------------------------------------------------------------------------------------
EditDlgMultiLines::EditDlgMultiLines(int count, HWND hWnd
		, wchar_t *title, wchar_t **title_data, wchar_t **data, bool(*ok)(HWND, wchar_t **)
		, bool(*cancel)(HWND, wchar_t **))
		: count(count)
		, title_text(title_data)
        , OkBtn(ok)
	    , CancelBtn(cancel)
{	
	buf = new wchar_t *[count];
	hEdit = new HWND[count];
	for(int i = 0; i < count; ++i)
	{
		buf[i] = new wchar_t[128];
		wcscpy(buf[i], data[i]);
	}
	char *p = (char *)LocalAlloc(LPTR, 2048);
	DLGTEMPLATE &d = *(DLGTEMPLATE *)p;
	d.style = DS_MODALFRAME | WS_POPUP | WS_CAPTION;
	d.dwExtendedStyle = 0;
	d.cdit = 0;
	wchar_t *c = (wchar_t *)&p[sizeof(DLGTEMPLATE) + 4];
    wcscpy(c, title);
	
	DialogBoxIndirectParam((HINSTANCE)::GetModuleHandle(NULL), &d, hWnd, (DLGPROC)Proc, (LPARAM)this);
    LocalFree((HLOCAL)p);
}
//----------------------------------------------------------------------------------------------
EditDlgMultiLines::~EditDlgMultiLines()
{
	for(int i = 0; i < count; ++i) delete[] buf[i];
	delete[] buf;
	delete[] hEdit;
}
//----------------------------------------------------------------------------------------------
LRESULT CALLBACK EditDlgMultiLines::Proc(HWND h, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch(msg)
	{
	case WM_COMMAND:
		switch(wParam)
		{
		case IDOK:
			{
				EditDlgMultiLines *e = (EditDlgMultiLines *)GetWindowLongPtr(h, GWLP_USERDATA);
				for(int i = 0; i < e->count; ++i) GetWindowText(e->hEdit[i], e->buf[i], 128);
				if((*e->OkBtn)(h, e->buf))EndDialog(h, TRUE);
			}
			return TRUE;
		case IDCANCEL: 
			{
				EditDlgMultiLines *e = (EditDlgMultiLines *)GetWindowLongPtr(h, GWLP_USERDATA);
				for(int i = 0; i < e->count; ++i) GetWindowText(e->hEdit[i], e->buf[i], 128);
				if((*e->CancelBtn)(h, e->buf))EndDialog(h, TRUE);
			}
			return TRUE;
		}
		break;
	case WM_INITDIALOG:
		{
			HWND hParent = GetParent(h);
			SetWindowLongPtr(h, GWLP_USERDATA, lParam);
			EditDlgMultiLines *e = (EditDlgMultiLines *)lParam;
			int dy = 10;
			for(int i = 0; i < e->count; ++i)
			{
				e->hEdit[i] = CreateWindow(L"edit", e->buf[i]
				, WS_BORDER | WS_VISIBLE | WS_CHILD | ES_LEFT | WS_TABSTOP
					,10, dy, 280, 20, h, 0, (HINSTANCE)::GetModuleHandle(NULL), NULL
					);
				CreateWindow(L"static", e->title_text[i]
				, WS_VISIBLE | WS_CHILD
					,280 + 20, dy + 3, 280, 20, h, 0, (HINSTANCE)::GetModuleHandle(NULL), NULL
					);
				dy += 25;
			}
			static const int width = 650;
			static const int offs = width / 2 - 100;
			CreateWindow(L"button", L"Применить"
				, WS_VISIBLE | WS_CHILD | WS_TABSTOP
				,offs, dy, 90, 25, h, (HMENU)IDOK, (HINSTANCE)::GetModuleHandle(NULL), NULL
				);
			CreateWindow(L"button", L"Отмена"
				, WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON | WS_TABSTOP
				,offs + 100, dy, 90, 25, h, (HMENU)IDCANCEL, (HINSTANCE)::GetModuleHandle(NULL), NULL
				);
			RECT r;
			GetWindowRect(GetDesktopWindow(), &r);
			
			static const int height = dy + 65;
			int x = r.left +(r.right - r.left - width) / 2;
			int y = r.top +(r.bottom - r.top - height) / 2;
			MoveWindow(h, x, y, width, height, FALSE);
		}
		return TRUE;
	}
	return FALSE;
}
//-----------------------------------------------------------------------------------------
EditDlgComboBox::EditDlgComboBox(HWND hWnd
								 , wchar_t *title
								 , Handlers &h ) 		
								 : handlers(h)
{
	char *p = (char *)LocalAlloc(LPTR, 2000);
	DLGTEMPLATE &d = *(DLGTEMPLATE *)p;
	d.style = DS_MODALFRAME | WS_POPUP | WS_CAPTION;
	d.dwExtendedStyle = 0;
	d.cdit = 0;
	wchar_t *c = (wchar_t *)&p[sizeof(DLGTEMPLATE) + 4];
	wcscpy(c, title);

	DialogBoxIndirectParam((HINSTANCE)::GetModuleHandle(NULL), &d, hWnd, (DLGPROC)Proc, (LPARAM)this);
	LocalFree((HLOCAL)p);
}
wchar_t *GetText(HWND h, wchar_t *b)
{
	LRESULT i = SendMessage(h, LB_GETCURSEL, 0, 0);
	if(LB_ERR != i)
	{
		SendMessage(h, LB_GETTEXT, i, (LPARAM)b);
		return b;
	}
	return NULL;
}
#define ID_LISTBOX 33
LRESULT CALLBACK EditDlgComboBox::Proc(HWND h, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch(msg)
	{
	case WM_COMMAND:
		switch(LOWORD(wParam))
		{
		case IDOK:
			{
				EditDlgComboBox *e = (EditDlgComboBox *)GetWindowLongPtr(h, GWLP_USERDATA);
				if(GetText(e->hEdit, e->buf) && e->handlers.OkBtn(e->buf))EndDialog(h, TRUE);
			}
			return TRUE;
		case IDCANCEL: 
			{
				EditDlgComboBox *e = (EditDlgComboBox *)GetWindowLongPtr(h, GWLP_USERDATA);
				if(e->handlers.CancelBtn())EndDialog(h, FALSE);
			}
			return TRUE;
		case ID_LISTBOX:
			switch(HIWORD(wParam))
			{
			case LBN_DBLCLK:
				{
					EditDlgComboBox *e = (EditDlgComboBox *)GetWindowLongPtr(h, GWLP_USERDATA);
					if(GetText(e->hEdit, e->buf) && e->handlers.OkBtn(e->buf))EndDialog(h, TRUE);
				}
				return 0;
			}
			return TRUE;			
		}
		break;
	case WM_INITDIALOG:
		{
			HWND hParent = GetParent(h);
			SetWindowLongPtr(h, GWLP_USERDATA, lParam);
			EditDlgComboBox *e = (EditDlgComboBox *)lParam;
			e->hEdit = CreateWindow(L"listbox", NULL
				,  WS_VISIBLE | WS_CHILD | LBS_NOTIFY | WS_VSCROLL | WS_BORDER
				,10, 10, 380, 205, h, (HMENU)ID_LISTBOX, (HINSTANCE)::GetModuleHandle(NULL), NULL
				);
			static const int offs = 100;
			CreateWindow(L"button", L"Применить"
				, WS_VISIBLE | WS_CHILD | WS_TABSTOP
				,offs, 220, 90, 25, h, (HMENU)IDOK, (HINSTANCE)::GetModuleHandle(NULL), NULL
				);
			CreateWindow(L"button", L"Отмена"
				, WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON | WS_TABSTOP
				,offs + 100, 220, 90, 25, h, (HMENU)IDCANCEL, (HINSTANCE)::GetModuleHandle(NULL), NULL
				);
			RECT r;
			GetWindowRect(hParent, &r);
			static const int width = 405;
			static const int height = 285;
			int x = r.left +(r.right - r.left - width) / 2;
			int y = r.top +(r.bottom - r.top - height) / 2;
			MoveWindow(h, x, y, width, height, FALSE);
			e->handlers.Init(e->hEdit);
		}
		return TRUE;
	}
	return FALSE;
}
//-----------------------------------------------------------------------------------------
UpDownDlg::UpDownDlg(HWND hWnd
		, wchar_t *title, int initValue, bool(*ok)(HWND, wchar_t *)
		, bool(*cancel)(HWND, wchar_t *))
		: initValue(initValue)
		, OkBtn(ok)
	    , CancelBtn(cancel)
{	
	char *p = (char *)LocalAlloc(LPTR, 2000);
	DLGTEMPLATE &d = *(DLGTEMPLATE *)p;
	d.style = DS_MODALFRAME | WS_POPUP | WS_CAPTION;
	d.dwExtendedStyle = 0;
	d.cdit = 0;
	wchar_t *c = (wchar_t *)&p[sizeof(DLGTEMPLATE) + 4];
    wcscpy(c, title);
	
	DialogBoxIndirectParam((HINSTANCE)::GetModuleHandle(NULL), &d, hWnd, (DLGPROC)Proc, (LPARAM)this);
    LocalFree((HLOCAL)p);
}
LRESULT CALLBACK UpDownDlg::Proc(HWND h, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch(msg)
	{
	case WM_COMMAND:
		switch(wParam)
		{
		case IDOK:
			{
				UpDownDlg *e = (UpDownDlg *)GetWindowLongPtr(h, GWLP_USERDATA);
				GetWindowText(e->hwndUpDown, e->buf, dimention_of(e->buf));
				if((*e->OkBtn)(h, e->buf))EndDialog(h, TRUE);
			}
			return TRUE;
		case IDCANCEL: 
			{
				UpDownDlg *e = (UpDownDlg *)GetWindowLongPtr(h, GWLP_USERDATA);
				GetWindowText(e->hwndUpDown, e->buf, dimention_of(e->buf));
				if((*e->CancelBtn)(h, e->buf))EndDialog(h, TRUE);
			}
			return TRUE;
		}
		break;
	case WM_INITDIALOG:
		{
			HWND hParent = GetParent(h);
			SetWindowLongPtr(h, GWLP_USERDATA, lParam);
			UpDownDlg *e = (UpDownDlg *)lParam;
			
			e->hwndUpDown = CreateWindowEx(WS_EX_CLIENTEDGE, L"edit", e->buf
				, //WS_BORDER | 
				WS_VISIBLE | WS_CHILD | ES_LEFT | WS_TABSTOP
				,10, 10, 100, 25, h, 0, (HINSTANCE)::GetModuleHandle(NULL), NULL
				);
			CreateUpDownControl (
				WS_CHILD | WS_BORDER | WS_VISIBLE | 
				UDS_WRAP | UDS_ARROWKEYS | UDS_ALIGNRIGHT | 
				UDS_SETBUDDYINT, 0, 0, 0, 0, h, 0, (HINSTANCE)::GetModuleHandle(NULL),
				e->hwndUpDown,50, 1, e->initValue);
			static const int offs = 10;
			CreateWindow(L"button", L"Применить"
				, WS_VISIBLE | WS_CHILD | WS_TABSTOP //| BS_PUSHBUTTON
				,offs, 45, 110, 30, h, (HMENU)IDOK, (HINSTANCE)::GetModuleHandle(NULL), NULL
				);
			CreateWindow(L"button", L"Отмена"
				, WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON | WS_TABSTOP
				,offs + 120, 45, 110, 30, h, (HMENU)IDCANCEL, (HINSTANCE)::GetModuleHandle(NULL), NULL
				);
			RECT r;
			GetWindowRect(hParent, &r);
			static const int width = 255;
			static const int height = 115;
			int x = r.left +(r.right - r.left - width) / 2;
			int y = r.top +(r.bottom - r.top - height) / 2;
			MoveWindow(h, x, y, width, height, FALSE);
		}
		return TRUE;
	}
	return FALSE;
}