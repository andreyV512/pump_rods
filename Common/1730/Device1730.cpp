#include "stdafx.h"
#include "Device1730.h"
#include "App/Config.h"
#include "tools_debug\DebugMess.h"

using namespace Automation::BDaq;

#define Mess(n)

Device1730::Device1730()
	: instantDoCtrl(NULL)
	, instantDiCtrl(NULL)
{
	InitializeCriticalSection(&cs);
}
//------------------------------------------------------------------------------
#ifndef DEBUG_ITEMS
bool Device1730::Init(wchar_t *deviceDescription)
{
	ErrorCode        ret = Success;
	Destroy();

	DeviceInformation devInfo(deviceDescription);
	
	instantDoCtrl = AdxInstantDoCtrlCreate();	
    ret = instantDoCtrl->setSelectedDevice(devInfo);
	if(BioFailed(ret))
	{
		Mess(DeviceOpenError)
		return isOpen = false;
	}

	instantDiCtrl = AdxInstantDiCtrlCreate();
    ret = instantDiCtrl->setSelectedDevice(devInfo);
	if(BioFailed(ret))
	{
		Mess(DeviceOpenError)
		return isOpen = false;
	}

	Mess(AllOk)	
	return isOpen = true;
}
//-------------------------------------------------------------------------
void Device1730::Destroy()
{
	if(instantDoCtrl != NULL)
	{
		instantDoCtrl->Dispose();
		instantDoCtrl = NULL;
	}
	if(instantDiCtrl != NULL)
	{
		instantDiCtrl->Dispose();
		instantDiCtrl = NULL;
	}
}
#else
bool Device1730::Init(wchar_t *)
{
	return true;
}
//-------------------------------------------------------------------------
void Device1730::Destroy()
{}
#endif
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
	instantDiCtrl->Read(startPoint, 2, (BYTE *)&input);
	return input;
}
//--------------------------------------------------------------------------
void Device1730::Write(unsigned output)
{
	instantDoCtrl->Write(startPoint, 2, (BYTE *)&output);
}
//--------------------------------------------------------------------------
unsigned Device1730::ReadOutput()
{
	unsigned res;
	instantDoCtrl->Read(startPoint, 2, (BYTE *)&res);
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
#include "App/App.h"
#include "Emulator\Emulator.h"
namespace
{
Emulator &emulator = Singleton<Emulator>::Instance();
}
unsigned Device1730::Read()
{
	return emulator.Inputs();
}
//--------------------------------------------------------------------------
void Device1730::Write(unsigned output)
{
	emulator.Outputs(output);
}
//--------------------------------------------------------------------------
unsigned Device1730::ReadOutput()
{	
	return emulator.Outputs();
}
//----------------------------------------------------------------------------
void Device1730::WriteOutput(unsigned output, unsigned maska)
{	
	 unsigned t = emulator.Outputs();
	 t &= ~maska;
	 t |= output;
	 Write(t);
}
#endif

