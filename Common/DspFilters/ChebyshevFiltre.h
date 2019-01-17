#pragma once

class DspFiltre
{
public:
		//.Setup(4000, 3, 50, 40); ������� �������������, ������� �������, ������� �����, ���������
	virtual void Setup(int sample_rate, int order, double cutoffFrequency, double stopBandDb) = 0;
	virtual double operator()(double value) = 0;
};

class LowFiltre: public DspFiltre
{
	class Impl;
	Impl &impl;
	char implementation_buffer[760];
public:
	LowFiltre();
	//.Setup(4000, 3, 50, 40); ������� �������������, ������� �������, ������� �����, ���������
	void Setup(int sample_rate, int order, double cutoffFrequency, double stopBandDb);
	double operator()(double value);
};

class HighFiltre: public DspFiltre
{
	class Impl;
	Impl &impl;
	char implementation_buffer[760];
public:
	HighFiltre();
	//.Setup(4000, 3, 50, 40); ������� �������������, ������� �������, ������� �����, ���������
	void Setup(int sample_rate, int order, double cutoffFrequency, double stopBandDb);
	double operator()(double value);
};