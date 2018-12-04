#pragma once
#include "Graphics/Chart.h"

class BarSeries
{
	class TObj{};
	typedef bool(TObj::*Tptr)(int, double &, unsigned &);
	bool GetColorBarDefault(int zone, double &data, unsigned &color);	
	TObj *obj;
	Tptr SetColorBar;
public:
	Chart &chart;
	BarSeries(Chart &chart);
	void Draw();
	template<class T>void SetColorBarHandler(T *t, bool(T::*ptr)(int, double &, unsigned &))
	{
		obj = (TObj *)t;
		SetColorBar = (Tptr)ptr;
	}
	double ValueY(int);
};

class BarSeriesDouble
{
	class TObj{};
	typedef bool(TObj::*Tptr)(int, double &, unsigned &, double &, unsigned &);
	bool GetColorBarDefault(int zone, double &, unsigned &, double &, unsigned &);	
	TObj *obj;
	Tptr SetColorBar;
public:
	Chart &chart;
	BarSeriesDouble(Chart &chart);
	void Draw();
	void DrawSingleBar(int , int);
	void OffsetToPixel(WORD &offs, int delta);
	template<class T>void SetColorBarHandler(T *t, bool(T::*ptr)(int, double &, unsigned &, double &, unsigned &))
	{
		obj = (TObj *)t;
		SetColorBar = (Tptr)ptr;
	}
};

class BarSeriesNoFixed
{
	class TObj{};
	typedef bool(TObj::*Tptr)(int, double &, unsigned &);
	bool GetColorBarDefault(int zone, double &data, unsigned &color);	
	TObj *obj;
	Tptr SetColorBar;
public:
	int dataLength;
	Chart &chart;
	BarSeriesNoFixed(Chart &chart);
	void Draw();
	template<class T>void SetColorBarHandler(T *t, bool(T::*ptr)(int, double &, unsigned &))
	{
		obj = (TObj *)t;
		SetColorBar = (Tptr)ptr;
	}
	double ValueY(int);
};