#include "MainWindow.h"
#include <CommCtrl.h>
#include "MainWindowMenu.hpp"
#include "CommonWindow/Common.h"
#include "window_tool\MenuAPI.h"
#include "templates\WindowsEventTemplate.hpp"
#include "window_tool\EmptyWindow.h"
#include "Windows\DetailWindow\DefectWindow.h"
#include "MessageText\StatusMessages.h"

namespace {
	struct __move_window_data__
	{
		int y, width, height, maxYHeight;
		int lengthTube;
	};
	template<class O, class P>struct __move_window__
	{
		void operator()(O *o, P *p)
		{		
			o->tchart.items.get<BottomAxesMeters>().maxBorder = p->lengthTube;
			int height =  2 * p->height;
			TSize size = {o->hWnd, WM_SIZE, 0, p->width, height};
			SendMessage(MESSAGE(size));
			MoveWindow(o->hWnd , 0, p->y, p->width, height, true);
			p->y += height;
		}
	};
	template<class P>struct __move_window__<ResultViewer, P>
	{
		typedef ResultViewer O;
		void operator()(O *o, P *p)
		{		
			o->tchart.items.get<BottomAxesMeters>().maxBorder = p->lengthTube;
			TSize size = {o->hWnd, WM_SIZE, 0, p->width, p->height};
			SendMessage(MESSAGE(size));
			MoveWindow(o->hWnd , 0, p->y, p->width, p->maxYHeight - p->y, true);
		}
	};
}

LRESULT MainWindow::operator()(TCreate &m)
{
	SetColor();

	DefectoscopeViewer &def = viewers.get<DefectoscopeViewer>();
	def.cursor.SetMouseMoveHandler(&def, &DefectoscopeViewer::Draw);
	StructureViewer &str = viewers.get<StructureViewer>();
	str.cursor.SetMouseMoveHandler(&str, &StructureViewer::Draw);	

	Menu<MainWindowMenu::MainMenu>().Init(m.hwnd);
	toolBar.Init(m.hwnd);
	select.Create(toolBar.hWnd);
	//
	hStatusWindow = CreateStatusWindow(WS_CHILD | WS_VISIBLE, NULL, m.hwnd, 0);
	int pParts[] = {200, 400, 600, 900};
	SendMessage(hStatusWindow, SB_SETPARTS, dimention_of(pParts), (LPARAM)pParts);
	HFONT font = CreateFont(30,0,0,90,900,TRUE,0,0,ANSI_CHARSET,0,0,0,0, L"Arial"); 
	SendMessage(hStatusWindow, WM_SETFONT, (WPARAM)font, (LPARAM)0);
	SendMessage(hStatusWindow, SB_SETMINHEIGHT, 30, (LPARAM)0);
	//
	checks.get<CheckBoxWidget<OnCheckBox<DefectSig<Check>, OnTheJobTable> >>().Init(toolBar.hWnd, L"Дефектоскопия");
	checks.get<CheckBoxWidget<OnCheckBox<StructSig<Check>, OnTheJobTable> >>().Init(toolBar.hWnd, L"Структура");
	checks.get<CheckBoxWidget<OnCheckBox<InterruptViewCheck, OnTheJobTable> >>().Init(toolBar.hWnd, L"Прерывание на просмотр");
	
	CreateChildWindow(m.hwnd, &topLabelViewer);
	topLabelViewer.label.fontHeight = 24;
	topLabelViewer.label = L"<ff>Test <ff00>string";
	//
	TL::foreach<viewers_list, Common::__create_window__>()(&viewers, &m.hwnd);
	return 0;
}
template<class O, class P>struct __mainWindow_delete_back_screen__
{
	void operator()(O &o)
	{
		delete o.backScreen;
		o.backScreen = NULL;
	}
};
void MainWindow::operator()(TDestroy &m)
{
	delete topLabelViewer.backScreen;
	topLabelViewer.backScreen = NULL;
	TL::foreach<viewers_list, __mainWindow_delete_back_screen__>()(viewers); 
	SetWindowLongPtr(m.hwnd, GWLP_USERDATA, 0);
	PostQuitMessage(0);
}

void MainWindow::operator()(TSize &m)
{
	if(m.resizing == SIZE_MINIMIZED || 0 == m.Width || 0 == m.Height) return;
	MoveWindow(hStatusWindow, 0, 0, 0, 0, false);
	toolBar.Size();

	RECT rt;
	GetWindowRect(toolBar.hWnd, &rt);
    RECT rs;
	GetClientRect(hStatusWindow, &rs);
	RECT r;
	GetClientRect(m.hwnd, &r);	
	
	static const int width = toolBar.Width();
	select.Size(width, 5, 400);
//
	checks.get<CheckBoxWidget<OnCheckBox<DefectSig<Check>, OnTheJobTable> >>().Size(width + 425, 5 , 400, 20);
	checks.get<CheckBoxWidget<OnCheckBox<StructSig<Check>, OnTheJobTable> >>().Size(width + 425, 25, 400, 20);
	checks.get<CheckBoxWidget<OnCheckBox<InterruptViewCheck, OnTheJobTable> >>().Size(width, 60, 400, 20);

	static const int topLabelHeight = 40;
	int y = rt.bottom - rt.top - 1;
	int t = r.bottom - rs.bottom - rt.bottom + rt.top + 2 - topLabelHeight;
	MoveWindow(topLabelViewer.hWnd , 0, y, r.right, topLabelHeight, true);
	y += topLabelHeight;
	int maxYHeight = t;
	t = int((double)t / 5);
//
	__move_window_data__ data = {y, r.right, t, m.Height - rs.bottom
		, Singleton<DeadAreaTable>::Instance().items.get<RodLenght>().value
	};
	TL::foreach<viewers_list, __move_window__>()(&viewers, &data);
}

