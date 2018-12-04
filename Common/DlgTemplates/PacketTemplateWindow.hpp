#pragma once

#include "tools_debug/DebugMess.h"
#include "DlgTemplates/ViewersMenu.hpp"
#include "window_tool\TEvent.h"
#include "DlgTemplates\GridWindow.h"
#include "window_tool\Grid.h"
#include "GridData/PacketTemplateData.hpp"
#include "DlgTemplates\PacketTemplateWindow.hpp"
#include "window_tool\Emptywindow.h"

template<class Base, class Table, class Col>class PacketTemplateWindow
{
public:
	class OkBtn: public TEvent
	{
	public:
		PacketTemplateWindow &owner;
		OkBtn(PacketTemplateWindow &);
		void Do(TCommand &);
	} okBtn;
	class CancelBtn: public TEvent
	{
	public:
		PacketTemplateWindow &owner;
		CancelBtn(PacketTemplateWindow &);
		void Do(TCommand &);
	} cancelBtn;
private:
	GridWindow gridViewer;
public:
	GridNotify grid;
public:
	PacketTemplateData<Base, Table, Col> data;
public:
	HWND hWnd, hOk, hCancel, hResult;
	wchar_t result[128];
	PacketTemplateWindow();
	void operator()(TSize &);
	void operator()(TCommand &);
	LRESULT operator()(TCreate &);
	void operator()(TDestroy &);
	LRESULT operator()(TNotify &);

	void RClick(int, wchar_t *);
	void LClick(int, wchar_t *);
	static wchar_t *Title();
	static void Show(HWND SendToHWND);
};

template<class T>struct OpenWindowBackGround
{
	static HWND Do(HWND)
	{
		HWND h = FindWindow(WindowClass<T>()(), 0);
		if(NULL != h)
		{
			SendMessage(h, WM_SYSCOMMAND, SC_RESTORE, 0);
			SetForegroundWindow(h);
		}
		else
		{
			RECT r;
			WindowPosition::Get<T>(r);
			h = WindowTemplate(new T, T::Title(), r.left, r.top, r.right, r.bottom, IDI_UPLOAD, (HBRUSH)COLOR_WINDOW);
			ShowWindow(h, SW_SHOWNORMAL);				
		}
		return h;
	}
};

template<class Base, class Table, class Col>void PacketTemplateWindow<Base, Table, Col>::Show(HWND SendToHWND)
{
	typedef PacketTemplateWindow<Base, Table, Col> Window;
	HWND hh = OpenWindowBackGround<Window>::Do(SendToHWND);
	Window *x = (Window *)GetWindowLongPtr(hh, GWLP_USERDATA);
	x->hResult = SendToHWND;
	SetWindowPos(hh, HWND_TOPMOST, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);
	LONG_PTR dwStyle = GetWindowLongPtr(hh, GWL_STYLE);
	dwStyle &= ~(WS_MAXIMIZEBOX | WS_MINIMIZEBOX);
	SetWindowLongPtr(hh, GWL_STYLE, dwStyle);
}

#pragma warning(disable : 4355)
template<class Base, class Table, class Col>PacketTemplateWindow<Base, Table, Col>::PacketTemplateWindow()
	: okBtn(*this)
	, cancelBtn(*this)
{}  

template<class Base, class Table, class Col>void PacketTemplateWindow<Base, Table, Col>::operator()(TSize &l)
{
	if(l.resizing == SIZE_MINIMIZED || 0 == l.Width || 0 == l.Height) return;
	int y = l.Height - 50;
	grid.Size(0, 0, l.Width, y);

	int width = (l.Width - 30) / 2;

	MoveWindow(hOk, 10, y + 10, width, 30, false);
	MoveWindow(hCancel, 20 + width, y + 10, width, 30, false);
}

template<class Base, class Table, class Col>void PacketTemplateWindow<Base, Table, Col>::operator()(TCommand &l)
{
	EventDo(l);
}	

namespace		
{															
	struct MainFile{};										
	MENU_TEXT(L"Файл", TopMenu<MainFile>)					

	struct MainExit{static void Do(HWND h){DestroyWindow(h);}};

	MENU_ITEM(L"Выход", MainExit)							   

		template<>struct TopMenu<MainFile>						   
	{														   
		typedef TL::MkTlst<									   
			MenuItem<MainExit>								   
		>::Result list;										   
	};														   

