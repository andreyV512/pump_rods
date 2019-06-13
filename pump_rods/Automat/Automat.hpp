#pragma once

namespace Automat
{
	template<class>struct Inv{};
	template<class>struct Proc{};
	template<class>struct Once{};
	template<class T>struct Ex;
	template<int N>struct Key{static const int value = N;};

	template<class List, template<class>class T>struct Filt;
	template<class Head, class Tail, template<class>class T>struct Filt<Tlst<Head, Tail>, T>
	{
		typedef typename Filt<Tail, T>::Result Result;
	};
	template<class Head, class Tail, template<class>class T>struct Filt<Tlst<T<Head>, Tail>, T>
	{
		typedef Tlst<Head, typename Filt<Tail, T>::Result> Result;
	};
	template<template<class>class T>struct Filt<NullType, T>
	{
		typedef NullType Result;
	};

	template<class List, template<int>class T>struct FiltKey;
	template<class Head, class Tail, template<int>class T>struct FiltKey<Tlst<Head, Tail>, T>
	{
		typedef typename FiltKey<Tail, T>::Result Result;
	};
	template<int N, class Tail, template<int>class T>struct FiltKey<Tlst<T<N>, Tail>, T>
	{
		typedef Tlst<T<N>, typename FiltKey<Tail, T>::Result> Result;
	};
	template<template<class>class T>struct FiltKey<NullType, T>
	{
		typedef NullType Result;
	};

	template<class bit0, class bit1 = NullType, class bit2 = NullType, class bit3 = NullType, class bit4 = NullType>struct Test;

	template<class List>struct FiltTestBits;
	template<class Head, class Tail>struct FiltTestBits<Tlst<Head, Tail>>
	{
		typedef typename FiltTestBits<Tail>::Result Result;
	};
	template<>struct FiltTestBits<NullType>
	{
		typedef NullType Result;
	};

	template<class bit0, class bit1, class bit2, class bit3, class bit4, class Tail>struct FiltTestBits<Tlst<Test<bit0, bit1, bit2, bit3, bit4>, Tail>>
	{
		typedef typename TL::MkTlst<bit0, bit1, bit2, bit3, bit4>::Result Result;
	};

	template<class O, class P>struct __test_bits_do__;
	template<class O, class P>struct __test_bits_do__<On<O>, P>
	{
		bool operator()(P &p)
		{
			if(!(p.bits & p.inputs_bits.get<O>().value))
			{
				Log::Mess<LogMess::Bits<typename TL::MkTlst<On<O>>::Result>>();
				return false;
			}
			return true;
		}
	};
	template<class O, class P>struct __test_bits_do__<Off<O>, P>
	{
		bool operator()(P &p)
		{
			if(!(p.bits & p.inputs_bits.get<O>().value))
			{
				Log::Mess<LogMess::Bits<typename TL::MkTlst<Off<O>>::Result>>();
				return false;
			}
			return true;
		}
	};
	
	template<class List>struct TestBitsDo
	{
		template<class T>bool operator()(T &t)
		{
			return TL::find<List, __test_bits_do__>()(t);
		}
	};

	template<>struct TestBitsDo<NullType>{template<class T>bool operator()(T &){return true;}};

	struct __test_output_bits_data__
	{		
		OutputBitTable::TItems &offs;
		unsigned bits;
		 __test_output_bits_data__()
			 : offs(Singleton<OutputBitTable>::Instance().items)
			 , bits(device1730.ReadOutput())
		 {}
	};
	template<class O, class P>struct __test_output_bits__;
	template<class O, class P>struct __test_output_bits__<On<O>, P>
	{
		bool operator()(P &p)
		{
			if(0 == (p.bits & (1 << p.offs.get<O>().value)))
			{
				Log::Mess<MessID<On<O>>::Result>();
				return false;
			}
			return true;
		}
	};
	template<class O, class P>struct __test_output_bits__<Off<O>, P>
	{
		bool operator()(P &p)
		{
			if(0 == (p.bits & (1 << p.offs.get<O>().value)))
			{
				Log::Mess<MessID<O>::Result>();
				return false;
			}
			return true;
		}
	};

	template<class A, class B>struct __all_lists_not_empty__
	{
		static const bool value = true;
	};
	template<>struct __all_lists_not_empty__<NullType, NullType>
	{
		static const bool value = false;
	};

	template<class A>struct __list_not_empty__
	{
		static const bool value = true;
	};
	template<>struct __list_not_empty__<NullType>
	{
		static const bool value = false;
	};

	template<class O, class P>struct __bits_0__
	{
		void operator()(O *o, P *p)
		{
			*p |= o->value;
		}
	};

