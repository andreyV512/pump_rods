#include "window_tool\Grid.h"
#include <strsafe.h>
#include <windowsx.h>
#include "templates/typelist.hpp"

#include "tools_debug\DebugMess.h"
//-----------------------------------------------------------------------------------------
void SetGridHeaderArray(HWND h, HeaderData *header, int count)
{
	LV_COLUMN lvc;
	lvc.mask = LVCF_FMT | LVCF_WIDTH | LVCF_TEXT | LVCF_SUBITEM;
	lvc.fmt = LVCFMT_LEFT;
	for(int i = 0; i < count; ++i)
	{
		lvc.iSubItem = i;
		lvc.pszText = header[i].name;
		lvc.cx = header[i].width;
		ListView_InsertColumn(h, lvc.iSubItem, &lvc);
	}
}
//------------------------------------------------------------------------------------
GridNotify::~GridNotify()
{
	SetWindowLongPtr(hWnd, GWL_USERDATA, 0);
}
//---------------------------------------------------------------------------------
void GridNotify::Create(HWND hwnd, GridHandlers *h)
{
	handlers = h;
	hWnd = CreateWindowEx(
		WS_EX_CLIENTEDGE
		, WC_LISTVIEW, L"",
		WS_VISIBLE | WS_CHILD | LVS_REPORT 	
		, 0, 0, 0, 0,
		hwnd, NULL, (HINSTANCE)::GetModuleHandle(NULL), NULL
		);
	SetWindowLongPtr(hWnd, GWLP_USERDATA, (LONG)this);
	ListView_SetExtendedListViewStyleEx(hWnd
		, 0
		, LVS_EX_GRIDLINES | LVS_EX_FULLROWSELECT | LVS_EX_HEADERDRAGDROP  
		);
#if 1
	HFONT font = CreateFont(16,0,0,90,900,TRUE,0,0,ANSI_CHARSET,0,0,0,0, L"Times New Roman"); 
	SendMessage(hWnd, WM_SETFONT, (WPARAM)font, (LPARAM)0);
#endif
	if(NULL != handlers) handlers->SetHeaderToGrid(hWnd);
}
//------------------------------------------------------------------------------------
void GridNotify::Size(int x, int y, int width, int height)
{	
	MoveWindow(hWnd, x, y, width, height, TRUE);
}
//--------------------------------------------------------------------------------------------
LRESULT GridNotify::Do(TNotify &m)
{
	switch (m.pnmh->code)
	{
	case LVN_GETDISPINFO:
		{
			if(handlers)handlers->SetDataToGrid((LV_DISPINFO *)m.pnmh);	
		}
		break;
	case NM_CUSTOMDRAW:
		{
			NMLVCUSTOMDRAW* d = (NMLVCUSTOMDRAW*)m.pnmh;			
			switch(d->nmcd.dwDrawStage)
			{
			case CDDS_PREPAINT: 
				return CDRF_NOTIFYITEMDRAW;
			case CDDS_ITEMPREPAINT: 
				return CDRF_NOTIFYSUBITEMDRAW;
			case (CDDS_ITEM | CDDS_SUBITEM | CDDS_PREPAINT):
				{
					if(handlers)handlers->SetColorToGrid(d);				
				}
				return CDRF_NEWFONT;	
			}
		}
		break;	
	case NM_CLICK:
		{
			if(handlers)handlers->LClick((LPNMITEMACTIVATE)m.pnmh);
		}
		break;
	case NM_RCLICK:
		{
			if(handlers)handlers->RClick((LPNMITEMACTIVATE)m.pnmh);
		}
		break;
	}
	return DefWindowProc(m.hwnd, WM_NOTIFY, (WPARAM)m.idCtrl, (LPARAM)m.pnmh);
}
//---------------------------------------------------------------------------------------------
