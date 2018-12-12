#pragma once

template<int N>struct IDtoMess;

#define __double_1 " %.1f" 
#define __double_2 " %.2f"
#define __int " %d"
#define __void ""
#define __bool " %s"
#define __const_char_ptr " %s"
#define _cat(a, b) a##b

#define MESS(name, tpe, txt, bc, tc)struct name\
{\
	static const int ID = __COUNTER__;\
	typedef tpe type;\
	static const unsigned backColor = bc;\
    static const unsigned textColor = tc;\
	static const char *mess(){return _cat(txt, __##tpe);}\
};\
template<>struct IDtoMess<name::ID>{typedef name Result;};

//#define MESSX(name, tpe, txt, bc, tc) template<>struct Row<name>\
//{\
//	typedef tpe type;\
//	static const unsigned backColor = bc;\
//    static const unsigned textColor = tc;\
//	static const char *mess(){return txt;}\
//};

#define MESS1(name, tpe, txt, bc, tc)struct name\
{\
	static const int ID = __COUNTER__;\
	typedef tpe type;\
	static const unsigned backColor = bc;\
    static const unsigned textColor = tc;\
	static const char *mess(){return _cat(txt, __##tpe##_1);}\
};\
template<>struct IDtoMess<name::ID>{typedef name Result;};

//#define MESS2(name, tpe, txt, bc, tc) template<>struct Row<name>\
//{\
//	typedef tpe type;\
//	static const unsigned backColor = bc;\
//    static const unsigned textColor = tc;\
//	static const char *mess(){return _cat(txt, __##tpe##_2);}\
//};



	static const int green = 0xff00;
	static const int blue  = 0xff0000;
	static const int black = 0x0;
	static const int white = 0xffffff;
	static const int red = 0xff;
	static const int yellow = 0xffff;
namespace LogMess
{
	///\brief сопоставление числу сообщениея
	///\param тип параметра
	///\param тип дополнительного параметра
	///\param сообщение
	///\param цвет фона
	///\param цвет шрифта
	MESS(StartCycle		          , void  , "Пуск цикла"              , blue, white)
	MESS(ProgramOpen		          , void  , "Программа открыта"              , black, white)
//							          
	MESS(TimeoutPipe		          , void  , "Превышенно время ожидания", red  , yellow)
						          
	MESS(ProgramClosed		          , void  , "Программа закрыта"	, red  , yellow)
//							          
	MESS(ErrStop                      , void  , "Прерывание цикла измерения", red, yellow)

	MESS(AlarmExitRunBitIn			  , void, "Ошибка частотного преобразователя\"", red  , yellow)
	MESS(AlarmExitControlCircuitBitIn , void, "Нет сигнала \"Цепи управления\""	   , red  , yellow)
	MESS(AlarmExitControlCycleBitIn , void, "Нет сигнала \"Цикл\""	   , red  , yellow)

	MESS(InfoOnSycleBitIn               , void, "Ожидание сигнала \"Цикл\""					  , blue , white)

	MESS(InfoUserStop					, void, "Пользователь нажал на кнопку \"СТОП\""			  , red  , yellow)

	MESS(Recompute					, void, "Перерасчёт"			, red  , yellow)
	MESS(RecomputeStop				, void, "Перерасчёт закончен"	, blue , white)


	MESS(CheckStatusFrequencyConverter, void, "Проверка состояния частотного преобразователя", blue , white)
//
	MESS(CompletionChangeInPositionModule, void, "Завершение по изменению положения какого-либо модуля", red, yellow)
//
//
	MESS1(TemperatureCoilSolenoidExceeded0, double, "Превышена температура обмотки соленоида 1", red, yellow)
	MESS1(TemperatureCoilSolenoidExceeded1, double, "Превышена температура обмотки соленоида 2", red, yellow)
//
	MESS(AnalogBoardFailure, void, "Авария платы 502", red, yellow)
	MESS(ExitMeshuringCycle, void, "Оператор вышел из цикла", red, yellow)
//
	MESS(SettingOperatingPositionControl, void , "Установка рабочего положения модулей контроля", blue, white)
	MESS(ModulesInNon_OperatingPosition, void, "Модули в нерабочем положении", red, yellow)
//
	MESS(time_overflow, void, "Ошибка COM-порта: превышено время ожидания", red, yellow)
	MESS(error_crc    , void, "Ошибка COM-порта: ошибка CRC", red, yellow)
	MESS(error_count  , void, "Ошибка COM-порта: принято 0 байт", red, yellow)
//
	MESS(iWork_pnevmoWait, void, "Ожидание готовности контроллера пневмооборудования", blue , white)
	MESS(iWork_pnevmAlarm, void, "Авария контроллера пневмооборудования", red, yellow)
//
	MESS(SOP_MODE, void, "Режим \"СОП\" ожидание трубы", blue , white) 
	MESS(TUBE_MODE, void, "Режим \"ТРУБА\" ожидание трубы", blue, white)
	MESS(RequestPipeNumber, void, "Запрос номера трубы", blue, white)
	MESS(ErrRequestPipeNumber, void, "Повторный запрос номера трубы", blue, white)
	MESS(WAITING_LONGITUDINAL_MODULE, void, "ОЖИДАНИЕ ГОТОВНОСТИ ПРОДОЛЬНОГО МОДУЛЯ", blue, white)
	MESS(WAITING_PERFORMANCE_THICKNESS_CONTROL_MODULE, void, "ОЖИДАНИЕ ГОТОВНОСТИ МОДУЛЯ КОНТРОЛЯ ТОЛЩИНЫ", blue, white)
	MESS(ThicknessModuleNotReady, void, "Модуль толщины не готов", red, yellow)
	MESS(PIPE_CONTROL_IMPLEMENTED, void, "ВЫПОЛНЯЕТСЯ КОНТРОЛЬ ТРУБЫ", blue, white)
//
	
//
	MESS(transferControlParametersThicknessGauge, void, "Передача параметров контроля в толщиномер", blue, white)
	//MESS(repeatTransferControlParametersThicknessGauge, void, "Повторить передачу параметров контроля в толщиномер",red, yellow)
	MESS(waitingThicknessGauge, void, "Oжидание готовности толщиномера", blue, white)
	MESS(waitingPipeEntranceRollerTable, void, "Ожидание трубы на входном рольганге", blue, white)
	MESS(waitingThicknessResult, void, "Oжидание результатов измерений толщиномера", blue, white)
//
	MESS(interruptView, void, "Прерывание на просмотр", blue, white)
//
	MESS(transferResultControlAutomatedControlSystem, void, "Передача результата контроля в АСУ", blue, white)
	MESS(ErrTransferResultControlAutomatedControlSystem, void, "Повторная передача результата контроля в АСУ", red, white)
	//MESS(contineRun, void, "\"F4 ЦИКЛ\"далее \"F11 Продол...\"повторить запрос к АСУ \"Esc Стоп\"выход из цикла", red, white)
	MESS(storedDataBase, void, "Сохранение в базе данных", blue, white)

	MESS(demagnetizationNotCorrect, void, "Авария. Размагничивание неисправно", red, yellow)
	MESS(demagnetizationTesting, void, "Тестирование размагничивания", blue, white)

	MESS(unit502SetupParams, void, "Плата L502 не инициализируется", red, yellow)
	MESS(unit502RunErr, void, "Плата L502 не запускается", red, yellow)

    MESS(emergencyExit, void, "Выход по аварии", red, yellow)

	MESS(tubeExit, void, "Труба вышла из установки", blue, white)

	MESS(transportExit, void, "ТРАНСПОРТ РАЗБЛОКИРОВАН. ВЫПОЛНЯЙТЕ ВЫГОН", blue, white)

static const int MAX_MESS_ID = __COUNTER__;

	class FactoryMessages
	{
		struct Inner;
		Inner &inner;
		FactoryMessages();
	public:
		void StartTime();
		bool Color(int i, unsigned &backColor, unsigned &textColor);
		bool Text(int i, char *buf, double val);
		static FactoryMessages &Instance();
	};
}
#undef MESS
#undef MESS1
#undef MESS2
#undef __double_1 
#undef __double_2 
#undef __int 
#undef __void 
#undef _cat