	template<class List, template<class, class>class b>struct __sel_bits__
	{
		template<class O, class P>void operator()(O *o, P *p)
		{
			TL::foreach<List, b>()(o, p);
		}
	};
	template<template<class, class>class b>struct __sel_bits__<NullType, b>
	{
		template<class O, class P>void operator()(O *o, P *p)
		{
		}
	};

	template<class List, class TList>struct __filtr__;
	template<class Head, class Tail, class TList>struct __filtr__<Tlst<Head, Tail>, TList>
	{
		typedef typename TL::_if<
			TL::TypeInList<TList, Head>::value
			, Tlst<Head, typename __filtr__<Tail, TList>::Result>
			, typename __filtr__<Tail, TList>::Result
		>::Result Result;
	};

	template<class TList>struct __filtr__<NullType, TList>
	{
		typedef NullType Result;
	};

	template<class List>struct SelectBits
	{
		void operator()(unsigned &bits)
		{
			bits = 0;
			__sel_bits__<typename __filtr__<List, InputBitTable::items_list>::Result, __bits_0__>()
				(&Singleton<InputBitTable>::Instance().items, &bits);
		}
	};
	template<>struct SelectBits<NullType>
	{
		void operator()(unsigned &bits){}
	};

	template<class List>struct SelectOutBits
	{
		void operator()(unsigned &bits)
		{
			bits = 0;
			__sel_bits__<typename __filtr__<List, OutputBitTable::items_list>::Result, __bits_0__>()
				(&Singleton<OutputBitTable>::Instance().items, &bits);
		}
	};
	template<>struct SelectOutBits<NullType>
	{
		void operator()(unsigned &bits){}
	};

	template<class O, class P>struct __default_do__
	{
		void operator()(P &p)
		{
			O::Do(p);
		}
	};

	template<class List>struct DefaultDo
	{
		template<class Result>unsigned operator()(Result &result)
		{
			TL::foreach<List, __default_do__>()(result);
			return result.ret;
		}
	};
	template<>struct DefaultDo<NullType>
	{
		template<class Result>unsigned operator()(Result &){return 0;}
	};
	template<class O, class P>struct __button_down__
	{
		bool operator()(P &p)
		{
			return O::value != p;
		}
	};

	template<class List>struct KeyDo
	{
		bool operator()(unsigned bits0)
		{
			return TL::find<List, __button_down__>()(bits0);
		}
	};
	template<>struct KeyDo<NullType>
	{
		bool operator()(unsigned){return true;}
	};

	template<class List, class Result, unsigned N>struct AND_Bits_WAIT_TIMEOUT
	{
		unsigned operator()(Result &result, unsigned delay)
		{
			if(delay < GetTickCount()) 
			{
				if(!TestBitsDo<TL::MultyListToList<
					List
					>::Result>()(result))
					return Status::alarm_bits;
				return Status::time_out; 
			}
			return 0;
		}
	};
	template<class List, class Result>struct AND_Bits_WAIT_TIMEOUT<List, Result, -1>
	{
		unsigned operator()(Result &, unsigned){return 0;}
	};

	template<class List>struct WaitFor
	{
		static const int count = TL::Length<List>::value;
		HANDLE h[count];
		template<class O, class P>struct __init__
		{
			void operator()(P *p)
			{
				p[TL::IndexOf<List, O>::value] = O::hEvent;
			}
		};
		unsigned operator()()
		{
			TL::foreach<List, __init__>()(h);
			return WaitForMultipleObjects(count, h, FALSE, 5);
		}
	};

	template<>struct WaitFor<NullType>
	{
		unsigned operator()()
		{
			return WaitForSingleObject(hEvent, 5);
		}
	};

	template<class List>struct __key_handler__
	{
		int ret;
		unsigned key;
		template<class O, class P>struct __loc__
		{
			bool operator()(P &p)
			{
				if(TL::IndexOf<List, O>::value == p.key)
				{
					p.ret = O::value == Status::stop ? Status::stop: 0; 
					return false;
				}
				return true;
			}
		};
		int operator()(unsigned key_)
		{
			key = key_;
			TL::find<List, __loc__>()(*this);
			return ret;
		}
	};

	template<>struct __key_handler__<NullType>
	{
		int operator()(unsigned key_)
		{
			return -1;
		}
	};

