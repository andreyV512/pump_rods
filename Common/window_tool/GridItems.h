#pragma once
#pragma warning (disable : 4995)
#include <windows.h>
#include <commctrl.h>
#include "window_tool/message.h"
//----------------------------------------------------------------------
template<class T>struct header_table
{
	LPWSTR name()
	{
		zprint("  header_table\n");
		return L"header_table";
	}
	static const int width = 128;
};
#define HEADER_TABLE(T, width_, txt)template<>struct header_table<T>\
{\
	LPWSTR name(){return txt;}\
	static const int width = width_;\
};
//-----------------------------------------------------------------
void SetRow(HWND hWnd, int count);
void UpdateRow(HWND h);
void UpdateRowAll(HWND h);
//-----------------------------------------------------------------
struct GridHandlers
{
	virtual void SetDataToGrid(LV_DISPINFO *){}
	virtual void SetColorToGrid(NMLVCUSTOMDRAW *){}
	virtual void SetHeaderToGrid(HWND){}
	virtual void ItemChanged(HWND){}
	virtual void Timer(HWND){}
	virtual void LClick(LPNMITEMACTIVATE ){}
	virtual void RClick(LPNMITEMACTIVATE ){}
	virtual ~GridHandlers(){}
};
//----------------------------------------------------------------