#pragma once
#include "DlgTemplates\ParamDlg.h"
#include <windowsx.h>
#include<clocale>
#include <CommCtrl.h>
#include "Base/tables.hpp"
#include "tools_debug\DebugMess.h"
 
//-----------------------------------------------------------------------
template<class T>struct ParamTitle
{
	wchar_t *operator()()
	{
		zprint(" ParamTitle\n");
		return L"Error";
	}
};
//{
//	typedef typename T::__ParamTitle_template_must_be_overridded__ noused;
//};

template<class T, class X>struct DlgSubItems
{
	typedef typename T::__DlgSubItems_template_must_be_overridded__ noused;
};

template<class T>struct DlgItem
{
	T &value;
	HWND hWnd;
	template<class Z>DlgItem(Z &z) : value(z.get<T>()){}
	void Init(HWND h, int &x, int &width, int &dy)
	{
		hWnd = DlgSubItems<T, typename T::type_value>().Init(h, x, width, dy, value);
	}
};
//-------------------------------------------------------------------------------------------------
template<class T>struct __update_data__
{
	Update<T> update;
	explicit __update_data__(CBase &base): update(base) {}
};
template<class O, class T>struct __data_from_widget__
{
	typedef typename T::__data_from_widget____template_must_be_overridded__ noused;
};
template<class O>struct __data_from_widget__<O, int>
{
	typedef int T;
    T operator()(O &o)
	{
		wchar_t buf[128];
		GetWindowText(o.hWnd, buf, dimention_of(buf));
		return Wchar_to<T>()(buf);
	}
};
template<class O>struct __data_from_widget__<O, unsigned>
{
	typedef unsigned T;
    T operator()(O &o)
	{
		wchar_t buf[128];
		GetWindowText(o.hWnd, buf, dimention_of(buf));
		return Wchar_to<T>()(buf);
	}
};
template<class O>struct __data_from_widget__<O, double>
{
	typedef double T;
    T operator()(O &o)
	{
		wchar_t buf[128];
		GetWindowText(o.hWnd, buf, dimention_of(buf));
		return Wchar_to<T>()(buf);
	}
};
template<class O>struct __data_from_widget__<O, bool>
{
    bool operator()(O &o)
	{
		 return  BST_CHECKED == Button_GetCheck(o.hWnd);
	}
};
template<class O, int N>struct __data_from_widget__<O, Holder<N> >
{
	typedef Holder<N> T;
	T t;
    T &operator()(O &o)
	{
		GetWindowText(o.hWnd, t.buffer, dimention_of(t.buffer));
	    return t;
	}
};
template<class O, class P>struct __ok_btn__
{
	void operator()(O *o, P *p)
	{
        o->value.value =  __data_from_widget__<O, typename TL::Inner<O>::Result::type_value>()(*o);
		p->update.set<typename TL::Inner<O>::Result>(o->value.value);
	}
};

template<class O, class Z>struct Skip
{
	typedef typename TL::Inner<O>::Result T;
	template<class P>bool operator()(O *o, P *p)
	{
		wchar_t buf[128];
		GetWindowText(o->hWnd, buf, dimention_of(buf));
		typename T::type_value t = Wchar_to<typename T::type_value>()(buf);
		typename T::type_value tt = t;
		bool res = allowable_limit_on_the_parameter<T>()(t, *p);
		if(res && tt != t)
		{
			SetWindowText(o->hWnd, Wchar_from<typename T::type_value>(t)());
		}
		return res;
	}
};

template<class O>struct Skip<O, bool>
{
	template<class P>bool operator()(O *o, P *p){return true;}
};

template<class T>struct Less;//{typedef typename T::__template_must_be_overridded__ noused;};
template<class T>struct Largen;//{typedef typename T::__template_must_be_overridded__ noused;};
template<class T>struct LessEqual;//{typedef typename T::__template_must_be_overridded__ noused;};
template<class T>struct LargenEqual;//{typedef typename T::__template_must_be_overridded__ noused;};

