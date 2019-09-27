#pragma once
#include "templates/typelist.hpp"
#include "Base/Base.hpp"
#include "App/AppBase.h"
#include "window_tool/message.h"
#include "DlgTemplates\ParamDlg.hpp"
#include "window_tool\Pass.h"
#include "tools_debug\DebugMess.h"
//--------------------------------------------------------------------------------------------------------------------------
bool TemplDlg_Do(HWND hWnd, wchar_t *title, DLGPROC proc, LPARAM param);

template<class O, class P>struct __make_btn__
{
	void operator()(O *o, P *p)
	{
		CreateWindow(L"button", o->Title()
			, WS_VISIBLE | WS_CHILD | WS_TABSTOP
			,p->offs, p->height, O::width, 30, p->h, (HMENU)O::ID, (HINSTANCE)::GetModuleHandle(NULL), NULL
			);
		p->offs += O::width + 10;
	}
};
template<class O, class P>struct TemplDialogCtlColorEdit 
{
	bool operator()(O *, P *)
	{
		return true;
	}
};

template<class O, class P>struct __command__ 
{
	bool operator()(O *o, P *p)
	{
		if(p->e.id == O::ID)
		{
			o->BtnHandler(p->owner, p->e.hwnd);
			return false;
		}
		return true;
	}
};

template<class, class Table>struct TestPassword;

struct OkBtn
{
	static const int width = 120;
	static const int height = 30;
	static const int ID = IDOK;
	wchar_t *Title(){return L"Применить";}
	template<class Owner>void BtnHandler(Owner &owner, HWND h)
	{
		if(TestPassword<Owner::Base, Owner::Table>()(h))
		{
			if(__ok_table_btn__<
				Owner::Base, Owner::Table
				, typename TL::SubListFromMultyList<typename Owner::Base::multy_type_list, Owner::Table>::Result
			>()(h, owner))  
			{
				EndDialog(h, TRUE);
			}
		}
	}
};

struct CancelBtn
{
	static const int width = 120;
	static const int height = 30;
	static const int ID = IDCANCEL;
	wchar_t *Title(){return L"Закрыть";}
	template<class Owner>void BtnHandler(Owner &o, HWND h)
	{
		EndDialog(h, FALSE);
	}
};

template<class Table>struct TestPassword<ParametersBase, Table>
{
	bool operator()(HWND h)
	{
		static const int t = TL::TypeInMultyList<ParametersBase::multy_type_list, Table>::Result::value;
		return (0 == t)
			? TypesizePasswordDlg().Do(h)
			: OptionPasswordDlg().Do(h);
	}
};



