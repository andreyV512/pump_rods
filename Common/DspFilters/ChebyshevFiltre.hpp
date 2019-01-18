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

	void Setup(int sample_rate, int order, double cutoffFrequency, double stopBandDb)
	{	
		Dsp::Params params;
		params[0] = sample_rate;
		params[1] = order;
		params[2] = cutoffFrequency;
		params[3] = stopBandDb;
		m_design.setParams (params);
	}
};

class LowFiltre : public DSPFiltre<Dsp::ChebyshevII::Design::LowPass<3>>{};
class HighFiltre: public DSPFiltre<Dsp::ChebyshevII::Design::HighPass<3>>{};