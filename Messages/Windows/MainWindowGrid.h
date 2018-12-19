#pragma once
#include "window_tool\GridNotify.hpp"
#include "Log\LogMessages.h"

class MainWindowGrid
{
public :
	struct Handlers
	{
		void operator()(TCellData &);
		void operator()(TCellColor &);
	};
	HWND hWnd;
	MainWindowGrid();
	GridEventHandler<Handlers> handlers;
	void Init(HWND );
};