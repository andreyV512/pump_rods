#include "stdafx.h"
#include "Device1730.h"
#include "Config.h"
#include "DebugMess.h"

using namespace Automation::BDaq;

#define Mess(n)

Device1730::Device1730()
	: device(NULL)
	, dio(NULL)	
{
	InitializeCriticalSection(&cs);
}
//------------------------------------------------------------------------------
bool Device1730::Init(int deviceDescription)
{
	Destroy();
	long errorCode = BDaqDevice::Open( deviceDescription, ModeWrite, device);
	if(BioFailed(errorCode))
	{
		Mess(DeviceOpenError)
		return isOpen = false;
	}
	errorCode = device->GetModule(0,dio);
	if(BioFailed(errorCode)) 
	{
		Mess(GetModuleError)
		return isOpen = false;
	}
	Mess(AllOk)	
	return isOpen = true;
}
//-------------------------------------------------------------------------
void Device1730::Destroy()
{
	if(device != NULL)
	{
		device->Close();
	}
}
//--------------------------------------------------------------------------
bool Device1730::IsOpen()
{
	return isOpen;
}
//--------------------------------------------------------------------------
#ifndef DEBUG_ITEMS
unsigned Device1730::Read()
{
	unsigned input;
	dio->DiRead(startPoint, 2, (BYTE *)&input);
	return input;
}
//--------------------------------------------------------------------------
void Device1730::Write(unsigned output)
{
	dio->DoWrite(startPoint, 2, (BYTE *)&output);
}
//--------------------------------------------------------------------------
unsigned Device1730::ReadOutput()
{
	unsigned res;
	dio->DoRead(startPoint, 2, (BYTE *)&res);
	return res;
}
//----------------------------------------------------------------------------
void Device1730::WriteOutput(unsigned output, unsigned maska)
{
	 EnterCriticalSection(&cs);
	 unsigned t = ReadOutput();
	 t &= ~maska;
	 t |= output;
	 Write(t);
	 LeaveCriticalSection(&cs);
}
#else
#pragma message("Порт 1730 отключён")
unsigned out = 0;
unsigned Device1730::Read()
{
	return out;
}
//--------------------------------------------------------------------------
void Device1730::Write(unsigned output)
{
	out = output;
}
//--------------------------------------------------------------------------
unsigned Device1730::ReadOutput()
{	
	return out;
}
//----------------------------------------------------------------------------
void Device1730::WriteOutput(unsigned output, unsigned maska)
{	
	 unsigned t = out;
	 t &= ~maska;
	 t |= output;
	 Write(t);
}
#endif

