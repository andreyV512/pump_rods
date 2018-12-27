﻿#pragma once
#include "templates\typelist.hpp"
#include "1730/Device1730.h"
#include "L502\Device502.h"

template<class T>struct DefectSig; 
template<class T>struct StructSig; 

template<class>struct On{};
template<class>struct Off{};

namespace DataItem
{
	struct Buffer;
}

namespace App
{
	static const int buffer_size = 60 * 10000;
	typedef TL::MkTlst<DefectSig<DataItem::Buffer>, StructSig<DataItem::Buffer>>::Result data_item_list;
	void Init();
	void Destroy();

	wchar_t *Salt();

	void UpdateViewers();
	void CleanViewers();

	void TopLabel(wchar_t *);
	bool InterruptView();

	bool DataCollectionDefectoscope();
	bool DataCollectionStructure();
}

extern Device1730 device1730;
extern Device502 l502;