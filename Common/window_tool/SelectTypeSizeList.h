#pragma once
#include "TEvent.h"
class SelectTypeSizeList: public TEvent
{
	void (*ptr)();
public:
   HWND hWnd;
public: 
	SelectTypeSizeList();
	void Create(HWND);
	void Size(int x, int y, int width);
	void Do(TCommand &);
	void AddMenuItem(wchar_t *);
	void DelMenuItem(wchar_t *);
	void SetCommandHandler(void (*t_ptr)())
	{
		ptr = t_ptr;
	}
};