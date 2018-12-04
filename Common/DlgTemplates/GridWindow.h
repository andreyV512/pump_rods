#pragma once
#include "window_tool\GridItems.h"

class TableData;
class GridWindow: public GridHandlers
{
	struct TClick{};
	typedef void(TClick::*TptrClick)(int , wchar_t *);
	TClick *oRClick, *oLClick;
	TptrClick ptrRClick, ptrLClick; 
	TableData *data;
	void (*ptrProc)(int);
public:
	GridWindow();
	void SetHeaderToGrid(HWND);
	void SetDataToGrid(LV_DISPINFO *);
	void LClick(LPNMITEMACTIVATE );
	void RClick(LPNMITEMACTIVATE );
	template<class T>void SetRClick(T *t,  void(T::*ptr)(int , wchar_t *))
	{
		oRClick   = (TClick *)t;
		ptrRClick = (TptrClick)ptr;
	}
	template<class T>void SetLClick(T *t,  void(T::*ptr)(int , wchar_t *))
	{
		oLClick   = (TClick *)t;
		ptrLClick = (TptrClick)ptr;
	}
	void SetData(TableData *);
};