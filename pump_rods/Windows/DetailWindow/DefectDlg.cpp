#include "DefectDlg.h"
#include "tools_debug/DebugMess.h"
#include "App/AppBase.h"
#include "DlgTemplates\ParamDlgNew.h"
#include "DefectWindow.h"
#include "Compute\Compute.h"

namespace
{
	MIN_EQUAL_VALUE(DefectSig<MedianFiltreWidth>, 3)
		MAX_EQUAL_VALUE(DefectSig<MedianFiltreWidth>, 15)
		PARAM_TITLE(DefectSig<MedianFiltreWidth>, L"Ширина фильтра")
		PARAM_TITLE(DefectSig<MedianFiltreON>, L"Включение фильтра")

		MIN_EQUAL_VALUE(StructSig<MedianFiltreWidth>, 3)
		MAX_EQUAL_VALUE(StructSig<MedianFiltreWidth>, 15)
		PARAM_TITLE(StructSig<MedianFiltreWidth>, L"Ширина фильтра")
		PARAM_TITLE(StructSig<MedianFiltreON>, L"Включение фильтра")

		//PARAM_TITLE(GROUP_BOX(DefectSig<MedianFiltreWidth>, DefectSig<MedianFiltreON>), L"Дефектоскопия")
		//PARAM_TITLE(GROUP_BOX(StructSig<MedianFiltreWidth>, StructSig<MedianFiltreON>), L"Структура")

		template<class O, class P>struct __def_ok_btn__
	{
		void operator()(O &o)
		{
			o.value.value =  __data_from_widget__<O, typename TL::Inner<O>::Result::type_value>()(o);
		}
	};
}

struct DefOkBtn
{
	static const int width = 120;
	static const int height = 30;
	static const int ID = IDOK;
	wchar_t *Title(){return L"Применить";}
	template<class Owner>void BtnHandler(Owner &t, HWND h)
	{
		if(!TL::find<typename Owner::list, __test__>()(&t.items, &h))return;
		TL::foreach<typename Owner::list, __def_ok_btn__>()(t.items);
		EndDialog(h, TRUE);
	}
};

void DefectMedianFiltre::Do(HWND h)
{
	DefectWindow *e = (DefectWindow *)GetWindowLongPtr(h, GWLP_USERDATA);
	FrameViewer &frame =  e->viewers.get<FrameViewer>();
	DefectWindow::Def &def = e->viewers.get<DefectWindow::Def>(); 
	MedianFiltreTable par;
	par.items.get< DefectSig<MedianFiltreWidth>>().value = frame.medianFiltreLength;
	par.items.get< DefectSig<MedianFiltreON>>().value = frame.medianFiltreON;
	if(Dialog::Templ<NullType, MedianFiltreTable
		, TL::MkTlst<
			DefectSig<MedianFiltreWidth>, DefectSig<MedianFiltreON>
		>::Result
		, 550
		, TL::MkTlst<DefOkBtn, CancelBtn>::Result
	>(Singleton<MedianFiltreTable>::Instance()).Do(h, L"Настройки медианного фильтра"))
	{
		frame.medianFiltreLength = par.items.get< DefectSig<MedianFiltreWidth>>().value;
		frame.medianFiltreON = par.items.get< DefectSig<MedianFiltreON>>().value;
//----------------------------------------------------------------------------------------
		L502ParametersTable::TItems &l502Param = Singleton<L502ParametersTable>::Instance().items;
		DefectSig<DataItem::Buffer> &o = Singleton<DefectSig<DataItem::Buffer>>::Instance();
		Compute::Compute(
			  o.inputData
			, o.currentOffset
			, frame.cutoffFrequency
			, frame.cutoffFrequencyON
			, frame.medianFiltreLength
			, frame.medianFiltreON
			, def.buffer
			, DataItem::output_buffer_size
			, l502Param.get<DefectSig<ChannelSamplingRate>>().value
			);

		double adcRange =  100.0 / DataItem::ADC_RANGE(l502Param.get<DefectSig<RangeL502>>().value);
		for(int i = 0; i < DataItem::output_buffer_size; ++i)
		{
			def.buffer[i] *= adcRange * frame.koef;
		}
	}
}