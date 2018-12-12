#pragma once

class L502Work
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
	template<template<class>class W>bool SetupParams(){return true;}
	int Start();
	int Stop();
	int Read(unsigned &startChennel, double *data, unsigned &count);
};
