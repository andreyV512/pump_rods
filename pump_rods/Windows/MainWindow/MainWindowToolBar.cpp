#include "MainWindowToolBar.h"
#include "tools_debug\DebugMess.h"
#include "Resource.h"
#include "templates/typelist.hpp"
#include "window_tool/InitToolBar.hpp"
#include "AppKeyHandler.h"
#include "MainWindow.h"
#include "Automat\Automat.h"
#include "Automat\EquipmentCheck.h"
#include "window_tool/Emptywindow.h"

namespace 
{

	template<int N>struct Key;

#define KEY(ID, text)template<>struct Key<ID> \
	{\
	static void Click(HWND);\
	static wchar_t *Text(){return text;}\
	};
	KEY(IDB_CycleBtn   , L"F4 Работа")
	KEY(IDB_Reset      , L"Esc Стоп")
	KEY(IDB_QueryBtn   , L"Тест")
	KEY(IDB_Continue, L"F11 Повтор")
	KEY(IDB_OkTumb, L"F8 Норма")
#undef KEY
#define BUTTON_KEY(ID)ButtonToolbar<ID, Key<ID> > 
		typedef TL::MkTlst<
		SeparatorToolbar<0>
		, BUTTON_KEY(IDB_CycleBtn)
		, BUTTON_KEY(IDB_Continue)
		, BUTTON_KEY(IDB_Reset)
		, BUTTON_KEY(IDB_OkTumb)
		, SeparatorToolbar<1>
#ifdef DEBUG_ITEMS
		, BUTTON_KEY(IDB_arrow_down) 
		, BUTTON_KEY(IDB_arrow_up)
		, BUTTON_KEY(IDB_arrow_left) 
		, BUTTON_KEY(IDB_arrow_right)
		, SeparatorToolbar<2>
#endif
		>::Result tool_button_list;
#undef BUTTON_KEY
//----------------------------------------------------------------------------------
	static bool closed_packet_dialog = true;
	static bool run_once_per_sycle = false;
	void Key<IDB_CycleBtn>::Click(HWND h)
	{
		Automat::Run();
	}
	void Key<IDB_Continue>::Click(HWND h)
	{
		Automat::Contine();
	}
//-------------------------------------------------------------------------------
	void Key<IDB_QueryBtn>::Click(HWND h)
	{
		EquipmentCheck();
	}
//----------------------------------------------------------------------------
	void Key<IDB_Reset>::Click(HWND h)
	{
		Automat::Stop();
	}
/////////////////////////////////////////////////////////////////////////////////
	HANDLE handleTimer = INVALID_HANDLE_VALUE;
	VOID CALLBACK WaitOrTimerCallback(PVOID lpParameter, BOOLEAN TimerOrWaitFired)
	{
		HWND hWnd = FindWindow(WindowClass<MainWindow>()(), 0);
		if(NULL != hWnd)
		{
			MainWindow *w = (MainWindow *)GetWindowLongPtr(hWnd, GWLP_USERDATA);
			SendMessage(w->toolBar.hWnd, TB_ENABLEBUTTON, TL::IntToType<IDB_OkTumb>::value, MAKELONG(TRUE, 0));
		}
		DeleteTimerQueueTimer(NULL, handleTimer, NULL);
	}
//////////////////////////////////////////////////////////////////////////////////
	void Key<IDB_OkTumb>::Click(HWND h)
	{
		MainWindow *w = (MainWindow *)GetWindowLongPtr(h, GWLP_USERDATA);
		SendMessage(w->toolBar.hWnd, TB_ENABLEBUTTON, TL::IntToType<IDB_OkTumb>::value, MAKELONG(FALSE, 0));
		CreateTimerQueueTimer(&handleTimer, NULL, WaitOrTimerCallback, NULL, 2000, 0, WT_EXECUTEONLYONCE);
		Automat::UpSort();
	}
}
//--------------------------------------------------------------------------------------------
HWND MainWindowToolBar::Init(HWND h)
{
	hWnd = InitToolbar<tool_button_list>()(h);
	return hWnd;
}
//-------------------------------------------------------------------
void MainWindowToolBar::Size()
{
	MoveWindow(hWnd, 0, 0, 0, 0, false);
}
//----------------------------------------------------------------------------
const int MainWindowToolBar::Width()
{
	return __tool_bar_width__<tool_button_list>::value;
}
//------------------------------------------------------------------------


