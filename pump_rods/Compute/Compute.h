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
	template<template<class>class>class Meander;

	template<>class Meander<StructSig>
	{
		double mn, mx, pred;;
		bool changed;
	public:
		Meander():  mn(0), mx(0), pred(0), changed(false)
		{
		}
		double operator()(double x)
		{
			double res = 0;
			if(pred < x)
			{
				if(x >= mx)mx = x;
				res = -mn;
				if(changed)
				{
					mx = 0;
					changed ^= true;
				}
			}
			else
			{
				if(x < mn)mn = x;
				res = mx;
				if(!changed)
				{
					mn = 0;
					changed ^= true;
				}
			}
			pred = x;
			return res;
		}
	};

	template<>class Meander<DefectSig>
	{
	public:
		Meander(){}
		double operator()(double next)
		{
			return next;
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

	/*
	template<class Filtre, class Meander = Noop>struct ComputeFrame
	{
	void operator()(double *inputData, int offs, int inputLenght, int cutoffFrequency, bool cutoffFrequencyON, int medianWidth, bool medianON, double *outputData, int outputLength, int samplingRate, bool wave = true)
	{
	MedianFiltre filtre;
	if(medianWidth > 2)filtre.InitWidth(medianWidth);

	Filtre analogFiltre;
	if(0 != cutoffFrequency) InitFiltre()(analogFiltre, samplingRate, cutoffFrequency);
	Meander meander;

	for(int i = 0; i < offs; ++i)
	{
	double t = inputData[i];
	if(medianON) t = filtre(inputData[i]);
	if(cutoffFrequencyON) t = analogFiltre(t);
	meander(t);
	}

	inputData += offs;

	double delta = (double)inputLenght / outputLength;
	memset(outputData, 0, outputLength * sizeof(double));
	--inputLenght;

	for(int i = 0; i <= inputLenght; ++i)
	{
	double t = inputData[i];
	if(medianON) t = filtre(inputData[i]);
	if(cutoffFrequencyON) t = analogFiltre(t);
	t = meander(t);
	int k = int(i / delta);
	if(k >= outputLength) break;
	double absT = t > 0 ? t: -t;
	if(absT > abs(outputData[k]))
	{
	outputData[k] = wave? absT: t;
	}
	}		
	}
	};
	*/
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
	void ComputeFrame(double *inputData, int offs, int inputLenght, double *outputData, int outputLength, Filtre &analogFiltre, Filtre &medianFiltre);//, bool wave = true);
	void ComputeResult();
	unsigned  Recalculation(unsigned = 1);
	unsigned Result(unsigned res);

	void Reverse(double *data, int len);
};