struct __LessText__
{
	wchar_t *operator()(){return L"больше";};
};
struct __LargenText__
{
	wchar_t *operator()(){return L"меньше";};
};
struct __LessEqualText__
{
	wchar_t *operator()(){return L"больше или равно";};
};
struct __LargenEqualText__
{
	wchar_t *operator()(){return L"меньше или равно";};
};
template<class T>struct ErrMessText;
template<class T>struct ErrMessText<Less<T> >:__LessText__{};
template<class T>struct ErrMessText<Largen<T> >:__LargenText__{};
template<class T>struct ErrMessText<LessEqual<T> >:__LessEqualText__{};
template<class T>struct ErrMessText<LargenEqual<T> >:__LargenEqualText__{};

template<class min_type, class max_type, class T>struct SubErrMess
{
	void operator()(wchar_t (&buf)[256])
	{
		wsprintf(buf, L"Параметр \"%s\" должен быть %s %s и %s %s"
			, ParamTitle<T>()()
			, ErrMessText<min_type>()()
			, Wchar_from<T::type_value>(min_type()())()
			, ErrMessText<max_type>()()
			, Wchar_from<T::type_value>(max_type()())()
			);
	}
};

template<class T>struct SubErrMess<NullType, NullType, T>
{
	void operator()(wchar_t (&buf)[256])
	{
		const char *s = typeid(T).name();
		mbstowcs(buf, s, strlen(s));
	}
};

template<class T>struct ErrMess
{
	void operator()(typename T::type_value &t, HWND h)
	{
        wchar_t buf[256];
		typedef typename TL::TypeExist<typename TL::MkTlst<Less<T>, LessEqual<T> >::Result>::Result min_type;
		typedef typename TL::TypeExist<typename TL::MkTlst<Largen<T>, LargenEqual<T> >::Result>::Result max_type;
		SubErrMess<min_type, max_type, T>()(buf);
		MessageBox(h, buf, L"Ошибка!!!", MB_ICONEXCLAMATION);
	}
};

template<class _0, class _1, class Z>struct __compare_param__;

template<class T>struct __compare_param__<Less<T>, Largen<T>, T>
{
	bool operator()(typename T::type_value &t)
	{
		return t > Less<T>()() && t < Largen<T>()();
	}
};
template<class T>struct __compare_param__<LessEqual<T>, Largen<T>, T>
{
	bool operator()(typename T::type_value &t)
	{
		return t >= LessEqual<T>()() && t < Largen<T>()();
	}
};
template<class T>struct __compare_param__<Less<T>, LargenEqual<T>, T>
{
	bool operator()(typename T::type_value &t)
	{
		return t > Less<T>()() && t <= LargenEqual<T>()();
	}
};
template<class T>struct __compare_param__<LessEqual<T>, LargenEqual<T>, T>
{
	bool operator()(typename T::type_value &t)
	{
		return t >= LessEqual<T>()() && t <= LargenEqual<T>()();
	}
};

template<class T>struct allowable_limit_on_the_parameter
{
	bool operator()(typename T::type_value &t, HWND h)
	{
		if(__compare_param__<
			typename TL::TypeExist<typename TL::MkTlst<Less<T>, LessEqual<T> >::Result>::Result
			, typename TL::TypeExist<typename TL::MkTlst<Largen<T>, LargenEqual<T> >::Result>::Result
			, T
		>()(t)) return true;
		/**********************************************************************************************//*
		 * \brief	Если попал сюда то определи
		 * 	MIN_VALUE(какойто_тип_из_AppBase_h, минимальная_величина)
		 *  MAX_VALUE(какойто_тип_из_AppBase_h, максимальная_величина)
		 *  MIN_EQUAL_VALUE(...
		 *  MIN_EQUAL_VALUE(...
		 *  DO_NOT_CHECK(...
		 *  смотри определение макровов ниже в файле
		 **************************************************************************************************/

		ErrMess<T>()(t, h);
		return false;
	}	
};

