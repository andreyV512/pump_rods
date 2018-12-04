#pragma once
#pragma warning(disable : 4996)
template<class Items>struct DlgTemplate
{
	typedef DlgTemplate<Items> TSelf;
	static LRESULT CALLBACK Proc(HWND h, UINT msg, WPARAM wParam, LPARAM lParam)
	{
		switch(msg)
		{
		case WM_COMMAND:
			switch(wParam)
			{
			case IDOK:		
				{			
					TSelf *e = (TSelf *)GetWindowLongPtr(h, GWLP_USERDATA);
					if(e->items->Ok(h)) EndDialog(h, IDOK);		
				}						
				return TRUE;
			case IDCANCEL: 
				{
					TSelf *e = (TSelf *)GetWindowLongPtr(h, GWLP_USERDATA);
					e->items->Cancel(h);
					EndDialog(h, IDCANCEL);
				}
				return TRUE;
			}
			break;
		case WM_INITDIALOG:
			{				
                SetWindowLongPtr(h, GWLP_USERDATA, lParam);
				TSelf *e = (TSelf *)lParam;	
				
				int width = 300;
				int dy = 0;
				e->items->Init(h, dy, width);

				RECT r;
				GetWindowRect(GetParent(h), &r);
				dy += 35;

                static const int offs = 50;
				CreateWindow(L"button", L"Применить"
				, WS_VISIBLE | WS_CHILD | WS_TABSTOP
				,offs, dy - 30, 90, 25, h, (HMENU)IDOK, (HINSTANCE)::GetModuleHandle(NULL), NULL
				);
				CreateWindow(L"button", L"Отмена"
				, WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON | WS_TABSTOP
				,offs + 100, dy - 30, 90, 25, h, (HMENU)IDCANCEL, (HINSTANCE)::GetModuleHandle(NULL), NULL
				);
                dy += 35;
				int x = r.left +(r.right - r.left - width) / 2;
				int y = r.top +(r.bottom - r.top - dy) / 2;
				MoveWindow(h, x, y, width, dy, FALSE);
			}
			return TRUE;			
		}
		return FALSE;
	}
public:
    Items *items;
	unsigned result;
	DlgTemplate(HWND hWnd, wchar_t *title, Items *items)
	: items(items)
	{
		char *p = (char *)LocalAlloc(LPTR, 2000);
		DLGTEMPLATE &d = *(DLGTEMPLATE *)p;
		d.style = DS_MODALFRAME | WS_POPUP | WS_CAPTION;
		d.dwExtendedStyle = 0;
		d.cdit = 0;
		wchar_t *c = (wchar_t *)&p[sizeof(DLGTEMPLATE) + 4];
		wcscpy(c, title);
		result = DialogBoxIndirectParam((HINSTANCE)::GetModuleHandle(NULL), &d, hWnd, (DLGPROC)Proc, (LPARAM)this);
		LocalFree((HLOCAL)p);
	}
};
//---------------------------------------------------------------------


