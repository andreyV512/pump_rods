#include "LogMessages.h"
#include <Windows.h>
#include "templates/typelist.hpp"
#include <stdio.h>
#include "PerformanceCounter\PerformanceCounter.h"

#pragma warning(disable : 4996)

namespace LogMess
{
	struct Loc
	{
		unsigned backColor, textColor;
		void(*proc)(char *, double);
	};
	template<class T, class C>struct GenFunc;
	template<class C>struct GenFunc<void, C>
	{
		static void Func(char *buf, double)
		{
			sprintf(buf, C::mess());
		}
	};
	
	template<class C>struct GenFunc<int, C>
	{
		static void Func(char *buf, double val)
		{
			sprintf(buf, C::mess(), (int)val);			
		}
	};
	template<class C>struct GenFunc<double, C>
	{
		static void Func(char *buf, double val)
		{
			sprintf(buf, C::mess(), val);			
		}
	};
	template<class C>struct GenFunc<bool, C>
	{
		static void Func(char *buf, double val)
		{
			sprintf(buf, C::mess(), 0.0 != val ? "¬ À" : "Œ“ À");			
		}
	};
	template<class C>struct GenFunc<const char *, C>
	{
		static void Func(char *buf, double val)
		{
			sprintf(buf, C::mess(), (char *)*(unsigned *)&val);			
		}
	};
	
	template<int i = 0, int = MAX_MESS_ID>struct Item: Item<i + 1, MAX_MESS_ID>
	{
		typedef typename IDtoMess<i>::Result O;
		Item()
		{
			arr[i].backColor = O::backColor;
			arr[i].textColor = O::textColor;
			arr[i].proc = GenFunc<O::type, O >::Func;
		}
	};
	template<>struct Item<MAX_MESS_ID, MAX_MESS_ID>
	{
		Loc arr[MAX_MESS_ID];
	};
	struct FactoryMessages::Inner
	{
		unsigned start;
		Item<> items;
		static Inner &Instance(){static Inner x; return x;}
	};
	FactoryMessages::FactoryMessages()
		: inner(Inner::Instance())
	{
	}
	void FactoryMessages::StartTime()
	{
		inner.start = Performance::Counter();
	}
	bool FactoryMessages::Color(int i, unsigned &backColor, unsigned &textColor)
	{
		if(i < MAX_MESS_ID) 
		{
			backColor = inner.items.arr[i].backColor;
			textColor = inner.items.arr[i].textColor;
			return true;
		}
		return false;
	}
	bool FactoryMessages::Text(int i, char *buf, double val)
	{
		if(i < MAX_MESS_ID) 
		{
			inner.items.arr[i].proc(buf, val);
			return true;
		}
		return false;
	}
	FactoryMessages &FactoryMessages::Instance()
	{
		static FactoryMessages x; return x;
	}
}
