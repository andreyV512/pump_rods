#pragma once
#include "TEvent.h"

class Button : public TEvent
{
	class TObj{} *obj;
	void(TObj::*ptr)();
	HWND hWnd;
public:
	Button();
	void Do(TCommand &);
	void Create(HWND, wchar_t *);
	void Size(int, int);
	template<class T>void SetCommandHandler(T *o, void(T::*p)())
	{
		obj = (TObj *)o;
		ptr = (void(TObj::*)())p;
	}
};