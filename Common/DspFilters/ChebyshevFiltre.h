#pragma once
#include "include/ChebyshevII.h"

template<class T>class DSPFiltre: public T
{
public:
	double operator()(double value)
	{
		return m_state.m_state[0].process(value, typename T::m_design);
	}

	void Setup(int sample_rate, int order, double cutoffFrequency, double stopBandDb)
	{	
		Dsp::Params params;
		params[0] = sample_rate;
		params[1] = order;
		params[2] = cutoffFrequency;
		params[3] = stopBandDb;
		setParams (params);
	}
};

class LowFiltre : public DSPFiltre<Dsp::FilterDesign<Dsp::ChebyshevII::Design::LowPass<5>, 1>>{};
class HighFiltre: public DSPFiltre<Dsp::FilterDesign<Dsp::ChebyshevII::Design::HighPass<5>, 1>>{};