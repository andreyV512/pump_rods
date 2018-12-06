#pragma once
#include "App\App.h"

namespace DataItem
{
	static const int output_buffer_size = 1000;
	struct Defectoscope
	{		
		int currentOffset;
		int outputLength;
		int deathZoneFirst, deathZoneSecond;
		double threshSortDown, threshDefect;
		int result;
		double inputData[App::buffer_size];
		double outputData[output_buffer_size];
	};

	struct Structure
	{
		int currentOffset;
		int outputLength;
		int deathZoneFirst, deathZoneSecond;
		double threshSortDown, threshDefect;
		int result;
		double inputData[App::buffer_size];
		double outputData[output_buffer_size];
	};

	struct ResultData
	{
		int currentOffset;
		int currentOffsetZones;
	};

	double ADC_RANGE(int range);

	void Test();
}
