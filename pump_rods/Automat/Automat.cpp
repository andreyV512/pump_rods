#include "App/App.h"
#include "templates/typelist.hpp"
#include "tools_debug/DebugMess.h"
#include "App/AppBase.h"
#include "Log/LogBuffer.h"
#include "Log/LogMessages.h"
#include "Automat/Automat.h"
#include "Automat/Automat.hpp"
#include "Windows.h"
#include "Windows/MainWindow/AppKeyHandler.h"
#include "Compute/Compute.h"
#include "l502Automat.hpp"
#include "Dlg/Dialogs.h"


namespace Automat
{
	DWORD WINAPI  Loop(PVOID);
	HANDLE hThread;
	HANDLE hEvent;

	
	HANDLE Key<Status::start>::hEvent;

	
	HANDLE Key<Status::stop>::hEvent;

	HANDLE Key<Status::contine_btn>::hEvent;

	int sortResult = 0;

	Result::Result()
		: inputs_bits(Singleton<InputBitTable>::Instance().items)
	{}

	Result result;

	void Init()
	{
		hEvent = CreateEvent(NULL, FALSE, FALSE, NULL);
		Key<Status::start>::hEvent = CreateEvent(NULL, FALSE, FALSE, NULL);
		Key<Status::stop>::hEvent  = CreateEvent(NULL, FALSE, FALSE, NULL);
		Key<Status::contine_btn>::hEvent  = CreateEvent(NULL, FALSE, FALSE, NULL);
		hThread = CreateThread(NULL, 0, Loop, NULL, 0, NULL);
	}

	void Destroy()
	{
		CloseHandle(hEvent);
		CloseHandle(Key<Status::start>::hEvent);
		CloseHandle(Key<Status::stop>::hEvent );
		CloseHandle(Key<Status::contine_btn>::hEvent );
		WaitForSingleObject(hThread, INFINITE);
		CloseHandle(hThread);
	}

	void Run()
	{
		SetEvent(Key<Status::start>::hEvent);
	}

	void Stop()
	{
		SetEvent(Key<Status::stop>::hEvent);
	}