template<class O, class P>struct __test__
{
	bool operator()(O *o, P *p)
	{
		return Skip<O, typename TL::Inner<O>::Result::type_value>()(o, p);
	}
};

template<class Base, class Table, class T>struct __ok_table_btn__
{
	typedef typename T::__template_must_be_overridded__ noused; 
};

template<class Base, class Table>struct __ok_table_btn__<Base, Table, ParametersBase::one_row_table_list>
{
	template<class T>bool operator()(HWND h, T &t)
	{
		if(!TL::find<T::list, __test__>()(&t.items, &h))return false;
		CBase base(Base().name());
		if(base.IsOpen())
		{
			__update_data__<Table> _data(base);
			TL::foreach<T::list, __ok_btn__>()(&t.items, &_data);
			_data.update.Where().ID(1).Execute();
		}
		return true;
	}
};
template<class, class, template<class>class, class>class TemplDialog;

template<class Base, class Table, class B>struct Insert
{
	template<class Z>void operator()(Z &t, CBase &base)
	{
		Insert_Into<Table>(t.table, base).Execute();
		int id = Select<Table>(base).eq_all<Table::items_list>(&t.table.items).Execute();
		UpdateId<ID<Table> >(base, id);
	}
};

template<class Base, class Table>struct __ok_table_btn__<Base, Table, typename Base::multy_row_table_list>
{
	template<class T>bool operator()(HWND h, T &t)
	{
		if(!TL::find<typename T::list, __test__>()(&t.items, &h))return false;
		CBase base(Base().name());
		if(base.IsOpen())
		{
			int id = CurrentId<ID<Table> >();	
			__update_data__<Table> _data(base);			
			TL::foreach<typename T::list, __ok_btn__>()(&t.items, &_data);
			if(1 == CountId<ID<Table> >(base, id))
			{
				_data.update.Where().ID(id).Execute();
			}
			else
			{
				Insert<Base, Table, T>()(t, base);
			}
		}
		return true;
	}
};
//-------------------------------------------------
template<class O, class P>struct __init__
{
	void operator()(O *o, P *p)
	{
	   o->Init(p->h, p->x, p->width, p->height);	
	}
};
struct __table_data__
{
   HWND h; 
   int &x;
   int &width; 
   int &height;
   __table_data__(HWND h, int &x, int &width, int &height): h(h), x(x), width(width), height(height){}
};
//////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////
static const int dlg_width = 600;
//-------------------------------------------------------------------------
template<class T>struct DlgSubItems<T, bool>
{
	static const int DY = 20;
	HWND Init(HWND h, int &x, int &width, int &dy, T &t)
	{
		HWND hWnd = CreateWindow(L"button", ParamTitle<T>()()
				,  WS_CHILD | WS_VISIBLE | BS_AUTOCHECKBOX | WS_TABSTOP
				, x, dy, width - 15, 15, h, NULL, (HINSTANCE)::GetModuleHandle(NULL), NULL
				);
		Button_SetCheck(hWnd, t.value ? BST_CHECKED : BST_UNCHECKED);
		dy += DY;
		return hWnd;
	}
};
//------------------------------------------------------------------------------
template<class T>struct DlgItemsRadio
	{
		static const int DY = 20;
		HWND Init(HWND h, int &x, int &width, int &dy, T &t)
		{
			HWND hWnd = CreateWindow(L"button", ParamTitle<T>()()
				,  WS_CHILD | WS_VISIBLE | BS_RADIOBUTTON
				, x, dy, width - 15, 15, h, NULL, (HINSTANCE)::GetModuleHandle(NULL), NULL
				);
			Button_SetCheck(hWnd, t.value ? BST_CHECKED : BST_UNCHECKED);
			dy += DY;
			return hWnd;
		}
	};

