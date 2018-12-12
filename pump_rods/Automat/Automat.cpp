#include "Automat/Automat.h"
#include "App\App.h"
#include "templates\typelist.hpp"
#include "tools_debug\DebugMess.h"
#include "App/AppBase.h"
#include "Log\LogBuffer.h"
#include "Log\LogMessages.h"
#include "Automat/Automat.hpp"
#include "Windows.h"


namespace Automat
{
	DWORD WINAPI  Loop(PVOID);
	HANDLE hThread;
	HANDLE hEvent;

	struct Status
	{
		enum e{
			undefined
			, exit_loop
			, time_out
			, start
			, stop
			, alarm_bits
		};
	};

	struct Result
	{
		unsigned error;
		unsigned key;
		unsigned bits;
		unsigned ret;
		InputBitTable::TItems &inputs_bits;
		Result()
			: inputs_bits(Singleton<InputBitTable>::Instance().items)
		{}
	} result;

	//Status::e status = Status::exit_loop;
	unsigned status = 0;

	//struct Buttons
	//{
	//	enum{
	//		undefined
	//		, start
	//		, stop
	//		
	//	};
	//};

	void Init()
	{
		hEvent = CreateEvent(NULL, FALSE, FALSE, NULL);
		hThread = CreateThread(NULL, 0, Loop, NULL, 0, NULL);
	}

	void Destroy()
	{
		CloseHandle(hEvent);
		WaitForSingleObject(hThread, INFINITE);
		CloseHandle(hThread);
	}

	void Run()
	{
		result.key = Status::start;
		SetEvent(hEvent);
	}

	void Stop()
	{
		result.key = Status::stop;
		SetEvent(hEvent);
	}
//-----------------------------------------------------------------------------------
	//wchar_t *mess = L"";

	DWORD WINAPI  Loop(PVOID)
	{
		for(;;)
		{
			for(;;)
			{
				device1730.Write(0);
				AND_BITS(-1, Key<Status::start>);	
				Log::Mess<LogMess::StartCycle>();
				AND_BITS(3000, On<iСU>);	
				AND_BITS(3000, On<iCycle>, Test<On<iСU>>);	

				OUT_BITS(On<oWork>);

				AND_BITS(-1, Key<Status::stop>, On<iCOPT>, Test<On<iСU>, On<iCycle>>);

				unsigned bits = device1730.Read();

				bool p1 = 0 != (bits & (1 << result.inputs_bits.get<iP1>().value));
				bool p2 = 0 != (bits & (1 << result.inputs_bits.get<iP2>().value));
			}

			switch(status)
			{
			case Status::exit_loop: 
				dprint("Status::exit_loop\n");
				return 0;
			case Status::stop:
				if(result.key == Status::stop)
				{
						Log::Mess<LogMess::ExitMeshuringCycle>();
				}
				dprint("Status::stop\n");
				break;
			case Status::time_out:
				dprint("Status::time_out\n");
				break;
			case Status::alarm_bits:
				dprint("Status::alarm_bits\n");
				break;
			}
		}
		return 0;
	}
}