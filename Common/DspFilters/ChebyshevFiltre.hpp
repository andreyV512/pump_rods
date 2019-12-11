#pragma once
#include "include/DspFilters/ChebyshevII.h"
#include "include/DspFilters/SmoothedFilter.h"

static const int filter_max_order = 10;
template<class X>class DSPFiltre
{
public:
	typedef X T;
private:
	X filtre;
	typename T::State<Dsp::DirectFormII> state;
public:
	double Simple(double value)
	{
		return state.process(value, filtre);
	}
	void Setup(int order, int sample_rate, double cutoffFrequency, double stopBandDb)
	{	
		filtre.setup(order, sample_rate, cutoffFrequency, stopBandDb);
	}
};

class LowFiltre : public DSPFiltre<Dsp::ChebyshevII::LowPass<filter_max_order>>{};
class HighFiltre: public DSPFiltre<Dsp::ChebyshevII::HighPass<filter_max_order>>{};

class BandPassFiltre
{
public:
	typedef Dsp::ChebyshevII::BandPass<filter_max_order> T;
private:
	T filtre;
	T::State<Dsp::DirectFormII> state;
public:
	double Simple(double value)
	{
		return state.process(value, filtre);
	}
	void Setup(int order, int sample_rate, int widthFrequency, double centerFrequency, double passBandRippleDb)
	{
		filtre.setup(order, sample_rate, centerFrequency, widthFrequency, passBandRippleDb);
	}
};

class DoubleFiltre
{
public:
	class O{};
	O *o;
	double (O::*ptr)(double );
public:
	LowFiltre lowFiltre;
	BandPassFiltre bandPassFiltre;
	double Simple(double value)
	{
		return (o->*ptr)(value);
	}
};