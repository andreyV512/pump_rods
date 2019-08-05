#pragma once
#include "LogMessages.h"

class Log
{
public:
	static void Insert(int id, double val, bool);
private:
	template<class T>struct Param{typedef T Result;};
	template<>struct Param<void>{typedef int Result;};

	template<class T>static double Convert(T t){return (double)t;}
public:	
	struct TData
	{
		unsigned id;
		unsigned time;
		double value;
	};
#pragma warning(disable: 4101)
	static LONG lastMessage;
	//template<LogMess::ID N>static void Mess(typename the_index_variable_is_out_of_range_look_at_N<typename LogMess::Row<N>::type>::Result value = 0)
	//template<int N>struct Filter{static const bool value = true;};
	//template<int N>static void Mess(typename the_index_variable_is_out_of_range_look_at_N<typename IDtoMessN<N>::Result value = 0)
	//{	
	//	Insert(N, Convert(value), Filter<N>::value);
	//}
	template<class T>struct Filter{static const bool value = true;};
	template<class T>static void Mess(typename Param<typename T::type>::Result value = 0)
	{
		Insert(T::ID, Convert(value), Filter<T>::value);
	}
#pragma warning(default: 4101)
	static bool IsRow(int , TData *&);
	static int LastMessageIndex();
	static bool LastMessage(TData *&d);
	static void TailMessage(TData *&d);
};

#define DROP_MESSAGE(N)template<>struct Log::Filter<N>{static const bool value = false;};
DROP_MESSAGE(LogMess::ProgramOpen)

#define DROP_ID_PARAMS iCU, iKM2_DC, iKM3_AC, iCycle, iP1, iP2, iCOPT, iControl, oDC_ON1, oAC_ON, oDC_ON2, oWork, oStart, oToShift, oC1, oC2

#define DROP_ON(n) DROP_MESSAGE(MessBit<On<n>>)
FOR_EACH(DROP_ON, DROP_ID_PARAMS)
#undef DROP_ON

#define DROP_OFF(n) DROP_MESSAGE(MessBit<Off<n>>)
FOR_EACH(DROP_OFF, DROP_ID_PARAMS)
#undef DROP_OFF

#undef DROP_MESSAGE
