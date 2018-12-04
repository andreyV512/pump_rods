#pragma once 

class Chart;
class StepSeries
{
public:
	Chart &chart;
	int count;
	double *data;
	StepSeries(Chart &chart);
	int color;
	void Draw();
	void SetData(double *data, int countSamples, double minValue, double maxValue);
};