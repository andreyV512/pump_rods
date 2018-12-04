#pragma once

#include <vector>
#include <algorithm>
#include "TableData.h"

template<class Base, class Table, class Col>class PacketTemplateData: public TableData
{
	template<class, class>struct __load__;
	std::vector<std::wstring> data;
public:		
	PacketTemplateData();
	void Init();
	void Add(wchar_t *);
	void Del(wchar_t *);
	const wchar_t *Cell(int, int);
	unsigned Count();
	void Header(HWND);
};

template<class Base, class Table, class Col>PacketTemplateData<Base, Table, Col>::PacketTemplateData()
{
	Init();
}

template<class Base, class TableX, class ColX>template<class Table, class Data>struct PacketTemplateData<Base, TableX, ColX>::__load__
{
	bool operator()(unsigned, Table &table, Data &data)
	{
		data.data.push_back(table.items.get<ColX>().value.buffer);
		return false;
	}
};

template<class Base, class Table, class Col>void PacketTemplateData<Base, Table, Col>::Init()
{
	CBase base(Base().name());
	if(base.IsOpen())
	{
		Select<Table>(base).ExecuteLoop<__load__>(*this);
	}
}

template<class Base, class Table, class Col>void PacketTemplateData<Base, Table, Col>::Add(wchar_t *s)
{
	CBase base(Base().name());
	if(base.IsOpen())
	{
		typename Col::type_value val;
		val = s;
		if(0 == Select<Table>(base).eq<Col>(val).Execute())
		{
			Table t;
			t.items.get<Col>().value = s;
			Insert_Into<Table>(t, base).Execute();
			data.push_back(s);
		}
	}
}

namespace
{
	struct Compare
	{
		wchar_t *w;
		Compare(wchar_t *w) : w(w){}
		bool operator()(std::wstring &s) { return 0 == wcscmp(s.c_str(), w); }
	};
}

template<class Base, class Table, class Col>void PacketTemplateData<Base, Table, Col>::Del(wchar_t *s)
{
	CBase base(Base().name());
	if(base.IsOpen())
	{
		typename Col::type_value val;
		val = s;
		unsigned id = Select<Table>(base).eq<Col>(val).Execute();
		if(0 != id)
		{			
			Delete<Table>(base).ID(id).Execute();			
			data.erase(remove_if(data.begin(), data.end(), Compare(s)), data.end());
		}
	}
}

template<class Base, class Table, class Col>const wchar_t *PacketTemplateData<Base, Table, Col>::Cell(int i, int)
{
	return data[i].c_str();
}

template<class Base, class Table, class Col>unsigned PacketTemplateData<Base, Table, Col>::Count()
{
	return (unsigned)data.size();
}

template<class Base, class Table, class Col>void PacketTemplateData<Base, Table, Col>::Header(HWND h)
{
	 SetGridHeader<typename Table::items_list> _(h);	
}