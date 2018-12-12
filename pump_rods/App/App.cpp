#include "App.h"
#include "Windows\MainWindow\MainWindow.h"
#include "templates\typelist.hpp"
#include "window_tool\Emptywindow.h"
#include "window_tool\WindowsPosition.h"
#include "AppBase.h"
#include "window_tool\HookKey.h"
#include "Windows\MainWindow\AppKeyHandler.h"
#include "DataItem\DataItem.h"
#include "MessageText\ListMess.hpp"
#include "Log\LogMessageToTopLabel.h"
#include "Automat\Automat.h"

namespace App
{
	template<class O, class P>struct _test_
	{
		void operator()()
		{
			dprint("%s\n", typeid(O).name());
		}
	};

	void Init()
	{
//test
		DataItem::Test();
		TL::foreach<StatusMessages::list_items, _test_>()();
//test end
		AppBase().Init();

		MainWindow &mainWindow = Singleton<MainWindow>::Instance();

		RECT r;
		WindowPosition::Get<MainWindow>(r);

		HWND h = WindowTemplate(
			&mainWindow
			, L"Контроль насосных штанг"
			, r.left, r.top, r.right, r.bottom
			);
		ShowWindow(h, SW_SHOWNORMAL);
		Singleton<LogMessageToTopLabel>::Instance().Run();
		StartKeyHook(h, &AppKeyHandler::KeyPressed);

		bool ok = true;

		if(!device1730.Init(Singleton<NamePlate1730ParametersTable>::Instance().items.get<NamePlate1730>().value))
		{
			MessageBox(h, L"Не могу инициировать плату 1730 номер 1", L"Ошибка !!!", MB_ICONERROR);
			ok = false;
		}

		Automat::Init();
	}

	void Destroy()
	{
		Automat::Destroy();
		Singleton<LogMessageToTopLabel>::Instance().Stop();
	}

	wchar_t *Salt()
	{
		return L"_pump_rods_";
	}

	void UpdateViewers()
	{
		MainWindow &mainWindow = Singleton<MainWindow>::Instance();
		mainWindow.CleanChart(false);
		RepaintWindow(mainWindow.hWnd);
	}

	void CleanViewers()
	{
		MainWindow &mainWindow = Singleton<MainWindow>::Instance();
		mainWindow.CleanChart(true);
		RepaintWindow(mainWindow.hWnd);
	}

	void TopLabel(wchar_t *mess)
	{
		Singleton<MainWindow>::Instance().topLabelViewer.SetMessage(mess);
	}
}

Device1730 device1730;