	void Contine()
	{
		SetEvent(Key<Status::contine_btn>::hEvent);
	}
//-----------------------------------------------------------------------------------
	DWORD WINAPI  Loop(PVOID)
	{
		unsigned status = 0;
		unsigned status502 = 0;
		unsigned c1c2 = 0;
		bool sortOnce = true;
		App::IsRun() = true;
		
		for(;;)
		{
			for(;;)
			{
				AppKeyHandler::Stop();
				//ожидание нажатия кнопки СТАРТ
				if(App::IsRun())
				{
					AND_BITS(-1, Key<Status::start>, Key<Status::contine_btn>);
					if(result.ret == Status::contine_btn) sortOnce = false;
				}
				App::IsRun() = false;
				App::StatusBar(0, L"");
				dprint("AUTOMAT_RUN------------------------------\n");

				bool dCheck = Singleton<OnTheJobTable>::Instance().items.get<DefectSig<Check>>().value;

				
				//включена кнопка СТОП
				AppKeyHandler::Run();
				//сообщение СТАРТ ЦИКЛА
				Log::Mess<LogMess::StartCycle>();
				//проверка сигнала ЦЕПИ УПРАВЛЕНИЯ
				AND_BITS(6000 * 1000, Key<Status::stop>, On<iCU>);	

				//выставил выходной сигнал РАБОТА
				OUT_BITS(On<oWork>);
				Sleep(1000);

				//ожидание сигнала ЦИКЛ, проверка ЦЕПИ УПРАВЛЕНИЯ, выход из цикла по кнопке СТОП
				AND_BITS(-1,  Key<Status::stop>, On<iCycle>);//, Test<On<iCU>>);	
				AND_BITS(-1,  Key<Status::stop>, Off<iKM2_DC>, Off<iKM3_AC>);//, Test<On<iCU>, On<iCycle>>);	
			//	c1c2 = 0;
				if(sortOnce)
				{
					//ожидание сигнала СОРТ, проверка сигналов ЦЕПИ УПРАВЛЕНИЯ и ЦИКЛ, выход по кнопке СТОП
					AND_BITS(-1, Key<Status::stop>, On<iCOPT>);//, Test<On<iCU>, On<iCycle>>);
					//чтение дискретного рорта
					unsigned bits = device1730.Read();
					//чтение сигнала П1 и П2
					c1c2  = 0 != (bits & (1 << result.inputs_bits.get<iP1>().value))? 2: 0;
					c1c2 |= 0 != (bits & (1 << result.inputs_bits.get<iP2>().value))? 1: 0;
					
					switch(c1c2)
					{
					case 0: Log::Mess<LogMess::Off_iP1_Off_iP2>(); break;
					case 1: Log::Mess<LogMess::Off_iP1_On_iP2>(); break;
					case 2: Log::Mess<LogMess::On_iP1_Off_iP2>(); break;
					case 3: Log::Mess<LogMess::On_iP1_On_iP2>(); break;
					}
					sortOnce = false;
				}
				AND_BITS(-1, Key<Status::stop>, On<iCU>, On<iCycle>);
				if(dCheck)
				{
					//выставлен сигнал DC_ON1
					OUT_BITS(On<oDC_ON1>);
					AND_BITS(-1, Key<Status::stop>, On<iKM2_DC>);
				}
				//ВЫСТАВЛЕН СИГНАЛ ПУСК
				OUT_BITS(On<oStart>);
				Sleep(1000);
				//ожидание выключения сигналов СОРТ, П1, П2, проверка сигналов ЦЕПИ УПРАВЛЕНИЯ и ЦИКЛ, выход по кнопке СТОП
				AND_BITS(-1, Key<Status::stop>, Off<iCOPT>, Off<iP1>, Off<iP2>);//, Test<On<iCU>, On<iCycle>>);
				//ожидание включения сигнала КОНТРОЛЬ, проверка сигналов ЦЕПИ УПРАВЛЕНИЯ и ЦИКЛ, выход по кнопке СТОП
				AND_BITS(-1, Key<Status::stop>, On<iControl> ,Test<On<iCU>, On<iCycle>>);
				//ожидание включения сигнала КОНТРОЛЬ и П1, проверка сигналов ЦЕПИ УПРАВЛЕНИЯ и ЦИКЛ, выход по кнопке СТОП
				AND_BITS(-1, Key<Status::stop>, On<iControl>, On<iP1>,Test<On<iCU>, On<iCycle>>);

				//очистить главное окно
				App::CleanViewers();

				if(dCheck)
				{
					//выставлен сигнал DC_ON2
					OUT_BITS(On<oDC_ON2>);
				}
				else
				{
					OUT_BITS(On<oAC_ON>);
				}
					
				{
					//сбор данных
					Log::Mess<LogMess::DataCollectionDEF>();
					l502Run<DefectSig<DataItem::Buffer>> def;
					if(0 != (status502 = def()))
					{
						status = Status::alarm_l502;
						break;
					}
					//ожидание выключения сигнала П1, проверка сигналов ЦЕПИ УПРАВЛЕНИЯ и ЦИКЛ, выход по кнопке ЦИКЛ
					//, при превышении сбора 120 сек выход из цикла
					AND_BITS(120000, Key<Status::stop>, Off<iP1>,Test<On<iCU>, On<iCycle>>);
				}
				Compute::RecalculationDefect();
				OUT_BITS(Off<oStart>);
				if(dCheck)
				{
					//отключение сигнала DC_ON2
					OUT_BITS(Off<oDC_ON2>);
					Sleep(2000);
					//отключение сигнала DC_ON1
					OUT_BITS(Off<oDC_ON1>);
				}
				else
				{
					OUT_BITS(Off<oAC_ON>);
					Sleep(2000);
				}

				//убеждаемся что сигнал  отключён
		      AND_BITS(-1,  Key<Status::stop>, Off<iKM2_DC>, Off<iKM3_AC>, Test<On<iCU>, On<iCycle>>);	
				OUT_BITS(On<oStart>);	
				//ожидание включения сигнала КОНТРОЛЬ и П2, проверка сигналов ЦЕПИ УПРАВЛЕНИЯ и ЦИКЛ, выход по кнопке СТОП
				AND_BITS(-1, Key<Status::stop>, On<iControl>, On<iP2>,Test<On<iCU>, On<iCycle>>);
				
				//включение сигнала AC_ON
				StructSig<DataItem::Buffer> &structBuff = Singleton<StructSig<DataItem::Buffer>>::Instance();
				OUT_BITS(On<oAC_ON>);
				{
					//сбор данных
					Log::Mess<LogMess::DataCollectionSTR>();
					l502Run<StructSig<DataItem::Buffer>> str;
					if(0 != (status502 = str()))
					{
						status = Status::alarm_l502;
						break;
					}
					AND_BITS(-1, Key<Status::stop>, On<iKM3_AC> ,Test<On<iCU>, On<iCycle>>);
					//ожидание выключения сигнала П2, проверка сигналов ЦЕПИ УПРАВЛЕНИЯ и ЦИКЛ, выход по кнопке ЦИКЛ
					//, при превышении сбора 120 сек выход из цикла
					AND_BITS(120000, Key<Status::stop>, Off<iP2>,Test<On<iCU>, On<iCycle>>);
				}
				Sleep(1500);
				OUT_BITS(Off<oAC_ON>);	
				//реверс данных с структуры
				Compute::Reverse(structBuff.inputData, structBuff.currentOffset);
				//расчёт и отображение данных
				unsigned res = Compute::Recalculation(c1c2);
				Log::Mess<LogMess::DataCollectionCompleted>();
				OUT_BITS(Off<oStart>);
				
				if(0 != (res & 2))
				{
					OUT_BITS(On<oC1>);
				}
				else
				{
					OUT_BITS(Off<oC1>);
				}
				if(0 != (res & 1))
				{
					OUT_BITS(On<oC2>);
				}
				else
				{
					OUT_BITS(Off<oC2>);
				}
				sortResult = res;
				//прерывание на просмотр
				if(App::InterruptView()) // || !sortOnce)
				{
					//включены кнопки ЦИКЛ и СТОП
					AppKeyHandler::Continue();
					//кнопка ПУСК-продолжение, кнопка СТОП-выход из цикла, проверка сигналов ЦЕПИ УПРАВЛЕНИЯ и ЦИКЛ
					AND_BITS(-1, Key<Status::start>, Key<Status::contine_btn>, Key<Status::stop>);//, Test<On<iCU>, On<iCycle>>);
					if(result.ret == Status::contine_btn) continue;
				}

				//подтверждение результата
				Sleep(200);
				OUT_BITS(On<oToShift>);  //перекладка

				sortOnce = true;
				AutoStoredData();
				
				Sleep(1000);
				//включена кнопка СТОП
				AppKeyHandler::Run();
				//ожидание снятия сигнала КОНТРОЛЬ, выход из цикла-кнопка СТОП
				AND_BITS(-1, Key<Status::stop>, Off<iControl>);		
				Sleep(1000);
				//переход в начало цикла
				OUT_BITS(Off<oToShift>, Off<oC1>, Off<oC2>);
			}

			device1730.Write(0);

			App::IsRun() = true;

			switch(status)
			{
			case Status::exit_loop: 
				dprint("Status::exit_loop\n");
				return 0;
			case Status::stop:
				Log::Mess<LogMess::ExitMeshuringCycle>();
				dprint("Status::stop\n");
				sortOnce = true;
				break;
			case Status::time_out:
				dprint("Status::time_out\n");
				Log::Mess<LogMess::TimeOut>();
				break;
			case Status::alarm_bits:
				dprint("Status::alarm_bits\n");
				Log::Mess<LogMess::AlarmBits>();
				break;
			case Status::alarm_l502:
				dprint("Status::alarm_l502  %d\n", status502);
				Log::Mess<LogMess::Alarm502>(status502);
				break;
			}
		}
		return 0;
	}

	void UpSort()
	{
		--sortResult;
		sortResult &= 3;
		int res = sortResult;
		if(0 != (res & 2))
		{
			OUT_BITS(On<oC1>);
		}
		else
		{
			OUT_BITS(Off<oC1>);
		}
		if(0 != (res & 1))
		{
			OUT_BITS(On<oC2>);
		}
		else
		{
			OUT_BITS(Off<oC2>);
		}
		if(0 == res)
		{
			Log::Mess<LogMess::Brak>();
			App::StatusBar(0, L"Брак");
		}
		else
		{
			wchar_t buf[32];
			wsprintf(buf, L"Сорт %d", res);
			App::StatusBar(0, buf);
			Log::Mess<LogMess::Copt>(res);
		}
	}
}