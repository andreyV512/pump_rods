#pragma once

#include "window_tool/message.h"
#include "tools_debug\DebugMess.h"
#include "DlgTemplates\ParamDlg.h"

template<class BaseParam, class TableParam, class List>class TabPage
{
	List &owner;
	HWND hTab;
	bool create;
	template<class O, class P>struct __show__
	{
		void operator()(O *o, P *p)
		{
			ShowItem()(o->hWnd, *p);
		}
	};
	template<class O, class P>struct __init__
	{
		void operator()(O *o, P *p)
		{
			o->Init(p->h, p->width, p->height);	
		}
	};
	struct __table_data__
	{
		HWND h; 
		int &width; 
		int &height;
		__table_data__(HWND h, int &width, int &height): h(h), width(width), height(height){}
	};
public:
	typedef BaseParam Base;
	typedef TableParam Table;
public:
	typedef typename TL::TypeToTypeLst<typename Table::items_list, DlgItem>::Result list;
	TL::Factory<list> items;
	void Show(bool b)
	{
		if(create)
		{
			TL::foreach<list, __show__>()(&items, &b);
		}
		else
		{
			create = true;
			int width = 450;
			int height = 30;
			TL::foreach<list, __init__>()(&items, &__table_data__(owner.hTab, width, height));
		}
	}
	
public:
	TabPage(List &l) 
		: owner(l)
		, create(false)
		, items(l.data.items.get<Table>().items)
	{}	
};

//--------------------------------------------------------------------------------------------------------
#pragma warning(disable: 4355)

//template<class Base, class Table>struct TestPassword;
template<class Base, class Table>struct TestPassword{bool operator()(HWND){return true;}};

struct OkTabBtn
{
	static const int width = 120;
	static const int height = 30;
	static const int ID = IDOK;
	HWND hWnd;
	wchar_t *Title(){return L"Применить";}
	template<class O, class P>struct loc
	{
		bool operator()(O *o, P *p)
		{
			if(p->currentPage == TL::IndexOf<typename P::list, O>::value)
			{
				typedef typename O::Table Table;
				typedef typename O::Base Base;
				zprint("\ncurrent page %d \n%s\n%s\n", p->currentPage, typeid(Table).name(), typeid(p->items.get<O>()).name());
				
				if(TestPassword<Table>()(p->hWnd))
				{
					if(__ok_table_btn__<Base, Table
						, typename TL::SubListFromMultyList<Base::multy_type_list, Table>::Result
					>()(p->hWnd, p->items.get<O>()))
					{
						EndDialog(p->hWnd, TRUE);
					}
				}
				return false;
			}
			return true;
		}
	};
	template<class Owner>void BtnHandler(Owner &owner, HWND h)
	{
		TL::foreach<Owner::list, loc>()(&owner.items, &owner);
	}
};

