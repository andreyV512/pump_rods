#pragma once
#include "include/DspFilters/ChebyshevII.h"
#include "include/DspFilters/SmoothedFilter.h"

static const int filter_size = 7;
template<class X>class DSPFiltre: public X::State<Dsp::DirectFormII>
{
public:
	typedef X T;
private:
	X m_design;
public:
	double Simple(double value)
	{
		return process(value, m_design);
	}
	void Setup(int order, int sample_rate, double cutoffFrequency, double stopBandDb)
	{	
		m_design.setup(order, sample_rate, cutoffFrequency, stopBandDb);
	}
};

class LowFiltre : public DSPFiltre<Dsp::ChebyshevII::LowPass<filter_size>>{};
class HighFiltre: public DSPFiltre<Dsp::ChebyshevII::HighPass<filter_size>>{};

//class BandPassFiltre: public Dsp::ChebyshevII::BandPass<filter_size>::State<Dsp::DirectFormII>
//{
//public:
//	typedef Dsp::ChebyshevII::BandPass<filter_size> T;
//private:
//	T m_design;
//public:
//	inline double operator()(double value)
//	{
//		return process(value, m_design);
//	}
//	void Setup(int order, int sample_rate, int widthFrequency, double centerFrequency, double passBandRippleDb)
//	{
//		m_design.setup(order, sample_rate, centerFrequency, widthFrequency, passBandRippleDb);
//	}
//};

class BandPassFiltre//: public Dsp::ChebyshevII::BandPass<filter_size>//::State<Dsp::DirectFormII>
{
	// Dsp::CascadeStages<filter_size>::State<Dsp::DirectFormII> state;

public:
	typedef Dsp::ChebyshevII::BandPass<filter_size> T;
	//Dsp::ChebyshevII::Design::BandPass<filter_size> f;
	//Dsp::ChebyshevII::Design::BandPass <filter_size>::State<Dsp::DirectFormII> xxx;
	//BandPassFiltre(): f(1){}
	 Dsp::SimpleFilter <T, 1> f;
	double Simple(double value)
	{
		//return xxx.process(value, f);
		return f.m_state.m_state[0].process(value, f);
	}
	void Setup(int order, int sample_rate, int widthFrequency, double centerFrequency, double passBandRippleDb)
	{
		f.setup(order, sample_rate, centerFrequency, widthFrequency, passBandRippleDb);
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