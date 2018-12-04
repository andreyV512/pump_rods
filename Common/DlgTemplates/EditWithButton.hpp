#pragma once

 template<class T, int N, int edit_width = 140>struct EditWithButtonItems
{
	static const int DY = 25;
	HWND Init(HWND h, int &x, int &width, int &dy, T &t)
	{
		HWND hWnd = CreateWindowEx(WS_EX_CLIENTEDGE, L"edit", Wchar_from<typename T::type_value>(t.value)()
		, WS_BORDER | WS_VISIBLE | WS_CHILD | ES_LEFT | WS_TABSTOP | ES_READONLY
			, x, dy, edit_width, 25, h, 0, (HINSTANCE)::GetModuleHandle(NULL), NULL
			);
		HWND bt = CreateWindow(L"button", L"..."
				, WS_VISIBLE | WS_CHILD | WS_TABSTOP 
				,edit_width + 20, dy, 25, 25, h, (HMENU)N, (HINSTANCE)::GetModuleHandle(NULL), NULL
				);
		CreateWindow(L"static", ParamTitle<T>()()
			, WS_VISIBLE | WS_CHILD
			, edit_width + 55, dy + 3, width - edit_width - 30, 20, h, 0, (HINSTANCE)::GetModuleHandle(NULL), NULL
			);
		dy += DY;
		return hWnd;
	}
};