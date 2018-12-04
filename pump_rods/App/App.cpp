#include "App.h"
#include "Windows\MainWindow\MainWindow.h"
#include "templates\typelist.hpp"
#include "window_tool\Emptywindow.h"
#include "window_tool\WindowsPosition.h"
#include "AppBase.h"
#include "window_tool\HookKey.h"
#include "Windows\MainWindow\AppKeyHandler.h"
#include "DataItem\DataItem.h"

namespace App
{
	void Init()
	{
//test
		DataItem::Test();
//test end
		AppBase().Init();

		MainWindow &mainWindow = Singleton<MainWindow>::Instance();

		RECT r;
		WindowPosition::Get<MainWindow>(r);

		HWND h = WindowTemplate(
			&mainWindow
			, L"Группа прочности"
			, r.left, r.top, r.right, r.bottom
			);
		ShowWindow(h, SW_SHOWNORMAL);
		StartKeyHook(h, &AppKeyHandler::KeyPressed);
	}

	void Destroy()
	{
	}

	wchar_t *Salt()
	{
		return L"_pump_rods_";
	}

	void UpdateViewers()
	{
		MainWindow &mainWindow = Singleton<MainWindow>::Instance();
		RepaintWindow(mainWindow.hWnd);
	}
}