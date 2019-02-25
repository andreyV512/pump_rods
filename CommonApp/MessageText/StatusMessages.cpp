#include "StatusMessages.h"
#include "App/MessageItems.h"
#include "templates\sort.hpp"
#include "App\AppBase.h"
#include "MessageText\ListMess.hpp"
namespace StatusMessages
{
	template<class T>struct Txt;
	template<>struct Txt<DeathZone			  >{wchar_t *operator()(){return L"\"мёртвая зона\"";}};
	template<>struct Txt<Nominal			  >{wchar_t *operator()(){return L"\"норма\"";}};
	template<>struct Txt<SensorOff			  >{wchar_t *operator()(){return L"\"датчик отключён\"";}};

	template<>struct Txt< DefectSig<Defect>  >{wchar_t *operator()(){return L"\"дефект дефектоскопии\"";}};
	template<>struct Txt< StructSig<Defect>  >{wchar_t *operator()(){return L"\"дефект структуры\"";}};

	template<>struct Txt< DefectSig<SortDown>  >{wchar_t *operator()(){return L"\"сорт дефектоскопии\"";}};
	template<>struct Txt< StructSig<SortDown>  >{wchar_t *operator()(){return L"\"сорт структуры\"";}};

	struct __arr_proc_data__
	{
		wchar_t *buf;
		int color;
	};

	template<class O, class P>struct __arr_proc__
	{
		void operator()(P &p)
		{
			wcscpy(p.buf, Txt<O>()());
			p.buf = &p.buf[wcslen(p.buf)];
		}
	};

	template<class T>struct __skip_clr_wapper__
	{
		typedef T Result;
	};

	template<template<class>class W, class T>struct __skip_clr_wapper__<W<T>>
	{
		typedef T Result;
	};

	template<class T>struct ArrProc
	{
		static void Do(wchar_t *buf, unsigned &color)
		{
			if(NULL != buf)
			{
				__arr_proc_data__ data = {buf};
				TL::foreach<T, __arr_proc__>()(data);
			}
			color = Singleton<ColorTable>::Instance().items.get<Clr<typename __skip_clr_wapper__<typename T::Head>::Result>>().value;
		}
	};

	typedef void(*PtrMess)(wchar_t *, unsigned &);
	struct IndBits
	{
		int id;
		int bits;
		bool operator >(IndBits &val){return bits > val.bits;}		
	};

	PtrMess ptrMess[TL::Length<lst_lst::num_list>::value];
	IndBits bits[TL::Length<lst_lst::num_list>::value];

	template<class List>struct InitArrayProc;
	template<class Head, class Tail>struct InitArrayProc<Tlst<Head, Tail>>: InitArrayProc<Tail>
	{
		InitArrayProc(): InitArrayProc<Tail>()
		{
			static const int i = TL::IndexOf<lst_lst::num_list, Head>::value;
			ptrMess[i] = ArrProc<typename TL::TypeAt<lst_lst::lst_list, i>::Result>::Do;
			IndBits t = {i, Head::value};
			bits[i] = t;
		}
	};

	template<>struct InitArrayProc<NullType>{};

	InitArrayProc<lst_lst::num_list> initArrayProc;

	
	struct IdBits
	{
		IdBits()
		{
			QuickSort(bits, dimention_of(bits));
		}
		unsigned operator()(int res)
		{
			unsigned mid = (dimention_of(bits) - 1) / 2;
			unsigned start = 0;
			unsigned stop = dimention_of(bits) - 1;
			IndBits *xxx = bits; 
			while(stop != start)
			{
				if(bits[mid].bits > res)
				{
					stop = mid;
					mid = (stop + start) / 2;
				}
				else if(bits[mid].bits < res)
				{
					start = mid;
					mid = (stop + start) / 2;
				}				
				if(bits[mid].bits == res)
				{
					break;
				}
				if(stop - start <= 1)
				{
					bool b = false;
					if(b =(bits[stop].bits == res)) mid = stop;
					else if(b = (bits[start].bits == res)) mid = start;
					break;
				}
			}
			return bits[mid].id;
		}
	};

