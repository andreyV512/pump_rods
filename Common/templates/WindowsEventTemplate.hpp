#pragma once
#include <windows.h>
#include "window_tool/message.h"
#include "templates/typelist.hpp"

namespace{
	///\brief в типе Т присутствует член-тип "Parent"
	template<class T>struct IsParent
	{
		template<bool, class Z>struct Ret
		{
			typedef NullType Result;
		};
		template<class Z>struct Ret<true, Z>
		{
			typedef typename Z::Parent Result;
		};
		template<class Z>static char Is(Z *, typename Z::Parent * = 0);
		static double Is(...);
		typedef typename Ret<sizeof(char) == sizeof(Is((T *)0)), T>::Result Result;
	};
	template<class T>struct TypeToEvent
	{
		///\brief необходимо для типа Т сопоставить сообщение windows
		typedef typename T::_must_match_the_class_of_message_windows nonexist;
	};

	template<>struct TypeToEvent<TMouseMove>{static const int value = WM_MOUSEMOVE;};///<-------см. ниже комментарий
	template<>struct TypeToEvent<TSize>{static const int value = WM_SIZE;};
	template<>struct TypeToEvent<TPaint>{static const int value = WM_PAINT;};
	template<>struct TypeToEvent<TActivate>{static const int value = WM_ACTIVATE;};
	template<>struct TypeToEvent<TUser>{static const int value = WM_USER;};
	template<>struct TypeToEvent<TDestroy>{static const int value = WM_DESTROY;};
	template<>struct TypeToEvent<TLButtonDown>{static const int value = WM_LBUTTONDOWN;};
	template<>struct TypeToEvent<TRButtonDown>{static const int value = WM_RBUTTONDOWN;};
	template<>struct TypeToEvent<TLButtonUp>{static const int value = WM_LBUTTONUP;};
	template<>struct TypeToEvent<TCommand>{static const int value = WM_COMMAND;};
	template<>struct TypeToEvent<TNotify>{static const int value = WM_NOTIFY;};
	template<>struct TypeToEvent<TGetMinMaxInfo>{static const int value = WM_GETMINMAXINFO;};
	template<>struct TypeToEvent<TLButtonDbClk>{static const int value = WM_LBUTTONDBLCLK;};
	template<>struct TypeToEvent<TKeyDown>{static const int value = WM_KEYDOWN;};
	template<>struct TypeToEvent<TMouseWell>{static const int value =  WM_MOUSEWHEEL;};
	template<>struct TypeToEvent<TMoving>{static const int value =  WM_MOVING;};
	template<>struct TypeToEvent<TSizing>{static const int value =  WM_SIZING;};
	template<>struct TypeToEvent<TMessage>{static const int value =  WM_MESSAGE;};
	template<>struct TypeToEvent<TTimer>{static const int value =  WM_TIMER;};
	template<>struct TypeToEvent<TClose>{static const int value =  WM_CLOSE;};
	template<>struct TypeToEvent<TCopyData>{static const int value =  WM_COPYDATA;};
	template<>struct TypeToEvent<TDropFiles>{static const int value =  WM_DROPFILES;};
	template<>struct TypeToEvent<TSysKeyDown>{static const int value = WM_SYSKEYDOWN;};

	template<class O, class P>class IsFuncExist
	{
		template<class T, T>struct Helper{};
		///\bug Необходимо определить в классе unsigned operator()(XXX &){}, где XXX - TMouseMove, TSize ... TTimer(см выше) 
		///\bug или необходимо определить в классе void operator()(XXX &){}, где XXX - TMouseMove, TSize ... TTimer(см выше) 
		//template<class Z>static double Is(O *, Helper<unsigned(O::*)(Z &), &O::operator()> * = NULL);
		template<class Z>static double Is(O *, Helper<void(O::*)(Z &), &O::operator()> * = NULL);
		template<class Z>static double Is(O *, Helper<LRESULT(O::*)(Z &), &O::operator()> * = NULL);
		template<class Z>static char Is(...);
	public:
		static const bool value = sizeof(Is<P>((O *)0)) == sizeof(double);
		typedef typename TL::_if<value, P, NullType>::Result Result;
	};

	template<class O>class IsCreateExist
	{
		template<class T, T>struct Helper{};
		static double Is(O *, Helper<unsigned(O::*)(TCreate &), &O::operator()> * = NULL);
		static double Is(O *, Helper<void(O::*)(TCreate &), &O::operator()> * = NULL);
		static char Is(...);
	public:
		static const bool value = sizeof(Is((O *)0)) == sizeof(double);
	};

	template<class O, class P>struct Couple
	{
		typedef O OBJ;
		typedef P PAR;
	};

	template<class O, class P>struct RES
	{
		typedef Couple<O, P> Result;
	};

	template<class O>struct RES<O, NullType>
	{
		typedef NullType Result;
	};

	template<class List, class T, class tmp = NullType>struct AddTypeEvent;
	template<class Head, class Tail, class T, class tmp>struct AddTypeEvent<Tlst<Head, Tail>, T, tmp>
	{
		typedef typename AddTypeEvent<Tail, T, typename TL::Append<tmp, typename RES<T, typename IsFuncExist<T, Head>::Result>::Result>::Result>::Result Result;
	};

	template<class List, class T, class tmp>struct AddMore;
	template<class Head, class Tail, class T, class tmp>struct AddMore<Tlst<Head, Tail>, T, tmp>
	{
		typedef typename AddTypeEvent<Tail, T, typename TL::Append<tmp, typename RES<T, typename IsFuncExist<T, Head>::Result>::Result>::Result>::Result Result;
	};

