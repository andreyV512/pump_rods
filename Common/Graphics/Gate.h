#pragma once

class Chart;

class Gate
{
public:
	Chart &chart;
	double x, y, width;
	unsigned color;	
	Gate(Chart &);
	void Draw();
};