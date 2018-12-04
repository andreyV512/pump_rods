#include "GridWindow.h"
#include "GridData\TableData.h"
#include "templates/typelist.hpp"
#include <strsafe.h>
#include "window_tool\Grid.h"

GridWindow::GridWindow()
	: oRClick(NULL)
	, oLClick(NULL)
	, ptrRClick(NULL)
	, ptrLClick(NULL)
{}

void GridWindow::SetHeaderToGrid(HWND h)
{	
	data->Header(h);
}
void GridWindow::SetDataToGrid(LV_DISPINFO *d)
{
	unsigned i = d->item.iItem;
	if(i < data->Count())
	{
		const wchar_t *s = data->Cell(i, d->item.iSubItem);
		if(0 != *s) StringCchCopy(d->item.pszText, wcslen(s) + 1, s);
	}
}
void GridWindow::LClick(LPNMITEMACTIVATE d)
{
	if(d->iItem >= 0)
	{
		wchar_t buffer[256];
		ListView_GetItemText(d->hdr.hwndFrom, d->iItem, d->iSubItem, buffer, 256);
		if(oLClick&&ptrLClick)(oLClick->*ptrLClick)(d->iSubItem, buffer);
	}
}

void GridWindow::RClick(LPNMITEMACTIVATE d)
{
	if(d->iItem >= 0)
	{
		wchar_t buffer[256];
		ListView_GetItemText(d->hdr.hwndFrom, d->iItem, d->iSubItem, buffer, 256);
		if(oRClick&&ptrRClick)(oRClick->*ptrRClick)(d->iSubItem, buffer);
	}
}

void GridWindow::SetData(TableData *d)
{
	data = d;
}