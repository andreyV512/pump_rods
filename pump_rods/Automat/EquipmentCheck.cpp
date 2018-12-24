#include "EquipmentCheck.h"
#include "Windows\MainWindow\AppKeyHandler.h"
#include "App/App.h"
#include "App/AppBase.h"
#include "Log\LogBuffer.h"
#include "templates\typelist.hpp"
#include "Automat\Automat.h"
#include "Automat\Automat.hpp"

#include "Automat\l502Automat.hpp"
#include "Compute\Compute.h"

namespace Automat
{
	struct EquipmentCheckDelay
	{
		static unsigned delay;
		static void Init()
		{
			delay =  GetTickCount();
			dprint("start delay %d\n", delay);
			delay += 10000;
		}
		static void Do(Result &r)
		{
			if(delay < GetTickCount())
			{
				r.ret = Status::contine;
				dprint("stop delay  %d\n", GetTickCount());
			}
		}
	};
	unsigned EquipmentCheckDelay::delay;
}

//template

DWORD WINAPI EquipmentCheck__(PVOID)
{
	using namespace Automat;
	unsigned status = 0;
	unsigned status502 = 0;
	AppKeyHandler::Run();
	device1730.Write(0);
	GUARD{
		device1730.Write(0);
		AppKeyHandler::Stop();
		l502.Stop();
	};
	//очистить главное окно
	App::CleanViewers();
	do
	{
		OUT_BITS(On<oDC_ON1>);
		Sleep(200);
		OUT_BITS(On<oDC_ON2>);
		{
			//сбор данных
			Log::Mess<LogMess::DataCollectionDEF>();
			l502Run<DefectSig<DataItem::Buffer>> def(Singleton<DefectSig<DataItem::Buffer>>::Instance());
			if(def.ok != (status502 = def()))
			{
				status = Status::alarm_l502;
				break;
			}
			////выход по кнопке ЦИКЛ, при превышении сбора 10 сек выход из цикла
			EquipmentCheckDelay::Init();
			AND_BITS(-1, Key<Status::stop>, Proc<EquipmentCheckDelay>);
		}
		OUT_BITS(Off<oDC_ON2>);
		Sleep(200);
		OUT_BITS(Off<oDC_ON1>);

		StructSig<DataItem::Buffer> &structBuff = Singleton<StructSig<DataItem::Buffer>>::Instance();
		OUT_BITS(On<oAC_ON>);
		{
			//сбор данных
			Log::Mess<LogMess::DataCollectionSTR>();
			l502Run<StructSig<DataItem::Buffer>> str(structBuff);
			if(str.ok != (status502 = str()))
			{
				status = Status::alarm_l502;
				break;
			}
			////выход по кнопке ЦИКЛ, при превышении сбора 10 сек выход из цикла
			EquipmentCheckDelay::Init();
			AND_BITS(-1, Key<Status::stop>, Proc<EquipmentCheckDelay>);
		}
		OUT_BITS(Off<oAC_ON>);

		Compute::Recalculation();
		Log::Mess<LogMess::DataCollectionCompleted>();
	}while(false);

	switch(status)
	{
	case Status::stop:
		Log::Mess<LogMess::ExitMeshuringCycle>();
		dprint("Status++++++++++++++++++++::stop\n");
		break;
	case Status::alarm_l502:
		dprint("Status::alarm_l502  %d\n", status502);
		Log::Mess<LogMess::Alarm502>(status502);
		break;
	}
	return 0;
}

void EquipmentCheck()
{
	CloseHandle(CreateThread(NULL, 0, EquipmentCheck__, NULL, 0, NULL));
}