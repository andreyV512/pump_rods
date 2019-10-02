#include "Dialogs.h"
#include "DlgTemplates\ParamDlgNew.h"

MIN_EQUAL_VALUE(DefectSig<CenterFrequency>, 0)
	MAX_EQUAL_VALUE(DefectSig<CenterFrequency>, 1000)
	PARAM_TITLE(DefectSig<CenterFrequency>, L"Средняя частота фильтра")

	MIN_EQUAL_VALUE(DefectSig<WidthFrequency>, 0)
	MAX_EQUAL_VALUE(DefectSig<WidthFrequency>, 1000)
	PARAM_TITLE(DefectSig<WidthFrequency>, L"Ширина пропускания фильтра")

	MIN_EQUAL_VALUE(DefectSig<CutoffFrequency>, 0)
	MAX_EQUAL_VALUE(DefectSig<CutoffFrequency>, 1000)
	PARAM_TITLE(DefectSig<CutoffFrequency>, L"Частота отсечения фильтра")

	PARAM_TITLE(DefectSig<TypeFiltre>, L"Тип фильтра")

	PARAM_TITLE(DefectSig<CutoffFrequencyON>, L"Включение фильтра")

	MIN_EQUAL_VALUE(StructSig<CutoffFrequency>, 0)
	MAX_EQUAL_VALUE(StructSig<CutoffFrequency>, 1000)
	PARAM_TITLE(StructSig<CutoffFrequency>, L"Частота отсечения фильтра")
	PARAM_TITLE(StructSig<CutoffFrequencyON>, L"Включение фильтра")

	typedef GROUP_BOX(DefectSig<CenterFrequency>, DefectSig<WidthFrequency>, DefectSig<TypeFiltre>, DefectSig<CutoffFrequencyON>) __def_band_pass_param__;
PARAM_TITLE(__def_band_pass_param__, L"Дефектоскопия")

	typedef GROUP_BOX(DefectSig<CutoffFrequency>, DefectSig<TypeFiltre>, DefectSig<CutoffFrequencyON>) __def_low_param__;
PARAM_TITLE(__def_low_param__, L"Дефектоскопия")

	typedef GROUP_BOX(StructSig<CutoffFrequency>, StructSig<CutoffFrequencyON>) __str_param__;
PARAM_TITLE(__str_param__, L"Структура")

template<>struct FillComboboxList<DefectSig<TypeFiltre>>			 
{															 
	void operator()(HWND h, DefectSig<TypeFiltre> &t)			 
	{	
		static const wchar_t *typeFiltreNames[] ={L"Низкочастотный фильтр", L"Полосовой фильтр"};
		for(int i = 0; i < dimention_of(typeFiltreNames); ++i)	
		{													 
			ComboBox_AddString(h, typeFiltreNames[i]);			 
		}													 
	}														
};															 
template<>struct CurrentValue<DefectSig<TypeFiltre>>				 
{															 
	void operator()(HWND h, DefectSig<TypeFiltre> &t)			 
	{		
		ComboBox_SetCurSel(h, t.value);
	}
};
template<>struct DlgSubItems<DefectSig<TypeFiltre>, int>: ComboBoxSubItem<DefectSig<TypeFiltre>>{};

DO_NOT_CHECK(DefectSig<TypeFiltre>)


template<>struct Dialog::NoButton<DefectSig<TypeFiltre>>{};

template<class P>struct __command__<Dialog::NoButton<DefectSig<TypeFiltre>>, P>
{
	typedef Dialog::NoButton<DefectSig<TypeFiltre>> O;
	bool operator()(O *o, P *p)
	{
		if(1 == p->e.isAcselerator)
		{
			HWND h = p->owner.items.get<Dialog::DlgItem2<DefectSig<TypeFiltre>, P::Owner>>().hWnd;
			//
			if(p->e.hControl == h)
			{
				int t = ComboBox_GetCurSel(h);
				dprint("%s %d\n", typeid(O).name(), t);
				if(p->owner.additionalData->id != t)
				{
					p->owner.additionalData->current = t;
					p->owner.additionalData->changed = true;
					EndDialog(p->e.hwnd, FALSE);
				}
				return false;
			}			
		}
		return true;
	}
};

template<template<class, class>class W, class Dlg, class P>struct Dialog::__init_unit__<W<DefectSig<TypeFiltre>, Dlg>, P>
{
	typedef W<DefectSig<TypeFiltre>, Dlg> O;
	void operator()(P &p)
	{
		O &item = p.items.get<O>();
		item.value.value = item.owner->additionalData->id;
		item.Init(p.h, p.x, p.width, p.height);
	};
};

struct AdditionalParams
{
	const int id;
	int &current;
	bool &changed;
	AdditionalParams(int id, int &current, bool &changed): id(id), current(current), changed(changed){}
};

template<template<class, class>class W, class Dlg>struct __data_from_widget__<W<DefectSig<TypeFiltre>, Dlg>, int>
{
	typedef W<DefectSig<TypeFiltre>, Dlg> O;
	int operator()(O &o)
	{
		return ComboBox_GetCurSel(o.hWnd);
	}
};

template<class To, class From>struct __copy_items__
{
	void operator()(To &to, From &from)
	{
		to.value = from.get<To>().value;
	}
};

void FilterDlg::Do(HWND h)
{
	bool changed = true;
	int current_filtre = Singleton<AnalogFilterTable>::Instance().items.get<DefectSig<TypeFiltre>>().value;
	AdditionalParams typeLowFiltre(TypeLowFiltre, current_filtre, changed);
	AdditionalParams typeBandPassFiltre(TypeBandPassFiltre, current_filtre, changed);
	AnalogFilterTable table;

	TL::foreach<AnalogFilterTable::items_list, __copy_items__>()(table.items, Singleton<AnalogFilterTable>::Instance().items);

	while(changed)
	{
		changed = false;
		switch(current_filtre)
		{
		case TypeLowFiltre:
			{
				if(Dialog::Templ<ParametersBase, AnalogFilterTable
					, TL::MkTlst<
					__def_low_param__
					, __str_param__			
					>::Result
					, 550
					, TL::MkTlst<OkBtn, CancelBtn, Dialog::NoButton<DefectSig<TypeFiltre>>>::Result
					, AdditionalParams
					>(table, &typeLowFiltre).Do(h, L"Настройки низкочастотного фильтра"))
				{
					TL::foreach<AnalogFilterTable::items_list, __copy_items__>()(Singleton<AnalogFilterTable>::Instance().items, table.items);
					return;
				}
			}
			break;
		case TypeBandPassFiltre:
			{
				if(Dialog::Templ<ParametersBase, AnalogFilterTable
					, TL::MkTlst<
					__def_band_pass_param__
					, __str_param__
					>::Result
					, 550
					, TL::MkTlst<OkBtn, CancelBtn, Dialog::NoButton<DefectSig<TypeFiltre>>>::Result
					, AdditionalParams
					>(table, &typeBandPassFiltre).Do(h, L"Настройки полосового фильтра"))
				{
					TL::foreach<AnalogFilterTable::items_list, __copy_items__>()(Singleton<AnalogFilterTable>::Instance().items, table.items);
					return;
				}
			}
			break;
		}
	}
}

