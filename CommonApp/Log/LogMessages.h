﻿#pragma once

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

#define MESS1(name, tpe, txt, bc, tc)struct name\
{\
	static const int ID = __COUNTER__;\
	typedef tpe type;\
	static const unsigned backColor = bc;\
    static const unsigned textColor = tc;\
	static const char *mess(){return _cat(txt, __##tpe##_1);}\
};\
template<>struct IDtoMess<name::ID>{typedef name Result;};

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

	MESS(InfoCycle		          , void  , "ВНИМАТЕЛЬНО ПРОВЕРЬ ПОЛОЖЕНИЕ ШТАНГИ В ЗАХВАТАХ!"	, red  , yellow)
	MESS(ExitMeshuringCycle, void, "Оператор вышел из цикла", red, yellow)
	MESS(AlarmExitControlCircuitBitIn , void, "Нет сигнала \"Цепи управления\""	   , red  , yellow)
	MESS(AlarmExitControlCycleBitIn , void, "Нет сигнала \"Цикл\""	   , red  , yellow)

	MESS(TimeOverlappedDefectoscope , void, "Превышено время сбора сигнала \"ДЕФЕКТОСКОПИЯ\" "	   , red  , yellow)
	MESS(TimeOverlappedStructure , void, "Превышено время сбора сигнала \"СТРУКТУРА\" "	   , red  , yellow)

	MESS(outputsAC_ON, bool, "вход \"AC_ON\"", red, yellow)
	MESS(outputsDC_ON2, bool, "вход \"DC_ON2\"", red, yellow)	
	MESS(outputsDC_ON1, bool, "вход \"DC_ON1\"", red, yellow)

	MESS(WaitingForControlCircuitActivation, void, "проверка сигнала \"ЦЕПИ УПРАВЛЕНИЯ\"", blue, white)
	MESS(WaitingForTheCycleSignal, void, "ожидание сигнала \"ЦИКЛ\"", blue, white)
	MESS(WaitingForTheCortSignal, void, "ожидание сигнала \"СОРТ\"", blue, white)
	MESS(WaitingForTheCortP1P2SignalToTurnOff, void, "ожидание отключения сигналов \"СОРТ\"\"П1\"\"П2\"", blue, white)
	MESS(WaitingForTheControlSignal, void, "ожидание сигнала \"КОНТРОЛЬ\"", blue, white)
	MESS(WaitingForTheP1Signal, void, "ожидание сигнала \"П1\"", blue, white)
	MESS(WaitingForTheP1SignalTurnOff, void, "ожидание отключения сигнала \"П1\"", blue, white)
	MESS(DataCollectionDEF, void, "сбор данных \"ДЕФЕКТОСКОП\"", blue, white)

	MESS(WaitingForTheP2Signal, void, "ожидание сигнала \"П2\"", blue, white)
	MESS(WaitingForTheP2SignalTurnOff, void, "ожидание отключения сигнала \"П2\"", blue, white)
	MESS(DataCollectionSTR, void, "сбор данных \"СТРУКТУРА\"", blue, white)

	MESS(Alarm502, int, "Ошибка платы L502  ", red  , yellow)

	MESS(DataCollectionCompleted, void, "Цикл сбора данных закончен", green, white)

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


