#include "Dialogs.h"
#include "DlgTemplates\ParamDlgNew.h"

DO_NOT_CHECK(RodLenght)
PARAM_TITLE(RodLenght, L"Длина трубы(мм)")

template<template<class>class W>struct ParamTitle<W<First<DeathZone>>>
{
	wchar_t *operator()(){return L"Мёртвая зона начало трубы(мм)";}
};
template<template<class>class W>struct ParamTitle<W<Second<DeathZone>>>
{
	wchar_t *operator()(){return L"Мёртвая зона конец трубы(мм)";}
};

template<template<class>class W, template<class>class Z>struct LessEqual<W<Z<DeathZone>>>
{typename W<Z<DeathZone>>::type_value operator()(){return 0;}};
template<template<class>class W, template<class>class Z>struct LargenEqual<W<Z<DeathZone>>>
{typename W<Z<DeathZone>>::type_value operator()(){return 1000;}};

void DeathZonesDlg::Do(HWND h)
{
	if(Dialog::Templ<ParametersBase, DeadAreaTable>(
		Singleton<DeadAreaTable>::Instance()
		).Do(h, L"Мёртвые зоны"))
	{
	}
}