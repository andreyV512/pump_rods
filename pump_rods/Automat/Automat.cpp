#include "Automat/Automat.h"
#include "App\App.h"
#include "templates\typelist.hpp"
#include "tools_debug\DebugMess.h"
#include "App/AppBase.h"
#include "Log\LogBuffer.h"
#include "Log\LogMessages.h"
#include "Automat/Automat.hpp"
#include "Windows.h"
#include "Windows\MainWindow\AppKeyHandler.h"
#include "Compute\Compute.h"
#include "l502Automat.hpp"


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
			, alarm_l502
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
				//обнуляем выходные сигналы
				device1730.Write(0);
				//включены кнопки ЦИКЛ ТЕСТ
				AppKeyHandler::Stop();
				//ожидание нажатия кнопки ТЕСТ
				AND_BITS(-1, Key<Status::start>);
				//очистить главное окно
				App::CleanViewers();
				//включена кнопка СТОП
				AppKeyHandler::Run();
				//сообщение СТАРТ ЦИКЛА
				Log::Mess<LogMess::StartCycle>();
				//проверка сигнала ЦЕПИ УПРАВЛЕНИЯ
				AND_BITS(3000, On<iСU>);	
				//ожидание сигнала ЦИКЛ, проверка ЦЕПИ УПРАВЛЕНИЯ, выход из цикла по кнопке СТОП
				AND_BITS(-1,  Key<Status::stop>, On<iCycle>, Test<On<iСU>>);	
				//выставил выходной сигнал РАБОТА
				OUT_BITS(On<oWork>);
				//ожидание сигнала СОРТ, проверка сигналов ЦЕПИ УПРАВЛЕНИЯ и ЦИКЛ, выход по кнопке СТОП
				AND_BITS(-1, Key<Status::stop>, On<iCOPT>, Test<On<iСU>, On<iCycle>>);
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
				AND_BITS(-1, Key<Status::start>, Key<Status::stop>, Test<On<iСU>, On<iCycle>>);
				//включена кнопка СТОП
				AppKeyHandler::Run();

				//убеждаемся что сигнал ACON DCON1 отключён
				TEST_OUTPUT_BITS(Off<oAC_ON>, Off<oDC_ON2>);
				//выставлен сигнал DC_ON1
				OUT_BITS(On<oDC_ON1>);
				//ВЫСТАВЛЕН СИГНАЛ ПУСК
				OUT_BITS(On<oStart>);
				//ожидание выключения сигналов СОРТ, П1, П2, проверка сигналов ЦЕПИ УПРАВЛЕНИЯ и ЦИКЛ, выход по кнопке СТОП
				AND_BITS(-1, Key<Status::stop>, Off<iCOPT>, Off<iP1>, Off<iP2>,Test<On<iСU>, On<iCycle>>);
				//ожидание включения сигнала КОНТРОЛЬ, проверка сигналов ЦЕПИ УПРАВЛЕНИЯ и ЦИКЛ, выход по кнопке СТОП
				AND_BITS(-1, Key<Status::stop>, On<iControl>,Test<On<iСU>, On<iCycle>>);
				//ожидание включения сигнала КОНТРОЛЬ и П1, проверка сигналов ЦЕПИ УПРАВЛЕНИЯ и ЦИКЛ, выход по кнопке СТОП
				AND_BITS(-1, Key<Status::stop>, On<iControl>, On<iP1>,Test<On<iСU>, On<iCycle>>);
				//выставлен сигнал DC_ON2
				OUT_BITS(On<oDC_ON2>);
				//сбор данных
				if(!l502Run<DefectSig>()())
				{
					status = Status::alarm_l502;
					break;
				}
				//ожидание выключения сигнала П1, проверка сигналов ЦЕПИ УПРАВЛЕНИЯ и ЦИКЛ, выход по кнопке ЦИКЛ
				//, при превышении сбора 120 сек выход из цикла
				AND_BITS(120000, Key<Status::stop>, Off<iP1>,Test<On<iСU>, On<iCycle>>);
				//отключение сигнала DC_ON2
				OUT_BITS(Off<oDC_ON2>);
				Sleep(200);
				//отключение сигнала DC_ON1
				OUT_BITS(Off<oDC_ON1>);

				//убеждаемся что сигнал  отключён
				TEST_OUTPUT_BITS(Off<oDC_ON1>, Off<oDC_ON2>);
				//ожидание включения сигнала КОНТРОЛЬ и П2, проверка сигналов ЦЕПИ УПРАВЛЕНИЯ и ЦИКЛ, выход по кнопке СТОП
				AND_BITS(-1, Key<Status::stop>, On<iControl>, On<iP2>,Test<On<iСU>, On<iCycle>>);
				//включение сигнала AC_ON
				OUT_BITS(On<oAC_ON>);
				//сбор данных
				if(!l502Run<StructSig>()())
				{
					status = Status::alarm_l502;
					break;
				}
				//ожидание выключения сигнала П2, проверка сигналов ЦЕПИ УПРАВЛЕНИЯ и ЦИКЛ, выход по кнопке ЦИКЛ
				//, при превышении сбора 120 сек выход из цикла
				AND_BITS(120000, Key<Status::stop>, Off<iP2>,Test<On<iСU>, On<iCycle>>);
				OUT_BITS(Off<oAC_ON>, Off<oStart>);
				//расчёт и отображение данных
				Compute::Recalculation();
				//прерывание на просмотр
				if(App::InterruptView())
				{
					//включены кнопки ЦИКЛ и СТОП
					AppKeyHandler::Continue();
					//кнопка ПУСК-продолжение, кнопка СТОП-выход из цикла, проверка сигналов ЦЕПИ УПРАВЛЕНИЯ и ЦИКЛ
					AND_BITS(-1, Key<Status::start>, Key<Status::stop>, Test<On<iСU>, On<iCycle>>);
				}
				//формирование результата
				c1c2 = Compute::Result(c1c2);
				OUT_BITS(Off<oC1>, Off<oC2>);
				if(0 != (c1c2 & 2)) OUT_BITS(On<oC1>);
				if(0 != (c1c2 & 1)) OUT_BITS(On<oC2>);
				//подтверждение результата
				OUT_BITS(On<oToShift>);  //перекладка
				//включена кнопка СТОП
				AppKeyHandler::Run();
				//ожидание снятия сигнала КОНТРОЛЬ, выход из цикла-кнопка СТОП
				AND_BITS(-1, Key<Status::stop>, Off<iControl>);
				//переход в начало цикла
			}

			l502.Stop();

			switch(status)
			{
			case Status::exit_loop: 
				device1730.Write(0);
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
			case Status::alarm_l502:
				dprint("Status::alarm_l502\n");
				break;
			}
		}
		return 0;
	}
}