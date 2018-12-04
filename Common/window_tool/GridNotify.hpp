#pragma once
//#include "Notify.h"
#include "CommCtrl.h"
#include "templates/typelist.hpp"
#include <strsafe.h>
#include "window_tool\Grid.h"
#include "tools_debug\DebugMess.h"

struct TCellColor
{
	static const unsigned mess = NM_CUSTOMDRAW;
	DWORD_PTR col, row;
	COLORREF &textColor, &bkColor;
	TCellColor(TNotify &m)
		: col(((NMLVCUSTOMDRAW *)m.pnmh)->iSubItem)
		, row(((NMLVCUSTOMDRAW *)m.pnmh)->nmcd.dwItemSpec)
		, textColor(((NMLVCUSTOMDRAW *)m.pnmh)->clrText)
		, bkColor(((NMLVCUSTOMDRAW *)m.pnmh)->clrTextBk)
	{}
};

struct TCellData
{
	static const unsigned mess = LVN_GETDISPINFO;
	int col, row;
	HWND h;
	wchar_t data[128];
	TCellData(TNotify &m)
		: col(((LV_DISPINFO *)m.pnmh)->item.iSubItem)
		, row(((LV_DISPINFO *)m.pnmh)->item.iItem)
		, h(m.hwnd)
	{
		data[0] = 0;
	}
};

struct TLClick
{
	static const unsigned mess = NM_CLICK;
	int col, row;
	TLClick(TNotify &m)
		: col(((NMITEMACTIVATE *)m.pnmh)->iSubItem)
		, row(((NMITEMACTIVATE *)m.pnmh)->iItem)
	{}
};

struct TRClick
{
	static const unsigned mess = NM_RCLICK;
	int col, row;
	TRClick(TNotify &m)
		: col(((NMITEMACTIVATE *)m.pnmh)->iSubItem)
		, row(((NMITEMACTIVATE *)m.pnmh)->iItem)
	{}
};

struct TItemChanged
{
	static const unsigned mess = LVN_ITEMCHANGED;
	int row;
	bool newState;
	bool oldState;
	TItemChanged(TNotify &m)
		: row(((NMLISTVIEW *)m.pnmh)->iItem)
		, newState(0 != ((((LPNMLISTVIEW )m.pnmh)->uNewState) >> 12) - 1)
		, oldState(0 != ((((LPNMLISTVIEW )m.pnmh)->uOldState) >> 12) - 1)
	{}
};

struct NotifyHandler
{
  virtual LRESULT operator()(TNotify &) = 0;
};


template<class T>class GridEventHandler: NotifyHandler
{
	template<class T, T>struct Helper{};
	template<class Z>static char IsMember(...);
	template<class Z>static double IsMember(T *, Helper<void(T::*)(Z &), &T::operator()> * = NULL);

	typedef TL::MkTlst<TCellColor, TCellData, TLClick, TRClick, TItemChanged>::Result all_events;//добавить новые события 

	template<class List, class tmp>struct ExistsEvents;

	template<class Head, class Tail, class tmp>struct ExistsEvents<Tlst<Head, Tail>, tmp>
	{
		typedef typename ExistsEvents<
			Tail
			, typename TL::Append<
			tmp
			, typename TL::_if<sizeof(double) == sizeof(IsMember<Head>((T *)0)), Head, NullType>::Result
			>::Result
		>::Result Result;
	};
	template<class tmp>struct ExistsEvents<NullType, tmp>
	{
		typedef tmp Result;
	};
	typedef typename ExistsEvents<all_events, NullType>::Result event_list;	

	template<class O, class P>struct loc
	{
		bool operator()(P *p)
		{
			if(O::mess == p->mess.pnmh->code)
			{
				(*p->obj)(O(p->mess))
				return false;
			}
			return true;
		}
	};

	template<class P>struct loc<TCellColor, P>
	{
		typedef TCellColor O;
		bool operator()(P *p)
		{
			if(O::mess == p->mess.pnmh->code)
			{				
				switch(((NMLVCUSTOMDRAW*)p->mess.pnmh)->nmcd.dwDrawStage)
				{
				case CDDS_PREPAINT: 
					p->result =  CDRF_NOTIFYITEMDRAW; 
					break;
				case CDDS_ITEMPREPAINT: 
					p->result = CDRF_NOTIFYSUBITEMDRAW; 
					break;
				case CDDS_ITEM | CDDS_SUBITEM | CDDS_PREPAINT:
					(*p->obj)(O(p->mess));			
					p->result = CDRF_NEWFONT;
					break;
				}
				return false;
			}
			return true;
		}
	};

	template<class P>struct loc<TCellData, P>
	{
		typedef TCellData O;
		bool operator()(P *p)
		{			
			if(O::mess == p->mess.pnmh->code)
			{
				O x(p->mess);
				(*p->obj)(x);
				LV_DISPINFO *d = (LV_DISPINFO *)p->mess.pnmh;
                StringCchCopy(d->item.pszText, wcslen(x.data) + 1, x.data);	
				return false;
			}
			return true;
		}
	};

	template<class P>struct loc<TItemChanged, P>
	{
		typedef TItemChanged O;
		bool operator()(O *o, P *p)
		{			
			if(O::mess == p->mess.pnmh->code)
			{
				O x(p->mess);
				(*p->obj)(x);				
				return false;
			}
			return true;
		}
	};

	struct Data
	{
		TNotify &mess;
		T *obj;
		unsigned result;
		Data(TNotify &mess)
			: mess(mess)
			, obj((T *)GetWindowLongPtr(mess.hwnd, GWLP_USERDATA))
			, result(0)
		{}
	};
	template<class List, class T>struct TypeInList;
	template<class Head, class Tail, class T>struct TypeInList<Tlst<Head, Tail>, T>
	{
		static const bool value = TypeInList<Tail, T>::value;
	};
	template<class Tail, class T>struct TypeInList<Tlst<T, Tail>, T>
	{
		static const bool value = true;
	};
	template<class T>struct TypeInList<NullType, T>
	{
		static const bool value = false;
	};
public:
	LRESULT operator()(TNotify &m)
	{
		Data d(m);
		TL::find<event_list, loc>()(&d);
		return d.result;
	}	
	template<class colums_name_list>HWND Init(HWND h, int countRows)
	{
		HWND hWnd = CreateWindowEx(
			WS_EX_CLIENTEDGE
			, WC_LISTVIEW, L"",
			WS_VISIBLE | WS_CHILD | LVS_REPORT 	
			, 0, 0, 0, 0,
			h, NULL, (HINSTANCE)::GetModuleHandle(NULL), NULL
			);
		SetWindowLongPtr(hWnd, GWLP_USERDATA, (LONG)this);
		unsigned param = LVS_EX_GRIDLINES | LVS_EX_FULLROWSELECT | LVS_EX_HEADERDRAGDROP;
		if(TypeInList<event_list, TItemChanged>::value) param |= LVS_EX_CHECKBOXES;
		ListView_SetExtendedListViewStyleEx(hWnd, 0, param);
		SetGridHeader<colums_name_list> x(hWnd);
		SetRow(hWnd, countRows);
		return hWnd;
	}
};