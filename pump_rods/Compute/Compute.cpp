#include "Compute.h"
#include "MedianFiltre\MedianFiltre.h"
#include "DspFilters\ChebyshevFiltre.hpp"
#include "templates\typelist.hpp"
#include "App\AppBase.h"
#include "DataItem\DataItem.h"
#include "Windows\MainWindow\MainWindow.h"
#include "App\MessageItems.h"
#include "MessageText\StatusMessages.h"
#include "MessageText\ListMess.hpp"
#include <cmath>

namespace Compute
{

	void Compute(double *inputData, int inputLenght, int cutoffFrequency, bool cutoffFrequencyON, int medianWidth, bool medianON, double *outputData, int outputLength, int samplingRate, bool wave
		)
	{
		MedianFiltre filtre;
		if(medianWidth > 2)filtre.InitWidth(medianWidth);

		LowFiltre analogFiltre;
		if(0 != cutoffFrequency)analogFiltre.Setup(
			samplingRate
			, 3
			, cutoffFrequency
			, 40
			);

		double delta = (double)inputLenght / outputLength;
		memset(outputData, 0, outputLength * sizeof(double));
		--inputLenght;
		for(int i = 0; i <= inputLenght; ++i)
		{
			double t = medianON? filtre(inputData[i]): inputData[i];
			if(cutoffFrequencyON) t = analogFiltre(t);
			int k = int(i / delta);
			if(k >= outputLength) break;
			if(abs(t) > abs(outputData[k])) outputData[k] = wave? abs(t): t;
		}		
	}

	//void Normalization()
	//{
	//	L502ParametersTable::TItems &l502Param = Singleton<L502ParametersTable>::Instance().items;
	//
	//	DataItem::Defectoscope &def = Singleton<DataItem::Defectoscope>::Instance();
	//	double adcRange =  100.0 / DataItem::ADC_RANGE(l502Param.get<DefectSig<RangeL502>>().value);
	//	for(int i = 0; i < def.currentOffset; ++i)
	//	{
	//		def.inputData[i] *= adcRange;
	//	}
	//
	//	DataItem::Structure &str = Singleton<DataItem::Structure>::Instance();
	//	adcRange =  100.0 / DataItem::ADC_RANGE(l502Param.get<StructSig<RangeL502>>().value);
	//	for(int i = 0; i < str.currentOffset; ++i)
	//	{
	//		str.inputData[i] *= adcRange;
	//	}
	//}

	typedef App::data_item_list __data_item_list__;

	struct __rec_data__
	{
		AnalogFilterTable::TItems &cutoffFrequency;
		MedianFiltreTable::TItems &medianFiltreWidth;
		KoeffSignTable::TItems &koeffSign;
		ThresholdsTable::TItems &tresh;
		L502ParametersTable::TItems l502Param;
		DeadAreaTable::TItems deadArea;
		__rec_data__()
			: cutoffFrequency(Singleton<AnalogFilterTable>::Instance().items)
			, medianFiltreWidth(Singleton<MedianFiltreTable>::Instance().items)
			, koeffSign(Singleton<KoeffSignTable>::Instance().items)
			, tresh(Singleton<ThresholdsTable>::Instance().items)
			, l502Param(Singleton<L502ParametersTable>::Instance().items)
			, deadArea(Singleton<DeadAreaTable>::Instance().items)
		{}
	};

