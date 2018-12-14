#pragma once

namespace Compute
{	
	void Compute(double *inputData, int inputLenght, int cutoffFrequency, bool cutoffFrequencyON, int medianWidth, bool medianON, double *outputData, int outputLength, int samplingRate);
	void ComputeResult();
	void Recalculation();
	//void Normalization();
	unsigned Result(unsigned res);

	void Reverse(double *data, int len);
};