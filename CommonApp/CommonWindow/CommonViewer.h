#pragma once

#include "window_tool/message.h"
#include "Graphics/Chart.h"
#include "Graphics/ColorLabel.h"

class CommonViewer
{
public:
	HWND hWnd;	
	Gdiplus::Bitmap *backScreen;
	Chart *chart;
	bool mouseMove;
	const int count_sensors;
public:
	TMouseMove storedMouseMove;
	ColorLabel label;
public:
	Cursor cursor;
	int currentX;
//	int currentY;
public:
	CommonViewer(Chart *, const int &);
	virtual ~CommonViewer();
	LRESULT operator()(TCreate &);
	void operator()(TSize &);
	void operator()(TPaint &);
	void operator()(TMouseMove &);
	void operator()(TLButtonDbClk &);
	void operator()(TMouseWell &);
	void operator()(TLButtonDown &);
	void operator()(TCommand &);
};