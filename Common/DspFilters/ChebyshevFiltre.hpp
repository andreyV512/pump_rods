#pragma once
#include "include/ChebyshevII.h"

template<class T>class DSPFiltre: public T::State<Dsp::DirectFormII>
{
	template<class T>struct I;
	template<template<int >class W, int N>struct I<W<N> >
	{
		static const int value = N;
	};
	T m_design;
public:
	inline double operator()(double value)
	{
		return process(value, m_design);
	}
	void Setup(int sample_rate, double cutoffFrequency, double stopBandDb)
	{	
		m_design.setup(I<T>::value, sample_rate, cutoffFrequency, stopBandDb);
	}
};

class LowFiltre : public DSPFiltre<Dsp::ChebyshevII::LowPass<3>>{};
class HighFiltre: public DSPFiltre<Dsp::ChebyshevII::HighPass<3>>{};

class BandPassFiltre: public Dsp::ChebyshevII::BandPass<3>::State<Dsp::DirectFormII>
{
	template<class T>struct I;
	template<template<int >class W, int N>struct I<W<N> >
	{
		static const int value = N;
	};
	typedef Dsp::ChebyshevII::BandPass<3> T;
	T m_design;
public:
	inline double operator()(double value)
	{
		return process(value, m_design);
	}
	void Setup(int sample_rate, int widthFrequency, double centerFrequency, double stopBandDb)
	{
		m_design.setup(I<T>::value, sample_rate, centerFrequency, widthFrequency, 0.01);
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