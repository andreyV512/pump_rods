#pragma once
#include "templates\typelist.hpp"

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
}