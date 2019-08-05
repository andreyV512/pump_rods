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

template<class T>struct MessBit;

#define MESS_BIT(name, tpe, txt, bc, tc)template<>struct MessBit<name>\
{\
	static const int ID = __COUNTER__;\
	typedef tpe type;\
	static const unsigned backColor = bc;\
    static const unsigned textColor = tc;\
	static const char *mess(){return _cat(txt, __##tpe);}\
};\
template<>struct IDtoMess<MessBit<name>::ID>{typedef MessBit<name> Result;};

	static const int green = 0xff00;
	static const int blue  = 0xff0000;
	static const int black = 0x0;
	static const int white = 0xffffff;
	static const int red = 0xff;
	static const int yellow = 0xffff;
	static const int sea = 0xffa279;
	static const int pink = 0xaBa0FF;
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

	MESS(DataCollectionDEF, void, "сбор данных \"ДЕФЕКТОСКОП\"", blue, white)
	MESS(DataCollectionSTR, void, "сбор данных \"СТРУКТУРА\"", blue, white)

	MESS(Alarm502, int, "Ошибка платы L502  ", red  , yellow)

	MESS(DataCollectionCompleted, void, "Цикл сбора данных закончен", green, white)

	MESS(Copt, int, "Сорт", blue, white)
	MESS(Brak, void, "Брак", red  , yellow)

	MESS(Off_iP1_Off_iP2, void, "Сигналы отключены: \"П1\",\"П2\"", blue, white)
	MESS(Off_iP1_On_iP2 , void, "Сигнал отключён\"П1\", включён\"П2\"", blue, white)
	MESS(On_iP1_Off_iP2 , void, "Сигнал включен\"П1\", отключён\"П2\"", blue, white)
	MESS(On_iP1_On_iP2  , void, "Сигналы включены: \"П1\",\"П2\"", blue, white)

	MESS_BIT(On<iCU    	>, void, "Включен сигнал \"Цепи управления\"", red, white);
	MESS_BIT(On<iKM2_DC >, void, "Включен сигнал \"Пускатель (КМ2 DC)\"", red, white);
	MESS_BIT(On<iKM3_AC >, void, "Включен сигнал \"Пускатель (КМ3 AC)\"", red, white);
	MESS_BIT(On<iCycle 	>, void, "Включен сигнал \"ЦИКЛ\"", red, white);
	MESS_BIT(On<iP1     >, void, "Включен сигнал \"транспортной системы (П1)\"", red, white);
	MESS_BIT(On<iP2     >, void, "Включен сигнал \"транспортной системы (П2)\"", red, white);
	MESS_BIT(On<iCOPT   >, void, "Включен сигнал \"СОРТ\"", red, white);
	MESS_BIT(On<iControl>, void, "Включен сигнал \"КОНТРОЛЬ\"", red, white);

	MESS_BIT(On<oDC_ON1 >, void, "Включен сигнал \"DC ON1\"", red, white); 
	MESS_BIT(On<oAC_ON  >, void, "Включен сигнал \"AC ON\"", red, white); 
	MESS_BIT(On<oDC_ON2 >, void, "Включен сигнал \"DC ON2\"", red, white); 
	MESS_BIT(On<oWork   >, void, "Включен сигнал \"РАБОТА\"", red, white); 
	MESS_BIT(On<oStart  >, void, "Включен сигнал \"ПУСК\"", red, white); 
	MESS_BIT(On<oToShift>, void, "Включен сигнал \"РЕЗУЛЬТАТ\"", red, white); 
	MESS_BIT(On<oC1     >, void, "Включен сигнал \"С1\"", red, white); 
	MESS_BIT(On<oC2  	>, void, "Включен сигнал \"С2\"", red, white); 

	MESS_BIT(Off<iCU    	>, void, "Отключен сигнал \"Цепи управления\"", blue, white);
	MESS_BIT(Off<iKM2_DC >, void, "Отключен сигнал \"Пускатель (КМ2 DC)\"", blue, white);
	MESS_BIT(Off<iKM3_AC >, void, "Отключен сигнал \"Пускатель (КМ3 AC)\"", blue, white);
	MESS_BIT(Off<iCycle 	>, void, "Отключен сигнал \"ЦИКЛ\"", blue, white);
	MESS_BIT(Off<iP1     >, void, "Отключен сигнал \"транспортной системы (П1)\"", blue, white);
	MESS_BIT(Off<iP2     >, void, "Отключен сигнал \"транспортной системы (П2)\"", blue, white);
	MESS_BIT(Off<iCOPT   >, void, "Отключен сигнал \"СОРТ\"", blue, white);
	MESS_BIT(Off<iControl>, void, "Отключен сигнал \"КОНТРОЛЬ\"", blue, white);
				 
	MESS_BIT(Off<oDC_ON1 >, void, "Отключен сигнал \"DC ON1\"", blue, white); 
	MESS_BIT(Off<oAC_ON  >, void, "Отключен сигнал \"AC ON\"", blue, white); 
	MESS_BIT(Off<oDC_ON2 >, void, "Отключен сигнал \"DC ON2\"", blue, white); 
	MESS_BIT(Off<oWork   >, void, "Отключен сигнал \"РАБОТА\"", blue, white); 
	MESS_BIT(Off<oStart  >, void, "Отключен сигнал \"ПУСК\"", blue, white); 
	MESS_BIT(Off<oToShift>, void, "Отключен сигнал \"РЕЗУЛЬТАТ\"", blue, white); 
	MESS_BIT(Off<oC1     >, void, "Отключен сигнал \"С1\"", blue, white); 
	MESS_BIT(Off<oC2  	>, void, "Отключен сигнал \"С2\"", blue, white); 

	template<class T>struct Bits;
	template<>struct Bits<NullType>{};
#define BITS_XX(color, txt, ...)template<>struct Bits<TL::MkTlst<__VA_ARGS__>::Result>\
	{\
		static const int ID = __COUNTER__;\
		typedef int type;\
		static const unsigned backColor = color;\
		static const unsigned textColor = black;\
		static const char *mess(){zprint(" mess\n");return txt;}\
	};\
	template<>struct IDtoMess<Bits<TL::MkTlst<__VA_ARGS__>::Result>::ID>\
	{\
		typedef Bits<TL::MkTlst<__VA_ARGS__>::Result> Result;\
	};

	BITS_XX(sea, "Ожидание сигнала \"ЦЕПИ УПРАВЛЕНИЯ\"", On<iCU>)
	BITS_XX(pink, "Снят сигнал \"ЦЕПИ УПРАВЛЕНИЯ\"", Off<iCU>)

	BITS_XX(sea, "Ожидание сигналов: \"ЦЕПИ УПРАВЛЕНИЯ\",\"ЦИКЛ\"", On<iCU>, On<iCycle>)

	BITS_XX(sea, "Ожидание сигнала \"ЦИКЛ\"", On<iCycle>)
	BITS_XX(sea, "Снят сигнал \"ЦИКЛ\"", Off<iCycle>)

	BITS_XX(sea, "Ожидание сигнала \"СОРТ\"", On<iCOPT>)
	BITS_XX(sea, "Ожидание сигнала \"КОНТРОЛЬ\"", On<iControl>)
	BITS_XX(sea, "Ожидание сигналов: \"КОНТРОЛЬ\",\"П1\"",  On<iControl>, On<iP1>)
	BITS_XX(sea, "Ожидание сигналов: \"КОНТРОЛЬ\",\"П2\"",  On<iControl>, On<iP2>)
	BITS_XX(sea, "Ожидание отключения сигналов: \"СОРТ\",\"П1\",\"П2\"", Off<iCOPT>, Off<iP1>, Off<iP2>)
	BITS_XX(sea, "Ожидание отключения сигнала \"П1\"", Off<iP1>)
	BITS_XX(pink, "Выставлен сигнал \"П1\"", On<iP1>)

	BITS_XX(sea, "Ожидание отключения сигнала \"П2\"", Off<iP2>)
	BITS_XX(pink, "Выставлен сигнал \"П2\"", On<iP2>)

	BITS_XX(sea, "Ожидание отключения сигнала \"КОНТРОЛЬ\"", Off<iControl>)

	BITS_XX(pink, "Выставлен сигнал \"РАБОТА\"", On<oWork>)
	BITS_XX(pink, "Выставлен сигнал \"DC ON1\"", On<oDC_ON1>)
	BITS_XX(pink, "Выставлен сигнал \"DC ON2\"", On<oDC_ON2>)
	BITS_XX(pink, "Снят сигнал \"DC ON2\"", Off<oDC_ON2>)
	BITS_XX(pink, "Снят сигнал \"DC ON1\"", Off<oDC_ON1>)
			 
	BITS_XX(pink, "Выставлен сигнал \"AC ON\"", On<oAC_ON>)
	BITS_XX(pink, "Снят сигнал \"AC ON\"", Off<oAC_ON>)
			
	BITS_XX(pink, "Выставлен сигнал \"Пуск\"", On<oStart>)
			
	BITS_XX(pink, "Сняты сигналы \"AC ON\"\"ПУСК\"", Off<oAC_ON>, Off<oStart>)
	BITS_XX(pink, "Снят сигнал \"ПУСК\"", Off<oStart>)
	//BITS_XX(pink, "Сняты сигналы \"C1\"\"C2\"", Off<oC1>, Off<oC2>)
			
	BITS_XX(pink, "Выставлен сигнал \"C1\"", On<oC1>)
	BITS_XX(pink, "Выставлен сигнал \"C2\"", On<oC2>)

	BITS_XX(sea, "Снят сигнал \"C1\"", Off<oC1>)
	BITS_XX(sea, "Снят сигнал \"C2\"", Off<oC2>)

	BITS_XX(pink, "Выставлен сигнал \"Результат\"", On<oToShift>)

	BITS_XX(pink, "Проверка включения \"KM2 DC\"", On<iKM2_DC>)
	BITS_XX(pink, "Проверка включения \"KM3 AC\"", On<iKM3_AC>)
				 
	BITS_XX(pink, "Проверка отключения \"KM2 DC\"", Off<iKM2_DC>)
	BITS_XX(pink, "Проверка отключения \"KM3 AC\"", Off<iKM3_AC>)
	BITS_XX(pink, "Проверка отключения \"КМ DC и KM3 AC\"",  Off<iKM2_DC>, Off<iKM3_AC>)
	BITS_XX(pink, "Отключение \"Результат, С1 и С2\"", Off<oToShift>, Off<oC1>, Off<oC2>)

	
					
#undef BITS_XX


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


