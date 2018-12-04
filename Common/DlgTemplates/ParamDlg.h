#pragma once
#include "templates/typelist.hpp"
#include "Base/Base.hpp"
#include "App/AppBase.h"
#include "window_tool/message.h"
#include "DlgTemplates\ParamDlg.hpp"
#include "window_tool\Pass.h"
#include "tools_debug\DebugMess.h"
//--------------------------------------------------------------------------------------------------------------------------
bool TemplDlg_Do(HWND hWnd, wchar_t *title, DLGPROC proc, LPARAM param);

template<class O, class P>struct __make_btn__
{
	void operator()(O *o, P *p)
	{
		CreateWindow(L"button", o->Title()
			, WS_VISIBLE | WS_CHILD | WS_TABSTOP
			,p->offs, p->height, O::width, 30, p->h, (HMENU)O::ID, (HINSTANCE)::GetModuleHandle(NULL), NULL
			);
		p->offs += O::width + 10;
	}
};
template<class O, class P>struct TemplDialogCtlColorEdit 
{
	bool operator()(O *, P *)
	{
		return true;
	}
};
template<class O, class P>struct __command__
{
	bool operator()(O *o, P *p)
	{
		if(p->id == O::ID)
		{
			o->BtnHandler(p->owner, p->hwnd);
			return false;
		}
		return true;
	}
};
/*
template<class BaseParam, class TableParam, template<class>class DlgItem, class ButtonsList = TL::MkTlst<OkBtn, CancelBtn>::Result>class TemplDialog
{
	struct __command_data__
	{
		HWND hwnd;
		WPARAM id;
		TemplDialog &owner;
		__command_data__(HWND hwnd, WPARAM id, TemplDialog &owner)
			: hwnd(hwnd)
			, id(id)
			, owner(owner)
		{}
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

	static LRESULT CALLBACK Proc(HWND h, UINT msg, WPARAM wParam, LPARAM lParam)
	{
		switch(msg)
		{
		case WM_COMMAND:
			{
				TemplDialog *e = (TemplDialog *)GetWindowLongPtr(h, GWLP_USERDATA);	
				if(!TL::find<ButtonsList, __command__>()(&e->buttons
					, &__command_data__(h, wParam, *e))
					) return TRUE;
			}
			break;
		case WM_INITDIALOG:
			{			
				SetWindowLongPtr(h, GWLP_USERDATA, lParam);
				TemplDialog *e = (TemplDialog *)lParam;
				int width = 550;
				int height = 10;
				TL::foreach<list, __init__>()(&e->items, &__table_data__(h, width, height));
				int offs = __btn_width__<ButtonsList>::value + (TL::Length<ButtonsList>::value - 1) * 10;

				offs = (width - offs) / 2;
				height += 15;

				TL::foreach<ButtonsList, __make_btn__>()(&e->buttons, &__make_btn_data__(offs, height, h));

				RECT r;
				GetWindowRect(GetDesktopWindow(), &r);

				height += 65;
				int x = r.left +(r.right - r.left - width) / 2;
				int y = r.top +(r.bottom - r.top - height) / 2;
				MoveWindow(h, x, y, width, height, FALSE);
			}
			return TRUE;
		}
		return FALSE;
	}
protected:
public:
	typedef BaseParam Base;
	typedef TableParam Table;
	Table &table;	
	typedef typename TL::TypeToTypeLst<typename Table::items_list, DlgItem>::Result list;
	TL::Factory<list> items;
	TL::Factory<ButtonsList> buttons;
public:
	TemplDialog(Table &table_) : table(table_), items(table_.items){}
	bool Do(HWND hWnd, wchar_t *title)
	{
		return TemplDlg_Do(hWnd, title, (DLGPROC)Proc, (LPARAM)this);
	}
};
*/
template<class, class Table>struct TestPassword;

struct OkBtn
{
	static const int width = 120;
	static const int height = 30;
	static const int ID = IDOK;
	wchar_t *Title(){return L"Применить";}
	template<class Owner>void BtnHandler(Owner &owner, HWND h)
	{
		if(TestPassword<Owner::Base, Owner::Table>()(h))
		{
			if(__ok_table_btn__<
				Owner::Base, Owner::Table
				, typename TL::SubListFromMultyList<typename Owner::Base::multy_type_list, Owner::Table>::Result
			>()(h, owner))  
			{
				EndDialog(h, TRUE);
			}
		}
	}
};

struct CancelBtn
{
	static const int width = 120;
	static const int height = 30;
	static const int ID = IDCANCEL;
	wchar_t *Title(){return L"Закрыть";}
	template<class Owner>void BtnHandler(Owner &o, HWND h)
	{
		EndDialog(h, FALSE);
	}
};

template<class Table>struct TestPassword<ParametersBase, Table>
{
	bool operator()(HWND h)
	{
		static const int t = TL::TypeInMultyList<ParametersBase::multy_type_list, Table>::Result::value;
		return (0 == t)
			? TypesizePasswordDlg().Do(h)
			: OptionPasswordDlg().Do(h);
	}
};
/*
//-------------------------------------------------------------------
template<class BaseParam, class TableParam, class List, class ButtonsList = TL::MkTlst<OkBtn, CancelBtn>::Result>class TemplDialogList
{
	struct __command_data__
	{
		HWND hwnd;
		WPARAM id;
		TemplDialogList &owner;
		__command_data__(HWND hwnd, WPARAM id, TemplDialogList &owner)
			: hwnd(hwnd)
			, id(id)
			, owner(owner)
		{}
	};
	template<class O, class P>struct __command__
	{
		bool operator()(O *o, P *p)
		{
			if(p->id == O::ID)
			{
				o->BtnHandler(p->owner, p->hwnd);
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

	static LRESULT CALLBACK Proc(HWND h, UINT msg, WPARAM wParam, LPARAM lParam)
	{
		switch(msg)
		{
		case WM_COMMAND:
			{
				TemplDialogList *e = (TemplDialogList *)GetWindowLongPtr(h, GWLP_USERDATA);	
				if(!TL::find<ButtonsList, __command__>()(&e->buttons
					, &__command_data__(h, wParam, *e))
					) return TRUE;
			}
			break;
		case WM_INITDIALOG:
			{			
				SetWindowLongPtr(h, GWLP_USERDATA, lParam);
				TemplDialogList *e = (TemplDialogList *)lParam;
				int width = 450;
				int height = 10;
				TL::foreach<list, __init__>()(&e->items, &__table_data__(h, width, height));
				int offs = __btn_width__<ButtonsList>::value + (TL::Length<ButtonsList>::value - 1) * 10;

				offs = (width - offs) / 2;
				height += 15;

				TL::foreach<ButtonsList, __make_btn__>()(&e->buttons, &__make_btn_data__(offs, height, h));

				RECT r;
				GetWindowRect(GetDesktopWindow(), &r);

				height += 65;
				int x = r.left +(r.right - r.left - width) / 2;
				int y = r.top +(r.bottom - r.top - height) / 2;
				MoveWindow(h, x, y, width, height, FALSE);
			}
			return TRUE;
		}
		return FALSE;
	}
protected:
public:
	typedef BaseParam Base;
	typedef TableParam Table;
	Table &table;	
	typedef typename TL::TypeToTypeLst<List, DlgItem>::Result list;
	TL::Factory<list> items;
	TL::Factory<ButtonsList> buttons;
public:
	TemplDialogList(Table &table_) : table(table_), items(table_.items){}
	bool Do(HWND hWnd, wchar_t *title)
	{
		return TemplDlg_Do(hWnd, title, (DLGPROC)Proc, (LPARAM)this);
	}
};

*/


