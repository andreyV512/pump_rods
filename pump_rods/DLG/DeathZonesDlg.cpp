#include "Dialogs.h"
#include "DlgTemplates\ParamDlgNew.h"

DO_NOT_CHECK(RodLenght)
PARAM_TITLE(RodLenght, L"Длина штанги(мм)")

template<template<class>class W>struct ParamTitle<W<First<DeathZone>>>
{
	wchar_t *operator()(){return L"Смещение начало штанги(мм)";}
};
template<template<class>class W>struct ParamTitle<W<Second<DeathZone>>>
{
	wchar_t *operator()(){return L"Смещение конец штанги(мм)";}
};

template<template<class>class W, template<class>class Z>struct LessEqual<W<Z<DeathZone>>>
{typename W<Z<DeathZone>>::type_value operator()(){return 0;}};
template<template<class>class W, template<class>class Z>struct LargenEqual<W<Z<DeathZone>>>
{typename W<Z<DeathZone>>::type_value operator()(){return 3000;}};

typedef GROUP_BOX(DefectSig<First<DeathZone>>, DefectSig<Second<DeathZone>>) death_def;
typedef GROUP_BOX(StructSig<First<DeathZone>>, StructSig<Second<DeathZone>>) death_str;

PARAM_TITLE(death_def, L"Дефектоскопия")
PARAM_TITLE(death_str, L"Структура")


void DeathZonesDlg::Do(HWND h)
{
	if(Dialog::Templ<ParametersBase, DeadAreaTable, TL::MkTlst<death_def, death_str, RodLenght>::Result>(
		Singleton<DeadAreaTable>::Instance()).Do(h, L"Смещения")
		)
	{
	}
}