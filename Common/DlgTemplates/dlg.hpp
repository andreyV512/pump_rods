#pragma once
#include "templates.hpp"
#pragma warning(disable : 4995 4996)
template<class items_list>struct Dlg
{
	int dy;
	HWND hDlg;
	TL::Factory<items_list> *factory;
	template<class O, class P>struct Init
	{
		void operator()(O *o, Dlg *dlg)
		{
			o->hEdit = CreateWindow(L"edit", o->Data()
				, WS_BORDER | WS_VISIBLE | WS_CHILD | ES_LEFT | WS_TABSTOP
				, 10, dlg->dy, 280, 20, dlg->hDlg, 0, (HINSTANCE)::GetModuleHandle(NULL), NULL
				);
			CreateWindow(L"static", o->Text()
				, WS_VISIBLE | WS_CHILD
				, 280 + 20, dlg->dy + 3, 280, 20, dlg->hDlg, 0, (HINSTANCE)::GetModuleHandle(NULL), NULL
				);
			dlg->dy += 25;
		}
	};
	template<class O, class P>struct Test
	{
		bool operator()(O *o, P *p)
		{
			wchar_t buffer[128];
			GetWindowText(o->hEdit, buffer, dimention_of(buffer));
			bool ret = ::Test<typename O::TypeValue>()(buffer);
			if(ret)
			{
				o->tmp = Wchar_to<O::TypeValue>()(buffer);
			}
			else MessageBox(GetParent(o->hEdit), o->Text(), L"Ошибка!!!", MB_ICONEXCLAMATION);
			return ret;
		}
	};
	template<class O, class P>struct Ok{void operator()(O *o, P *){o->value = o->tmp;}};
	Dlg(HWND hwnd, wchar_t *title, TL::Factory<items_list> *factory) : dy(10), factory(factory)
	{
		char *p = (char *)LocalAlloc(LPTR, 2048);
		DLGTEMPLATE &d = *(DLGTEMPLATE *)p;
		d.style = DS_MODALFRAME | WS_POPUP | WS_CAPTION;
		d.dwExtendedStyle = 0;
		d.cdit = 0;
		wchar_t *c = (wchar_t *)&p[sizeof(DLGTEMPLATE) + 4];
		wcscpy(c, title);

		DialogBoxIndirectParam((HINSTANCE)::GetModuleHandle(NULL), &d, hwnd, (DLGPROC)Proc, (LPARAM)this);
		LocalFree((HLOCAL)p);
	}
	static LRESULT CALLBACK Proc(HWND h, UINT msg, WPARAM wParam, LPARAM lParam)
	{
		switch(msg)
		{
		case WM_COMMAND:
			switch(wParam)
			{
			case IDOK:
				{
					Dlg *e = (Dlg *)GetWindowLongPtr(h, GWLP_USERDATA);
					if(TL::find<items_list, Test>()(e->factory, e))
					{
						TL::foreach<items_list, Ok>()(e->factory, e);
						EndDialog(h, TRUE);
					}

				}
				return TRUE;
			case IDCANCEL: EndDialog(h, TRUE); return TRUE;			
			}
			break;
		case WM_INITDIALOG:
			{
				SetWindowLongPtr(h, GWLP_USERDATA, lParam);
				Dlg *e = (Dlg *)lParam;
				e->hDlg = h;
				TL::foreach<items_list, Init>()(e->factory, e);
				static const int width = 650;
				static const int offs = width / 2 - 100;
				CreateWindow(L"button", L"Применить"
					, WS_VISIBLE | WS_CHILD | WS_TABSTOP
					,offs, e->dy, 90, 25, h, (HMENU)IDOK, (HINSTANCE)::GetModuleHandle(NULL), NULL
					);
				CreateWindow(L"button", L"Отмена"
					, WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON | WS_TABSTOP
					,offs + 100, e->dy, 90, 25, h, (HMENU)IDCANCEL, (HINSTANCE)::GetModuleHandle(NULL), NULL
					);
				RECT r;
				GetWindowRect(GetParent(h), &r);

				static const int height = e->dy + 65;
				int x = r.left +(r.right - r.left - width) / 2;
				int y = r.top +(r.bottom - r.top - height) / 2;
				MoveWindow(h, x, y, width, height, FALSE);
			}
			return TRUE;
		}
		return FALSE;
	}
};