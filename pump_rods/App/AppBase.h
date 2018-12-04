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

template<class>struct DefectSig;
template<class>struct StructSig;
template<class>struct Thresh;
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

struct OnTheJobTable
{
	typedef TL::MkTlst<
		DefectSig<Check>
		, StructSig<Check>
	>::Result items_list;
	typedef TL::Factory<items_list> TItems;
	TItems items;
	const wchar_t *name(){return L"OnTheJobTable";}
};

struct CutoffFrequency; 
DEFINE_PARAM_WAPPER(DefectSig, CutoffFrequency, int, 20)
DEFINE_PARAM_WAPPER(StructSig, CutoffFrequency, int, 20)

 struct AnalogFilterTable
{
	typedef TL::MkTlst<
		DefectSig<CutoffFrequency>
		, StructSig<CutoffFrequency>
	>::Result items_list;
	typedef TL::Factory<items_list> TItems;
	TItems items;
	const wchar_t *name(){return L"AnalogFilterTable";}
};

struct MedianFiltreWidth;
DEFINE_PARAM_WAPPER(DefectSig, MedianFiltreWidth, int, 5)
DEFINE_PARAM_WAPPER(StructSig, MedianFiltreWidth, int, 5)

 struct MedianFiltreTable
 {
	 typedef TL::MkTlst<
		 DefectSig<MedianFiltreWidth>
		, StructSig<MedianFiltreWidth>
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
struct OffsetL502;
struct ChannelSamplingRate;
DEFINE_PARAM_WAPPER(DefectSig, ChannelSamplingRate, int, 10000)
DEFINE_PARAM_WAPPER(StructSig, ChannelSamplingRate, int, 10000)
DEFINE_PARAM_WAPPER(DefectSig, RangeL502, int, 0)
DEFINE_PARAM_WAPPER(StructSig, RangeL502, int, 0)
DEFINE_PARAM_WAPPER(DefectSig, OffsetL502, int, 1)
DEFINE_PARAM_WAPPER(StructSig, OffsetL502, int, 2)

struct L502ParametersTable
{
	typedef TL::MkTlst<
		DefectSig<ChannelSamplingRate>
		, StructSig<ChannelSamplingRate>
		, DefectSig<RangeL502>
		, StructSig<RangeL502>
		, DefectSig<OffsetL502>
		, StructSig<OffsetL502>
	>::Result items_list;
	typedef TL::Factory<items_list> TItems;
	TItems items;
	const wchar_t *name(){return L"L502ParametersTable";}
};

STR_PARAM(NameParam, 128, L"NONAME")
DEFINE_PARAM(DiametrPumpRods            , int, 19)
DEFINE_PARAM_ID(ThresholdsTable, int, 1)
DEFINE_PARAM_ID(DeadAreaTable, int, 1)
DEFINE_PARAM_ID(OnTheJobTable, int, 1)
DEFINE_PARAM_ID(AnalogFilterTable, int, 1)
DEFINE_PARAM_ID(MedianFiltreTable, int, 1)
DEFINE_PARAM_ID(L502ParametersTable, int, 1)
DEFINE_PARAM_ID(KoeffSignTable	   , int, 1)

 struct ParametersTable
 {
	typedef TL::MkTlst<
		ID<ThresholdsTable>
		, ID<DeadAreaTable>
		, ID<OnTheJobTable>
		, ID<AnalogFilterTable>
		, ID<MedianFiltreTable>
		, NameParam
		, DiametrPumpRods
	>::Result items_list;
	typedef TL::Factory<items_list> TItems;
	TItems items;
	const wchar_t *name(){return L"ParametersTable";}
 };

struct ViewerCount;
DEFINE_PARAM_WAPPER(DefectSig, ViewerCount, int, 10000)
DEFINE_PARAM_WAPPER(StructSig, ViewerCount, int, 10000)

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


struct ParametersBase
 {
	 typedef TL::MkTlst<
		 ColorTable	
		 , OnTheJobTable
		 , ViewerCountTable
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