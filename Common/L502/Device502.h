#pragma once

class Device502
{
	void *hnd;
public:
 // static const int buffer_length = 1024;
 // int &ADC_FREQ;
 // int &referenceV;
 // int &dataV;
 // int READ_TIMEOUT;
public:
//	L502Work();
	bool Init();
	void Destroy();
	bool SetupParams(int *f_channels, int *f_ch_ranges, int ADC_FREQ, int countChannels);
	int Start();
	int Stop();
	int Read(unsigned &startChennel, double *data, unsigned &count);
};
