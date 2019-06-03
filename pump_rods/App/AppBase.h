#pragma once
#include "Base/tables.hpp"
#include "Base/TablesDefine.h"

void TrimTypeList(wchar_t *, wchar_t *);

DEFINE_PARAM(CurrentID, int, 1)
struct CurrentParametersTable
{
	typedef TL::MkTlst<
		CurrentID
	>::Result items_list;
	typedef TL::Factory<items_list> TItems;
	TItems items;
	const wchar_t *name(){return L"CurrentParametersTable";}
};

template<class>struct Clr;
struct Nominal;
struct SortDown;
struct Defect;
struct DeathZone;
struct SensorOff;
struct Axes;

DEFINE_PARAM_WAPPER(Clr, Nominal  , int, 0xff00ff00)
DEFINE_PARAM_WAPPER(Clr, SortDown, int, 0xff4286f4)
DEFINE_PARAM_WAPPER(Clr, Defect, int, 0xffff0000)
DEFINE_PARAM_WAPPER(Clr, DeathZone, int, 0xff939393)
DEFINE_PARAM_WAPPER(Clr, SensorOff, int, 0xffA9A9A9)

struct ColorTable
{
	typedef TL::MkTlst<	
		Clr<Nominal  >
		, Clr<SortDown> 
		, Clr<Defect>
		, Clr<DeathZone>
		, Clr<SensorOff>
	>::Result items_list;
	typedef TL::Factory<items_list> TItems;
	TItems items;
	const wchar_t *name(){return L"ColorTable";}
};

template<class T>struct DefectSig;
template<class T>struct StructSig;
template<class T>struct Thresh;
DEFINE_PARAM_WAPPER2(DefectSig, Thresh, SortDown, int, 40)
DEFINE_PARAM_WAPPER2(DefectSig, Thresh, Defect, int, 70)
DEFINE_PARAM_WAPPER2(StructSig, Thresh, SortDown, int, 40)
DEFINE_PARAM_WAPPER2(StructSig, Thresh, Defect, int, 70)

struct ThresholdsTable
{
	typedef TL::MkTlst<
		  DefectSig<Thresh<SortDown>> 
		, DefectSig<Thresh<Defect>>
		, StructSig<Thresh<SortDown>> 
		, StructSig<Thresh<Defect>>
	>::Result items_list;
	typedef TL::Factory<items_list> TItems;
	TItems items;
	const wchar_t *name(){return L"ThresholdsTable";}
};

template<class>struct First ;
template<class>struct Second;
DEFINE_PARAM_WAPPER2(DefectSig, First , DeathZone, int, 500)
DEFINE_PARAM_WAPPER2(DefectSig, Second, DeathZone, int, 500)
DEFINE_PARAM_WAPPER2(StructSig, First , DeathZone, int, 500)
DEFINE_PARAM_WAPPER2(StructSig, Second, DeathZone, int, 500)
DEFINE_PARAM(RodLenght, int, 8000) //длина штанги

 struct DeadAreaTable
 {
	typedef TL::MkTlst<
		DefectSig<First<DeathZone>>
		, DefectSig<Second<DeathZone>>
		, StructSig<First<DeathZone>>
		, StructSig<Second<DeathZone>>
		, RodLenght
	>::Result items_list;
	typedef TL::Factory<items_list> TItems;
	TItems items;
	const wchar_t *name(){return L"DeadAreaTable";}
 };
struct Check;
DEFINE_PARAM_WAPPER(DefectSig, Check, bool, true)
DEFINE_PARAM_WAPPER(StructSig, Check, bool, true)
DEFINE_PARAM(InterruptViewCheck, bool, true)

struct OnTheJobTable
{
	typedef TL::MkTlst<
		DefectSig<Check>
		, StructSig<Check>
		, InterruptViewCheck
	>::Result items_list;
	typedef TL::Factory<items_list> TItems;
	TItems items;
	const wchar_t *name(){return L"OnTheJobTable";}
};

struct CutoffFrequency; 
struct CutoffFrequencyON;
DEFINE_PARAM_WAPPER(DefectSig, CutoffFrequency, int, 20)
DEFINE_PARAM_WAPPER(StructSig, CutoffFrequency, int, 20)
DEFINE_PARAM_WAPPER(DefectSig, CutoffFrequencyON, bool, true)
DEFINE_PARAM_WAPPER(StructSig, CutoffFrequencyON, bool, true)


 struct AnalogFilterTable
{
	typedef TL::MkTlst<
		DefectSig<CutoffFrequency>
		, DefectSig<CutoffFrequencyON>
		, StructSig<CutoffFrequency>
		, StructSig<CutoffFrequencyON>
	>::Result items_list;
	typedef TL::Factory<items_list> TItems;
	TItems items;
	const wchar_t *name(){return L"AnalogFilterTable";}
};

