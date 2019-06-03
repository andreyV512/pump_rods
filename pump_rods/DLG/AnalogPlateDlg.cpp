#include "Dialogs.h"
#include "DlgTemplates\ParamDlgNew.h"

/// \brief Настройки смещения и усиления каналов  платы сбора данных

PARAM_TITLE(DefectSig<ChannelL502>, L"Канал")
PARAM_TITLE(StructSig<ChannelL502>, L"Канал")
MIN_EQUAL_VALUE(DefectSig<ChannelL502>, 1)
MAX_EQUAL_VALUE(DefectSig<ChannelL502>, 32)
MIN_EQUAL_VALUE(StructSig<ChannelL502>, 1)
MAX_EQUAL_VALUE(StructSig<ChannelL502>, 32)

template<>struct DlgSubItems<DefectSig<ChannelL502>, int>: UpDownSubItem<DefectSig<ChannelL502>, 1, 32, 140>{};
template<>struct DlgSubItems<StructSig<ChannelL502>, int>: UpDownSubItem<StructSig<ChannelL502>, 1, 32, 140>{};

const wchar_t *SyncGainData[] ={L"10", L"5", L"2", L"1", L"0.5", L"0.2"};

#define COMBO_ITEMS(TYPE)\
	template<>struct FillComboboxList<TYPE>			 \
	{															 \
	void operator()(HWND h, TYPE &t)			 \
	{														 \
	for(int i = 0; i < dimention_of(SyncGainData); ++i)	 \
	{													 \
	ComboBox_AddString(h, SyncGainData[i]);			 \
	}													 \
	}														 \
	};															 \
	template<>struct CurrentValue<TYPE>				 \
	{															 \
	void operator()(HWND h, TYPE &t)			 \
	{														 \
	ComboBox_SetCurSel(h, Singleton<L502ParametersTable>::Instance().items.get<TYPE>().value);\
	}\
	};\
	template<>struct DlgSubItems<TYPE, int>: ComboBoxSubItem<TYPE>{};\
	template<class X, class P>struct __ok_btn__<Dialog::DlgItem2<TYPE, X>, P>\
	{  \
	typedef Dialog::DlgItem2<TYPE, X> O; \
	void operator()(O *o, P *p)	 \
	{						   \
	wchar_t s[128];					\
	GetWindowText(o->hWnd, s, dimention_of(s));	\
	int i = 0;											  \
	for(; i < dimention_of(SyncGainData); ++i)	\
	{																		  \
	if(0 == wcscmp(s, SyncGainData[i])) break;	  \
	}												 \
	o->value.value =  i;		   \
	p->update.set<typename TL::Inner<O>::Result>(o->value.value);  \
	dprint("set %d\n", i);\
	}	 \
	};

PARAM_TITLE(DefectSig<RangeL502>, L"Усиление")
PARAM_TITLE(StructSig<RangeL502>, L"Усиление")
COMBO_ITEMS(DefectSig<RangeL502>)
COMBO_ITEMS(StructSig<RangeL502>)
DO_NOT_CHECK(DefectSig<RangeL502>)
DO_NOT_CHECK(StructSig<RangeL502>)


PARAM_TITLE(DefectSig<ChannelSamplingRate>, L"Частота")
PARAM_TITLE(StructSig<ChannelSamplingRate>, L"Частота")

MIN_EQUAL_VALUE(DefectSig<ChannelSamplingRate>, 100)
MAX_EQUAL_VALUE(DefectSig<ChannelSamplingRate>, 100000)

MIN_EQUAL_VALUE(StructSig<ChannelSamplingRate>, 100)
MAX_EQUAL_VALUE(StructSig<ChannelSamplingRate>, 100000)

typedef GROUP_BOX(DefectSig<ChannelL502>, DefectSig<RangeL502>, DefectSig<ChannelSamplingRate>) Defect_Sig;
typedef GROUP_BOX(StructSig<ChannelL502>, StructSig<RangeL502>, StructSig<ChannelSamplingRate>) Struct_Sig;

PARAM_TITLE(Defect_Sig, L"Дефектоскопия")
PARAM_TITLE(Struct_Sig, L"Структура")

template<>struct TestPassword<ParametersBase, L502ParametersTable>
{
	bool operator()(HWND h)
	{
		return OptionPasswordDlg().Do(h);
	}
};

void AnalogPlatelDlg::Do(HWND h)
{
	if(Dialog::Templ<ParametersBase, L502ParametersTable
		, TL::MkTlst<
		Defect_Sig
		, Struct_Sig
		>::Result
		, 300
	>(Singleton<L502ParametersTable>::Instance()).Do(h, L"Настройка аналоговой платы"))
	{
	}
}
	