template<class T, int edit_width = 140>struct EditItems
{
	static const int DY = 25;
	HWND Init(HWND h, int &x, int &width, int &dy, T &t)
	{
		HWND hWnd = CreateWindowEx(WS_EX_CLIENTEDGE, L"edit", Wchar_from<typename T::type_value>(t.value)()
		, WS_BORDER | WS_VISIBLE | WS_CHILD | ES_LEFT | WS_TABSTOP
			, x, dy, edit_width, 25, h, 0, (HINSTANCE)::GetModuleHandle(NULL), NULL
			);
		CreateWindow(L"static", ParamTitle<T>()()
			, WS_VISIBLE | WS_CHILD
			, edit_width + 2 * x, dy + 3, width - edit_width - 6 * x, 20, h, 0, (HINSTANCE)::GetModuleHandle(NULL), NULL
			);
		dy += DY;
		return hWnd;
	}
};

template<class T, int edit_width = 140>struct EditReadOnlyItems
{
	static const int DY = 25;
	HWND Init(HWND h, int &x, int &width, int &dy, T &t)
	{
		HWND hWnd = CreateWindowEx(WS_EX_CLIENTEDGE, L"edit", Wchar_from<typename T::type_value>(t.value)()
		, WS_BORDER | WS_VISIBLE | WS_CHILD | ES_LEFT | WS_TABSTOP | ES_READONLY
			, x, dy, edit_width, 25, h, 0, (HINSTANCE)::GetModuleHandle(NULL), NULL
			);
		CreateWindow(L"static", ParamTitle<T>()()
			, WS_VISIBLE | WS_CHILD
			, edit_width + 2 * x, dy + 3, dlg_width, 20, h, 0, (HINSTANCE)::GetModuleHandle(NULL), NULL
			);
		dy += DY;
		return hWnd;
	}
};

template<class T, int edit_width = 140>struct NOPItems
{
	static const int DY = 0;
	HWND Init(HWND h, int &width, int &dy, T &t)
	{
		return 0;
	}
};
template<class T>struct MultyEditItems
{
	static const int DY = 125;
	HWND Init(HWND h, int &x, int &width, int &dy, T &t)
	{
		static const int edit_width = 420;
		HWND hWnd = CreateWindowEx(WS_EX_CLIENTEDGE, L"edit", ParamTitle<T>()()
		, WS_BORDER | WS_VISIBLE | WS_CHILD | ES_LEFT | WS_TABSTOP | ES_MULTILINE | ES_AUTOVSCROLL 
			, x, dy, edit_width, 120, h, 0, (HINSTANCE)::GetModuleHandle(NULL), NULL
			);
		dy += DY;
		return hWnd;
	}
};
template<class T>struct DateTimeItem
{
	static const int DY = 32;
	HWND Init(HWND h, int &x, int &width, int &dy, T &t)
	{
		static const int edit_width = 420;
		HWND hWnd = CreateWindowEx(0,                  //extended styles
			DATETIMEPICK_CLASS, //control 'class' name
			0,                  //control caption
			WS_CHILD|WS_VISIBLE|DTS_LONGDATEFORMAT| WS_TABSTOP,            //wnd style
			x,            //position: left
			dy,             //position: top
			140,           //width
			30,          //height
			h,            //parent window handle
			0,
			(HINSTANCE)::GetModuleHandle(NULL),              //instance
			0); 
		dy += 32;
		return hWnd;
	}
};

