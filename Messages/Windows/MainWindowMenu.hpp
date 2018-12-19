#pragma once
#include "Dlg\Dialogs.h"
#include "window_tool/WindowsPosition.h"
namespace MainWindowMenu
{
struct MainFile{};
struct MainOptionUnits{};
struct SaveCoordinateWindow: WindowPositionDlg<MainWindow>{}; 
struct MainExit{static void Do(HWND h){DestroyWindow(h);}};

template<>struct TopMenu<MainFile>
{
	typedef TL::MkTlst<
		 MenuItem<SaveDateFile>
		, Separator<0>		
	    , MenuItem<MainExit>
	>::Result list;
};

template<>struct TopMenu<MainOptionUnits>
{
	typedef TL::MkTlst<	
		MenuItem<SaveCoordinateWindow>
	>::Result list;
};

typedef TL::MkTlst<
	TopMenu<MainFile>
	, TopMenu<MainOptionUnits>
>::Result MainMenu;
//--------------------------------------------------------------------------------------------------------
MENU_TEXT(L"Файл", TopMenu<MainFile>)	
MENU_TEXT(L"Настройки", TopMenu<MainOptionUnits>)

MENU_ITEM(L"Выход", MainExit)
MENU_ITEM(L"Сохранить координаты окна", SaveCoordinateWindow)
MENU_ITEM(L"Сохранить сообщения", SaveDateFile)
}
