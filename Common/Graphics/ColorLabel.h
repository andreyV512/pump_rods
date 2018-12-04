#pragma once
#include "Graphics\Color.h"
#include <Windows.h>
#include <Gdiplus.h>
class ColorLabel
{		
public:  
	int fontHeight;
	int left;
	int top;
	unsigned backColor;
	wchar_t buffer[512];	
	Gdiplus::RectF rect;
	ColorLabel(): fontHeight(7), left(30), top(5), backColor(BACK_GROUND){buffer[0] = 0;}
	wchar_t *operator=(wchar_t *);
	wchar_t *operator=(char *);
	void Draw(Gdiplus::Graphics &);	
};