#include "ChebyshevFiltre.h"
#include "include/ChebyshevII.h"

//template<int N>struct  Size
//{
//	static const int value = N;
//};

template<int size, int make_the_buffer_equal>struct implementation_buffer_must_be_same_size_as_Dsp_FilterDesign;

//HighPass LowPass
class LowFiltre::Impl: public Dsp::FilterDesign<Dsp::ChebyshevII::Design::LowPass<5>, 1>
{
public:
	static void *operator new(size_t, void *p){return p;}
	static void operator delete(void *, void *){}
	inline double OneSample(double value)
	{
		return m_state.m_state[0].process(value, FilterDesignBase<Dsp::ChebyshevII::Design::LowPass<5>>::m_design);
	}
};

template<>struct implementation_buffer_must_be_same_size_as_Dsp_FilterDesign<sizeof(LowFiltre::Impl), sizeof(LowFiltre::Impl)>{};

LowFiltre::LowFiltre()
	: impl(*(LowFiltre::Impl *)implementation_buffer)
{
	implementation_buffer_must_be_same_size_as_Dsp_FilterDesign<sizeof(implementation_buffer), sizeof(LowFiltre::Impl)>();
	new(implementation_buffer) LowFiltre::Impl;
}

void LowFiltre::Setup(int sample_rate, int order, double cutoffFrequency, double stopBandDb)
{	
	Dsp::Params params;
	params[0] = sample_rate;
	params[1] = order;
	params[2] = cutoffFrequency;
	params[3] = stopBandDb;
	impl.setParams (params);
}
double LowFiltre::operator()(double value)
{
	return impl.OneSample(value);
}


class HighFiltre::Impl: public Dsp::FilterDesign<Dsp::ChebyshevII::Design::HighPass<5>, 1>
{
public:
	static void *operator new(size_t, void *p){return p;}
	static void operator delete(void *, void *){}
	inline double OneSample(double value)
	{
		return m_state.m_state[0].process(value, FilterDesignBase<Dsp::ChebyshevII::Design::HighPass<5>>::m_design);
	}
};

//template<>struct implementation_buffer_must_be_same_size_as<sizeof(HighFiltre::Impl)>{};

HighFiltre::HighFiltre()
	: impl(*(HighFiltre::Impl *)implementation_buffer)
{
	implementation_buffer_must_be_same_size_as_Dsp_FilterDesign<sizeof(implementation_buffer), sizeof(HighFiltre::Impl)>();
	new(implementation_buffer) HighFiltre::Impl;
}

void HighFiltre::Setup(int sample_rate, int order, double cutoffFrequency, double stopBandDb)
{
	Dsp::Params params;
	params[0] = sample_rate;
	params[1] = order;
	params[2] = cutoffFrequency;
	params[3] = stopBandDb;
	impl.setParams (params);
}
double HighFiltre::operator()(double value)
{
	return impl.OneSample(value);
}



