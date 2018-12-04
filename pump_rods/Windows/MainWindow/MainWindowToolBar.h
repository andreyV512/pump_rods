#pragma once
#include <Windows.h>

class MainWindowToolBar
{
public:
	HWND hWnd;
	HWND Init(HWND);
	void Size();
	static const int Width();
};





