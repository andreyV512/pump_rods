#pragma once
#include "include/ChebyshevII.h"

template<class T>class DSPFiltre: public T::State<Dsp::DirectFormII>
{
	T m_design;
public:
	inline double operator()(double value)
	{
		return process(value, m_design);
	}

	void Setup(int sample_rate, double cutoffFrequency, double stopBandDb)
	{	
		Dsp::Params params;
		params[0] = sample_rate;
		params[1] = 3;
		params[2] = cutoffFrequency;
		params[3] = stopBandDb;
		m_design.setParams (params);
	}
};

class LowFiltre : public DSPFiltre<Dsp::ChebyshevII::Design::LowPass<3>>{};
class HighFiltre: public DSPFiltre<Dsp::ChebyshevII::Design::HighPass<3>>{};

class BandPassFiltre: public Dsp::ChebyshevII::BandPass<3>::State<Dsp::DirectFormII>
{
	Dsp::ChebyshevII::BandPass<3> m_design;
public:
	inline double operator()(double value)
	{
		return process(value, m_design);
	}
	void Setup(int sample_rate, int widthFrequency, double centerFrequency, double stopBandDb)
	{
		m_design.setup(3, sample_rate, centerFrequency, widthFrequency, 0.01);
	}
};