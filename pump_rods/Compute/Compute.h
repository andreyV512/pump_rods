#pragma once

namespace Compute
{
	void Compute(double *inputData, int inputLenght, int cutoffFrequency, int medianWidth, double *outputData, int outputLength, int samplingRate);
	void Recalculation();
	//void Normalization();
};