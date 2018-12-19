#pragma once
#include "window_tool/message.h"
#include "MainWindowGrid.h"

class MainWindow
{
	int lastIndex;
	MainWindowGrid mainWindowGrid;
public:
	static const int IDT_TIMER1 = 999;
	HWND hWnd;
	MainWindow();
	void operator()(TSize &);
	void operator()(TCommand &);
	void operator()(TGetMinMaxInfo &);
	LRESULT operator()(TCreate &);
	void operator()(TDestroy &);
	void operator()(TTimer &);
	LRESULT operator()(TNotify &);
};
