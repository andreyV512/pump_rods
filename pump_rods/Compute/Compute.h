#pragma once
#include "MedianFiltre\MedianFiltre.h"
#include "tools_debug\DebugMess.h"
#include "DspFilters\ChebyshevFiltre.hpp"
#include "App\AppBase.h"
#include "MessageText/ListMess.hpp"

template<class>struct StructSig;
template<class>struct DefectSig;
namespace Compute
{	
	template<class O, class P>struct __set_data__
	{
		void operator()(P &p)
		{
			typedef Viewer<O>::Result V;
			V &v = p.get<Viewer<O>::Result>();
			O &item = Singleton<O>::Instance();
			if(Singleton<OnTheJobTable>::Instance().items.get<typename ChangeWapper<O, Check>::Result>().value)
			{
				memmove(v.buffer, item.outputData, sizeof(v.buffer));
				memmove(v.status, item.status, sizeof(v.status));
				v. deathZoneFirst = item.deathZoneFirst;
				v.deathZoneSecond = item.deathZoneSecond;
				v.threshSortDown = item.threshSortDown; 
				v.threshDefect = item.threshDefect;
				v.result = item.result;
				v.tchart.maxAxesX = item.currentOffset - 1;
				v.currentOffset = item.currentOffset;
				v.inputData = item.inputData;
				v.count = DataItem::output_buffer_size;
			}
			else
			{
				memset(v.buffer, 0, sizeof(v.buffer));
				memset(v.status, STATUS_ID(SensorOff), sizeof(v.status));
				v.result = STATUS_ID(SensorOff);
				v.count = 0;
			}
		}
	};

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
				if(d[j] < 0) d[j] = -d[j];
			}
			o.structMinVal = t;
		}
	};

	template<template<class>class>struct get_ampl
	{
		template<class O>double *operator()(O &o)
		{
			return o.inputData;
		}
	};
	template<>struct get_ampl<StructSig>
	{
		template<class O>double *operator()(O &o)
		{
			int i = 0;
			double *d = o.inputData;
			double ampl = 0;
			while(i < o.currentOffset)
			{
				while(d[i] < 0)
				{
					if(d[i] > d[1 + i] && d[1 + i] <= d[2 + i])
					{
						ampl = -d[1 + i];
					}
					o.inputDataX[i] = ampl;
					++i;
				}
				while(d[i] > 0)
				{
					if(d[i] < d[1 + i] && d[1 + i] >= d[2 + i])
					{
						ampl = d[1 + i];
					}
					o.inputDataX[i] = ampl;
					++i;
				}
			}
			return o.inputDataX;
		}
	};

	//template<class O, class P>struct __set_data__
	//{
	//	void operator()(P &p)
	//	{
	//		typedef Viewer<O>::Result V;
	//		V &v = p.get<Viewer<O>::Result>();
	//		O &item = Singleton<O>::Instance();
	//		if(Singleton<OnTheJobTable>::Instance().items.get<typename ChangeWapper<O, Check>::Result>().value)
	//		{
	//			memmove(v.buffer, item.outputData, sizeof(v.buffer));
	//			memmove(v.status, item.status, sizeof(v.status));
	//			v. deathZoneFirst = item.deathZoneFirst;
	//			v.deathZoneSecond = item.deathZoneSecond;
	//			v.threshSortDown = item.threshSortDown; 
	//			v.threshDefect = item.threshDefect;
	//			v.result = item.result;
	//			v.tchart.maxAxesX = item.currentOffset - 1;
	//			v.currentOffset = item.currentOffset;
	//			v.inputData = item.inputData;
	//			v.count = DataItem::output_buffer_size;
	//		}
	//		else
	//		{
	//			memset(v.buffer, 0, sizeof(v.buffer));
	//			memset(v.status, STATUS_ID(SensorOff), sizeof(v.status));
	//			v.result = STATUS_ID(SensorOff);
	//			v.count = 0;
	//		}
	//	}
	//};

	template<class >struct OrderFiltre;
	template<template<int>class W, int N>struct OrderFiltre<W<N> >
	{
		static const int value = N;
	};

	struct InitFiltre
	{
		template<class T>void operator()(T &analogFiltre, int order, double stopBandDb, double, int samplingRate, int cutoffFrequency, int, int, int)
		{
			static const int max_order = OrderFiltre<typename T::T>::value;
			analogFiltre.Setup(
				order > 0 && order < max_order ? order: max_order
				, samplingRate
				, cutoffFrequency
				, stopBandDb
				);
		}

		void operator()(BandPassFiltre &analogFiltre, int order, double, double passBandRippleDb, int samplingRate, int cutoffFrequency, int centerFrequency, int widthFrequency, int)
		{
			static const int max_order = OrderFiltre<BandPassFiltre::T>::value;
			analogFiltre.Setup(
				order > 0 && order < max_order ? order: max_order
				, samplingRate
				, widthFrequency
				, centerFrequency
				, passBandRippleDb
				);
		}

		void operator()(DoubleFiltre &analogFiltre, int order, double passBand, double passBandRippleDb, int samplingRate, int cutoffFrequency, int centerFrequency, int widthFrequency, int typeFiltre)
		{
			switch(typeFiltre)
			{
			case TypeLowFiltre:
				(*this)(analogFiltre.lowFiltre, order, passBand, passBandRippleDb, samplingRate, cutoffFrequency, 0, 0, 0);
				analogFiltre.o = (DoubleFiltre::O *)&analogFiltre.lowFiltre;
				analogFiltre.ptr = (double(DoubleFiltre::O::*)(double))&LowFiltre::Simple;
				break;
			case TypeBandPassFiltre: 
				(*this)(analogFiltre.bandPassFiltre, order, passBand, passBandRippleDb, samplingRate, 0, centerFrequency, widthFrequency, 0);
				analogFiltre.o = (DoubleFiltre::O *)&analogFiltre.bandPassFiltre;
				analogFiltre.ptr = (double(DoubleFiltre::O::*)(double))&BandPassFiltre::Simple;
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