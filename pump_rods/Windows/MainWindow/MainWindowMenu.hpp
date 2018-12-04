#pragma once
#include "window_tool/MenuApi.h"
//#include "Dlg/Dialogs.h"
#include "Compute/Compute.h"

namespace MainWindowMenu
{
	struct MainFile{};
	MENU_TEXT(L"Файл", TopMenu<MainFile>)
	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	struct LoadDateFile {static void Do(HWND h){zprint("");}};
	struct SaveDateFile {static void Do(HWND h){zprint("");}};
	struct Compute_     {static void Do(HWND h){Compute::Recalculation();}};;// {static void Do(HWND h){zprint("");}};
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
	struct DeadZones                 {static void Do(HWND h){zprint("");}};//: DeadZonesDlg{};//								 																				   //
	struct MainCreateTypesize        {static void Do(HWND h){zprint("");}};//: AddTypeSizeDlg{};//{static void Do(HWND h){zprint("");}};
	struct MainDeleteTypeSize        {static void Do(HWND h){zprint("");}};//: DelTypeSizeDlg{};//{static void Do(HWND h){zprint("");}};
	struct CrossThresholdWindow__    {static void Do(HWND h){zprint("");}};//: ThresholdCrossDlg{};//{static void Do(HWND h){zprint("");}};																   //
	struct LongThresholdWindow__     {static void Do(HWND h){zprint("");}};//: ThresholdLongDlg{};//{static void Do(HWND h){zprint("");}};
	struct ThickThresholdWindow__    {static void Do(HWND h){zprint("");}};
	struct MedianFiltre              {static void Do(HWND h){zprint("");}};
	struct PipeThicknes              {static void Do(HWND h){zprint("");}};//: MinMaxThresholdsDlg{};//{static void Do(HWND h){zprint("");}};
	struct AcfCutOffBorders          {static void Do(HWND h){zprint("");}};
	struct Filter_                   {static void Do(HWND h){zprint("");}};//: FilterDlg{};//{static void Do(HWND h){zprint("");}};

	MENU_ITEM(L"Поперечные пороги", CrossThresholdWindow__)
	MENU_ITEM(L"Продольные пороги", LongThresholdWindow__)
	MENU_ITEM(L"Пороги толщины", ThickThresholdWindow__)
	MENU_ITEM(L"Мёртвые зоны", DeadZones)
	MENU_ITEM(L"Создать типоразмер", MainCreateTypesize)
	MENU_ITEM(L"Удалить типоразмер", MainDeleteTypeSize)
	MENU_ITEM(L"Медианный фильтр", MedianFiltre)

	MENU_ITEM(L"Минимальная и максимальная стенки трубы", PipeThicknes)
	MENU_ITEM(L"Границы отсечения в частотной области", AcfCutOffBorders)
	MENU_ITEM(L"Настройки цифрового фильтра", Filter_)

	template<>struct TopMenu<MainOptionTypeSize>
	{
		typedef TL::MkTlst<
			MenuItem<CrossThresholdWindow__>
			, MenuItem<LongThresholdWindow__>
			, MenuItem<ThickThresholdWindow__>
			, Separator<1>
			, MenuItem<DeadZones>
			, MenuItem<MedianFiltre>
			, MenuItem<Filter_>
			, MenuItem<PipeThicknes>
			, MenuItem<AcfCutOffBorders>
			, Separator<0>
			, MenuItem<MainCreateTypesize>
			, MenuItem<MainDeleteTypeSize>
		>::Result list;
	 };
	//--------------------------------------------------------------------------------
	struct Options{};
	MENU_TEXT(L"Настройки", TopMenu<Options>)
	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	struct WindowPosition    {static void Do(HWND h){zprint("");}};
	struct IOportsView      {static void Do(HWND h){zprint("");}};//: TestIOPorts{};//{static void Do(HWND h){zprint("");}};
	struct GroupOptions    {static void Do(HWND h){zprint("");}};
	struct SignalOptions    {static void Do(HWND h){zprint("");}};//: SignalOptionsDlg{};//{static void Do(HWND h){zprint("");}};