	template<class Head, class Tail, class tmp>struct AddMore<Tlst<Head, Tail>, NullType, tmp>
	{
		typedef tmp Result;
	};

	template<class T>struct isParentExist
	{
		typedef typename TL::_if<
			IsFuncExist<T, TCreate>::value
			, T
			, typename isParentExist<typename IsParent<T>::Result>::Result
			>:: Result Result;
	};
	template<>struct isParentExist<NullType>
	{
		typedef NullType Result;
	};

	typedef TL::MkTlst<
		TMouseMove, TSize, TPaint, TActivate, TNotify
		, TLButtonDown, TLButtonUp, TLButtonDbClk, TKeyDown 
		, TUser, TCommand, TDestroy
		, TGetMinMaxInfo, TMouseWell, TRButtonDown, TMoving, TSizing
		, TMessage, TTimer, TClose
		, TCopyData, TDropFiles	, TSysKeyDown
	>::Result type_events_all_list; //<-------см. ниже комментарий

	template<class T, class tmp>struct AddTypeEvent<NullType, T, tmp>
	{
		typedef typename AddMore<type_events_all_list, typename IsParent<T>::Result, tmp>::Result Result;
	};

	template<bool >struct Wapper
	{
		template<class O, class P>unsigned operator()(O &o, P *p)
		{
			p->obj(o);
			return 0;
		}
	};

	template<>struct Wapper<false>
	{
		template<class O, class P>LRESULT operator()(O &o, P *p)
		{
			return p->obj(o);
		}
	};

	template<class List, class T>struct SubReTypeDelete;
	template<class A, class B, class Tail, class T>struct SubReTypeDelete<Tlst<Couple<A, B>, Tail>, T>
	{
		typedef Tlst<Couple<A, B>, typename SubReTypeDelete<Tail, T>::Result> Result;
	};
	template<class A, class Tail, class T>struct SubReTypeDelete<Tlst<Couple<A, T>, Tail>, T>
	{
		typedef typename SubReTypeDelete<Tail, T>::Result Result;
	};

	template<class T>struct SubReTypeDelete<NullType, T>
	{
		typedef NullType Result;
	};

	template<class List>struct ReTypeDelete;
	template<class Head, class Tail>struct ReTypeDelete<Tlst<Head, Tail> >
	{
		typedef Tlst<Head, typename ReTypeDelete<typename SubReTypeDelete<Tail, typename Head::PAR>::Result>::Result> Result;
	};
	template<>struct ReTypeDelete<NullType>
	{
		typedef NullType Result;
	};	
	template<class T>struct EventHandler
	{
		template<class O, class P>struct loc
		{
			template<class T, T>struct Helper{};
			template<class X, class Z>static double Is(X *, Helper<void(X::*)(Z &), &X::operator()> * = NULL);
			template<class X, class Z>static char Is(...);
			bool operator()(P *p)
			{
				if(TypeToEvent<typename O::PAR>::value != p->mess.uMsg) return true;
				static const bool b = sizeof(double) == sizeof(Is<typename O::OBJ, typename O::PAR>((typename O::OBJ *)0));
				p->result = Wapper<b>()((typename O::PAR &)p->mess, (EventHandler<typename O::OBJ> *)p);
				return false;
			}
		};
		template<class List, class T>struct TestNotNullType
		{
			typedef List Result;
		};
		template<class T>struct TestNotNullType<NullType, T>
		{
			///\brief класс должен иметь обработчик в виде- LRESULT T::operator()(XXX &) или void T::operator()(XXX &);
			///где: XXX - TMouseMove, TSize, TPaint, TActivate(добавить при необходимости в список type_events_all_list, см. выше)
			typedef typename T::_class_does_not_have_any_handler nonexist;
		};
		typedef typename TestNotNullType<typename AddTypeEvent<type_events_all_list, T>::Result, T>::Result list_0;
		TMessage &mess;
		T &obj;
		LRESULT result;
	public:
		typedef typename ReTypeDelete<list_0>::Result list;
		EventHandler(TMessage &mess, T &obj)
			: mess(mess)
			, obj(obj)
			, result(0)
		{}
		LRESULT operator()()
		{			
			if(!TL::find<list, loc>()(this))return result;
			return DefWindowProc(MESSAGE(mess));
		}
	};
}
//------------------------------------------------------------------------------------------------
template<class T>class Viewer
{
	template<class Z>struct Create
	{
		LRESULT operator()(TCreate &l)
		{
			T *o = (T *)l.create->lpCreateParams;
			o->hWnd = l.hwnd;
			SetWindowLongPtr(l.hwnd, GWLP_USERDATA, (LONG_PTR)o);
			return (*(Z *)o)(l);
		}
	};
	template<>struct Create<NullType>
	{
		LRESULT operator()(TCreate &l)
		{
			T *o = (T *)l.create->lpCreateParams;
			o->hWnd = l.hwnd;
			SetWindowLongPtr(l.hwnd, GWLP_USERDATA, (LONG_PTR)o);
			return 0;
		}
	};	
public:
	static LRESULT CALLBACK Proc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
	{
		T *o = (T *)GetWindowLongPtr(hWnd, GWLP_USERDATA);
		TMessage m = {hWnd, message, wParam, lParam};
		if(NULL != o)
		{
			return EventHandler<T>(m, *o)();
		}	
		else if(message == WM_CREATE)
		{
			return Create<isParentExist<T>::Result>()((TCreate &)m);
		}
		return DefWindowProc(hWnd, message, wParam, lParam);
	}	
};
//--------------------------------------------------------------------------------------------------------------