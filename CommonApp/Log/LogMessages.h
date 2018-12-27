#pragma once
#include "templates\typelist.hpp"
#include "App\App.h"
#include "App\AppBase.h"

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

//	MESS(WaitingForControlCircuitActivation, void, "проверка сигнала \"ЦЕПИ УПРАВЛЕНИЯ\"", blue, white)
////	MESS(WaitingForTheCycleSignal, void, "ожидание сигнала \"ЦИКЛ\"", blue, white)
//	MESS(WaitingForTheCortSignal, void, "ожидание сигнала \"СОРТ\"", blue, white)
//	MESS(WaitingForTheCortP1P2SignalToTurnOff, void, "ожидание отключения сигналов \"СОРТ\"\"П1\"\"П2\"", blue, white)
	//MESS(WaitingForTheControlSignal, void, "ожидание сигнала \"КОНТРОЛЬ\"", blue, white)
//	MESS(WaitingForTheP1Signal, void, "ожидание сигнала \"П1\"", blue, white)
	//MESS(WaitingForTheP1SignalTurnOff, void, "ожидание отключения сигнала \"П1\"", blue, white)
	MESS(DataCollectionDEF, void, "сбор данных \"ДЕФЕКТОСКОП\"", blue, white)

//	MESS(WaitingForTheP2Signal, void, "ожидание сигнала \"П2\"", blue, white)
//	MESS(WaitingForTheP2SignalTurnOff, void, "ожидание отключения сигнала \"П2\"", blue, white)
	MESS(DataCollectionSTR, void, "сбор данных \"СТРУКТУРА\"", blue, white)

	MESS(Alarm502, int, "Ошибка платы L502  ", red  , yellow)

	MESS(DataCollectionCompleted, void, "Цикл сбора данных закончен", green, white)

	template<class T>struct Bits;
	//{
	//	static const int ID = 99;//__COUNTER__;
	//	typedef int type;
	//	static const char *mess(){return L"xx";}
	//};
	//template<class T>struct IDtoMess<Bits<T>::ID>
	//{
	//		typedef typename Bits<T> Result;
	//};
	template<>struct Bits<NullType>{};
	//template<class On>struct Bits<On, NullType>{};
	//template<class Off>struct Bits<NullType, Off>{};
	/*
	#define MESS1(name, tpe, txt, bc, tc)struct name\
{\
	static const int ID = __COUNTER__;\
	typedef tpe type;\
	static const unsigned backColor = bc;\
    static const unsigned textColor = tc;\
	static const char *mess(){return _cat(txt, __##tpe##_1);}\
};\
template<>struct IDtoMess<name::ID>{typedef name Result;};
	*/
//	template<>struct Bits<Tlst<Tlst<On<iСU>, Tlst<On<iCycle>, NullType>>, Tlst<Off<iP2>, /N/ullType>>>
//	{
//		static const int ID = __COUNTER__;
//		typedef int type;
//		static const unsigned backColor = white;
//		static const unsigned textColor = white;
//		static const char *mess(){return "";}
//	};
//	
//	template<>struct IDtoMess<Bits<Tlst<Tlst<On<iСU>, Tlst<On<iCycle>, NullType>>, /T/lst<Off<iP2>, NullType>>>::ID>
//	{
//			typedef Bits<Tlst<Tlst<On<iСU>, Tlst<On<iCycle>, NullType>>, Tlst<Off<iP2>, /N/ullType>>> Result;
//	};

#define BITS_ON(txt, ...)template<>struct Bits<TL::MkTlst<__VA_ARGS__>::Result>\
	{\
		static const int ID = __COUNTER__;\
		typedef int type;\
		static const unsigned backColor = white;\
		static const unsigned textColor = white;\
		static const char *mess(){return txt;}\
	};\
	template<>struct IDtoMess<Bits<TL::MkTlst<__VA_ARGS__>::Result>::ID>\
	{\
		typedef Bits<TL::MkTlst<__VA_ARGS__>::Result> Result;\
	};

	BITS_ON("<ff>Ожидание сигнала \"ЦЕПИ УПРАВЛЕНИЯ\"", On<iСU>)
	BITS_ON("<ff>Ожидание сигнала \"ЦИКЛ\"", On<iCycle>)
	BITS_ON("<ff>Ожидание сигнала \"СОРТ\"", On<iCOPT>)
	BITS_ON("<ff>Ожидание сигнала \"КОНТРОЛЬ\"", On<iControl>)
	BITS_ON("<ff>Ожидание сигналов: \"КОНТРОЛЬ\",\"П1\"",  On<iControl>, On<iP1>)
	BITS_ON("<ff>Ожидание сигналов: \"КОНТРОЛЬ\",\"П2\"",  On<iControl>, On<iP2>)

	BITS_ON("<ff>Ожидание отключения сигналов: \"СОРТ\",\"П1\",\"П2\"", Off<iCOPT>, Off<iP1>, Off<iP2>)
	BITS_ON("<ff>Ожидание отключения сигнала \"П1\"", Off<iP1>)
	BITS_ON("<ff>Ожидание отключения сигнала \"П2\"", Off<iP2>)
	BITS_ON("<ff>Ожидание отключения сигнала \"КОНТРОЛЬ\"", Off<iControl>)
#undef BITS_ON
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


