#pragma once
#include "Graphics/Chart.h"

class LeftAxesGrid
{
public:
	double minA, maxA;
public:
  Chart &chart;
  LeftAxesGrid(Chart &chart);
  void Draw();
};
class BottomAxesGrid
{
	double minA, maxA;
public:
  Chart &chart;
  BottomAxesGrid(Chart &chart);
  void Draw();
};
class GridGrid
{
public:
  Chart &chart;
  GridGrid(Chart &chart) : chart(chart) {}
  void Draw();
  void DrawSample(Chart &, int);
};

class GridSeries
{
	class TObj{};
	typedef bool(TObj::*Tptr)(unsigned, unsigned, double &, unsigned &);
	bool GetColorCellDefault(unsigned sensor, unsigned zone, double &data, unsigned &color);	
	TObj *obj;
	Tptr SetColorBar;
public:
	unsigned sensorCount;
	Chart &chart;
	GridSeries(Chart &chart);
	void Draw();
	template<class T>void SetColorCellHandler(T *t, bool(T::*ptr)(unsigned, unsigned, double &, unsigned &))
	{
		obj = (TObj *)t;
		SetColorBar = (Tptr)ptr;
	}
};