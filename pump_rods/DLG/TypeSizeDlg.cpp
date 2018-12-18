#include "Dialogs.h"
#include "Windows/MainWindow/MainWindow.h"
#include "DlgTemplates\ParamDlgNew.h"

namespace
{
	struct AddOkBtn;
	typedef Dialog::Templ<ParametersBase, ParametersTable
		, TL::MkTlst<NameParam>::Result
		, 400
		, TL::MkTlst<AddOkBtn, CancelBtn>::Result
	> Add_Typesize;
	struct AddOkBtn
	{
		static const int width = 120;
		static const int height = 30;
		static const int ID = IDOK;
		wchar_t *Title(){return L"Применить";}
		template<class Owner>void BtnHandler(Owner &owner, HWND h)
		{
			if(TestPassword<Owner::Base, Owner::Table>()(h))
			{
				wchar_t buf[128];

				//GetWindowText(owner.items.get<Dialog::DlgItem2<DiametrTube, Add_Typesize> >().hWnd, buf, dimention_of(buf));
				//int t = owner.table.items.get<DiametrTube>().value = _wtoi(buf);
				//for(int i = 0; i < dimention_of(diameterTubes); ++i)
				//{
				//	if(diameterTubes[i] == t)
				//	{
				//		owner.table.items.get<CrossCountSensors>().value = countSens[i];
				//		break;
				//	}
				//}
				//
				GetWindowText(owner.items.get<Dialog::DlgItem2<NameParam, Add_Typesize> >().hWnd, buf, dimention_of(buf));
				if(0 == buf[0] || 0 == wcscmp(L"NONAME", buf))
				{
					MessageBox(h, L"Введите название типоразмера", L"Ошибка!!!", MB_ICONERROR);
					return;
				}				
				CBase base(Owner::Base().name());
				if(base.IsOpen())
				{
					//wchar_t sub[16];
					//_itow(t, sub, 10);
					//size_t len = wcslen(sub);
					//if(0 != wcsncmp(sub, buf, len))
					//{
					//	wsprintf(owner.table.items.get<NameParam>().value.buffer, L"%d%s", t, buf);
					//}
					//else
					{
						owner.table.items.get<NameParam>().value = buf;
					}
					int id = Select<Owner::Table>(base).eq<NameParam>(owner.table.items.get<NameParam>().value).Execute();
					if(0 != id)
					{
						MessageBox(h, L"Название типоразмера есть в базе данных", L"Предупреждение!!!", MB_ICONEXCLAMATION);
					}
					else
					{
						Insert_Into<Owner::Table>(owner.table, base).Execute<Owner::Table::items_list>();
						id = Select<Owner::Table>(base).eq<NameParam>(owner.table.items.get<NameParam>().value).Execute();	
						MessageBox(h, L"Типоразмер создан", L"Сообщение!!!", MB_ICONINFORMATION);
					}
					CurrentParametersTable &curr = Singleton<CurrentParametersTable>::Instance();
					curr.items.get<CurrentID>().value = id;
					UpdateWhere<CurrentParametersTable>(curr, base).ID(1).Execute();
					//
					MainWindow &o = Singleton<MainWindow>::Instance();
					o.select.AddMenuItem(owner.table.items.get<NameParam>().value.buffer);					
					o.select.Do(TCommand(o.hWnd, 0, 1, o.select.hWnd));
				}
				EndDialog(h, TRUE);
			}
		}
	};

	template<class T, class D>struct  __more_than_one__
	{
		bool operator()(unsigned, T &t, D &d)
		{
			++d;
			return d > 1;
		}
	};

	struct DelOkBtn;
	typedef Dialog::Templ<ParametersBase, ParametersTable
		, TL::MkTlst<NameParam>::Result, 550, TL::MkTlst<DelOkBtn, CancelBtn>::Result> Del_Typesize;

	struct DelOkBtn
	{
		static const int width = 120;
		static const int height = 30;
		static const int ID = IDOK;
		wchar_t *Title(){return L"Применить";}
		template<class Owner>void BtnHandler(Owner &owner, HWND h)
		{
			if(TestPassword<Owner::Base, Owner::Table>()(h))
			{
				wchar_t buf[128];
				GetWindowText(owner.items.get<Dialog::DlgItem2<NameParam, Del_Typesize> >().hWnd, buf, dimention_of(buf));
				if(0 == buf[0])
				{
					MessageBox(h, L"Введите название типоразмера", L"Ошибка!!!", MB_ICONERROR);
					return;
				}
				CBase base(Owner::Base().name());
				if(base.IsOpen())
				{
					int countItems = 0;
					Select<Owner::Table>(base).ExecuteLoop<__more_than_one__>(countItems);
					if(countItems < 2)
					{
						MessageBox(h, L"В базе должно быть больше одного типоразмера", L"Предупреждение!!!", MB_ICONEXCLAMATION);
						return;
					}					
					wchar_t buf2[128];
					wcsncpy_s(buf2, buf, wcslen(buf));
				
					MainWindow &o = Singleton<MainWindow>::Instance();
					o.select.DelMenuItem(buf);
				
					if(buf[0] != 0)
					{
						NameParam n;
						n.value = buf2;
						Delete<Owner::Table>(base).eq<NameParam>(n.value).Execute();
						n.value = buf;
						int id = Select<Owner::Table>(base).eq<NameParam>(n.value).Execute();
						if(id)
						{
							Update<CurrentParametersTable>(base).set<CurrentID>(id).Where().ID(1).Execute();
							AppBase::InitTypeSizeTables(base);
							
							o.select.Do(TCommand(o.hWnd, 0, 1, o.select.hWnd));
						}
					}
				}
				EndDialog(h, TRUE);
			}
		}
	};
}

DO_NOT_CHECK(NameParam)
PARAM_TITLE(NameParam, L"")
template<int N>struct DlgSubItems<NameParam, Holder<N> >: EditItems<NameParam, 420>{};

void AddTypeSizeDlg::Do(HWND h)
{
	ParametersTable t;
	if(Dialog::Templ<ParametersBase, ParametersTable
		, TL::MkTlst<NameParam>::Result
		, 400
		, TL::MkTlst<AddOkBtn, CancelBtn>::Result
	>(t).Do(h, L"Создать типоразмер")
	)
	{}
}

void DelTypeSizeDlg::Do(HWND h)
{
	ParametersTable t;
	t.items.get<NameParam>().value = Singleton<ParametersTable>::Instance().items.get<NameParam>().value;
	if(Dialog::Templ<ParametersBase, ParametersTable
		, TL::MkTlst<NameParam>::Result
		, 550
		, TL::MkTlst<DelOkBtn, CancelBtn>::Result
	>(t).Do(h, L"Удалить типоразмер")
	)
	{}
}