	template<class List>struct __mess_bits__;
	template<class Head, class Tail>struct __mess_bits__<Tlst<Head, Tail>>
	{
		typedef typename __mess_bits__<Tail>::Result Result;
	};
	template<class Head, class Tail>struct __mess_bits__<Tlst<On<Head>, Tail>>
	{
		typedef Tlst<On<Head>, typename __mess_bits__<Tail>::Result> Result;
	};
	template<class Head, class Tail>struct __mess_bits__<Tlst<Off<Head>, Tail>>
	{
		typedef Tlst<Off<Head>, typename __mess_bits__<Tail>::Result> Result;
	};
	template<>struct __mess_bits__<NullType>
	{
		typedef NullType Result;
	};

	template<class List>struct MessBits
	{
		void operator()(){
			Log::Mess<LogMess::Bits<List>>(); 
		}
	};

	template<>struct MessBits<NullType>
	{
		void operator()(){}
	};

	template<unsigned DELAY, class List>struct AND_Bits
	{
		template<class Result>unsigned operator()(Result &result)
		{
			result.ret = 0;
			unsigned delay = DELAY;
			if((unsigned)-1 != DELAY) delay += GetTickCount();
			unsigned bitOn = 0, bitOff = 0, bitInv = 0;
			typedef typename Filt<List, On>::Result list_on;
			typedef typename Filt<List, Off>::Result list_off;
			typedef typename Filt<List, Proc>::Result list_proc;
			static const bool bitsNotEmpty = __all_lists_not_empty__<list_on, list_off>::value;
			SelectBits<list_on>()(bitOn);
			SelectBits<list_off>()(bitOff);
			SelectBits<typename Filt<List, Inv>::Result>()(bitInv);

			typedef typename FiltKey<List, Key>::Result list_key;

			MessBits<typename __mess_bits__<List>::Result>()();

			while(true)
			{
				unsigned ev = WaitFor<list_key>()();
				unsigned bits = 0;
				if(bitsNotEmpty || __list_not_empty__<list_proc>::value)bits = device1730.Read();
				
				switch(ev)
				{
				case WAIT_TIMEOUT:
					{
						unsigned ret = AND_Bits_WAIT_TIMEOUT<
							Tlst<typename TL::TypeToTypeLst<list_on, On>::Result
							, Tlst<typename TL::TypeToTypeLst<list_off, Off>::Result
							, NullType>>
							, Result, DELAY
							>()(result, delay);
						if(0 != ret) return ret;

						if(bitsNotEmpty &&(bitOn || bitOff))
						{						
							unsigned t = bits ^ bitInv;
							if(bitOn == (t & (bitOn | bitOff))) return 0;
						}	
						result.bits = bits;
						switch(DefaultDo<list_proc>()(result))
						{
						case Status::stop: return Status::stop;
						case Status::contine: return 0;
						}
						typedef typename FiltTestBits<List>::Result __test_bits_list__;
						if(!TestBitsDo<__test_bits_list__>()(result)) return Status::alarm_bits;
					}
					break;

				case WAIT_FAILED:
					return Status::exit_loop;

				default:
					switch(__key_handler__<list_key>()(ev - WAIT_OBJECT_0))
					{
					case -1          : break;
					case 0           : return 0;
					case Status::stop: return Status::stop;
					}
					break;
				}
			}
			return 0;
		}
	};

	template<class List>struct BitsOut
	{
		void operator()(unsigned &bits)
		{
			bits = 0;
			__sel_bits__<typename __filtr__<List, OutputBitTable::items_list>::Result, __bits_0__>()
				(&Singleton<OutputBitTable>::Instance().items, &bits);
		}
	};
	template<>struct BitsOut<NullType>
	{
		void operator()(unsigned &bits)
		{
		}
	};

	template<class List>struct OUT_Bits
	{
		void operator()()
		{
			unsigned bitOn, bitOff;
			bitOn = bitOff = 0;
			BitsOut<typename Filt<List, On>::Result>()(bitOn);
			BitsOut<typename Filt<List, Off>::Result>()(bitOff);

			unsigned res = device1730.ReadOutput();

            res &= ~bitOff;
			res |= bitOn;

			device1730.Write(res);
			MessBits<typename __mess_bits__<List>::Result>()();
		}
	};

	template<class List>struct Set_OUT_Bits
	{
		void operator()()
		{
			unsigned bitOn = 0;
			BitsOut<typename Filt<List, On>::Result>()(bitOn);

			device1730.Write(bitOn);
			MessBits<typename __mess_bits__<List>::Result>()();
		}
	};

#define AND_BITS(delay, ...) if(0 != (status = AND_Bits<delay, TL::MkTlst<__VA_ARGS__>::Result>()(result)))break
#define OUT_BITS(...) OUT_Bits<TL::MkTlst<__VA_ARGS__>::Result>()()
#define SET_OUT_BITS(...) Set_OUT_Bits<TL::MkTlst<__VA_ARGS__>::Result>()()
}