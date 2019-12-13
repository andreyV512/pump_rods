// testFiltre.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "DspFilters\include\DspFilters\ChebyshevII.h"
#include "DspFilters\ChebyshevFiltre.hpp"


int _tmain(int argc, _TCHAR* argv[])
{
	 Dsp::SimpleFilter <Dsp::ChebyshevII::BandPass <3>, 1> f;
    f.setup (3,    // order
             44100,// sample rate
             4000, // center frequency
             880,  // band width
             1);   // ripple dB
	int numSamples = 1000;
	float *arrayOfChannels[1];// = NULL;
	arrayOfChannels[0] = new float[1024];
    f.process (numSamples, arrayOfChannels);

	//BandPassFiltre b;
	//b.Setup(44100, 4000, 800, 1);
	//b.s
	double *data;// = NULL;
	data = new double[1024];
	Dsp::ChebyshevII::Design::BandPass <3> x;
	x.setup(3, 44100, 4000, 800, 1);
	Dsp::ChebyshevII::Design::BandPass <3>::State<Dsp::DirectFormII> xxx;
	x.process(100, data, xxx);
	return 0;
}