	template<class O, class P>struct __recalculation__;
	template<class T, template<class>class W, class P>struct __recalculation__<W<T>, P>
	{
		typedef W<T> O;
		void operator()(P &p)
		{
			if(!Singleton<OnTheJobTable>::Instance().items.get<W<Check>>().value) return;
			O &o = Singleton<O>::Instance();
			double adcRange =  100.0 / DataItem::ADC_RANGE(p.l502Param.get<W<RangeL502>>().value);
			double koef = p.koeffSign.get<W<KoeffSign>>().value;
			o.deathZoneFirst  = p.deadArea.get<W<First<DeathZone>>>().value;
			o.deathZoneSecond = p.deadArea.get<W<Second<DeathZone>>>().value;
			o.threshSortDown = p.tresh.get<W<Thresh<SortDown>>>().value;
			o.threshDefect   = p.tresh.get<W<Thresh<Defect>>>().value;
			Compute(
			  o.inputData
			, o.currentOffset
			, p.cutoffFrequency.get<W<CutoffFrequency>>().value
			, p.cutoffFrequency.get<W<CutoffFrequencyON>>().value
			, p.medianFiltreWidth.get<W<MedianFiltreWidth>>().value
			, p.medianFiltreWidth.get<W<MedianFiltreON>>().value
			, o.outputData
			, DataItem::output_buffer_size
			, p.l502Param.get<W<ChannelSamplingRate>>().value
			);

			for(int i = 0; i < DataItem::output_buffer_size; ++i)
			{
				o.outputData[i] *= adcRange * koef;
			}

			DeadAreaTable::TItems &dead = Singleton<DeadAreaTable>::Instance().items;
			
			int rodLength = dead.get<RodLenght>().value;
			int firstDeathZone = dead.get<W<First<DeathZone>>>().value;
			int secondDeathZone = dead.get<W<Second<DeathZone>>>().value;
			o.deathZoneFirst =  int((double )o.deathZoneFirst * DataItem::output_buffer_size / rodLength);
			o.deathZoneSecond =  DataItem::output_buffer_size - int((double )o.deathZoneSecond * DataItem::output_buffer_size / rodLength);
             
			o.result = STATUS_ID(Nominal);
			for(int i = o.deathZoneFirst; i < o.deathZoneSecond; ++i)
			{
				double t = o.outputData[i];
				if(t > o.threshDefect)
				{
					o.result = STATUS_ID(W<Defect>);
					o.status[i] = STATUS_ID(W<Defect>);
				}
				else if(t > o.threshSortDown)
				{
					if(STATUS_ID(W<Defect>) != o.result)o.result = STATUS_ID(W<SortDown>);
					o.status[i] = STATUS_ID(W<SortDown>);
				}
				else
				{
					o.status[i] = STATUS_ID(Nominal);
				}
			}
			for(int i = 0; i < o.deathZoneFirst; ++i)
			{
				o.status[i] = STATUS_ID(DeathZone);
			}
			for(int i = o.deathZoneSecond; i < dimention_of(o.status); ++i)
			{
				o.status[i] = STATUS_ID(DeathZone);
			}
		}
	};

	template<class T>struct Viewer;
	template<>struct Viewer<DefectSig<DataItem::Buffer>>
	{
		typedef MainWindow::DefectoscopeViewer Result;
	};
	template<>struct Viewer<StructSig<DataItem::Buffer>>
	{
		typedef MainWindow::StructureViewer Result;
	};

	template<class O, class T>struct ChangeWapper;
	template<template<class>class W, class O, class T>struct ChangeWapper<W<O>, T>
	{
		typedef W<T> Result;
	};

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

	void ComputeResult()
	{
		char (&def)[DataItem::output_buffer_size] = Singleton<DefectSig<DataItem::Buffer>>::Instance().status;
		char (&sig)[DataItem::output_buffer_size] = Singleton<StructSig<DataItem::Buffer>>::Instance().status;
		char (&res)[DataItem::output_buffer_size] = Singleton<DataItem::ResultData>::Instance().status;
		unsigned st[3];
		bool bdef = App::DataCollectionDefectoscope();
		bool bstr = App::DataCollectionStructure();
		for(int i = 0; i < DataItem::output_buffer_size; ++i)
		{
			int ind = 0;
			if(bdef)st[ind++] = def[i];
			if(bstr)st[ind++] = sig[i];
			st[ind] = -1;
			res[i] = ResultMessageId(st);
		}
	}

	void Recalculation()
	{
		MainWindow &mainWindow = Singleton<MainWindow>::Instance();
		
		TL::foreach<__data_item_list__, __recalculation__>()(__rec_data__());
		ComputeResult();
		TL::foreach<__data_item_list__, __set_data__>()(mainWindow.viewers);		
		App::UpdateViewers();

	}

	static const unsigned brak = 0;
	static const unsigned cort1 = 1;
	static const unsigned cort2 = 2;
	static const unsigned cort3 = 3;
	
	unsigned Result(unsigned res)
	{
		DefectSig<DataItem::Buffer> &def = Singleton<DefectSig<DataItem::Buffer>>::Instance();
		StructSig<DataItem::Buffer> &str    = Singleton<StructSig<DataItem::Buffer>>::Instance();

		if(def.result == STATUS_ID(DefectSig<Defect>)) return brak;
		if(str.result == STATUS_ID(StructSig<Defect>)) return brak;

		if(res <= 0) return brak;

		if(def.result == STATUS_ID(DefectSig<SortDown>)) return res - 1;
		if(str.result == STATUS_ID(StructSig<SortDown>)) return res - 1;
		return brak;
	}

	void Reverse(double *data, int len)
	{
		--len;
		int len2 = len / 2;

		for(int i = 0; i < len2; ++i)
		{
			int k = len - i;
			double d = data[i];
			data[i] = data[k];
			data[k] = d;
		}
	}
}