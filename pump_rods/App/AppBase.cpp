#include "AppBase.h"

void TrimTypeList(wchar_t *buf, wchar_t *s)
{
	wchar_t *res = buf;
	int len = (int)wcslen(s);
	for(int i = 0; i < len; ++i)
	{
		if(isalnum(s[i])) *res++ = s[i];
	}
}

namespace
{
template<class O, class P>struct row_table
{
	void operator()(P *p)
	{
		O &o = Singleton<O>::Instance();
		Select<O>(*p).ID(1).Execute(o);
	}
};

template<int N>void DPrint(Holder<N> &d){dprint("%S", (wchar_t *)d);}
	template<class O, class P>struct __default_param__
	{
		void operator()(O *o, P *){}
	};	
	template<class X, class P>struct __default_param__<ID<X>, P>
	{
		typedef ID<X> O;
		void operator()(O *o, P *base)
		{
			X &x = Singleton<X>::Instance();
		    Select<X>(*base).ID(o->value).Execute(x);
			TL::foreach<typename X::items_list, __default_param__XXX>()(&x.items, base);
		}
	};
	template<class O, class P>struct __default_param__XXX
	{
		void operator()(O *o, P *){}
	};	
	template<class X, class P>struct __default_param__XXX<ID<X>, P>
	{
		typedef ID<X> O;
		void operator()(O *o, P *base)
		{
			X &x = Singleton<X>::Instance();
		    Select<X>(*base).ID(o->value).Execute(x);
			TL::foreach<typename X::items_list, __default_param__>()(&x.items, base);
		}
	};
}

void AppBase::InitTypeSizeTables(CBase &base)
{
	CurrentParametersTable x;
	Select<CurrentParametersTable>(base).ID(1).Execute(x);
	ParametersTable	&p = Singleton<ParametersTable>::Instance();
	Select<ParametersTable>(base).ID(x.items.get<CurrentID>().value).Execute(p);
	TL::foreach<typename ParametersTable::items_list, __default_param__>()(&p.items, &base);
}

template<class T>struct __need_to_redefine__;
#define VALUE_DEFAULT(item, val)template<>struct __need_to_redefine__<item>{item::type_value value;__need_to_redefine__():value(val){}};

VALUE_DEFAULT(RodLenght, 7000)
VALUE_DEFAULT(DefectSig<First<DeathZone>>, 400)
VALUE_DEFAULT(DefectSig<Second<DeathZone>>, 400)
VALUE_DEFAULT(StructSig<First<DeathZone>>, 400)
VALUE_DEFAULT(StructSig<Second<DeathZone>>, 400)

template<class O, class P>struct __insert_value__
{
	void operator()(P &p)
	{
		p.get<O>().value = __need_to_redefine__<O>().value;
		//VALUE_DEFAULT(DefectSig<Second<DeathZone>>, 400) <---- определить с помощью макроса(тип, значение)
	}
};

#define SET_PARAMS(table, ...)\
	table _##table;\
	TL::foreach<TL::MkTlst<__VA_ARGS__>::Result, __insert_value__>()(_##table.items);\
	Insert_Into<table>(_##table, base).Execute();\
	par.items.get<ID<table>>().value = Select<table>(base).eq_all<table::items_list>(&_##table.items).Execute();

#undef VALUE_DEFAULT

void AppBase::Init()
{
	ParametersBase parameters;
	CBase base(
		parameters.name()
		, CreateDataBase<ParametersBase::type_list, SetDefault<ParametersBase::type_list> >()
		, parameters.tables
		);
	if(base.IsOpen())
	{
		TL::foreach<ParametersBase::one_row_table_list, row_table>()(&base);

		InitTypeSizeTables(base);

		NameParam name;
		name.value = L"НШ22";
		int id = Select<ParametersTable>(base).eq<NameParam>(name.value).Execute();
		if(0 == id)
		{
			ParametersTable par;
			par.items.get<NameParam>().value = name.value;
//...........................................................
			SET_PARAMS(DeadAreaTable
				, DefectSig<First<DeathZone>>
				, DefectSig<Second<DeathZone>>
				, StructSig<First<DeathZone>>
				, StructSig<Second<DeathZone>>
				, RodLenght
				)
//...................................................................................................
			Insert_Into<ParametersTable>(par, base).Execute();
		}
	}
}
//------------------------------------------------------------------------
const wchar_t *ParametersBase::name()
{
	GetModuleFileName(0, path, dimention_of(path));
	int len = (int)wcslen(path);
	path[len - 4] = 0;
	return path;
}
