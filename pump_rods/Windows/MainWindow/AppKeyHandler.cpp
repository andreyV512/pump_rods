#include "AppKeyHandler.h"
#include <Windows.h>
#include <CommCtrl.h>
#include "templates/typelist.hpp"
#include "MainWindow.h"
#include "Resource.h"
#include "tools_debug\DebugMess.h"

namespace AppKeyHandler
{
	typedef TL::MkTlst<
		TL::IntToType<IDB_CycleBtn>
		, TL::IntToType<IDB_Reset>
		, TL::IntToType<IDB_QueryBtn>
		, TL::IntToType<IDB_DownArrow>
		, TL::IntToType<IDB_UpArrow>
		, TL::IntToType<IDB_LeftArrow>
		, TL::IntToType<IDB_RightArrow>
		, TL::IntToType<IDB_ExitTubeBtn>
		, TL::IntToType<IDB_Ok>
		, TL::IntToType<IDB_Continue>
	>::Result button_list;

	template<int ID>struct On {static const int ID = ID; static const bool state = true;};
	template<int ID>struct Off{static const int ID = ID; static const bool state = false;};
	template<class List>struct __btn__;
	template<class Head, class Tail>struct __btn__<Tlst<Head, Tail>>
	{
		void operator()()
		{
			SendMessage(Singleton<MainWindow>::Instance().toolBar.hWnd, TB_ENABLEBUTTON, Head::ID, MAKELONG(Head::state, 0));
			__btn__<Tail>()();
		}
	};

	template<>struct __btn__<NullType>{void operator()(){}};

	bool IsEnabled(int id)
	{
		return 0 != SendMessage(Singleton<MainWindow>::Instance().toolBar.hWnd, TB_ISBUTTONENABLED, id, 0);
	}

	template<class List, template<int>class W>struct __all_button_xx__;
	template<class Head, class Tail, template<int>class W>struct __all_button_xx__<Tlst<Head, Tail>, W>
	{
		typedef typename Tlst<W<Head::value>, typename __all_button_xx__<Tail, W>::Result> Result;
	};
	template<template<int>class W>struct __all_button_xx__<NullType, W>
	{
		typedef NullType Result;
	};


	template<class List, class T>struct InList;
	template<class Head, class Tail, class T>struct InList<Tlst<Head, Tail>, T>
	{
		static const bool value = InList<Tail, T>::value;
	};
	template<class Tail, class T>struct InList<Tlst<T, Tail>, T>
	{
		static const bool value = true;
	};
	template<class T>struct InList<NullType, T>
	{
		static const bool value = false;
	};

	template<class List, class SubList, template<int>class on, template<int>class off>struct __all_button_OnOff__;
	template<class Head, class Tail, class SubList, template<int>class on, template<int>class off>struct __all_button_OnOff__<Tlst<Head, Tail>, SubList, on, off>
	{
		typedef typename Tlst<
			typename TL::_if< InList<SubList, Head>::value
			  , on<Head::value>
			  , off<Head::value>
			>::Result
			, typename __all_button_OnOff__<Tail, SubList, on, off>::Result
		> Result;
	};
	template<class SubList, template<int>class on, template<int>class off>struct __all_button_OnOff__<NullType, SubList, on, off>
	{
		typedef NullType Result;
	};

	void Init()
	{		
		__btn__<
			__all_button_OnOff__<
			  button_list
			  , TL::MkTlst<TL::IntToType<IDB_Reset>/*, TL::IntToType<IDB_Ok>, TL::IntToType<IDB_Continue>*/>::Result
			  , Off, On
			>::Result
		>()();
		zprint("\n");
	}

	void Run()
	{		
		__btn__<
			__all_button_OnOff__<
			  button_list
			  , TL::MkTlst<TL::IntToType<IDB_Reset>>::Result
			  , On, Off
			>::Result
		>()();
		zprint("\n");		
	}
	
	void Continue()
	{
		__btn__<
			__all_button_OnOff__<
			  button_list
			  , TL::MkTlst<TL::IntToType<IDB_CycleBtn>, TL::IntToType<IDB_Reset>, TL::IntToType<IDB_Continue>>::Result
			  , On, Off
			>::Result
		>()();
		zprint("\n");
	}
	//void RunContine()
	//{
	//	typedef TL::MkTlst<
	//		On<IDB_CycleBtn>, On<IDB_Ok>
	//	>::Result list;
	//	__btn__<list>()();
	//	zprint("\n");
	//}
	void Stop()
	{
		Init();
		zprint("\n");
	}

	//void ExitTubeMode()
	//{
	//	Run();
	//	zprint("\n");
	//}
	//
	//void ReturnTubeMode()
	//{
	//	Run();
	//	zprint("\n");
	//}

	void VK_(unsigned id)
	{
		MainWindow &mw = Singleton<MainWindow>::Instance();
		TCommand c(mw.hWnd, id, 0, mw.toolBar.hWnd);
		SendMessage(MESSAGE(c));
	}

	template<int id>struct __is_key__{static const unsigned value = 0;};

	template<>struct __is_key__<IDB_CycleBtn   >{static const unsigned value = VK_F4;};
	template<>struct __is_key__<IDB_Continue   >{static const unsigned value = VK_F11;};
	template<>struct __is_key__<IDB_Reset      >{static const unsigned value = VK_ESCAPE;};
	template<>struct __is_key__<IDB_DownArrow >{static const unsigned value = VK_F5;};
	template<>struct __is_key__<IDB_UpArrow   >{static const unsigned value = VK_F6;};
	template<>struct __is_key__<IDB_LeftArrow >{static const unsigned value = VK_F7;};
	//template<>struct __is_key__<IDB_RightArrow>{static const unsigned value = VK_F8;};
	template<>struct __is_key__<IDB_Ok>{static const unsigned value = VK_F8;};

	template<>struct __is_key__<IDB_ExitTubeBtn >{static const unsigned value = VK_F9;};

	template<unsigned id, unsigned key>struct __sub_test_key__
	{
		bool operator()(unsigned k)
		{
			if(key == k)
			{
				if(IsEnabled(id))VK_(id);
				return false;
			}
			return true;
		}
	};
	template<unsigned id>struct __sub_test_key__<id, 0>
	{
		bool operator()(unsigned k){return true;}
	};
	template<class O, class P>struct __test_key__
	{
		bool operator()(P *p)
		{
			return __sub_test_key__<O::value, __is_key__<O::value>::value>()(*p);
		}
	};

	void KeyPressed(unsigned key)
	{
		TL::find<button_list, __test_key__>()(&key);
	}

	//void DisableAllBtn()
	//{
	//	__btn__<
	//		__all_button_OnOff__<
	//		  button_list
	//		  , NullType//TL::MkTlst<TL::IntToType<IDB_Reset>, TL::IntToType<IDB_Continue>>::Result
	//		  , On, Off
	//		>::Result
	//	>()();
	//}
}