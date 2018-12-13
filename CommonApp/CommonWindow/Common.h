#pragma once
#include "tools_debug\DebugMess.h"
#include "App\AppBase.h"
#include "window_tool\EmptyWindow.h"
#include "window_tool\WindowsPosition.h"
#include "Windows/MainWindow/MainWindow.h"


namespace Common
{
	template<class O, class P>struct __create_window__
	{
		void operator()(O *o, P *p)
		{
			wchar_t name[256];
			const char *s = &(typeid(O).name())[6];
			int len = int(1 + strlen(s));
			size_t converted;
			mbstowcs_s(&converted, name, s, len);
			o->hWnd = CreateChildWindow(*p, (WNDPROC)&Viewer<O>::Proc, name, o);
		}
	};

	template<class O, class P>struct __in_rect__
	{
		bool operator()(O *o, P *p)
		{
			RECT r;
			GetWindowRect(o->hWnd, &r);
			if(InRect(p->l.x, p->l.y, r))
			{
				p->l.hwnd = o->hWnd;
				SendMessage(MESSAGE(p->l));
				return false;
			}
			return true;
		}
	};	

	template<class T, class Owner>struct __event_data__
	{
		Owner &owner;
		T &l;
		__event_data__(Owner &owner, T &l)
			:	owner(owner)
			, l(l)
		{}
	};

	template<class O, class P>struct __sub_in_rect__
	{
		void operator()(O *o, P *p)
		{
			RECT r;
			GetWindowRect(o->hWnd, &r);
			p->l.hwnd = o->hWnd;
			o->currentX =  p->l.x;
			SendMessage(MESSAGE(p->l));
		}
	};

	template<class O, class P, class List>struct __in_rect_all__
	{
		bool operator()(O *o, P *p)
		{
			RECT r;
			GetWindowRect(o->hWnd, &r);
			if(InRect(p->l.x, p->l.y, r))
			{
				p->l.hwnd = o->hWnd;
				SendMessage(MESSAGE(p->l));

				p->l.x = o->currentX;
				p->l.delta = 0;
				typedef TL::EraseItem<List, O>::Result lst;
				TL::foreach<lst, __sub_in_rect__>()(&p->owner.viewers, p);
				return false;
			}
			return true;
		}
	};

	template<template<class, int>class Wapper, class T, int N, class P>struct __in_rect__<Wapper<T, N>, P>
	  : __in_rect_all__<Wapper<T, N>, P, typename T::line_list>{};

	template<class O, class P>struct __sub_in_rect_wapper_
	{
		void operator()(O *o, P *p)
		{
			RECT r;
			GetWindowRect(o->hWnd, &r);
			o->offsetX = p->offsetX;
			TMouseWell m = {o->hWnd, WM_MOUSEWHEEL, 0, 0, 0};
			SendMessage(MESSAGE(m));
		}
	};

	template<template<class, int>class W, class X, int N, class P, class List>struct __in_rect_all__<W<X,N>, P, List>
	{
		typedef W<X,N> O;
		bool operator()(O *o, P *p)
		{
			RECT r;
			GetWindowRect(o->hWnd, &r);
			if(InRect(p->l.x, p->l.y, r))
			{
				p->l.hwnd = o->hWnd;
				SendMessage(MESSAGE(p->l));

				p->l.x = o->storedMouseMove.x;
				p->l.delta = 0;
				typedef TL::EraseItem<List, O>::Result lst;
				TL::foreach<lst, __sub_in_rect_wapper_>()(&p->owner.viewers, o);
				return false;
			}
			return true;
		}
	};

	template<class T>bool DestroyWindow()
	{
		HWND hh = FindWindow(WindowClass<T>()(), 0);
		if(NULL != hh)
		{
			DestroyWindow(hh);
			return true;
		}
		return false;
	}