template<class T>struct DlgSubItems<T, int>: EditItems<T>{};
template<class T>struct DlgSubItems<T, unsigned>: EditItems<T>{};
template<class T>struct DlgSubItems<T, double>: EditItems<T>{};
template<class T, int N>struct DlgSubItems<T, Holder<N> >: EditItems<T>{};
//----------------------------------------------------------------------------
template<class T>struct CurrentValue
{
	void operator()(HWND h, T &t)
	{
		ComboBox_SetCurSel(h, 1);
	}
};
template<class T>struct FillComboboxList
{
	void operator()(HWND h, T &t)
	{
		ComboBox_AddString(h, L"1.0");
		ComboBox_AddString(h, L"2.0");
		ComboBox_AddString(h, L"5.0");
	}
};
template<class T>struct ComboBoxSubItem
{
	static const int DY = 30;
	HWND Init(HWND h, int &x, int &width, int &dy, T &t)
	{
		HWND hWnd = CreateWindow(L"COMBOBOX", NULL
			, WS_CHILD | WS_VISIBLE | CBS_DROPDOWNLIST | CBS_AUTOHSCROLL |WS_VSCROLL 
			, x, dy, 140, 125, h, 0, (HINSTANCE)::GetModuleHandle(NULL), 0
			);
		CreateWindow(L"static", ParamTitle<T>()()
			, WS_VISIBLE | WS_CHILD
			, 140 + 2 * x, dy + 3, dlg_width, 20, h, 0,  (HINSTANCE)::GetModuleHandle(NULL), NULL
			);
		dy += DY;		
	//	if(width < dlg_width) width = dlg_width;
		FillComboboxList<T>()(hWnd, t);
		CurrentValue<T>()(hWnd, t);
		return hWnd;
	}
};
template<class T>struct ComboEditSubItem
{
	static const int DY = 125;
	HWND Init(HWND h, int &x, int &width, int &dy, T &t)
	{
		static const int width_item = 250;
		HWND hWnd = CreateWindow(L"COMBOBOX", NULL
			, WS_CHILD | WS_VISIBLE | CBS_SIMPLE |CBS_AUTOHSCROLL |WS_VSCROLL 
			, x, dy, width_item, 125, h, 0, (HINSTANCE)::GetModuleHandle(NULL), 0
			);
		CreateWindow(L"static", ParamTitle<T>()()
			, WS_VISIBLE | WS_CHILD
			, width_item + 2 * x, dy + 3, dlg_width, 20, h, 0,  (HINSTANCE)::GetModuleHandle(NULL), NULL
			);
		dy += DY;		
		if(width < dlg_width) width = dlg_width;
		FillComboboxList<T>()(hWnd, t);
		CurrentValue<T>()(hWnd, t);
		return hWnd;
	}
};
//------------------------------------------------------------------------------
template<class T,  int min = 0, int max = 31, int edit_width = 60>struct UpDownSubItem
{
	static const int DY = 30;
	HWND Init(HWND h, int &x, int &width, int &dy, T &t)
	{
		HWND hWnd = CreateWindowEx(WS_EX_CLIENTEDGE, L"edit", Wchar_from<typename T::type_value>(t.value)()
			,  WS_CHILD | WS_VISIBLE
			, x, dy, edit_width, 25, h, 0, (HINSTANCE)::GetModuleHandle(NULL), NULL
			);

		HWND hUpdown = CreateWindowEx(0, UPDOWN_CLASS, 0, 
			UDS_ALIGNRIGHT | UDS_SETBUDDYINT | UDS_WRAP | WS_CHILD | WS_VISIBLE, 
			0, 0, 0, 0,
			h, 0, (HINSTANCE)::GetModuleHandle(NULL), NULL);
	    SetWindowLongPtr(hWnd, GWLP_USERDATA, (LONG)hUpdown);
		CreateWindow(L"static", ParamTitle<T>()()
			, WS_VISIBLE | WS_CHILD
			, edit_width + 2 * x, dy + 3, width - edit_width - 2 * x, 20, h, 0, (HINSTANCE)::GetModuleHandle(NULL), NULL
			);
		SendMessage(hUpdown, UDM_SETBUDDY, (WPARAM)hWnd, 0);
		SendMessage(hUpdown, UDM_SETRANGE, 0, (LPARAM) MAKELONG((short)max, (short)min));
		dy += DY;
		return hWnd;
	}
};
//------------------------------------------------------------------------------------
template<class T>struct EmptySubItem
{
	HWND Init(HWND h, int &width, int &dy, T &t)
	{		
		return 0;
	}
};
struct ShowItem
{
	void operator()(HWND h, bool b)
	{
		ShowWindow(h, b ? SW_SHOW : SW_HIDE);
		while(0 != (h = (HWND)GetWindowLongPtr(h, GWLP_USERDATA)))
		{
			ShowWindow(h, b ? SW_SHOW : SW_HIDE);
		}
	}
};
//---------------------------------------------------------------------------
//////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////
#define MIN_VALUE(sub_type, value)template<>struct Less<sub_type>\
{sub_type::type_value operator()(){return value;}};