	template<class Base, class Table, class Col>struct Options{};										   
	template<class Base, class Table, class Col>struct NameMenu<TopMenu<Options<Base, Table, Col>> >{wchar_t *operator()(HWND){return L"Настройки";}};

	template<class Base, class Table, class Col>struct NameMenu<MenuItem<Options<Base, Table, Col>> >{wchar_t *operator()(HWND){return L"Сохранить координаты окна";}};
	template<class Base, class Table, class Col>struct Event<MenuItem<Options<Base, Table, Col>> >:WindowPositionDlg<PacketTemplateWindow<Base, Table, Col>>{};

	template<class Base, class Table, class Col>struct TopMenu<Options<Base, Table, Col>>						   
	{														   
		typename typedef TL::MkTlst<									   
			MenuItem<Options<Base, Table, Col>>						   
		>::Result list;										   
	};														   								   
}

template<class Base, class Table, class Col>LRESULT PacketTemplateWindow<Base, Table, Col>::operator()(TCreate &l)
{
	typedef TL::MkTlst<	
		TopMenu<MainFile>	
		, TopMenu<Options<Base, Table, Col>>	
	>::Result MainMenu;	
	result[0] = '\0';
	hResult = NULL;
	Menu<MainMenu>().Init(l.hwnd);
	gridViewer.SetRClick(this, &PacketTemplateWindow::RClick);
	gridViewer.SetLClick(this, &PacketTemplateWindow::LClick);
	gridViewer.SetData(&data);
	grid.Create(l.hwnd, &gridViewer);
	SetRow(grid.hWnd, data.Count());

	hOk = CreateWindow(L"button", L"Применить"
					, WS_VISIBLE | WS_CHILD | WS_TABSTOP
					,0, 0, 0, 0, l.hwnd, NULL, (HINSTANCE)::GetModuleHandle(NULL), NULL
					);
	SetWindowLongPtr(hOk, GWLP_USERDATA, (LONG)&okBtn);

	hCancel = CreateWindow(L"button", L"Отмена"
					, WS_VISIBLE | WS_CHILD | WS_TABSTOP
					,0, 0, 0, 0, l.hwnd, NULL, (HINSTANCE)::GetModuleHandle(NULL), NULL
					);
	SetWindowLongPtr(hCancel, GWLP_USERDATA, (LONG)&cancelBtn);

	return 0;
}

template<class Base, class Table, class Col>void PacketTemplateWindow<Base, Table, Col>::operator()(TDestroy &l)
{
	SetWindowLongPtr(l.hwnd, GWLP_USERDATA, NULL);
	delete this;	
}

template<class Base, class Table, class Col>LRESULT PacketTemplateWindow<Base, Table, Col>::operator()(TNotify &l)
{
	return grid.Do(l);
}

template<class Base, class Table, class Col>void PacketTemplateWindow<Base, Table, Col>::LClick(int col, wchar_t *s)
{
	wcsncpy(result, s, dimention_of(result));
}

namespace
{
	template<class Base, class Table, class Col>struct AddItem{};
	template<class Base, class Table, class Col>struct DelItem{};

	template<class Base, class Table, class Col>struct TopMenu<AddItem<Base, Table, Col>>			   
	{										   
		typedef NullType list;				   
	};										   
	template<class Base, class Table, class Col>struct NameMenu<TopMenu<AddItem<Base, Table, Col>> >
	{
		wchar_t *operator()(HWND){return L"Добавить";}
	};

	template<class Base, class Table, class Col>struct TopMenu<DelItem<Base, Table, Col>>			   
	{										   
		typedef NullType list;				   
	};										   
	template<class Base, class Table, class Col>struct NameMenu<TopMenu<DelItem<Base, Table, Col>> >
	{
		wchar_t *operator()(HWND){return L"Удалить";}
	};

	template<class O, class P>struct __item__
	{
		void operator()(O *o, P *)
		{
			 o->value.value =  __data_from_widget__<O, typename TL::Inner<O>::Result::type_value>()(*o);
		}
	};

	struct __OkBtn
	{
		static const int width = 120;
		static const int height = 30;
		static const int ID = IDOK;
		wchar_t *Title(){return L"Применить";}
		template<class Owner>void BtnHandler(Owner &o, HWND h)
		{	
			if(!TL::find<typename Owner::list, __test__>()(&o.items, &h))return;
			TL::foreach<typename Owner::list, __item__>()(&o.items, &h);
			EndDialog(h, TRUE);
		}
	};