	template<class T>struct OpenWindow
	{
		static HWND Do(HWND)
		{
			HWND hh = FindWindow(WindowClass<T>()(), 0);
			if(NULL != hh)
			{
				SendMessage(hh, WM_SYSCOMMAND, SC_RESTORE, 0);
				SetForegroundWindow(hh);
			}
			else
			{
				RECT r;
				WindowPosition::Get<T>(r);
				hh = WindowTemplate(new T, T::Title(), r.left, r.top, r.right, r.bottom);
				ShowWindow(hh, SW_SHOWNORMAL);
			}
			return hh;
		}
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

	template<class O, class P>struct __destroy__window__
	{
		void operator()(O *o)
		{
			 delete o->backScreen;
		}
	};
//-----------------------------------------------------------------------------------------------
	//template<class T>struct ColorThreshold
	//{
	//	typedef typename T::__template_must_be_overridded__ noused;
	//};
	//
	//template<class X>struct ColorThreshold<Clr<X> >
	//{
	//	typedef Clr<X> Result;
	//};
	//
	//template<template<class>class W, class X>struct ColorThreshold<W<BorderKlass2<X> > >
	//{
	//	typedef BorderKlass2<X> Result;
	//};
	
	template<class T>struct __set_color_wapper__
	{
		typedef T Result;
	};
	
	template<class O, class P>struct __set_color__
	{
			void operator()(O *o, P *p)
			{
				typedef typename TL::Inner<O>::Result inner;
				typedef __set_color_wapper__<inner>::Result x;
				o->color = Singleton<ColorTable>::Instance().items.get<Clr<x>>().value;	  
			}
	};
	
	template<class O, class P>struct __set_border_color__
	{
		void operator()(O *o, P *p)
		{
			typedef TL::SelectWapper<typename O::TChart::items_list, Border>::Result lst;
			TL::foreach<lst, __set_color__>()(&((typename O::TChart *)o->chart)->items, p);
			o->owner = p;
			o->chart->minAxesY = Singleton<AxesTable>::Instance().items.get<AxesYMin<typename P::sub_type> >().value;
			o->chart->maxAxesY = Singleton<AxesTable>::Instance().items.get<AxesYMax<typename P::sub_type> >().value;
		}
	};
	
	template<class O, class P>struct __update__;
	template<template<class, int>class W, class X, int N, class P>struct __update__<W<X, N>, P>
	{
		typedef W<X, N> O;
		void operator()(O *o, P *p)
		{
			o->dataViewer.Do(p->lastZone, N, o->owner->adjustItem.get<AdjustingMultipliers<O>>().val);
			o->chart->maxAxesX = o->dataViewer.count;
			//typedef TL::SelectWapper<typename O::TChart::items_list, Border>::Result lst;
			//TL::foreach<lst, __set_thresholds__>()(&((typename O::TChart *)o->chart)->items, &p->lastZone);
			RepaintWindow(o->hWnd);
		}
	};
	template<class O, class P>struct __clr__;
	template<template<class, int>class W, class X, int N, class P>struct __clr__<W<X, N>, P>
	{
		typedef W<X, N> O;
		void operator()(O *o, P *p)
		{
			o->chart->maxAxesX = 0;
			RepaintWindow(o->hWnd);
		}
	};

	static const int window_height = 200;
	struct __data__
	{
		int y, width, height;
	};
	
	template<class O, class P>struct __draw__
	{
		void operator()(O *o, P *p)
		{
			 MoveWindow(o->hWnd, 0, 0, p->width, window_height, TRUE);
		}
	};
	template<template<class, int>class L, class W, int N, class P>struct __draw__<L<W, N>, P>
	{
		typedef L<W, N> O;
		void operator()(O *o, P *p)
		{
			//bool b = N % 2 == 0;
			//int x = b 
			//	? 0
			//	: p->width / 2
			//	;
			// MoveWindow(o->hWnd, x, p->y, p->width / 2, p->height, TRUE);
			// if(!b) p->y += p->height;

			int i = N % 4;
			int offs = p->width / 4;

			 MoveWindow(o->hWnd, offs * i, p->y, offs, p->height * 2, TRUE);
			 if(3 == i) p->y += p->height * 2;
		}
	};
}
//}

void CloseAllWindows();

template<class T, class OWNER>class NoSubMenu: public T
{
	static VOID CALLBACK WaitOrTimerCallback(PVOID lpParameter, BOOLEAN TimerOrWaitFired)
	{
		NoSubMenu *self = (NoSubMenu *)lpParameter;
		HWND h = FindWindow(WindowClass<MainWindow>()(), 0);
		TL::Inner<Owner::MainChart>::Result &mv = ((MainWindow *)GetWindowLongPtr(h, GWLP_USERDATA))->viewers.get<TL::Inner<Owner::MainChart>::Result>();
		self->mouseMove = false;
		self->currentX = mv.currentX;
		self->currentY = mv.currentY;
		SendMessage(self->hWnd, WM_MOUSEWHEEL, 0, 0);
	}
public:
	typedef OWNER Owner;
	typedef T Parent;
	Owner *owner;
	HANDLE hTimer;
	~NoSubMenu()
	{
		DeleteTimerQueueTimer(NULL, hTimer, NULL);
	}
	void operator()(TRButtonDown &){}
	LRESULT operator()(TCreate &l)
	{
		(*(Parent::Parent *)this)(l);
		CreateTimerQueueTimer(&hTimer, NULL, WaitOrTimerCallback, this, 50, 0, WT_EXECUTEONLYONCE);
		//WaitOrTimerCallback(this, TRUE);
		return 0;
	}	
};

class Chart;
void ZoneToCoord(Chart &, int zone, int sens, WORD &x, WORD &y);

class CrossWindow;
class ThickWindow;
class LongWindow;