void MainWindow::operator()(TCommand &m)
{
	EventDo(m);
}

//void MainWindow::operator()(TGetMinMaxInfo &m)
//{
//	if(NULL != m.pMinMaxInfo)
//	{
//		m.pMinMaxInfo->ptMinTrackSize.x = 600;
//		m.pMinMaxInfo->ptMinTrackSize.y = 600;
//	}	
//}
void MainWindow::operator()(TClose &l)
{
	if(IDYES == MessageBox(l.hwnd, L"Выйти из программы?", L"Cообщение", MB_ICONQUESTION | MB_YESNO))
	{
		DestroyWindow(l.hwnd);
	}
}

namespace Common
{
	template<class P>struct __in_rect__<ResultViewer, P>: __in_rect_all__<ResultViewer, P, MainWindow::viewers_list>{};
}

void MainWindow::operator()(TMouseWell &l)
{
	TL::find<viewers_list, Common::__in_rect__>()(
		&viewers
		, &Common::__event_data__<TMouseWell, MainWindow>(*this, l)
		);
}
void MainWindow::operator()(TUser &l)
{
	(*l.ptr)(l.data);
}

void MainWindow::StatusBar(int n, wchar_t *txt)
{
	SendMessage(hStatusWindow, SB_SETTEXT, n, (LONG)txt);
}

template<class >struct __main_window_viewer__;
template<>struct __main_window_viewer__<DefectSig<DataItem::Buffer>>
{
	typedef MainWindow::DefectoscopeViewer Result;
};
template<>struct __main_window_viewer__<StructSig<DataItem::Buffer>>
{
	typedef MainWindow::StructureViewer Result;
};

template<class O, class P>struct __main_window_set_color__
{
	void operator()(P &p)
	{
		__main_window_viewer__<O>::Result &o = p.get<__main_window_viewer__<O>::Result>();
		ColorTable::TItems &color = Singleton<ColorTable>::Instance().items;
		o.threshSortDownColor = color.get<Clr<SortDown>>().value;
		o.threshDefectColor = color.get<Clr<Defect>>().value;
		o.deathZoneColor = color.get<Clr<DeathZone>>().value;
		o.nominalColor = color.get<Clr<Nominal>>().value;
	}
};

void MainWindow::SetColor()
{
	TL::foreach<App::data_item_list, __main_window_set_color__>()(viewers);
}

template<class O, class P>struct __clean_chart__
{
	void operator()(O &o, P &p)
	{
		o.count = p;
	}
};

void MainWindow::CleanChart(bool b)
{
	int count = b? DataItem::output_buffer_size: 0;
	TL::foreach<viewers_list, __clean_chart__>()(viewers, count);
}

bool MainWindow::DefectoscopeViewer::Draw(TMouseMove &l, VGraphics &g)
{
	int x = 0;
	CoordCell(tchart, l.x, x, DataItem::output_buffer_size);

	int offs = int((double)x * currentOffset / DataItem::output_buffer_size);
	unsigned color = 0xffffffff;
	wchar_t txt[1024];
	StatusText()(status[x], color, txt);
	wsprintf(label.buffer, L"<ff>смещение <ff00>%d <ff>%s <%x>%s  status %d"
		, offs
		, Wchar_from<double, 2>(buffer[x])()
		, color
		, txt
		, status[x]
		);
	label.Draw(g());
	return true;
}

template<>struct TopMenu<MainWindow::DefectoscopeViewer>
{
	typedef NullType list;	
};
template<>struct TopMenu<MainWindow::StructureViewer>
{
	typedef NullType list;	
};
template<>struct NameMenu<TopMenu<MainWindow::DefectoscopeViewer> >{wchar_t *operator()(HWND){return L"Просмотр";}};
template<>struct NameMenu<TopMenu<MainWindow::StructureViewer> >{wchar_t *operator()(HWND){return L"Просмотр";}};

class DefectoscopeDetailWindow: public DefectWindow
{
public:
	typedef DefectWindow Parent; 
	static wchar_t *Title(){return L"Дефектоскоп";}
};

template<>struct Event<TopMenu<MainWindow::DefectoscopeViewer >> 	
{										
	static void Do(void *data)				
	{	
		MainWindow::DefectoscopeViewer *v = (MainWindow::DefectoscopeViewer *)data;
		HWND h = Common::OpenWindow<DefectoscopeDetailWindow>::Do(v->hWnd);
		if(NULL != h)
		{
		}
	}									
};

template<>struct Event<TopMenu<MainWindow::StructureViewer >> 	
{										
	static void Do(HWND h)				
	{	
		zprint("             event\n");
	}									
};

void MainWindow::DefectoscopeViewer::operator()(TRButtonDown &l)
{
	PopupMenu<TL::MkTlst<TopMenu<MainWindow::DefectoscopeViewer> >::Result>::Do(l.hwnd, this);
}

bool MainWindow::StructureViewer::Draw(TMouseMove &l, VGraphics &g)
{
	int x = 0;
	CoordCell(tchart, l.x, x, DataItem::output_buffer_size);

	int offs = int((double)x * currentOffset / DataItem::output_buffer_size);
	unsigned color = 0xffffffff;
	wchar_t txt[1024];
	StatusText()(status[x], color, txt);
	wsprintf(label.buffer, L"<ff>смещение <ff00>%d <ff>%s <%x>%s  status %d"
		, offs
		, Wchar_from<double, 2>(buffer[x])()
		, color
		, txt
		, status[x]
		);
	label.Draw(g());
	return true;
}

void MainWindow::StructureViewer::operator()(TRButtonDown &l)
{
	zprint("              rbutton\n");
}