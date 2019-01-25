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

#define _DEF_VAL(T, val) table.items.get<T>().value = val;
#define DEF_VAL(params) _DEF_VAL##params
#define SET_PARAMS(Table, ...) {\
	Table table;\
	FOR_EACH(DEF_VAL, __VA_ARGS__)\
	Insert_Into<Table>(table, base).Execute();\
	par.items.get<ID<Table>>().value = Select<Table>(base).eq_all<Table::items_list>(&table.items).Execute();\
}
	
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
				, (DefectSig<First<DeathZone>> , 400)
				, (DefectSig<Second<DeathZone>>, 400)
				, (StructSig<First<DeathZone>> , 400)
				, (StructSig<Second<DeathZone>>, 400)
				, (RodLenght				   , 7000)
				)
//...................................................................................................
			Insert_Into<ParametersTable>(par, base).Execute();
		}
	}
}
#undef _DEF_VAL
#undef DEF_VAL
#undef SET_PARAMS
//------------------------------------------------------------------------
const wchar_t *ParametersBase::name()
{
	GetModuleFileName(0, path, dimention_of(path));
	int len = (int)wcslen(path);
	path[len - 4] = 0;
	return path;
}
