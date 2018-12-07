#include "DataItem.h"
#define _USE_MATH_DEFINES
#include <math.h>
#include <stdlib.h>
#include "templates\typelist.hpp"

namespace DataItem
{
	void __Test__(double (&data)[App::buffer_size], double koeff)
	{
		double delta = 100.0 / App::buffer_size / 10;
		int i = 0;
		for(; i < App::buffer_size / 2 - 55000; ++i)
		{
			data[i] = delta * i;
		}

		for(; i < App::buffer_size / 2 + 55000; ++i)
		{
			data[i] =  delta * 100000;
		}

		for(; i < App::buffer_size; ++i)
		{
			data[i] = delta * i;
		}
	}

	void __Test1__(double (&data)[App::buffer_size], double koeff)
	{
		double delta = 100.0 / App::buffer_size / 10;
		int i = 0;
		for(; i < App::buffer_size / 2 - 55000; ++i)
		{
			data[App::buffer_size - 1 - i] = delta * i;
		}

		for(; i < App::buffer_size / 2 + 55000; ++i)
		{
			data[App::buffer_size - 1 - i] =  delta * 100000;
		}

		for(; i < App::buffer_size; ++i)
		{
			data[App::buffer_size - 1 - i] = delta * i;
		}
	}

	void Test()
	{
		Defectoscope &def = Singleton<Defectoscope>::Instance();
		def.currentOffset = App::buffer_size;
		//def.outputLength = output_buffer_size;
		__Test__(def.inputData, 1.0);
		Structure &str = Singleton<Structure>::Instance();
		str.currentOffset = App::buffer_size;
		//str.outputLength = output_buffer_size;
		__Test1__(str.inputData, 1.0);
	}

	double ADC_RANGE(int range)
	{
		//L502_ADC_RANGE_10 = 0, /**< Диапазон +/-10V */
		//L502_ADC_RANGE_5  = 1, /**< Диапазон +/-5V */
		//L502_ADC_RANGE_2  = 2, /**< Диапазон +/-2V */
		//L502_ADC_RANGE_1  = 3, /**< Диапазон +/-1V */
		//L502_ADC_RANGE_05 = 4, /**< Диапазон +/-0.5V */
		//L502_ADC_RANGE_02 = 5  /**< Диапазон +/-0.2V */
		static double arr[] = {10, 5, 2, 1, 0.5, 0.2};
		return range < dimention_of(arr)? arr[range]: 0;
	}
}