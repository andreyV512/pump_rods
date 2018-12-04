#pragma once

class Chart;
class EmptyLeftAxes
{
public:
  int leftOffset;
  Chart &chart;
  EmptyLeftAxes(Chart &);
  void Draw();
};