	//template<class T>struct XDlgItem
	//{
	//	T &value;
	//	HWND hWnd;
	//	template<class Z>XDlgItem(Z &z) : value(z.get<T>()){}
	//	void Init(HWND h, int &x, int &width, int &dy)
	//	{
	//		hWnd = EditItems<T, 256>().Init(h, x, width, dy, value);
	//	}
	//};

	template<class T, class Owner>struct DlgItemPacketWindow
	{
		static const int DY = DlgSubItems<T, typename T::type_value>::DY;
		Owner *owner;
		T &value;
		HWND hWnd;
		DlgItemPacketWindow(Owner *o) : owner(o), value(o->table.items.get<T>()){}
		void Init(HWND h, int &x, int &width, int &dy)
		{
			//hWnd = DlgSubItems<T, typename T::type_value>().Init(h, x, width, dy, value);
			hWnd = EditItems<T, 256>().Init(h, x, width, dy, value);
		}
	};

	template<class Base, class Table, class Col>struct Event<TopMenu<AddItem<Base, Table, Col>> >	   
	{										   
		static void Do(HWND h)				   
		{									   			
			Table ot;
			if(Dialog::Templ<Base, Table ,Table::items_list, 550, TL::MkTlst<__OkBtn, CancelBtn>::Result, DlgItemPacketWindow>(ot).Do(h, L"Добавить"))
			//if(TemplDialog<Base, Table, XDlgItem, TL::MkTlst<__OkBtn, CancelBtn>::Result>(ot).Do(h, L"Добавить"))
			{
				PacketTemplateWindow<Base, Table, Col> *w = (PacketTemplateWindow<Base, Table, Col> *)GetWindowLongPtr(h, GWLP_USERDATA);
				w->data.Add(ot.items.get<Col>().value);
				SetRow(w->grid.hWnd, w->data.Count());
				UpdateRow(w->grid.hWnd);
			}
		}									   
	};		

	template<class Base, class Table, class Col>struct Event<TopMenu<DelItem<Base, Table, Col>> >	   
	{										   
		static void Do(HWND h)				   
		{									   
			PacketTemplateWindow<Base, Table, Col> *o = (PacketTemplateWindow<Base, Table, Col> *)GetWindowLongPtr(h, GWLP_USERDATA);
			if(o->data.Count() > 1)
			{				
				o->data.Del(o->result);
				UpdateRow(o->grid.hWnd);
				o->result[0] = '\0';
			}
			else
			{
				MessageBox(h, L" Должна быть хотя бы одна запись", L"Предупреждение!!!", MB_ICONEXCLAMATION);
			}
		}									   
	};		
}

template<class Base, class Table, class Col>void PacketTemplateWindow<Base, Table, Col>::RClick(int col, wchar_t *s)
{
	typedef TL::MkTlst<
		TopMenu<AddItem<Base, Table, Col>>
		, Separator<0>
		, TopMenu<DelItem<Base, Table, Col>>
	>::Result popup_items_list;
	wcsncpy(result, s, dimention_of(result));
	PopupMenu<popup_items_list>::Do(hWnd, hWnd);
}

template<class Base, class Table, class Col>wchar_t *PacketTemplateWindow<Base, Table, Col>::Title()
{
	return L"Выбор";
}

template<class Base, class Table, class Col>PacketTemplateWindow<Base, Table, Col>::OkBtn::OkBtn(PacketTemplateWindow &owner)
	: owner(owner)
{}

template<class Base, class Table, class Col>void PacketTemplateWindow<Base, Table, Col>::OkBtn::Do(TCommand &l)
{
	if('\0' != owner.result[0])
	{
		SetWindowText(owner.hResult, owner.result); 
		DestroyWindow(l.hwnd);
		return;
	}
	MessageBox(l.hwnd, L"Необходимо выбрать значение", L"Предупреждение!!!", MB_ICONEXCLAMATION);
}

template<class Base, class Table, class Col>PacketTemplateWindow<Base, Table, Col>::CancelBtn::CancelBtn(PacketTemplateWindow &owner)
	: owner(owner)
{}

template<class Base, class Table, class Col>void PacketTemplateWindow<Base, Table, Col>::CancelBtn::Do(TCommand &l)
{
	DestroyWindow(l.hwnd);
}
