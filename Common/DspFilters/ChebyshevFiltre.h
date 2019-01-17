#pragma once

class LowFiltre
{
	class Impl;
	Impl &impl;
	char implementation_buffer[760];
public:
	LowFiltre();
	//f.Setup(4000, 3, 50, 40); частота дискретизации, порядок фильтра, частота среза, затухание
	void Setup(int sample_rate, int order, double cutoffFrequency, double stopBandDb);
	double operator()(double value);
};

class HighFiltre
{
	class Impl;
	Impl &impl;
	char implementation_buffer[760];
public:
	HighFiltre();
	//f.Setup(4000, 3, 50, 40); частота дискретизации, порядок фильтра, частота среза, затухание
	void Setup(int sample_rate, int order, double cutoffFrequency, double stopBandDb);
	double operator()(double value);
};