	IdBits idBits;


	bool Message(int id, wchar_t *mess, unsigned &color)
	{
		mess[0] = 0;
		if(id < dimention_of(ptrMess))
		{
			ptrMess[id](mess, color);
			return true;
		}
		return false;
	}

	template<class List = zone_status_list>struct BrakBits;
	template<class Head, class Tail>struct BrakBits<Tlst<Head, Tail>>
	{
		static const unsigned value = BrakBits<Tail>::value;
	};
	template<template<class>class Head, class Tail>struct BrakBits<Tlst<Head<Defect>, Tail>>
	{
		static const unsigned value = (1 << TL::IndexOf<zone_status_list, Head<Defect>>::value) | BrakBits<Tail>::value;
	};
	template<>struct BrakBits<NullType>
	{
		static const unsigned value = 0;
	};

	static const int brak_bits = BrakBits<>::value;
}

template<class O, class P>struct __skip__
{
	void operator()(P &p)
	{
		static unsigned skipBit = 1 << TL::IndexOf<StatusMessages::lst_lst::skip_list, O>::value;
		if(0 != (skipBit & p))
		{
			if(0 != (p & O::value))p &=~skipBit;
		}
	}
};

int ResultMessageId(unsigned *x)
{
	unsigned res = 0;
	while(-1 != *x)
	{
		if(*x < dimention_of(StatusMessages::bits)) res |= StatusMessages::bits[*x].bits;
		x = &x[1];
	}
	TL::foreach<StatusMessages::lst_lst::skip_list, __skip__>()(res);
	return StatusMessages::idBits(res);
}

void StatusText::FromSensors(unsigned *sens, unsigned &color, wchar_t *buf)
	{
		
		buf[0] = 0;
		unsigned id = ResultMessageId(sens);

		if(id < dimention_of(StatusMessages::ptrMess))
		{
			StatusMessages::ptrMess[id](buf, color);
		}
	}

unsigned StatusColor::operator()(unsigned id)
	{
		 unsigned color = 0;
		if(id < dimention_of(StatusMessages::bits))StatusMessages::ptrMess[id](NULL, color);
		return color;
	}

void StatusText::operator()(int id, unsigned &color, wchar_t *buf)
	{
		buf[0] = 0;

		if(id < dimention_of(StatusMessages::ptrMess))
		{
			StatusMessages::ptrMess[id](buf, color);
		}
	}


bool IsDefect(unsigned id)
{
	if(id < dimention_of(StatusMessages::bits))
	{
	   return 0 != (StatusMessages::brak_bits & StatusMessages::bits[id].bits);
	}
	return false;
}

namespace StatusMessages
{
	template<class List>struct __klass__;
	template<class Head, class Tail>struct __klass__<Tlst<Head, Tail>>
	{
		static const unsigned value = __klass__<Tail>::value;
	};
	template<>struct __klass__<NullType>
	{
		static const unsigned value = 0;
	};
}

unsigned StatBits(unsigned id)
{
	return StatusMessages::bits[id].bits;
}

namespace StatusMessages
{
	template<class T>struct __inner_tytpe__
	{
		typedef T Result;
	};
	template<template<class>class W, class T>struct __inner_tytpe__<W<T>>
	{
		typedef T Result;
	};
	struct __status_zone_data__
	{
		int id;
		int color;
		wchar_t *txt;
	};
	template<class O, class P>struct __status_zone__
	{
		bool operator()(P &p)
		{
			if(TL::IndexOf<zone_status_list, O>::value == p.id)
			{
				p.txt = Txt<O>()();
				p.color = Singleton<ColorTable>::Instance().items.get<Clr<typename __inner_tytpe__<O>::Result>>().value;
				return false;
			}
			return true;
		}
	};
}

