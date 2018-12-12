#pragma once
template<class T, int N>char (&__length_array_of_T_type_same_as_length_array_of_char__(T (&)[N]))[N];
#define dimention_of(x) sizeof(__length_array_of_T_type_same_as_length_array_of_char__(x))

struct NullType{};
template<class HEAD, class TAIL>struct Tlst
{
	typedef HEAD Head;
	typedef TAIL Tail;
};

namespace TL
{
#define c(n) class n=NullType
#define a(n) c(_##n##0),c(_##n##1),c(_##n##2),c(_##n##3),c(_##n##4),c(_##n##5),c(_##n##6),c(_##n##7),c(_##n##8),c(_##n##9)
#define b(n) _##n##0,_##n##1,_##n##2,_##n##3,_##n##4,_##n##5,_##n##6,_##n##7,_##n##8,_##n##9
	template<a(0),a(1),a(2),a(3),a(4),a(5),a(6),a(7),a(8),a(9),a(10),a(11),a(12),a(13),a(14),a(15)>struct MkTlst
	{
		typedef Tlst<_00
			, typename MkTlst<
			_01,_02,_03,_04,_05,_06,_07,_08,_09
			,b(1),b(2),b(3),b(4),b(5),b(6),b(7),b(8),b(9),b(10),b(11),b(12),b(13),b(14),b(15)
			>::Result
		> Result;
	};
	template<>struct MkTlst<NullType>
	{
		typedef NullType Result;
	};
#undef c
#undef a
#undef b
//-----------------------------------------------------------------------------------------------------------
#pragma warning(disable : 4503)
	template<class List, class T>struct IndexOf;
	template<class Head, class Tail, class T>struct IndexOf<Tlst<Head, Tail>, T>
	{
		static const int value = 1 + IndexOf<Tail, T>::value;
	};
	template<class Tail, class T>struct IndexOf<Tlst<T, Tail>, T>
	{
		static const int value = 0;
	};
//-------------------------------------------------------------------------------------------------------------
	template<class List>struct Length;
	template<class Head, class Tail>struct Length<Tlst<Head, Tail> >
	{
		static const int value = 1 + Length<Tail>::value;
	};
	template<class Head>struct Length<Tlst<Head, NullType> >
	{
		static const int value = 1;
	};
//--------------------------------------------------------------------------------------------------------------
	template<class List, int N>struct TypeAt;
	template<class Head, class Tail, int N>struct TypeAt<Tlst<Head, Tail>, N>
	{
		typedef typename TypeAt<Tail, N - 1>::Result Result;
	};
	template<class Head, class Tail>struct TypeAt<Tlst<Head, Tail>, 0>
	{
		typedef Head Result;
	};
	template<int N>struct TypeAt<NullType, N>
	{
		typedef NullType Result;
	};
//-------------------------------------------------------------------------------------------------------------
	template<class List, template<class>class Wapper>struct ListToWapperList;
	template<class Head, class Tail, template<class>class Wapper>struct ListToWapperList<Tlst<Head, Tail>, Wapper>
	{
		typedef Tlst<Wapper<Head>, typename ListToWapperList<Tail, Wapper>::Result> Result;
	};
	template<class Head, template<class>class Wapper>struct ListToWapperList<Tlst<Head, NullType>, Wapper>
	{
		typedef Tlst<Wapper<Head>, NullType> Result;
	};
//---------------------------------------------------------------------------
    template<template<int>class Wapper, int start, int max>struct CreateNumList
	{
		typedef Tlst<Wapper<start>, typename CreateNumList<Wapper, 1 + start, max>::Result> Result;
	};
	template<template<int>class Wapper, int max>struct CreateNumList<Wapper, max, max>
	{
		typedef Tlst<Wapper<max>, NullType> Result;
	};
//--------------------------------------------------------------------------------------------------------------
	template<template<class, int>class Wapper, class T, int start, int max>struct CreateWapperNumList
	{
		typedef Tlst<Wapper<T, start>, typename CreateWapperNumList<Wapper, T, 1 + start, max>::Result> Result;
	};
	template<template<class, int>class Wapper, class T, int max>struct CreateWapperNumList<Wapper, T, max, max>
	{
		typedef Tlst<Wapper<T, max>, NullType> Result;
	};
//--------------------------------------------------------------------------------------------------------------
	template<template<class, class, class, int>class Wapper, class T, class T1, class T2, int start, int max>struct CreateWapper3NumList
	{
		typedef Tlst<Wapper<T, T1, T2, start>, typename CreateWapper3NumList<Wapper, T, T1, T2, 1 + start, max>::Result> Result;
	};
	template<template<class, class, class, int>class Wapper, class T, class T1, class T2, int max>struct CreateWapper3NumList<Wapper, T, T1, T2, max, max>
	{
		typedef Tlst<Wapper<T, T1, T2, max>, NullType> Result;
	};
//--------------------------------------------------------------------------------------------------------------
	template<typename T, typename tmp = NullType>struct Reverse;
	template<typename Head, typename Tail, typename tmp>struct Reverse<Tlst<Head, Tail>, tmp>
	{
		typedef typename Reverse<Tail, Tlst<Head, tmp> >::Result Result;
	};
	template<typename tmp>struct Reverse<NullType, tmp>
	{
		typedef tmp Result;
	};
//-------------------------------------------------------------------------------------------------------------
	template<class List>struct Factory;
	template<class Head, class Tail>struct Factory<Tlst<Head, Tail> >: Head, Factory<Tail>
	{
		Factory() : Head(), Factory<Tail>(){}
		template<class T>Factory(T &t) : Head(t), Factory<Tail>(t){}
		template<class T>Factory(T *t) : Head(t), Factory<Tail>(t){}
		template<class T>T &get(){return *this;}
	};
	template<class Head>struct Factory<Tlst<Head, NullType> >: Head
	{
		Factory() : Head(){}
		template<class T>Factory(T &t) : Head(t){}
		template<class T>Factory(T *t) : Head(t){}
		template<class T>T &get(){return *this;}
	};
//-------------------------------------------------------------------------------------------------------------
#pragma warning(disable : 4503)
	template<class List, template<class, class>class Proc>struct foreach;
	template<class Head, class Tail, template<class, class>class Proc>struct foreach<Tlst<Head, Tail>, Proc>
	{
		template<class O, class P>void operator()(O *o, P *p)
		{
			Proc<Head, P>()(o, p);
			foreach<Tail, Proc>()(o, p);
		}
		template<class P>void operator()(P *p)
		{
			Proc<Head, P>()(p);
			foreach<Tail, Proc>()(p);
		}	
		void operator()(Tlst<Head, Tail> *o)
		{
			Proc<Head, int>()(o);
			foreach<Tail, Proc>()(o);
		}
		void operator()()
		{
			Proc<Head, int>()();
			foreach<Tail, Proc>()();
		}	

		template<class O, class P>void operator()(O &o, P &p)
		{
			Proc<Head, P>()(o, p);
			foreach<Tail, Proc>()(o, p);
		}
		template<class P>void operator()(P &p)
		{
			Proc<Head, P>()(p);
			foreach<Tail, Proc>()(p);
		}	
		void operator()(Tlst<Head, Tail> &o)
		{
			Proc<Head, int>()(o);
			foreach<Tail, Proc>()(o);
		}
	};
	template<class Head, template<class, class>class Proc>struct foreach<Tlst<Head, NullType>, Proc>
	{
		template<class O, class P>void operator()(O *o, P *p)
		{
			Proc<Head, P>()(o, p);
		}
		void operator()(Tlst<Head, NullType> *o)
		{
			Proc<Head, int>()(o);
		}
		template<class P>void operator()(P *p)
		{
		    Proc<Head, P>()(p);
		}
		void operator()()
		{
			Proc<Head, int>()();
		}	

		template<class O, class P>void operator()(O &o, P &p)
		{
			Proc<Head, P>()(o, p);
		}
		void operator()(Tlst<Head, NullType> &o)
		{
			Proc<Head, int>()(o);
		}
		template<class P>void operator()(P &p)
		{
		    Proc<Head, P>()(p);
		}
	};
//-------------------------------------------------------------------------------------------------------------
	template<class List, template<class, class>class Proc>struct find;
	template<class Head, class Tail, template<class, class>class Proc>struct find<Tlst<Head, Tail>, Proc>
	{
		template<class O, class P>bool operator()(O *o, P *p)
		{
			if(Proc<Head, P>()(o, p))return find<Tail, Proc>()(o, p);
			return false;
		}
		template<class P>bool operator()(P *p)
		{
			if(Proc<Head, P>()(p))return find<Tail, Proc>()(p);
			return false;
		}	
		bool operator()(Tlst<Head, Tail> *o)
		{
			if(Proc<Head, int>()(o))return find<Tail, Proc>()(o);
			return false;
		}
		bool operator()()
		{
			if(Proc<Head, int>()())return find<Tail, Proc>()();
			return false;
		}	

		template<class O, class P>bool operator()(O &o, P &p)
		{
			if(Proc<Head, P>()(o, p))return find<Tail, Proc>()(o, p);
			return false;
		}
		template<class P>bool operator()(P &p)
		{
			if(Proc<Head, P>()(p))return find<Tail, Proc>()(p);
			return false;
		}	
		bool operator()(Tlst<Head, Tail> &o)
		{
			if(Proc<Head, int>()(o))return find<Tail, Proc>()(o);
			return false;
		}
	};
	template<class Head, template<class, class>class Proc>struct find<Tlst<Head, NullType>, Proc>
	{
		template<class O, class P>bool operator()(O *o, P *p)
		{
			return Proc<Head, P>()(o, p);
		}
		bool operator()(Tlst<Head, NullType> *o)
		{
			return Proc<Head, int>()(o);
		}
		template<class P>bool operator()(P *p)
		{
			return Proc<Head, P>()(p);
		}
		bool operator()()
		{
			return Proc<Head, int>()();
		}	

		template<class O, class P>bool operator()(O &o, P &p)
		{
			return Proc<Head, P>()(o, p);
		}
		bool operator()(Tlst<Head, NullType> &o)
		{
			return Proc<Head, int>()(o);
		}
		template<class P>bool operator()(P &p)
		{
			return Proc<Head, P>()(p);
		}
	};
//-------------------------------------------------------------------------------------------------------------
	template<class List, template<class, class>class Proc>struct find_ret;
	template<class Head, class Tail, template<class, class>class Proc>struct find_ret<Tlst<Head, Tail>, Proc>
	{
		template<class O, class P>unsigned operator()(O *o, P *p)
		{
			unsigned ret = Proc<Head, P>()(o, p);
			if(0 == ret)return find_ret<Tail, Proc>()(o, p);
			return ret;
		}
		template<class O, class P>unsigned operator()(O &o, P &p)
		{
			unsigned ret = Proc<Head, P>()(o, p);
			if(0 == ret)return find_ret<Tail, Proc>()(o, p);
			return ret;
		}
	};
	template<class Head, template<class, class>class Proc>struct find_ret<Tlst<Head, NullType>, Proc>
	{
		template<class O, class P>unsigned operator()(O *o, P *p)
		{
			return Proc<Head, P>()(o, p);
		}
		template<class O, class P>unsigned operator()(O &o, P &p)
		{
			return Proc<Head, P>()(o, p);
		}
	};
//-------------------------------------------------------------------------------------------------------------
	template<class List, class Param>struct FactoryToArray
	{
	private:
		struct Parent
		{
			virtual void operator()(Param &) = 0;
		};
		template<class T>struct Loc
		{
			struct all : Parent
			{
				T &owner;
				all(T &t) : owner(t){}
				void operator()(Param &p){owner(p);}
			};
			Loc(T &t): all(t){}
		};
		typedef typename TL::ListToWapperList<List, Loc>::Result sub_typelist;
		template<class O, class P>struct __init__
		{
			void operator()(O *o, P *p)
			{
				p[IndexOf<sub_typelist, O>::value] = o;
			}
		};		
		TL::Factory<sub_typelist> factory;
		Parent *objects[TL::Length<List>::value];
	public:
		FactoryToArray(Factory<List> &fac)
			: factory(fac)
		{
			TL::foreach<sub_typelist, __init__>()(&factory, objects);
		}
		inline bool operator()(int index, Param &param)
		{
			return objects[index](param);
		}
	};
//-------------------------------------------------------------------------------------------------------------
        template<class List, class T>struct Append;
        template<class Head, class Tail, class T>struct Append<Tlst<Head, Tail>, T>
        {
          typedef Tlst<Head, typename Append<Tail, T>::Result> Result;
        };
        template<class T>struct Append<NullType, T>
        {
          typedef Tlst<T, NullType> Result;
        };
		 template<class Head, class Tail>struct Append<Tlst<Head, Tail>, NullType>
        {
          typedef Tlst<Head, Tail> Result;
        };
		 template<>struct Append<NullType, NullType>
        {
          typedef NullType Result;
        };
//-------------------------------------------------------------------------------------------------------------  
        template<class A, class B>struct AppendList;
        template<class A, class Head, class Tail>struct AppendList<A, Tlst<Head, Tail> >
        {
          typedef typename AppendList<typename Append<A, Head>::Result, Tail>::Result Result;
        };
        template<class A>struct AppendList<A, NullType>
        {
          typedef A Result;
        };
//-------------------------------------------------------------------------------------------------------------
		template<class List, class tmp = NullType>struct MultyListToList;
        template<class Head, class Tail, class tmp>struct MultyListToList<Tlst<Head, Tail>, tmp>
		{
			typedef typename TL::MultyListToList<Tail, typename AppendList<tmp, Head>::Result>::Result Result;
		};
		template<class tmp>struct MultyListToList<NullType, tmp>
		{
			typedef tmp Result;
		};
//-------------------------------------------------------------------------------------------------------------     
    template<bool t, class A, class B>struct _if
	{
          typedef A Result;
	};
	template<class A, class B>struct _if<false, A, B>
	{
          typedef B Result;
	};
//---------------------------------------------------------------------------------------------------------------- 
	template<int num>struct IntToType
	{
		static const int value = num;
	};
//-----------------------------------------------------------------------------------------------------------------
	template<class List, template<class>class Wapper>struct TypeToTypeLst;
	template<class Head, class Tail, template<class>class Wapper>struct TypeToTypeLst<Tlst<Head, Tail>, Wapper>
	{
		typedef Tlst<Wapper<Head>, typename TypeToTypeLst<Tail, Wapper>::Result> Result;
	};
	//template<class Head, template<class>class Wapper>struct TypeToTypeLst<Tlst<Head, NullType>, Wapper>
	//{
	//	typedef Tlst<Wapper<Head>, NullType> Result;
	//};
	template<template<class>class Wapper>struct TypeToTypeLst<NullType, Wapper>
	{
		typedef NullType Result;
	};
//--------------------------------------------------------------------------------------------------------------------------
	template<typename List, template<class>class T>struct EraseAllParam;
	template<template<class>class T>struct EraseAllParam<NullType, T>
	{
		typedef NullType Result;
	};
	template<typename Head, typename Tail, template<class>class T>struct EraseAllParam<Tlst<Head, Tail>, T>
	{
		typedef typename _if<
			T<Head>::value
			, Tlst<Head, typename EraseAllParam<Tail, T>::Result>
			, typename EraseAllParam<Tail, T>::Result
		>::Result Result;
	};	
//----------------------------------------------------------------------------------------------------------
	template<class T>struct Inner;
	template<template<class>class Wapper, class T>struct Inner<Wapper<T> >
	{
		typedef T Result;
	};
	template<template<class, class>class Wapper, class T, class X>struct Inner<Wapper<T, X>>
	{
		typedef T Result;
	};
//-------------------------------------------------------------------------------------------
	template<class List, class T>struct TypeInList;
	template<class Head, class Tail, class T>struct TypeInList<Tlst<Head, Tail>, T>
	{
		static const bool value = TypeInList<Tail, T>::value;
	};
	template<class Head, class Tail>struct TypeInList<Tlst<Head, Tail>, Head>
	{
		static const bool value = true;
	};
	template<class T>struct TypeInList<NullType, T>
	{
		static const bool value = false;
	};
	//-------------------------------------------------------------------------------------------
	template<class List, class T, class val = TL::IntToType<0> >struct TypeInMultyList;
	template<class SubListHead, class SubListTail, class T, class val>struct TypeInMultyList<Tlst<SubListHead, SubListTail>, T, val>
	{
		static const bool b = TL::TypeInList<SubListHead, T>::value;
		typedef typename TL::_if<b, val, typename TypeInMultyList<SubListTail, T, TL::IntToType<1 + val::value> >::Result>::Result Result;
	};
	template<class T, class val>struct TypeInMultyList<NullType, T, val>
	{
		typedef TL::IntToType<-1> Result;
	};
	//-------------------------------------------------------------------------------------------
	template<class List, class T>struct SubListFromMultyList					   
	{
		typedef typename TypeAt<List, TypeInMultyList<List, T>::Result::value>::Result Result;
	};
	//----------------------------------------------------------------------------------------------------
	template<typename List, template<typename,typename>class Wapper, class Param>struct TypeToTypeLstParam1;
	template<typename Head, typename Tail, template<typename,typename>class Wapper, class Param>struct TypeToTypeLstParam1<Tlst<Head, Tail>, Wapper, Param>
	{
		typedef Tlst<Wapper<Head, Param>, typename TypeToTypeLstParam1<Tail, Wapper, Param>::Result> Result;
	};
	template<typename Head, template<typename,typename>class Wapper, class Param>struct TypeToTypeLstParam1<Tlst<Head, NullType>, Wapper, Param>
	{
		typedef Tlst<Wapper<Head, Param>, NullType> Result;
	};
	//----------------------------------------------------------------------------------------------------
	template<typename List, template<typename,typename,typename>class Wapper, class Param, class Param1>struct TypeToTypeLstParam2;
	template<typename Head, typename Tail, template<typename,typename,typename>class Wapper, class Param, class Param1>struct TypeToTypeLstParam2<Tlst<Head, Tail>, Wapper, Param, Param1>
	{
		typedef Tlst<Wapper<Head, Param, Param1>, typename TypeToTypeLstParam2<Tail, Wapper, Param, Param1>::Result> Result;
	};
	template<typename Head, template<typename,typename,typename>class Wapper, class Param, class Param1>struct TypeToTypeLstParam2<Tlst<Head, NullType>, Wapper, Param, Param1>
	{
		typedef Tlst<Wapper<Head, Param, Param1>, NullType> Result;
	};
	//----------------------------------------------------------------------------------------------------
	template<typename List, template<typename,typename,typename>class Wapper, class Param, class Param1>struct TypeToTypeLstParam2_xx;
	template<typename Head, typename Tail, template<typename,typename,typename>class Wapper, class Param, class Param1>struct TypeToTypeLstParam2_xx<Tlst<Head, Tail>, Wapper, Param, Param1>
	{
		typedef Tlst<Wapper<Param, Head, Param1>, typename TypeToTypeLstParam2_xx<Tail, Wapper, Param, Param1>::Result> Result;
	};
	template<typename Head, template<typename,typename,typename>class Wapper, class Param, class Param1>struct TypeToTypeLstParam2_xx<Tlst<Head, NullType>, Wapper, Param, Param1>
	{
		typedef Tlst<Wapper<Param, Head, Param1>, NullType> Result;
	};
	//------------------------------------------------------------------------------------------------------------------------------------------------
	template<class List, class P, class R>class ArrayFunc
	{
		struct FuncSubType{virtual R operator()(P &)= 0;};
		template<class O>struct Proc:FuncSubType
		{
			O &o;
			template<class T>Proc(T &t) : o(t.get<O>()) {}
			R operator()(P &p){return o(p);}
		};
		typedef TL::Factory<List> TItems;
		typedef typename TL::TypeToTypeLst<List, Proc>::Result list;
		template<class O, class P>struct loc
		{
			void operator()(O *o, P *p)
			{
				p[TL::IndexOf<list, O>::value] = o;
			}
		};		
		TL::Factory<list> items;
		FuncSubType *arr[TL::Length<List>::value];
	public:
		ArrayFunc(TItems &o): items(o)
		{
			TL::foreach<list, loc>()(&items, arr);
		}	
		FuncSubType &operator[](int i)
		{
			return *arr[i];
		}
	};
//----------------------------------------------------------------------------------------------
	template<class T, template<class>class W>struct TestWapper
	{
		template<class Q, Q>struct inner{};
		template<class Z>static double Is(typename Z *, inner<typename Inner<Z>::Result::type_value(Z::*)(), &Z::operator()> * = NULL);
		template<class Z>static char   Is(...);
		static const bool value = sizeof(double) == sizeof(Is<W<T> >((W<T> *)0));
	};
//--------------------------------------------------------------------------------------------------
	template<class T>struct TestType
	{
		template<class Q, Q>struct inner{};
		template<class Z>static double Is(typename Z *, inner<typename Inner<Z>::Result::type_value(Z::*)(), &Z::operator()> * = NULL);
		template<class Z>static char   Is(...);
		static const bool value = sizeof(double) == sizeof(Is<T>((T *)0));
	};
//----------------------------------------------------------------------------------------------------------
	template<class List>struct TypeExist;
	template<class Head, class Tail>struct TypeExist<Tlst<Head, Tail> >
	{
		typedef typename TL::_if<TL::TestType<Head>::value, Head, typename TypeExist<Tail>::Result>::Result Result;
	};
	template<>struct TypeExist<NullType>
	{
		typedef NullType Result;
	};
//----------------------------------------------------------------------------------------------------------
	template<class T>struct NoWapperType
	{
		typedef T Result;
	};
	template<template<class>class W, class T>struct NoWapperType<W<T> >
	{
		typedef typename NoWapperType<T>::Result Result;
	};
//------------------------------------------------------------------------
	template<class List>struct NoWapperList;
	template<class Head, class Tail>struct NoWapperList<Tlst<Head, Tail> >
	{
		typedef Tlst<typename NoWapperType<Head>::Result, typename NoWapperList<Tail>::Result> Result;
	};
	template<>struct NoWapperList<NullType>
	{
		typedef NullType Result;
	};
//------------------------------------------------------------------------------------------------------
	template<class List, class T>struct EraseItem;
	template<class Head, class Tail, class T>struct EraseItem<Tlst<Head, Tail>, T>
	{
		typedef typename Tlst<Head, typename EraseItem<Tail, T>::Result> Result;
	};
	template<class Tail, class T>struct EraseItem<Tlst<T, Tail>, T>
	{
		typedef Tail Result;
	};
		template<class T>struct EraseItem<NullType, T>
	{
		typedef NullType Result;
	};
//------------------------------------------------------------------------------------------------------
	template<class List, class T>struct SelectT;
	template<class Head, class Tail, class T>struct SelectT<Tlst<Head, Tail>, T>
	{
		typedef typename SelectT<Tail, T>::Result Result;
	};
	template<class Tail, class T, template<class >class W>struct SelectT<Tlst<W<T>, Tail>, T>
	{
		typedef Tlst<W<T>, typename SelectT<Tail, T>::Result> Result;
	};
	template<class T>struct SelectT<NullType, T>
	{
		typedef NullType Result;
	};
//----------------------------------------------------------------------------------------------
	template<class List, template<class>class Wapper>struct SelectWapper;

