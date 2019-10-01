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
	//template<template<class>class>class Meander;
	//
	//template<>class Meander<StructSig>
	//{
	//	double mn, mx, pred;
	//	bool changed;
	//public:
	//	Meander():  mn(0), mx(0), pred(0), changed(false)
	//	{
	//	}
	//	double operator()(double x)
	//	{
	//		double res = 0;
	//		if(pred < x)
	//		{
	//			if(x >= mx)mx = x;
	//			res = -mn;
	//			if(changed)
	//			{
	//				mx = 0;
	//				changed ^= true;
	//			}
	//		}
	//		else
	//		{
	//			if(x < mn)mn = x;
	//			res = mx;
	//			if(!changed)
	//			{
	//				mn = 0;
	//				changed ^= true;
	//			}
	//		}
	//		pred = x;
	//		return res;
	//	}
	//};
	//
	//template<>class Meander<DefectSig>
	//{
	//public:
	//	Meander(){}
	//	double operator()(double next)
	//	{
	//		return next;
	//	}
	//};

	template<template<class>class>struct diff
	{
		template<class O>void operator()(O &o, double(&)[1000])
		{
		}
	};

	template<>struct diff<StructSig>//, Filtre>
	{
		template<class O>void operator()(O &o, double(&d)[1000])
		{

		//	double *d = o.outputData;
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
		template<class T>void operator()(T &analogFiltre, int samplingRate, int cutoffFrequency, int, int)
		{
			analogFiltre.Setup(
				samplingRate
				, cutoffFrequency
				, 40
				);
		}

		void operator()(BandPassFiltre &analogFiltre, int samplingRate, int cutoffFrequency, int centerFrequency, int widthFrequency)
		{
			analogFiltre.Setup(
				samplingRate
				//, Singleton<AnalogFilterTable>::Instance().items.get<DefectSig<WidthFrequency>>().value
				, widthFrequency
				, centerFrequency
				, 1
				);
		}

		void operator()(DoubleFiltre &analogFiltre, int samplingRate, int cutoffFrequency, int centerFrequency, int widthFrequency)
		{
			switch(Singleton<AnalogFilterTable>::Instance().items.get<DefectSig<TypeFiltre>>().value)
			{
			case TypeLowFiltre:
				(*this)(analogFiltre.lowFiltre, samplingRate, cutoffFrequency, 0, 0);
				analogFiltre.o = (DoubleFiltre::O *)&analogFiltre.lowFiltre;
				analogFiltre.ptr = (double(DoubleFiltre::O::*)(double))&LowFiltre::operator();
				break;
			case TypeBandPassFiltre: 
				(*this)(analogFiltre.bandPassFiltre, samplingRate, 0, centerFrequency, widthFrequency);
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