struct MedianFiltreWidth;
struct MedianFiltreON;
DEFINE_PARAM_WAPPER(DefectSig, MedianFiltreWidth, int, 5)
DEFINE_PARAM_WAPPER(StructSig, MedianFiltreWidth, int, 5)
DEFINE_PARAM_WAPPER(DefectSig, MedianFiltreON, bool, true)
DEFINE_PARAM_WAPPER(StructSig, MedianFiltreON, bool, true)

 struct MedianFiltreTable
 {
	 typedef TL::MkTlst<
		 DefectSig<MedianFiltreWidth>
		, DefectSig<MedianFiltreON>
		, StructSig<MedianFiltreWidth>
		, StructSig<MedianFiltreON>
	 >::Result items_list;
	 typedef TL::Factory<items_list> TItems;
	 TItems items;
	 const wchar_t *name(){return L"MedianFiltreTable";}
 };

 struct KoeffSign;
DEFINE_PARAM_WAPPER(DefectSig, KoeffSign, double, 1)
DEFINE_PARAM_WAPPER(StructSig, KoeffSign, double, 1)

 struct KoeffSignTable
 {
	 typedef TL::MkTlst<
		 DefectSig<KoeffSign>
		, StructSig<KoeffSign>
	 >::Result items_list;
	 typedef TL::Factory<items_list> TItems;
	 TItems items;
	 const wchar_t *name(){return L"KoeffSignTable";}
 };

struct RangeL502;
struct ChannelL502;
struct ChannelSamplingRate;
DEFINE_PARAM_WAPPER(DefectSig, ChannelSamplingRate, int, 10000)
DEFINE_PARAM_WAPPER(StructSig, ChannelSamplingRate, int, 10000)
DEFINE_PARAM_WAPPER(DefectSig, RangeL502, int, 0)
DEFINE_PARAM_WAPPER(StructSig, RangeL502, int, 0)
DEFINE_PARAM_WAPPER(DefectSig, ChannelL502, int, 2)
DEFINE_PARAM_WAPPER(StructSig, ChannelL502, int, 1)

struct L502ParametersTable
{
	typedef TL::MkTlst<
		DefectSig<ChannelSamplingRate>
		, StructSig<ChannelSamplingRate>
		, DefectSig<RangeL502>
		, StructSig<RangeL502>
		, DefectSig<ChannelL502>
		, StructSig<ChannelL502>
	>::Result items_list;
	typedef TL::Factory<items_list> TItems;
	TItems items;
	const wchar_t *name(){return L"L502ParametersTable";}
};

DEFINE_PARAM_WAPPER(DefectSig, Axes, int, 100)
DEFINE_PARAM_WAPPER(StructSig, Axes, int, 100)

struct AxesGraphsTable
{
	typedef TL::MkTlst<
		DefectSig<Axes>
		, StructSig<Axes>
	>::Result items_list;
	typedef TL::Factory<items_list> TItems;
	TItems items;
	const wchar_t *name(){return L"AxesGraphsTable";}
};

STR_PARAM(NameParam, 128, L"НШ19")
//DEFINE_PARAM(DiametrPumpRods            , int, 19)
DEFINE_PARAM_ID(ThresholdsTable, int, 1)
DEFINE_PARAM_ID(DeadAreaTable, int, 1)
DEFINE_PARAM_ID(OnTheJobTable, int, 1)
DEFINE_PARAM_ID(AnalogFilterTable, int, 1)
DEFINE_PARAM_ID(MedianFiltreTable, int, 1)
DEFINE_PARAM_ID(L502ParametersTable, int, 1)
DEFINE_PARAM_ID(KoeffSignTable	   , int, 1)
DEFINE_PARAM_ID(AxesGraphsTable	   , int, 1)

 struct ParametersTable
 {
	typedef TL::MkTlst<
		ID<ThresholdsTable>
		, ID<DeadAreaTable>
		, ID<OnTheJobTable>
		, ID<AnalogFilterTable>
		, ID<MedianFiltreTable>
		, ID<L502ParametersTable>
		, ID<KoeffSignTable>
		, ID<AxesGraphsTable>
		, NameParam
		//, DiametrPumpRods
	>::Result items_list;
	typedef TL::Factory<items_list> TItems;
	TItems items;
	const wchar_t *name(){return L"ParametersTable";}
 };

