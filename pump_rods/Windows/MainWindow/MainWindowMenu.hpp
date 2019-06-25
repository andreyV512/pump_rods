#pragma once
#include "window_tool/MenuApi.h"
#include "Dlg/Dialogs.h"
#include "Compute/Compute.h"
#include "window_tool\WindowsPosition.h"

namespace MainWindowMenu
{
	struct MainFile{};
	MENU_TEXT(L"Файл", TopMenu<MainFile>)
	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	struct LoadDateFile : LoadDlg{};//{static void Do(HWND h){zprint("");}};
	struct SaveDateFile : StoreDlg{};//{static void Do(HWND h){zprint("");}};
	struct Compute_     {static void Do(HWND h){Compute::Recalculation();}};

	struct MainExit
	{
		static void Do(HWND h)
		{
			TClose c = {h, WM_CLOSE, 0, 0};
			SendMessage(MESSAGE(c));
		}
	};

	MENU_ITEM(L"Загрузить данные", LoadDateFile)
	MENU_ITEM(L"Сохранить данные", SaveDateFile)
	MENU_ITEM(L"Перерасчёт", Compute_)
	MENU_ITEM(L"Выход", MainExit)

	template<>struct TopMenu<MainFile>
	{
		typedef TL::MkTlst<
			MenuItem<LoadDateFile>
			, MenuItem<SaveDateFile>
			, Separator<0>
			, MenuItem<Compute_>
			, Separator<1>
			, MenuItem<MainExit>
		>::Result list;
	};
	//------------------------------------------------------------------------
	struct MainOptionTypeSize{};
	MENU_TEXT(L"Типоразмер", TopMenu<MainOptionTypeSize>)
	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ 
	struct DeadZones                 : DeathZonesDlg{};								 																				   //
	struct MainCreateTypesize        : AddTypeSizeDlg{};
	struct MainDeleteTypeSize        : DelTypeSizeDlg{};
	struct MedianFiltre              : MedianFilterDlg{};
	struct Filter_                   : FilterDlg{};
	struct KoefDlg_                  : KoefDlg{};
	struct TresholdsDlg_             : TresholdsDlg{};
	struct AxesGraphs                : AxesGraphsDlg{};

	MENU_ITEM(L"Смещения", DeadZones)
	MENU_ITEM(L"Создать типоразмер", MainCreateTypesize)
	MENU_ITEM(L"Удалить типоразмер", MainDeleteTypeSize)
	MENU_ITEM(L"Медианный фильтр", MedianFiltre)
	MENU_ITEM(L"Корректировка датчиков", KoefDlg_)
	MENU_ITEM(L"Настройки цифрового фильтра", Filter_)
	MENU_ITEM(L"Настройки порогов", TresholdsDlg_)
	MENU_ITEM(L"Настройки осей", AxesGraphs)

	template<>struct TopMenu<MainOptionTypeSize>
	{
		typedef TL::MkTlst<
			 MenuItem<DeadZones>
			, MenuItem<TresholdsDlg_>
			, MenuItem<MedianFiltre>
			, MenuItem<Filter_>
			, MenuItem<KoefDlg_>
			, MenuItem<AxesGraphs>
			, Separator<0>
			, MenuItem<MainCreateTypesize>
			, MenuItem<MainDeleteTypeSize>
		>::Result list;
	 };
	//--------------------------------------------------------------------------------
	struct Options{};
	MENU_TEXT(L"Настройки", TopMenu<Options>)
	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	struct WindowPos    {static void Do(HWND h){WindowPosition::Set<MainWindow>(h);}};
	struct IOportsView      : TestIOPorts{};
	struct Message_      : MessageDlg{};

	MENU_ITEM(L"Сохранить координаты окна", WindowPos)
    MENU_ITEM(L"Просмотр дискретных портов", IOportsView)
	MENU_ITEM(L"Просмотр сообщений", Message_)

	template<>struct TopMenu<Options>
	{
		typedef TL::MkTlst<
			 MenuItem<WindowPos>
			, Separator<1>
			, MenuItem<IOportsView>
			, MenuItem<Message_>
		>::Result list;		
	};
	//-------------------------------------------------------------------------------------------------------
	struct Setting{};
	MENU_TEXT(L"Установка", TopMenu<Setting>)
	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	struct AnalogPlate__          : AnalogPlatelDlg{};
	struct DiscretePlate	      {};
	struct DiscretePlateInputs    : InputBitDlg{};
	struct DiscretePlateOutputs   : OutputBitDlg{};
	struct ColorItems             : ColorItemsDlg{};
	struct DiscretePlateDescriptor: DiscriptorBitDlg{};
	struct StoredFiles            : StoredFilesDlg{};
	struct Coefficient            {static void Do(HWND h){zprint("");}};
	struct Signal                 {static void Do(HWND h){zprint("");}};
	struct ACFBorder              {static void Do(HWND h){zprint("");}};

	struct OffsetChannels         {static void Do(HWND h){zprint("");}};
	struct AmplificationChannels  {static void Do(HWND h){zprint("");}};

	MENU_TEXT(L"Дискретная плата", SubMenu<DiscretePlate>)
	MENU_ITEM(L"Входные порты", DiscretePlateInputs)
	MENU_ITEM(L"Выходные порты", DiscretePlateOutputs)
	MENU_ITEM(L"Дискриптор дискретной платы", DiscretePlateDescriptor)
	MENU_ITEM(L"Коэффициенты пересчёта", Coefficient)
	
	MENU_ITEM(L"Аналоговая плата", AnalogPlate__)
	MENU_ITEM(L"Смещение каналов", OffsetChannels)
	MENU_ITEM(L"Диапазон каналов", AmplificationChannels)

	MENU_ITEM(L"Параметры сигнала", Signal)
	MENU_ITEM(L"Границы АЧХ", ACFBorder)
	MENU_ITEM(L"Цвета", ColorItems)
	MENU_ITEM(L"Сохранение файлов результата измерений", StoredFiles)

	template<>struct SubMenu<DiscretePlate>
	{
		typedef TL::TypeToTypeLst<
			typename TL::MkTlst<DiscretePlateDescriptor, DiscretePlateInputs, DiscretePlateOutputs>::Result 
			, MenuItem
		>::Result list;
	};

	template<>struct TopMenu<Setting>
	{
		typedef TL::MkTlst<
			 SubMenu<DiscretePlate>	
			 , MenuItem<AnalogPlate__>
			, MenuItem<ColorItems>
			, Separator<1>
			, MenuItem<StoredFiles>
		>::Result list;		
	};
	// ----------------------------------------------------------------------------------------------------
	struct MainAbout : AboutWindowDlg{};
	MENU_TEXT(L"О программе", TopMenu<MainAbout>)
	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	MENU_ITEM(L"О программе", MainAbout)

	template<>struct TopMenu<MainAbout>
	{
		typedef TL::MkTlst<
			MenuItem<MainAbout>
		>::Result list;
	};
	//---------------------------------------------------------------------------------------------------------------
	typedef TL::MkTlst<
		TopMenu<MainFile>
		, TopMenu<MainOptionTypeSize>
		, TopMenu<Options>
		, TopMenu<Setting>
		, TopMenu<MainAbout>
	>::Result MainMenu;	
}
