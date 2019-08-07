#pragma once
#include "MedianFiltre\MedianFiltre.h"
#include "tools_debug\DebugMess.h"
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
	template<class Filtre, class Meander = Noop>struct ComputeFrame
	{
		void operator()(double *inputData, int offs, int inputLenght, int cutoffFrequency, bool cutoffFrequencyON, int medianWidth, bool medianON, double *outputData, int outputLength, int samplingRate, bool wave = true)
		{
			dprint("Compute Filtre %s\n", typeid(Filtre).name());
			MedianFiltre filtre;
			if(medianWidth > 2)filtre.InitWidth(medianWidth);

			Filtre analogFiltre;
			if(0 != cutoffFrequency)analogFiltre.Setup(
				samplingRate
				, 3
				, cutoffFrequency
				, 40
				);
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
	void ComputeResult();
	unsigned  Recalculation(unsigned = 1);
	unsigned Result(unsigned res);

	void Reverse(double *data, int len);
};