	template<class Head, class Tail, template<class>class Wapper>struct SelectWapper<Tlst<Head, Tail>, Wapper>
	{
		typedef typename SelectWapper<Tail, Wapper>::Result Result;
	};

	template<class Head, class Tail, template<class>class Wapper>struct SelectWapper<Tlst<Wapper<Head>, Tail>, Wapper>
	{
		typedef Tlst<Wapper<Head>, typename SelectWapper<Tail, Wapper>::Result> Result;
	};

	template<template<class>class Wapper>struct SelectWapper<NullType, Wapper>
	{
		typedef NullType Result;
	};
//--------------------------------------------------------------------------------------------------
	/// \brief если тип содержится в списке возвращает true, иначе false
	template<class List, class T>struct ItemInList;
	template<class Head, class Tail, class T>struct ItemInList<Tlst<Head, Tail>, T>
	{
		static const bool value =  false || ItemInList<Tail, T>::value;
	};
	template<class Tail, class T>struct ItemInList<Tlst<T, Tail>, T>
	{
		static const bool value =  true;
	};
	template<class T>struct ItemInList<NullType, T>
	{
		static const bool value =  false;
	};
//---------------------------------------------------------------------------------------
	template<class List, template<int>class W>struct SubLength;
template<class Head, class Tail, template<int>class W>struct SubLength<Tlst<Head, Tail>, W>
{
	static const int value = SubLength<Tail, W>::value;
};

template<int N, class Tail, template<int>class W>struct SubLength<Tlst<W<N>, Tail>, W>
{
	static const int value = 1 + SubLength<Tail, W>::value;
};

template<template<int>class W>struct SubLength<NullType, W>
{
	static const int value = 0;
};

template<class List, template<int>class W>struct SubType;
template<class Head, class Tail, template<int>class W>struct SubType<Tlst<Head, Tail>, W>
{
	typedef typename SubType<Tail, W>::Result Result;
};
template<int N, class Tail, template<int>class W>struct SubType<Tlst<W<N>, Tail>, W>
{
	typedef Tlst<W<N>, typename SubType<Tail, W>::Result> Result;
};
template<template<int>class W>struct SubType<NullType, W>
{
	typedef NullType Result;
};

template<class T>struct GetNum;
template<int N, template<int>class W>struct GetNum<W<N>>
{
	static const int value = N;
};

template<class O, class P>struct __set_array__
{
	void operator()(O &o, P &p)
	{
		p[GetNum<O>::value] = o.value;
	}
};

#define TYPE_ARR(table, item)\
	item<0>::type_value _##item[TL::SubLength<table::items_list, item>::value];\
	TL::foreach<TL::SubType<table::items_list, item>::Result, TL::__set_array__>()(Singleton<table>::Instance().items, _##item);

#define INIT_TYPE_ARR(table, item)\
	TL::foreach<TL::SubType<table::items_list, item>::Result, TL::__set_array__>()(Singleton<table>::Instance().items, _##item);
}
#define __UNIQUENAME(n, c) n##c
#define _UNIQUENAME(n, c) __UNIQUENAME(n, c)
#define UNIQUENAME(n) _UNIQUENAME(n, __COUNTER__) 

namespace detail
{
	struct VAL{};
	template<class T>class Guard
	{
		T t;
	public:
		Guard(T &t) : t(t){}
		~Guard(){t();}
	};

	template<class T>Guard<T> operator +(VAL, T t)
	{
		return Guard<T>(t);
	}
}

#define GUARD auto UNIQUENAME(GUARD_NAME) = detail::VAL() + [&]

template<class T>struct Singleton
{
	static T& Instance(){return x;}
private:
	typedef T type_item;
	static type_item x;
    Singleton();
    ~Singleton();
    // необходимо также запретить копирование
    Singleton(Singleton const&); // реализация не нужна
    Singleton& operator= (Singleton const&);  // и тут
};
template<class T>typename Singleton<T>::type_item Singleton<T>::x;
///-------------------------------------------Генератор последовательностей
#define EXPAND(x) x
#define CONCATENATE(x,y) x##y

#define FOR_EACH_1(what, x, ...)what(x)
#define FOR_EACH_2(what, x, ...)what(x)EXPAND(FOR_EACH_1(what, __VA_ARGS__))
#define FOR_EACH_3(what, x, ...)what(x)EXPAND(FOR_EACH_2(what, __VA_ARGS__))
#define FOR_EACH_4(what, x, ...)what(x)EXPAND(FOR_EACH_3(what, __VA_ARGS__))
#define FOR_EACH_5(what, x, ...)what(x)EXPAND(FOR_EACH_4(what, __VA_ARGS__))
#define FOR_EACH_6(what, x, ...)what(x)EXPAND(FOR_EACH_5(what, __VA_ARGS__))
#define FOR_EACH_7(what, x, ...)what(x)EXPAND(FOR_EACH_6(what, __VA_ARGS__))
#define FOR_EACH_8(what, x, ...)what(x)EXPAND(FOR_EACH_7(what, __VA_ARGS__))
#define FOR_EACH_9(what, x, ...)what(x)EXPAND(FOR_EACH_8(what, __VA_ARGS__))

#define FOR_EACH_10(what, x, ...)what(x)EXPAND(FOR_EACH_9(what, __VA_ARGS__))
#define FOR_EACH_11(what, x, ...)what(x)EXPAND(FOR_EACH_10(what, __VA_ARGS__))
#define FOR_EACH_12(what, x, ...)what(x)EXPAND(FOR_EACH_11(what, __VA_ARGS__))
#define FOR_EACH_13(what, x, ...)what(x)EXPAND(FOR_EACH_12(what, __VA_ARGS__))
#define FOR_EACH_14(what, x, ...)what(x)EXPAND(FOR_EACH_13(what, __VA_ARGS__))
#define FOR_EACH_15(what, x, ...)what(x)EXPAND(FOR_EACH_14(what, __VA_ARGS__))
#define FOR_EACH_16(what, x, ...)what(x)EXPAND(FOR_EACH_15(what, __VA_ARGS__))
#define FOR_EACH_17(what, x, ...)what(x)EXPAND(FOR_EACH_16(what, __VA_ARGS__))

#define FOR_EACH_18(what, x, ...)what(x)EXPAND(FOR_EACH_17(what, __VA_ARGS__))
#define FOR_EACH_19(what, x, ...)what(x)EXPAND(FOR_EACH_18(what, __VA_ARGS__))

#define FOR_EACH_NARG(...) FOR_EACH_NARG_(__VA_ARGS__, FOR_EACH_RSEQ_N())
#define FOR_EACH_NARG_(...) EXPAND(FOR_EACH_ARG_N(__VA_ARGS__))
#define FOR_EACH_ARG_N(_19,_18,_17,_16,_15,_14,_13,_12,_11,_10,_9,_8,_7,_6,_5,_4,_3,_2,_1, N, ...) N
#define FOR_EACH_RSEQ_N() 19, 18, 17, 16, 15, 14, 13, 12, 11, 10, 9, 8, 7, 6, 5, 4, 3, 2, 1

#define FOR_EACH_(N, what, ...) EXPAND(CONCATENATE(FOR_EACH_, N)(what, __VA_ARGS__))
#define FOR_EACH(what, ...) FOR_EACH_(FOR_EACH_NARG(__VA_ARGS__), what, __VA_ARGS__)

//---------------------------------------------------тест генератора
#define INT_TO_TYPE(n) TL::IntToType<n>,
#define INT_TO_TYPE_LIST(...)  TL::MkTlst<FOR_EACH(INT_TO_TYPE, __VA_ARGS__)NullType>::Result
//typedef INT_TO_TYPE_LIST(1,3, 7, 8, 9) int_to_type_list ;