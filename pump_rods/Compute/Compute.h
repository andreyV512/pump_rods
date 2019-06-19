#pragma once
#include "MedianFiltre\MedianFiltre.h"

namespace Compute
{	
	template<class Filtre>void Compute(double *inputData, int inputLenght, int cutoffFrequency, bool cutoffFrequencyON, int medianWidth, bool medianON, double *outputData, int outputLength, int samplingRate, bool wave = true)
	{
		MedianFiltre filtre;
		if(medianWidth > 2)filtre.InitWidth(medianWidth);

		Filtre analogFiltre;
		if(0 != cutoffFrequency)analogFiltre.Setup(
			samplingRate
			, 3
			, cutoffFrequency
			, 40
			);

		double delta = (double)inputLenght / outputLength;
		memset(outputData, 0, outputLength * sizeof(double));
		--inputLenght;
		//double offs = 0;
		//for(int i = 0; i <= inputLenght; ++i)
		//{
		//	offs += inputData[i];
		//}
		//offs /= inputLenght;
		for(int i = 0; i <= inputLenght; ++i)
		{
			double t = inputData[i];// - offs;
			if(medianON) t = filtre(inputData[i]);
			if(cutoffFrequencyON) t = analogFiltre(t);
			int k = int(i / delta);
			if(k >= outputLength) break;
			double absT = t > 0 ? t: -t;
			if(absT > abs(outputData[k]))
			{
				outputData[k] = wave? absT: t;
			}
		}		
	}
	void ComputeResult();
	void Recalculation();
	unsigned Result(unsigned res);

	void Reverse(double *data, int len);
};