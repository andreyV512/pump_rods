// testData.cpp : Defines the entry point for the console application.
//
#define _USE_MATH_DEFINES
#include "stdafx.h"
#include <math.h>
#include <stdio.h>

static const int magic_number = 0xCAFE0000;

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
		int firstOffset, secondOffset;
		double inputData[100000];
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

template<class T>struct DefectSig;
template<class T>struct StructSig;

template<>struct DefectSig<DataItem::Buffer>: DataItem::Buffer{};
template<>struct StructSig<DataItem::Buffer>: DataItem::Buffer{};

void StoreData(FILE *f, DefectSig<DataItem::Buffer> &def, StructSig<DataItem::Buffer> &str)
{
	int magic = magic_number + 2;
		fwrite(&magic  , sizeof(magic), 1, f);

		fwrite(&def.currentOffset  , sizeof(def.currentOffset), 1, f);
		fwrite(&str.currentOffset  , sizeof(str.currentOffset), 1, f);

		fwrite(&def.inputData   , sizeof(double), def.currentOffset, f);
		fwrite(&str.inputData   , sizeof(double), str.currentOffset, f);
}
DefectSig<DataItem::Buffer> def; 
StructSig<DataItem::Buffer> str;

int _tmain(int argc, _TCHAR* argv[])
{
	double dw = 2.0 * M_PI / 8000.0;
	for(int i = 0; i < 80000; ++i)
	{
		double res =// 0.5 * sin(5 * dw * i)
			1 * sin(10 * dw * i)
			//+ 0.1 * sin(50 * dw * i)
			+ 1 * sin(100 * dw * i)
			+ 1 * sin(1000 * dw * i)
			;
		def.inputData[i] = res;
		str.inputData[i] = res;
	}
	def.currentOffset = str.currentOffset = 80000;

	FILE *f = fopen("C:\\Users\\Andrey\\Desktop\\aktau\\testX.dat", "wb");
		if(f)
		{
			StoreData(f, def, str);
			fclose(f);
		}
	return 0;
}

