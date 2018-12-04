#pragma once

class TableData
{
public:
	virtual void Init()					   = 0;
	virtual void Add(wchar_t *)			   = 0;
	virtual void Del(wchar_t *)			   = 0;
	virtual const wchar_t *Cell(int, int)  = 0;
	virtual unsigned Count()			   = 0;
	virtual void Header(HWND) = 0;
};