template<class BaseParam, class TableParam, class ButtonsList = TL::MkTlst<OkTabBtn, CancelBtn>::Result>class TabControl
{
	struct __command_data__
	{
		TMessage &mes;
		TabControl &owner;
		__command_data__(TMessage &mes, TabControl &owner)
			: mes(mes)
			, owner(owner)
		{}
	};
	template<class O, class P>struct __command__
	{
		bool operator()(O *o, P *p)
		{
			if(p->mes.wParam == O::ID)
			{
				o->BtnHandler(p->owner, p->mes.hwnd);
				return false;
			}
			return true;
		}
	};
	template<class List>struct __btn_width__;
	template<class Head, class Tail>struct __btn_width__<Tlst<Head, Tail> >
	{
		static const int value = Head::width + __btn_width__<Tail>::value;
	};
	template<>struct __btn_width__<NullType>
	{
		static const int value = 0;
	};
	struct __make_btn_data__
	{
		int offs;
		int height;
		HWND h;
		__make_btn_data__(int offs, int height, HWND h)
			: offs(offs)
			, height(height)
			, h(h)
		{}
	};
	template<class O, class P>struct __tab_name__
	{
		void operator()(O *, P *hTab)
		{
			TCITEM ti = {0};
			ti.mask = TCIF_TEXT;
			ti.pszText = ParamTitle<O>()();
			ti.cchTextMax = wcslen(ti.pszText);
			int iid = TL::IndexOf<typename Table::items_list, O>::value;
			int res = SendMessage(*hTab, TCM_INSERTITEM, iid, (LPARAM)&ti);
		}
	};
	struct __select_page_data__
	{
		typedef BaseParam Base;
		int id;
		bool show;
		TabControl *self;
	};
	template<class O, class P>struct __items__
	{
		void operator()(O *o, P *p)
		{
			Show(o->hWnd, p->show);
		}
	};
	template<class O, class P>struct __select_page__
	{
		bool operator()(O *, P *p)
		{
			if(p->id == TL::IndexOf<typename Table::items_list, O>::value)
			{
				p->self->items.get<TabPage<typename P::Base, O, TabControl> >().Show(p->show);
				return false;
			}
			return true;
		}
	};
	TL::Factory<ButtonsList> buttons;
	void Show(bool b)
	{
		int iPage = TabCtrl_GetCurSel(hTab);
		if(b) currentPage = iPage;  
		__select_page_data__ data = {iPage, b, this};
		TL::find<typename Table::items_list, __select_page__>()(&data);
	}
public:
	HWND hWnd;
	HWND hTab;
	HWND hParent;
	typedef BaseParam Base;
	typedef TableParam Table;
	Table &data;
	int x, y, width, height;
	int currentPage;
	typedef typename TL::TypeToTypeLstParam2_xx<typename Table::items_list, TabPage, Base, TabControl>::Result list;
	TL::Factory<list> items;
	TabControl(HWND hwnd, Table &t, int w, int h)
		: hParent(hwnd)
		, data(t)
		, items(*this)
		, width(w)
		, height(h)
	{} 
	void Init(HWND h)
	{
		hWnd = h;

		int offs = __btn_width__<ButtonsList>::value + (TL::Length<ButtonsList>::value - 1) * 10;

		offs = (width - offs) / TL::Length<ButtonsList>::value;
		RECT r;
		GetWindowRect(GetDesktopWindow(), &r);

		x = r.left +(r.right - r.left - width) / 2;
		y = r.top +(r.bottom - r.top - height) / 2;
		MoveWindow(h, x, y, width, height, FALSE);
		hTab = CreateWindowEx(0, WC_TABCONTROL, 0,   WS_CHILD | WS_VISIBLE, 
			0, 0, width, height - 70, h, (HMENU)NULL, (HINSTANCE)::GetModuleHandle(NULL), 0);

		TL::foreach<typename Table::items_list, __tab_name__>()((HWND *)&hTab);

		TL::foreach<ButtonsList, __make_btn__>()(&buttons, &__make_btn_data__(offs, height - 65, h));
		((TabControl *)GetWindowLongPtr(h, GWLP_USERDATA))->Show(true);
	}
	void Command(TMessage &h)
	{
		TL::find<ButtonsList, __command__>()(&buttons, &__command_data__(h, *this));
	}
	void Notify(TNotify &l)
	{
		switch (l.pnmh->code)
		{
		case TCN_SELCHANGING:
			{
				((TabControl *)GetWindowLongPtr(l.hwnd, GWLP_USERDATA))->Show(false);
			}
			break;

		case TCN_SELCHANGE:
			{ 
				((TabControl *)GetWindowLongPtr(l.hwnd, GWLP_USERDATA))->Show(true);
			} 
			break;
		}		
	}
	static LRESULT CALLBACK Proc(HWND h, UINT msg, WPARAM wParam, LPARAM lParam)
	{
		switch(msg)
		{
		case WM_COMMAND:
			{
				((TabControl *)GetWindowLongPtr(h, GWLP_USERDATA))->Command((TMessage &)h);
			}
			return TRUE;
		case WM_INITDIALOG:
			{	
				SetWindowLongPtr(h, GWLP_USERDATA, lParam);
				((TabControl *)lParam)->Init(h);				
			}
			return TRUE;
		case WM_NOTIFY:	  
			((TabControl *)lParam)->Notify((TNotify &)h);
			return TRUE;
		}
		return FALSE;
	}
	bool Do(wchar_t *title)
	{
		TemplDlg_Do(hParent, title, (DLGPROC)Proc, (LPARAM)this);
		return true;
	}
};