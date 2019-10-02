#pragma once
#include "MedianFiltre\MedianFiltre.h"
#include "tools_debug\DebugMess.h"
#include "DspFilters\ChebyshevFiltre.hpp"
#include "App\AppBase.h"

template<class>struct StructSig;
template<class>struct DefectSig;
namespace Compute
{	

	struct Noop
	{
		Noop(){}
		double operator()(double t)
		{
			return t;
		}
	};

	template<template<class>class>struct diff
	{
		template<class O>void operator()(O &o, double(&)[1000])
		{
		}
	};

	template<>struct diff<StructSig>
	{
		template<class O>void operator()(O &o, double(&d)[1000])
		{
			double t = 0;
			for(int j = 0; j < DataItem::output_buffer_size; ++j)
			{
				t += d[j];
			}
			t /=  DataItem::output_buffer_size;
			for(int j = 0; j < DataItem::output_buffer_size; ++j)
			{
				if(d[j] < 0) d[j] = -d[j];
				d[j] -= t;
				if(d[j] < 0) d[j] = 0;
			}
			o.structMinVal = t;
		}
	};

	struct InitFiltre
	{
		template<class T>void operator()(T &analogFiltre, int samplingRate, int cutoffFrequency, int, int, int)
		{
			analogFiltre.Setup(
				samplingRate
				, cutoffFrequency
				, 40
				);
		}

		void operator()(BandPassFiltre &analogFiltre, int samplingRate, int cutoffFrequency, int centerFrequency, int widthFrequency, int)
		{
			analogFiltre.Setup(
				samplingRate
				, widthFrequency
				, centerFrequency
				, 1
				);
		}

		void operator()(DoubleFiltre &analogFiltre, int samplingRate, int cutoffFrequency, int centerFrequency, int widthFrequency, int typeFiltre)
		{
			//switch(Singleton<AnalogFilterTable>::Instance().items.get<DefectSig<TypeFiltre>>().value)
			switch(typeFiltre)
			{
			case TypeLowFiltre:
				(*this)(analogFiltre.lowFiltre, samplingRate, cutoffFrequency, 0, 0, 0);
				analogFiltre.o = (DoubleFiltre::O *)&analogFiltre.lowFiltre;
				analogFiltre.ptr = (double(DoubleFiltre::O::*)(double))&LowFiltre::operator();
				break;
			case TypeBandPassFiltre: 
				(*this)(analogFiltre.bandPassFiltre, samplingRate, 0, centerFrequency, widthFrequency, 0);
				analogFiltre.o = (DoubleFiltre::O *)&analogFiltre.bandPassFiltre;
				analogFiltre.ptr = (double(DoubleFiltre::O::*)(double))&BandPassFiltre::operator();
				break;
			};
		}
	};

	class Filtre
	{
		struct O{};
		O *o;
		double(O::*ptr)(double);
		double noop(double t){return t;}
	public:
		Filtre(): ptr( (double(O::*)(double))&Filtre::noop){o = (O *)this;}
		template<class T>void Init(T *t, double(T::*p)(double))
		{
			o = (O *)t;
			ptr = (double(O::*)(double))p;
		}
		double operator()(double val){return (o->*ptr)(val);};
	};
	void ComputeFrame(double *inputData, int offs, int inputLenght, double *outputData, int outputLength, Filtre &analogFiltre, Filtre &medianFiltre);
	void ComputeResult();
	unsigned  Recalculation(unsigned = 1);
	unsigned Result(unsigned res);

	void Reverse(double *data, int len);
};