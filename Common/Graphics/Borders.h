#pragma once
#include "Graphics/Chart.h"
class VBorder
{
public:
	double value;
	int color;
	int widthPen;
	int dashStyle;
public:
  Chart &chart;
  VBorder(Chart &chart);
  void Draw();
  void SetCoordinate(POINTS &);
};

class HBorder
{
public:
	double value;
	int color;
	int widthPen;
	int dashStyle;
public:
  Chart &chart;
  HBorder(Chart &chart);
  void Draw();
};