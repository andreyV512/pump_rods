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
		double t;
		double last = 0;
		for(int i = 0; i <= inputLenght; ++i)
		{
			t = medianON? filtre(inputData[i]): inputData[i];
			if(cutoffFrequencyON) t = analogFiltre(t);
			int k = int(i / delta);
			if(k >= outputLength) break;
			if(abs(t) > abs(outputData[k]))
			{
				last = outputData[k] = wave? abs(t): t;
			}
			else outputData[k] = last;
		}		
	}
	void ComputeResult();
	void Recalculation();
	unsigned Result(unsigned res);

	void Reverse(double *data, int len);
};