	MENU_ITEM(L"Сохранить координаты окна", WindowPosition)
    MENU_ITEM(L"Просмотр дискретных портов", IOportsView)
    MENU_ITEM(L"Настройка группы прочности", GroupOptions)
	MENU_ITEM(L"Параметры сигнала", SignalOptions)

	template<>struct TopMenu<Options>
	{
		typedef TL::MkTlst<
			MenuItem<GroupOptions>
			, MenuItem<SignalOptions>
			, Separator<0>
			, MenuItem<WindowPosition>
			, Separator<1>
			, MenuItem<IOportsView>
		>::Result list;		
	};
	//-------------------------------------------------------------------------------------------------------
	struct Setting{};
	MENU_TEXT(L"Установка", TopMenu<Setting>)
	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	struct AnalogPlate__           {};//{static void Do(HWND h){zprint("");}};//: AnalogPlateDlg{};//{static void Do(HWND h){zprint("");}};
	struct DiscretePlate	      {};//{static void Do(HWND h){zprint("");}};//{};//static void Do(HWND h){zprint("");}};
	struct Lir__{};
	struct DiscretePlateInputs    {static void Do(HWND h){zprint("");}};
	struct DiscretePlateOutputs   {static void Do(HWND h){zprint("");}};
	struct ColorItems             {static void Do(HWND h){zprint("");}};//: ColorItemsDlg{};//{static void Do(HWND h){zprint("");}};
	struct DiscretePlateDescriptor{static void Do(HWND h){zprint("");}};
	struct Coefficient            {static void Do(HWND h){zprint("");}};
	struct Signal                 {static void Do(HWND h){zprint("");}};
	struct ACFBorder              {static void Do(HWND h){zprint("");}};

	struct OffsetChannels         {static void Do(HWND h){zprint("");}};
	struct AmplificationChannels  {static void Do(HWND h){zprint("");}};

	MENU_TEXT(L"Дискретная плата", SubMenu<DiscretePlate>)
	MENU_ITEM(L"Входные порты", DiscretePlateInputs)
	MENU_ITEM(L"Выодные порты", DiscretePlateOutputs)
	MENU_ITEM(L"Дискриптор дискретной платы", DiscretePlateDescriptor)
	MENU_ITEM(L"Коэффициенты пересчёта", Coefficient)
	
	MENU_TEXT(L"Аналоговая плата", SubMenu<AnalogPlate__>)
	MENU_ITEM(L"Смещение каналов", OffsetChannels)
	MENU_ITEM(L"Усиление каналов", AmplificationChannels)

	MENU_ITEM(L"Параметры сигнала", Signal)
	MENU_ITEM(L"Границы АЧХ", ACFBorder)
	MENU_ITEM(L"Цвета", ColorItems)
	
	template<>struct SubMenu<AnalogPlate__>
	{
		typedef TL::TypeToTypeLst<
			typename TL::MkTlst<OffsetChannels, AmplificationChannels>::Result 
			, MenuItem
		>::Result list;
	};

	template<>struct SubMenu<DiscretePlate>
	{
		typedef TL::TypeToTypeLst<
			typename TL::MkTlst<DiscretePlateDescriptor, DiscretePlateInputs, DiscretePlateOutputs>::Result 
			, MenuItem
		>::Result list;
	};

	struct LirDescriptorDlg         {static void Do(HWND h){zprint("");}};
	struct LirDlg  {static void Do(HWND h){zprint("");}};

	MENU_TEXT(L"Настройки лир", SubMenu<Lir__>)
	MENU_ITEM(L"Дискриптор лир", LirDescriptorDlg)
	MENU_ITEM(L"Датчики лир", LirDlg)

	template<>struct SubMenu<Lir__>
	{
		typedef TL::TypeToTypeLst<
			typename TL::MkTlst<LirDescriptorDlg, LirDlg>::Result 
			, MenuItem
		>::Result list;
	};

	template<>struct TopMenu<Setting>
	{
		typedef TL::MkTlst<
			 SubMenu<DiscretePlate>	
			 , SubMenu<AnalogPlate__>
			 , SubMenu<Lir__>
			, MenuItem<ColorItems>
		>::Result list;		
	};
	// ----------------------------------------------------------------------------------------------------
	struct MainAbout {static void Do(HWND h){zprint("");}};//: AboutWindowDlg{};//{static void Do(HWND h){zprint("");}};
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
