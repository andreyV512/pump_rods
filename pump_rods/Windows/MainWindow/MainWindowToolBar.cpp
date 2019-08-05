#include "MainWindowToolBar.h"
#include "tools_debug\DebugMess.h"
#include "Resource.h"
#include "templates/typelist.hpp"
#include "window_tool/InitToolBar.hpp"
#include "AppKeyHandler.h"
#include "MainWindow.h"
#include "Automat\Automat.h"
#include "Automat\EquipmentCheck.h"

namespace 
{

	template<int N>struct Key;

#define KEY(ID, text)template<>struct Key<ID> \
	{\
	static void Click(HWND);\
	static wchar_t *Text(){return text;}\
	};
	KEY(IDB_CycleBtn   , L"F4 ����")
	KEY(IDB_Reset      , L"Esc ����")
	KEY(IDB_QueryBtn   , L"����")
//	KEY(IDB_DownArrow , L"F5 ����")
//	KEY(IDB_UpArrow   , L"F6 ����")
//	KEY(IDB_LeftArrow , L"F7 �����")
	//KEY(IDB_RightArrow, L"F8 Clear")
//	KEY(IDB_Ok, L"F8 �����")
	KEY(IDB_Continue, L"F11 ������")
#undef KEY
#define BUTTON_KEY(ID)ButtonToolbar<ID, Key<ID> > 
		typedef TL::MkTlst<
		SeparatorToolbar<0>
		, BUTTON_KEY(IDB_CycleBtn)
		, BUTTON_KEY(IDB_Continue)
		, BUTTON_KEY(IDB_Reset)
		, BUTTON_KEY(IDB_QueryBtn) 		
	//	, BUTTON_KEY(IDB_Ok) 		
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
	//namespace closed_window
	//{		
	//	//typedef TL::MkTlst<
	//	//	WindowClass<CrossWindow		>
	//	//	, WindowClass<LongWindow		>
	//	//	, WindowClass<ThickWindow	>
	//	//	, WindowClass<ScanWindow		>
	//	//>::Result window_list;
	//	template<class O, class P>struct __close_window__
	//	{
	//		void operator()()
	//		{
	//			HWND hh = FindWindow(O()(), NULL);
	//	    	if(NULL != hh) DestroyWindow(hh);
	//		}
	//	};
	//
	//	void close_window()
	//	{
	//		//TL::foreach<window_list, __close_window__>()();
	//	}
	//}
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


