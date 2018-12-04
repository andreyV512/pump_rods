#include "Pass.h"
//#include "App/Config.h"
#if 0
template<class List, int offs = 0>struct MkPass;
template<class Head, class Tail, int offs>struct MkPass<Tlst<Head, Tail>, offs>: MkPass<Tail, offs + 1>
{
	MkPass() {buffer[offs] = Head::value;}
};
template<int offs>struct MkPass<NullType, offs>
{
  char buffer[offs];
};

bool Password(int c)
{
	static int index = 0;
	static MkPass<password_list> pass;	
	if(c == pass.buffer[index]) ++index; else index = 0;	
//#pragma message("ВКЛЮЧИТЬ ПАРОЛЬ ")
	return TL::Length<password_list>::value == index;
}
#endif
 
PasswordDlg::PasswordDlg(const wchar_t *pass, const wchar_t *title)
	: pass(pass), title(title)
{}
//---------------------------------------------------------------------------------
#pragma warning(disable : 4996)
bool PasswordDlg::Do(HWND h)
{
#ifndef DISABLE_PASSWORD	
	char *p = (char *)LocalAlloc(LPTR, 2048);
	DLGTEMPLATE &d = *(DLGTEMPLATE *)p;
	d.style = DS_MODALFRAME | WS_POPUP | WS_CAPTION;
	d.dwExtendedStyle = 0;
	d.cdit = 0;
	wchar_t *c = (wchar_t *)&p[sizeof(DLGTEMPLATE) + 4];
    wcscpy(c, title);
	
	bool ret = 0 != DialogBoxIndirectParam((HINSTANCE)::GetModuleHandle(NULL), &d, h, (DLGPROC)Proc, (LPARAM)this);
    LocalFree((HLOCAL)p);
	return ret;
#else
	return true;
#endif
}
//--------------------------------------------------------------------------------------
LRESULT CALLBACK PasswordDlg::Proc(HWND h, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch(msg)
	{
	case WM_COMMAND:
		switch(wParam)
		{
		case IDOK:
			{
				INT_PTR ret = FALSE;
				wchar_t buf[256];
				PasswordDlg *e = (PasswordDlg *)GetWindowLongPtr(h, GWLP_USERDATA);
				GetWindowText(e->hEdit, buf, 256);
				if(0 == wcscmp(buf, e->pass))
				{
				   EndDialog(h, TRUE);
				   return TRUE;
				}
			}
			return FALSE;
		case IDCANCEL: 
			{
				EndDialog(h, FALSE);
			}
			return TRUE;
		}
		break;
	case WM_INITDIALOG:
		{
			HWND hParent = GetParent(h);
			SetWindowLongPtr(h, GWLP_USERDATA, lParam);
			PasswordDlg *e = (PasswordDlg *)lParam;			
			e->hEdit = CreateWindowEx(WS_EX_CLIENTEDGE, L"edit", L""
				, WS_BORDER | WS_VISIBLE | WS_CHILD | ES_LEFT | WS_TABSTOP | ES_PASSWORD
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
//-------------------------------------------------------------------------------------------
TypesizePasswordDlg::TypesizePasswordDlg()
	: PasswordDlg(L"1234", L"Пароль для типоразмера"){}
//---------------------------------------------------------------------------------------------
bool TypesizePasswordDlg::Do(HWND h)
{
	return  PasswordDlg::Do(h);
}
//----------------------------------------------------------------------------------------
OptionPasswordDlg::OptionPasswordDlg()
	: PasswordDlg(L"5678", L"Пароль для настроек"){}
//------------------------------------------------------------------------------------------------
bool OptionPasswordDlg::Do(HWND h)
{
	return PasswordDlg::Do(h);
}
//--------------------------------------------------------------------------------------------------

