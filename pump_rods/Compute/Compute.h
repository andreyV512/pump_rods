#pragma once

namespace Compute
{	
	void Compute(double *inputData, int inputLenght, int cutoffFrequency, int medianWidth, double *outputData, int outputLength, int samplingRate);
	void ComputeResult();
	void Recalculation();
	//void Normalization();
	unsigned Result(unsigned res);
};