#pragma once
#include "Inc/BDaqCL.h" 
class Device1730
{
	static const int startPoint = 0;
	Automation::BDaq::BDaqDevice *device;
	Automation::BDaq::BDaqDio    *dio;
	CRITICAL_SECTION cs;
	bool isOpen;	
public:	
	Device1730();
	bool Init(int );
	void Destroy();
	bool IsOpen();
	unsigned Read();
	void Write(unsigned);
	unsigned ReadOutput();

	void WriteOutput(unsigned, unsigned = (unsigned)-1);
};

extern Device1730 device1730;

