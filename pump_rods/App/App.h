#pragma once
#include "templates\typelist.hpp"
#include "1730/Device1730.h"

template<class T>struct DefectSig; 
template<class T>struct StructSig; 

namespace DataItem
{
	struct Defectoscope;
	struct Structure;
}

namespace App
{
	static const int buffer_size = 60 * 10000;
	typedef TL::MkTlst<DefectSig<DataItem::Defectoscope>, StructSig<DataItem::Structure>>::Result data_item_list;
	void Init();
	void Destroy();

	wchar_t *Salt();

	void UpdateViewers();
	void CleanViewers();

	void TopLabel(wchar_t *);
}

extern Device1730 device1730;