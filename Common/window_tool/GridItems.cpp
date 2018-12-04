#include "window_tool\GridItems.h"
#include "CommCtrl.h"
void SetRow(HWND hWnd, int count)
{
	LV_ITEM l;
	l.mask = LVIF_TEXT | LVIF_IMAGE;
	int c = ListView_GetItemCount(hWnd);
	for(int i = c; i < count; ++i)
	{
		l.iItem = i;
		l.iSubItem = 0;
		l.pszText = LPSTR_TEXTCALLBACK;
		l.cchTextMax = 0;
		ListView_InsertItem(hWnd, &l);
	}
}

void UpdateRow(HWND h)
{
	int i = ListView_GetTopIndex(h);
	int count = i + ListView_GetCountPerPage(h);
	for(int z = i; z < count; ++z)
	{
		ListView_Update(h, z);
	}
}

void UpdateRowAll(HWND h)
{
	for(int z = 0, count = ListView_GetItemCount(h); z < count; ++z)
	{
		ListView_Update(h, z);
	}
}