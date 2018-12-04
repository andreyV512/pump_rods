#pragma once

#include <Windows.h>
class Chart;
class FixedGridSeries
{
	class TObj{};
	typedef bool(TObj::*Tptr)(unsigned, unsigned, double &, unsigned &);
	TObj *obj;
	Tptr SetColorBar;
	bool GetColorCellDefault(unsigned sensor, int zone, double &data, unsigned &color);
public:
	//int countZones;
	unsigned sensorCount;
	Chart &chart;
	FixedGridSeries(Chart &chart);
	void Draw();
	void OffsetToPixel(WORD &, WORD &, int , bool );
	template<class T>void SetColorCellHandler(T *t, bool(T::*ptr)(unsigned, int, double &, unsigned &))
	{
		obj = (TObj *)t;
		SetColorBar = (Tptr)ptr;
	}
};

class FixedGrid
{
public:
  Chart &chart;
  double &dX, &dY;
  FixedGrid(Chart &);
  void Draw();
};
class VerticalFixedGrid
{
public:
  Chart &chart;
  double &dX, &dY;
  VerticalFixedGrid(Chart &);
  void Draw();
};

class FixedLeftAxes
{
public:
  int leftOffset;
  Chart &chart;
  FixedLeftAxes(Chart &);
  void Draw();
};

class NoOffsetLeftAxes
{
public:
  int leftOffset;
  Chart &chart;
  NoOffsetLeftAxes(Chart &);
  void Draw();
};

void OffsetToPixel(Chart &chart, WORD &offsX, WORD &offsY, int delta, bool horisontalMove);
