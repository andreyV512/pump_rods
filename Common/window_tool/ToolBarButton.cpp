#include "stdafx.h"
#include "ToolBarButton.h"
#include <Commctrl.h>

void SetToolButtonText(HWND h, int id, wchar_t *txt)
{
	TBBUTTONINFO t;
	t.cbSize = sizeof(t);
	t.dwMask = TBIF_TEXT;
	t.cchText = int(1 + wcslen(txt));
	t.pszText = txt;
	SendMessage(h, TB_SETBUTTONINFO, id, (LPARAM)&t);
}