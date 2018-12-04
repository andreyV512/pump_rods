#include "Compute.h"
#include "MedianFiltre\MedianFiltre.h"
#include "DspFilters\ChebyshevFiltre.h"
#include "templates\typelist.hpp"
#include "App\AppBase.h"
#include "DataItem\DataItem.h"
#include "Windows\MainWindow\MainWindow.h"

namespace Compute
{

	void Compute(double *inputData, int inputLenght, int cutoffFrequency, int medianWidth, double *outputData, int outputLength, int samplingRate
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
		for(int i = 0; i < inputLenght; ++i)
		{
			double t = medianWidth > 2? filtre(inputData[i]): inputData[i];
			if(0 != cutoffFrequency) t = analogFiltre(t);
			int k = int(i / delta);
			if(k >= outputLength) break;
			if(t < 0) t = -t;
			if(t > outputData[k]) outputData[k] = t;
		}
		//for(int i = 0; i < outputLength; ++i)
		//{
		//
		//}
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
	template<class O, template<class>class W, class P>struct __recalculation__<W<O>, P>
	{
		void operator()(P &p)
		{
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
			, p.medianFiltreWidth.get<W<MedianFiltreWidth>>().value
			, o.outputData
			, o.outputLength
			, p.l502Param.get<W<ChannelSamplingRate>>().value
			);

			for(int i = 0; i < o.outputLength; ++i)
			{
				o.outputData[i] *= adcRange * koef;
			}

			DeadAreaTable::TItems &dead = Singleton<DeadAreaTable>::Instance().items;
			
			int rodLength = dead.get<RodLenght>().value;
			int firstDeathZone = dead.get<W<First<DeathZone>>>().value;
			int secondDeathZone = dead.get<W<Second<DeathZone>>>().value;
			o.deathZoneFirst =  int((double )o.deathZoneFirst * o.outputLength / rodLength);
			o.deathZoneSecond =  o.outputLength - int((double )o.deathZoneSecond * o.outputLength / rodLength);

			o.result = TL::IndexOf<ColorTable::items_list, Clr<Nominal>>::value;
			for(int i = o.deathZoneFirst; i < o.deathZoneSecond; ++i)
			{
				double t = o.outputData[i];
				if(t > o.threshDefect)
				{
					o.result = TL::IndexOf<ColorTable::items_list, Clr<Defect>>::value;
					return;
				}
				if(t > o.threshSortDown)
				{
					o.result = TL::IndexOf<ColorTable::items_list, Clr<SortDown>>::value;
				}
			}
		}
	};

	template<class T>struct Viewer;
	template<>struct Viewer<DefectSig<DataItem::Defectoscope>>
	{
		typedef MainWindow::DefectoscopeViewer Result;
	};
	template<>struct Viewer<DefectSig<DataItem::Structure>>
	{
		typedef MainWindow::StructureViewer Result;
	};

	template<class O, class P>struct __set_data__
	{
		void operator()(P &p)
		{
			typedef Viewer<O>::Result V;
			V &v = p.get<Viewer<O>::Result>();
			typedef TL::Inner<O>::Result Item;
			Item &item = Singleton<Item>::Instance();

			memmove(v.buffer, item.outputData, sizeof(v.buffer));
			v.count = item.outputLength;
			v. deathZoneFirst = item.deathZoneFirst;
			v.deathZoneSecond = item.deathZoneSecond;
			v.threshSortDown = item.threshSortDown; 
			v.threshDefect = item.threshDefect;
			v.result = item.result;
			v.tchart.maxAxesX = item.currentOffset - 1;
			v.currentOffset = item.currentOffset;
			v.inputData = item.inputData;
			v.lengthTube = Singleton<DeadAreaTable>::Instance().items.get<RodLenght>().value;
		}
	};

	void Recalculation()
	{
		MainWindow &mainWindow = Singleton<MainWindow>::Instance();
		
		TL::foreach<__data_item_list__, __recalculation__>()(__rec_data__());
		TL::foreach<__data_item_list__, __set_data__>()(mainWindow.viewers);
		App::UpdateViewers();

	}
}