#define MIN_EQUAL_VALUE(sub_type, value)template<>struct LessEqual<sub_type>\
{sub_type::type_value operator()(){return value;}};

#define MAX_VALUE(sub_type, value)template<>struct Largen<sub_type>\
{sub_type::type_value operator()(){return value;}};

#define MAX_EQUAL_VALUE(sub_type, value)template<>struct LargenEqual<sub_type>\
{sub_type::type_value operator()(){return value;}};

#define PARAM_TITLE(type, name)template<>struct ParamTitle<type>\
{wchar_t *operator()(){return name;}};

#define TEMPL_MIN_EQUAL_VALUE(sub_type, value)template<template<class>class W>struct LessEqual<W<sub_type>>\
{typename W<sub_type>::type_value operator()(){return value;}};
#define TEMPL_MAX_EQUAL_VALUE(sub_type, value)template<template<class>class W>struct LargenEqual<W<sub_type>>\
{typename W<sub_type>::type_value operator()(){return value;}};
#define TEMPL_PARAM_TITLE(type, name)template<template<class>class W>struct ParamTitle<W<type>>{wchar_t *operator()(){return name;}};

//#define CHECK_EMPTY_STRING(n)template<template<class>class Wapper, class Z>struct Skip<Wapper<n>, Z>\
//{\
//	template<class P>bool operator()(Wapper<n> *o, P *)\
//    {\
//	  wchar_t buf[128];\
//	  GetWindowText(o->hWnd, buf, dimention_of(buf));\
//	  if('\0' == buf[0])\
//      {  \
//	     wsprintf(buf, L"Поле данных \"%s\" должно быть заполнено", ParamTitle<n>()());\
//		 MessageBox(o->hWnd, buf, L"Ошибка!!!", MB_ICONEXCLAMATION);\
//		 return false;\
//      } \
//      return true;\
//    }\
//};

#define CHECK_EMPTY_STRING(n)template<template<class, class>class Wapper, class X, class Z>struct Skip<Wapper<n, X>, Z>\
{\
	template<class P>bool operator()(Wapper<n, X> *o, P *)\
    {\
	  wchar_t buf[128];\
	  GetWindowText(o->hWnd, buf, dimention_of(buf));\
	  if('\0' == buf[0])\
      {  \
	     wsprintf(buf, L"Поле данных \"%s\" должно быть заполнено", ParamTitle<n>()());\
		 MessageBox(o->hWnd, buf, L"Ошибка!!!", MB_ICONEXCLAMATION);\
		 return false;\
      } \
      return true;\
    }\
};



//#define DO_NOT_CHECK(n)template<template<class>class Wapper, class Z>struct Skip<Wapper<n>, Z>\
//{\
//	template<class P>bool operator()(Wapper<n> *o, P *)\
//    {\
//      return true;\
//    }\
//};

#define DO_NOT_CHECK(n)template<>struct __compare_param__<NullType, NullType, n>\
{\
	bool operator()(n::type_value &t)\
	{\
		return true;\
	}\
};

#define NO_USED_MENU_ITEM(name)template<>struct DlgSubItems<name, typename name::type_value>: EmptySubItem<name>{};

