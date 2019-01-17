#include "ChebyshevFiltre.h"
#include "include/ChebyshevII.h"

template<int size, int numbers_must_be_equal, bool = size == numbers_must_be_equal>struct implementation_buffer_must_be_same_size_as_Dsp_FilterDesign;
template<int size, int make_the_buffer_equal>
	struct implementation_buffer_must_be_same_size_as_Dsp_FilterDesign<size, make_the_buffer_equal, true>{};

template<class T>class Filtre: public T
{
public:
	typedef T type;
	static void *operator new(size_t, void *p){return p;}
	static void operator delete(void *, void *){}
	inline double OneSample(double value)
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

//HighPass LowPass
class LowFiltre::Impl: public Filtre<Dsp::FilterDesign<Dsp::ChebyshevII::Design::LowPass<5>, 1>>{};

LowFiltre::LowFiltre()
	: impl(*(Impl *)implementation_buffer)
{
	implementation_buffer_must_be_same_size_as_Dsp_FilterDesign<sizeof(Impl), sizeof(implementation_buffer)>();
	new(implementation_buffer) Impl;
}

void LowFiltre::Setup(int sample_rate, int order, double cutoffFrequency, double stopBandDb)
{	
	impl.Setup(sample_rate, order, cutoffFrequency, stopBandDb);
}

double LowFiltre::operator()(double value)
{
	return impl.OneSample(value);
}

class HighFiltre::Impl: public Filtre<Dsp::FilterDesign<Dsp::ChebyshevII::Design::HighPass<5>, 1>>{};

HighFiltre::HighFiltre()
	: impl(*(Impl *)implementation_buffer)
{
	implementation_buffer_must_be_same_size_as_Dsp_FilterDesign<sizeof(Impl), sizeof(implementation_buffer)>();
	new(implementation_buffer) Impl;
}

void HighFiltre::Setup(int sample_rate, int order, double cutoffFrequency, double stopBandDb)
{	
	impl.Setup(sample_rate, order, cutoffFrequency, stopBandDb);
}

double HighFiltre::operator()(double value)
{
	return impl.OneSample(value);
}




