#pragma once
#include "include/DspFilters/ChebyshevII.h"

template<class X>class DSPFiltre: public X::State<Dsp::DirectFormII>
{
public:
	typedef X T;
private:
	X m_design;
public:
	inline double operator()(double value)
	{
		return process(value, m_design);
	}
	void Setup(int order, int sample_rate, double cutoffFrequency, double stopBandDb)
	{	
		m_design.setup(order, sample_rate, cutoffFrequency, stopBandDb);
	}
};

class LowFiltre : public DSPFiltre<Dsp::ChebyshevII::LowPass<5>>{};
class HighFiltre: public DSPFiltre<Dsp::ChebyshevII::HighPass<5>>{};

class BandPassFiltre: public Dsp::ChebyshevII::BandPass<5>::State<Dsp::DirectFormII>
{
public:
	typedef Dsp::ChebyshevII::BandPass<5> T;
private:
	T m_design;
public:
	inline double operator()(double value)
	{
		return process(value, m_design);
	}
	void Setup(int order, int sample_rate, int widthFrequency, double centerFrequency, double passBandRippleDb)
	{
		m_design.setup(order, sample_rate, centerFrequency, widthFrequency, passBandRippleDb);
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
	inline double operator()(double value)
	{
		return (o->*ptr)(value);
	}
};