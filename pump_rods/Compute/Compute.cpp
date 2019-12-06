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
#include "Log/LogBuffer.h"
#include "Log/LogMessages.h"

namespace Compute
{
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

	template<class T>struct __wapper_filtre__
	{
		template<class O>typename T::type_value operator()(O &o){return o.get<T>().value;}
	};
	template<>struct __wapper_filtre__<StructSig<WidthFrequency>>
	{
		template<class O>typename int operator()(O &o){return 0;}
	};
	template<>struct __wapper_filtre__<StructSig<CenterFrequency>>
	{
		template<class O>typename int operator()(O &o){return 0;}
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

			int rodLength = p.deadArea.get<RodLenght>().value;

			o.firstOffset =  unsigned((double)o.deathZoneFirst * o.currentOffset / rodLength);
			o.secondOffset =  o.currentOffset - unsigned((double)o.deathZoneSecond * o.currentOffset / rodLength) - o.firstOffset;
			//----------------------
			typedef typename WapperFiltre<W>::Result WFiltre;
			WFiltre aFiltre;
			Compute::Filtre analog;
			if(p.cutoffFrequency.get<W<CutoffFrequencyON>>().value)
			{
				Compute::InitFiltre()(aFiltre
					, __wapper_filtre__<W<Order>>()(p.cutoffFrequency)
					, Singleton<L502ParametersTable>::Instance().items.get<W<ChannelSamplingRate>>().value
					, p.cutoffFrequency.get<W<CutoffFrequency>>().value
					, __wapper_filtre__<W<CenterFrequency>>()(p.cutoffFrequency)
					, __wapper_filtre__<W<WidthFrequency>>()(p.cutoffFrequency)
					, Singleton<AnalogFilterTable>::Instance().items.get<DefectSig<TypeFiltre>>().value
					);
				analog.Init<WFiltre>(&aFiltre, &WFiltre::operator());
			}

			MedianFiltre mFiltre;
			Compute::Filtre median;
			if(p.medianFiltreWidth.get<W<MedianFiltreON>>().value && p.medianFiltreWidth.get<W<MedianFiltreWidth>>().value > 2)
			{
				mFiltre.InitWidth(p.medianFiltreWidth.get<W<MedianFiltreWidth>>().value);
				median.Init(&mFiltre, &MedianFiltre::operator());
			}
			//-------------------------
			Compute::ComputeFrame(
				get_ampl<W>()(o), o.firstOffset
				, o.secondOffset
				, o.outputData
				, DataItem::output_buffer_size
				, analog
				, median
				);

			for(int i = 0; i < DataItem::output_buffer_size; ++i)
			{
				if(o.outputData[i] < 0) o.outputData[i] = -o.outputData[i];
			}

			for(int i = 0; i < DataItem::output_buffer_size; ++i)
			{
				o.outputData[i] *= adcRange * koef;
			}

			o.deathZoneFirst =  int((double )o.deathZoneFirst * DataItem::output_buffer_size / rodLength);
			o.deathZoneSecond =  DataItem::output_buffer_size - int((double )o.deathZoneSecond * DataItem::output_buffer_size / rodLength);

			diff<W>()(o, o.outputData);
			o.result = STATUS_ID(Nominal);

			for(int i = 0; i < DataItem::output_buffer_size; ++i)
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

	unsigned Recalculation(unsigned c1c2)
	{
		MainWindow &mainWindow = Singleton<MainWindow>::Instance();

		TL::foreach<__data_item_list__, __recalculation__>()(__rec_data__());
		ComputeResult();
		int res = Compute::Result(1);
		dprint("test Recalculation %d\n", res);
		TL::foreach<__data_item_list__, __set_data__>()(mainWindow.viewers);		
		App::UpdateViewers();

		res = Compute::Result(c1c2);
		if(0 == res)
		{
			Log::Mess<LogMess::Brak>();
			App::StatusBar(0, L"Брак");
		}
		else
		{
			wchar_t buf[32];
			wsprintf(buf, L"Сорт %d", res);
			App::StatusBar(0, buf);
			Log::Mess<LogMess::Copt>(res);
		}
		return res;
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

		if(def.result == STATUS_ID(DefectSig<SortDown>) || str.result == STATUS_ID(StructSig<SortDown>)) ++res;
		res &= 3;
		return res;
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

	void ComputeFrame(double *inputData, int offs, int inputLenght, double *outputData, int outputLength, Filtre &analogFiltre, Filtre &medianFiltre)
	{
		for(int i = 0; i < offs; ++i)
		{
			double t = medianFiltre(inputData[i]);
			t = analogFiltre(t); 
		}

		inputData += offs;

		double delta = (double)inputLenght / outputLength;
		memset(outputData, 0, outputLength * sizeof(double));
		--inputLenght;

		for(int i = 0; i <= inputLenght; ++i)
		{
			double t = medianFiltre(inputData[i]);
			t = analogFiltre(t);
			int k = int(i / delta);
			if(k >= outputLength) break;
			if(abs(t) > abs(outputData[k]))
			{
				outputData[k] = t;
			}
		}
	}
}