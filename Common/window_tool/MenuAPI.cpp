#pragma once
#include "MenuAPI.h"
#include <CommCtrl.h>
//---------------------------------------------------------------------------
void EventDo(TCommand &m)
{
	if(m.hControl)
	{
		if(m.id)
		{
			TBBUTTONINFO button_info = {};
			button_info.cbSize = sizeof(button_info);
			button_info.dwMask = TBIF_LPARAM;
			SendMessage(m.hControl, (UINT) TB_GETBUTTONINFO, m.id , (LPARAM)&button_info);
			if(NULL != (void *)button_info.lParam)((void (__cdecl *)(HWND))(button_info.lParam))(m.hwnd);
		}
		else if(TEvent *x = (TEvent *)GetWindowLongPtr(m.hControl, GWLP_USERDATA))
		{
			if(NULL != x)x->Do(m);
		}
	}
	else
	{
		MENUITEMINFO mii = {};
		mii.cbSize = sizeof(MENUITEMINFO);
		mii.fMask = MIIM_DATA;
		if(GetMenuItemInfo(GetMenu(m.hwnd), m.id, false, &mii))
		{
			if(NULL != mii.dwItemData)((void (__cdecl *)(HWND))(mii.dwItemData))(m.hwnd);
		}
	}

}

LRESULT EventDo(TNotify &m)
{
	TEvent *x = (TEvent *)GetWindowLongPtr(m.pnmh->hwndFrom, GWLP_USERDATA);
	if(NULL != x) return x->Do(m);
	return 0;
}