#pragma once
#include "App\App.h"

namespace DataItem
{
	struct Defectoscope
	{
		static const int output_buffer_size = 2000;
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
		static const int output_buffer_size = 2000;
		int currentOffset;
		int outputLength;
		int deathZoneFirst, deathZoneSecond;
		double threshSortDown, threshDefect;
		int result;
		double inputData[App::buffer_size];
		double outputData[output_buffer_size];
	};

	double ADC_RANGE(int range);

	void Test();
}
