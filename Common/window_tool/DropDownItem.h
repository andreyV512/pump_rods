#pragma once
#include "TEvent.h"
class DropDown: public TEvent
{
	struct TObj{};
	void (TObj::*ptr)(int);
	TObj *obj;
	int width;
public:
	int current;
	HWND hWnd;
	DropDown();
	void Init(HWND, int fontSize, const wchar_t **items, int count);
	void Do(TCommand &);
	void Size(int x, int y, int width);
	template<class T>void SetCommandHandler(T *t, void (T::*t_ptr)(int))
	{
		obj = (TObj *)t;
		ptr = (void (TObj::*)(int))t_ptr;
	}
};