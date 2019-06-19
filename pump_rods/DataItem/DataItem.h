#pragma once
#include "App\App.h"

namespace DataItem
{
	static const int output_buffer_size = 1000;
	struct Buffer
	{		
		int currentOffset;
		int deathZoneFirst, deathZoneSecond;
		double threshSortDown, threshDefect;
		int result;
		double structMinVal;
		unsigned firstOffset, secondOffset;
		double inputData[App::buffer_size];
		double outputData[output_buffer_size];
		char status[output_buffer_size];
	};

	struct ResultData
	{
		int currentOffset;
		int currentOffsetZones;
		char status[output_buffer_size];
	};

	double ADC_RANGE(int range);

	void Test();
}

template<>struct DefectSig<DataItem::Buffer>: DataItem::Buffer{};
template<>struct StructSig<DataItem::Buffer>: DataItem::Buffer{};


