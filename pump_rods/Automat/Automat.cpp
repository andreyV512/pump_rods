﻿#include "App/App.h"
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


namespace Automat
{
	DWORD WINAPI  Loop(PVOID);
	HANDLE hThread;
	HANDLE hEvent;

	
	HANDLE Key<Status::start>::hEvent;

	
	HANDLE Key<Status::stop>::hEvent;

	Result::Result()
		: inputs_bits(Singleton<InputBitTable>::Instance().items)
	{}

	Result result;

	void Init()
	{
		hEvent = CreateEvent(NULL, FALSE, FALSE, NULL);
		Key<Status::start>::hEvent = CreateEvent(NULL, FALSE, FALSE, NULL);
		Key<Status::stop>::hEvent  = CreateEvent(NULL, FALSE, FALSE, NULL);
		hThread = CreateThread(NULL, 0, Loop, NULL, 0, NULL);
	}

	void Destroy()
	{
		CloseHandle(hEvent);
		CloseHandle(Key<Status::start>::hEvent);
		CloseHandle(Key<Status::stop>::hEvent );
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
//-----------------------------------------------------------------------------------
	DWORD WINAPI  Loop(PVOID)
	{
		unsigned status = 0;
		unsigned status502 = 0;
		
		for(;;)
		{
			for(;;)
			{
				//обнуляем выходные сигналы
				//device1730.Write(0);
				//выставил выходной сигнал РАБОТА
				SET_OUT_BITS(On<oWork>);
				//включены кнопки ЦИКЛ ТЕСТ
				AppKeyHandler::Stop();
				App::IsRun() = true;
				//ожидание нажатия кнопки СТАРТ
				AND_BITS(-1, Key<Status::start>);
				App::IsRun() = false;
				dprint("AUTOMAT_RUN------------------------------\n");

				//очистить главное окно
				App::CleanViewers();
				//включена кнопка СТОП
				AppKeyHandler::Run();
				//сообщение СТАРТ ЦИКЛА
				Log::Mess<LogMess::StartCycle>();
				//проверка сигнала ЦЕПИ УПРАВЛЕНИЯ
				AND_BITS(500, Key<Status::stop>, On<iCU>);	

					//выставил выходной сигнал РАБОТА
				//OUT_BITS(On<oWork>);

				//ожидание сигнала ЦИКЛ, проверка ЦЕПИ УПРАВЛЕНИЯ, выход из цикла по кнопке СТОП
				AND_BITS(-1,  Key<Status::stop>, On<iCycle>, Test<On<iCU>>);	
				dprint("x 1\n");

				AND_BITS(-1,  Key<Status::stop>, Off<iKM2_DC>, Off<iKM3_AC>, Test<On<iCU>, On<iCycle>>);	
				dprint("x 2\n");

		//		//выставил выходной сигнал РАБОТА
		//		OUT_BITS(On<oWork>);
				//ожидание сигнала СОРТ, проверка сигналов ЦЕПИ УПРАВЛЕНИЯ и ЦИКЛ, выход по кнопке СТОП
				AND_BITS(-1, Key<Status::stop>, On<iCOPT>, Test<On<iCU>, On<iCycle>>);
				dprint("x 3\n");
				//чтение дискретного рорта
				unsigned bits = device1730.Read();
				//чтение сигнала П1 и П2
				unsigned c1c2  = 0 != (bits & (1 << result.inputs_bits.get<iP1>().value))? 2: 0;
				         c1c2 |= 0 != (bits & (1 << result.inputs_bits.get<iP2>().value))? 1: 0;
				//сообщение "ВНИМАТЕЛЬНО ПРОВЕРЬ ПОЛОЖЕНИЕ ШТАНГИ В ЗАХВАТАХ!"
				Log::Mess<LogMess::InfoCycle>();
				//включены кнопки ЦИКЛ и СТОП
				AppKeyHandler::Continue();
				//кнопка ЦИКЛ - продолжение, СТОП - выход из цикла, проверка сигналов ЦЕПИ УПРАВЛЕНИЯ и ЦИКЛ
				AND_BITS(-1, Key<Status::start>, Key<Status::stop>/*, Test<On<iCU> On<iCycle>>*/);
				dprint("x 4\n");
				//включена кнопка СТОП
				AppKeyHandler::Run();

				//убеждаемся что сигнал ACON DCON1 отключён
				//xxxxxxxxxxxxxxx TEST_OUTPUT_BITS(Off<oAC_ON>, Off<oDC_ON2>);
				//выставлен сигнал DC_ON1
				OUT_BITS(On<oDC_ON1>);
				AND_BITS(-1, Key<Status::stop>, On<iKM2_DC>, Test<On<iCU>, On<iCycle>>);
				dprint("x 5\n");
				//ВЫСТАВЛЕН СИГНАЛ ПУСК
				OUT_BITS(On<oStart>);
				//ожидание выключения сигналов СОРТ, П1, П2, проверка сигналов ЦЕПИ УПРАВЛЕНИЯ и ЦИКЛ, выход по кнопке СТОП
				AND_BITS(-1, Key<Status::stop>, Off<iCOPT>, Off<iP1>, Off<iP2>);//todo, Test<On<iCU>, On<iCycle>, On<iKM2_DC>, Off<iKM3_AC>>);
				dprint("x 6\n");
				//ожидание включения сигнала КОНТРОЛЬ, проверка сигналов ЦЕПИ УПРАВЛЕНИЯ и ЦИКЛ, выход по кнопке СТОП
				AND_BITS(-1, Key<Status::stop>, On<iControl>);//todo ,Test<On<iCU>, On<iCycle>, On<iKM2_DC>, Off<iKM3_AC>>);
				dprint("x 7\n");
				//ожидание включения сигнала КОНТРОЛЬ и П1, проверка сигналов ЦЕПИ УПРАВЛЕНИЯ и ЦИКЛ, выход по кнопке СТОП
				AND_BITS(-1, Key<Status::stop>, On<iControl>, On<iP1>);//todo,Test<On<iCU>, On<iCycle>, On<iKM2_DC>, Off<iKM3_AC>>);
				dprint("x 8\n");
				//выставлен сигнал DC_ON2
				OUT_BITS(On<oDC_ON2>);
				{
					//сбор данных
					Log::Mess<LogMess::DataCollectionDEF>();
					l502Run<DefectSig<DataItem::Buffer>> def;//(Singleton<DefectSig<DataItem::Buffer>>::Instance());
					if(0 != (status502 = def()))
					{
						status = Status::alarm_l502;
						break;
					}
					//ожидание выключения сигнала П1, проверка сигналов ЦЕПИ УПРАВЛЕНИЯ и ЦИКЛ, выход по кнопке ЦИКЛ
					//, при превышении сбора 120 сек выход из цикла
					AND_BITS(120000, Key<Status::stop>, Off<iP1>);//todo,Test<On<iCU>, On<iCycle>, On<iKM2_DC>, Off<iKM3_AC>>);
					dprint("x 9\n");
				}
				//отключение сигнала DC_ON2
				OUT_BITS(Off<oDC_ON1>);
		//todo		AND_BITS(-1, Key<Status::stop>);//, Off<iKM2_DC>);//todo, Test<On<iCU>, On<iCycle>>);
				dprint("x 10\n");
				Sleep(2000);
				//отключение сигнала DC_ON1
  			OUT_BITS(Off<oDC_ON1>);

				//убеждаемся что сигнал  отключён
		//todo		AND_BITS(-1,  Key<Status::stop>);//todo, Off<iKM2_DC>, Off<iKM3_AC>);//todo, Test<On<iCU>, On<iCycle>>);	
				dprint("x 11\n");
				//ожидание включения сигнала КОНТРОЛЬ и П2, проверка сигналов ЦЕПИ УПРАВЛЕНИЯ и ЦИКЛ, выход по кнопке СТОП
				AND_BITS(-1, Key<Status::stop>, On<iControl>, On<iP2>);//todo,Test<On<iCU>, On<iCycle>>);
				dprint("x 12\n");
				
				//включение сигнала AC_ON
				StructSig<DataItem::Buffer> &structBuff = Singleton<StructSig<DataItem::Buffer>>::Instance();
				OUT_BITS(On<oAC_ON>);
				{
					//сбор данных
					Log::Mess<LogMess::DataCollectionSTR>();
					l502Run<StructSig<DataItem::Buffer>> str;//(structBuff);
					if(0 != (status502 = str()))
					{
						status = Status::alarm_l502;
						break;
					}
			//		AND_BITS(1200, Key<Status::stop>);//todo, On<iKM3_AC>);//todo,Test<On<iCU>, On<iCycle>, Off<iKM2_DC>>);
					dprint("x 13\n");
					//ожидание выключения сигнала П2, проверка сигналов ЦЕПИ УПРАВЛЕНИЯ и ЦИКЛ, выход по кнопке ЦИКЛ
					//, при превышении сбора 120 сек выход из цикла
					AND_BITS(120000, Key<Status::stop>, Off<iP2>);//todo,Test<On<iCU>, On<iCycle>, Off<iKM2_DC>, On<iKM3_AC>>);
					dprint("x 14\n");
				}
				OUT_BITS(Off<oAC_ON>, Off<oStart>);	
				//реверс данных с структуры
				Compute::Reverse(structBuff.inputData, structBuff.currentOffset);
				//расчёт и отображение данных
				Compute::Recalculation();
				Log::Mess<LogMess::DataCollectionCompleted>();

				int res = Compute::Result(c1c2);
				if(0 == res) Log::Mess<LogMess::Brak>();
				else Log::Mess<LogMess::Copt>(res);

				//прерывание на просмотр
				if(App::InterruptView())
				{
					//включены кнопки ЦИКЛ и СТОП
					AppKeyHandler::Continue();
					//кнопка ПУСК-продолжение, кнопка СТОП-выход из цикла, проверка сигналов ЦЕПИ УПРАВЛЕНИЯ и ЦИКЛ
					AND_BITS(-1, Key<Status::start>, Key<Status::stop>, Test<On<iCU>, On<iCycle>>);
					dprint("x 15\n");
				}
				//формирование результата
				res = Compute::Result(c1c2);
				if(0 == res) Log::Mess<LogMess::Brak>();
				else Log::Mess<LogMess::Copt>(res);

				OUT_BITS(Off<oC1>, Off<oC2>);
				if(0 != (res & 2)) OUT_BITS(On<oC1>);
				if(0 != (res & 1)) OUT_BITS(On<oC2>);
				//подтверждение результата
				OUT_BITS(On<oToShift>);  //перекладка
				//включена кнопка СТОП
				AppKeyHandler::Run();
				//ожидание снятия сигнала КОНТРОЛЬ, выход из цикла-кнопка СТОП
				AND_BITS(-1, Key<Status::stop>, Off<iControl>);
				dprint("x 16\n");
				//переход в начало цикла
			}

		//	l502.Stop();
			dprint("x 999\n");

			switch(status)
			{
			case Status::exit_loop: 
				device1730.Write(0);
				dprint("Status::exit_loop\n");
				return 0;
			case Status::stop:
				Log::Mess<LogMess::ExitMeshuringCycle>();
				dprint("Status::stop\n");
				break;
			case Status::time_out:
				dprint("Status::time_out\n");
				break;
			case Status::alarm_bits:
				dprint("Status::alarm_bits\n");
				break;
			case Status::alarm_l502:
				dprint("Status::alarm_l502  %d\n", status502);
				Log::Mess<LogMess::Alarm502>(status502);
				break;
			}
		}
		return 0;
	}
}