struct ViewerCount;
DEFINE_PARAM_WAPPER(DefectSig, ViewerCount, int, 3000)
DEFINE_PARAM_WAPPER(StructSig, ViewerCount, int, 3000)

 struct ViewerCountTable
 {
	typedef TL::MkTlst<
		DefectSig<ViewerCount>
		, StructSig<ViewerCount>
	>::Result items_list;
	typedef TL::Factory<items_list> TItems;
	TItems items;
	const wchar_t *name(){return L"ViewerCountTable";}
 };

DEFINE_PARAM(iCU     , unsigned, 1 << 0) //цепи управления
DEFINE_PARAM(iKM2_DC , unsigned, 1 << 1)
DEFINE_PARAM(iKM3_AC , unsigned, 1 << 2)
DEFINE_PARAM(iCycle , unsigned, 1 << 3)
DEFINE_PARAM(iP1     , unsigned, 1 << 4)
DEFINE_PARAM(iP2     , unsigned, 1 << 5)
DEFINE_PARAM(iCOPT   , unsigned, 1 << 6)
DEFINE_PARAM(iControl, unsigned, 1 << 7)

struct InputBitTable
 {
	typedef TL::MkTlst<
		iCU    
		, iKM2_DC 
		, iKM3_AC 
		, iCycle 
		, iP1     
		, iP2     
		, iCOPT   
		, iControl
	>::Result items_list;
	typedef TL::Factory<items_list> TItems;
	TItems items;
	const wchar_t *name(){return L"InputBitTable";}
 };

DEFINE_PARAM(oDC_ON1 , unsigned, 1 << 0)
DEFINE_PARAM(oAC_ON  , unsigned, 1 << 1)
DEFINE_PARAM(oDC_ON2 , unsigned, 1 << 2)
DEFINE_PARAM(oWork   , unsigned, 1 << 3)
DEFINE_PARAM(oStart  , unsigned, 1 << 4)
DEFINE_PARAM(oToShift, unsigned, 1 << 5)
DEFINE_PARAM(oC1     , unsigned, 1 << 6)
DEFINE_PARAM(oC2     , unsigned, 1 << 7)

struct OutputBitTable
 {
	typedef TL::MkTlst<
		oDC_ON1 
		, oAC_ON  
		, oDC_ON2 
		, oWork   
		, oStart  
		, oToShift
		, oC1     
		, oC2     
	>::Result items_list;
	typedef TL::Factory<items_list> TItems;
	TItems items;
	const wchar_t *name(){return L"OutputBitTable";}
 };

STR_PARAM(NamePlate1730, 64, L"PCIE-1730,BID#0")
 struct NamePlate1730ParametersTable
 {
	 typedef TL::MkTlst<
		 NamePlate1730
	 >::Result items_list;
	 typedef TL::Factory<items_list> TItems;
	 TItems items;
	 const wchar_t *name(){return L"NamePlate1730ParametersTable";}
 };

struct ParametersBase
 {
	 typedef TL::MkTlst<
		 ColorTable	
		 , OnTheJobTable
		 , ViewerCountTable
		 , InputBitTable
		 , OutputBitTable
		 , NamePlate1730ParametersTable
	 >::Result one_row_table_list;

	 typedef TL::MkTlst<
		  CurrentParametersTable		 
		 , ParametersTable			   
		 , ThresholdsTable			  
		 , DeadAreaTable	
		 , MedianFiltreTable		 
		 , AnalogFilterTable
		 , L502ParametersTable
		 , KoeffSignTable
		 , AxesGraphsTable
	 >::Result multy_row_table_list;

	 typedef TL::MkTlst<
		  multy_row_table_list
		 , one_row_table_list
	 >::Result multy_type_list; 

	 typedef TL::MultyListToList<multy_type_list>::Result type_list;
	 typedef TL::Factory<type_list> TTables;
	 TTables tables;
	 wchar_t path[512];
	 const wchar_t *name();
 };

struct AppBase
{
	void Init();
	static void InitTypeSizeTables(CBase &);
};

template<class T>int CurrentId()
{
	  return Singleton<ParametersTable>::Instance().items.get<T>().value;
}

template<class T>int CountId(CBase &base, int num)
{
	ADODB::_RecordsetPtr rec;
	Select<ParametersTable>(base).eq<T>(num).Execute(rec);
	int i = 0;
	while (!rec->EndOfFile) 
	{			
		++i;
		rec->MoveNext(); 
	}
	return i;
}
template<class T>void UpdateId(CBase &base, int num)
{
   CurrentParametersTable &current = Singleton<CurrentParametersTable>::Instance();
   Select<CurrentParametersTable>(base).ID(1).Execute(current);
   ParametersTable &t = Singleton<ParametersTable>::Instance();
   t.items.get<T>().value = num;
   UpdateWhere<ParametersTable>(t, base).ID(current.items.get<CurrentID>().value).Execute();
}