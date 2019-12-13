#include "TemplDlg.hpp"
#include "Windows/DetailWindow/TemplWindow.hpp"

TEMPL_MIN_EQUAL_VALUE(CenterFrequency, 0)
	TEMPL_MAX_EQUAL_VALUE(CenterFrequency, 2000)
	TEMPL_PARAM_TITLE(CenterFrequency, L"Средняя частота фильтра Гц")

	TEMPL_MIN_EQUAL_VALUE(WidthFrequency, 0)
	TEMPL_MAX_EQUAL_VALUE(WidthFrequency, 2000)
	TEMPL_PARAM_TITLE(WidthFrequency, L"Полоса пропускания фильтра Гц")

	TEMPL_MIN_EQUAL_VALUE(PassBandRippleDb, 0.001)
	TEMPL_MAX_EQUAL_VALUE(PassBandRippleDb, 200.0)
	TEMPL_PARAM_TITLE(PassBandRippleDb, L"Затухание в полосе подавления Db")

struct AdditionalParams
{
	const int id;
	int &current;
	bool &changed;
	AdditionalParams(int id, int &current, bool &changed): id(id), current(current), changed(changed){}
};
template<>struct Dialog::NoButton<DefectSig<TypeFiltre>>{};

template<class P>struct __command__<Dialog::NoButton<DefectSig<TypeFiltre>>, P>
{
	typedef Dialog::NoButton<DefectSig<TypeFiltre>> O;
	bool operator()(O *o, P *p)
	{
		if(1 == p->e.isAcselerator)
		{
			auto &item = p->owner.items.get<Dialog::DlgItem2<DefectSig<TypeFiltre>, P::Owner>>();
			HWND h = item.hWnd;

			if(p->e.hControl == h)
			{
				int t = ComboBox_GetCurSel(h);
				dprint("%s %d\n", typeid(O).name(), t);
				item.value.value = t;
				if(p->owner.additionalData->id != t)
				{
					p->owner.additionalData->current = t;
					p->owner.additionalData->changed = true;
					EndDialog(p->e.hwnd, FALSE);
				}
				return false;
			}
		}
		return true;
	}
};

namespace TemplDlg
{
	void  FilterDlg<DefectSig>::Do(HWND h)
	{
		TemplWindow<DefectSig> &e = *(TemplWindow<DefectSig> *)GetWindowLongPtr(h, GWLP_USERDATA);
		FrameViewer &frame =  e.viewers.get<FrameViewer>();
		AnalogFilterTable par;

		int frame_cutoffFrequency = par.items.get< DefectSig<CutoffFrequency>>().value   = frame.cutoffFrequency;
		bool frame_cutoffFrequencyON = par.items.get< DefectSig<CutoffFrequencyON>>().value = frame.cutoffFrequencyON;
		int frame_centerFrequency = par.items.get< DefectSig<CenterFrequency>>().value   = frame.centerFrequency;
		int frame_widthFrequency = par.items.get< DefectSig<WidthFrequency>>().value    = frame.widthFrequency;
		int frame_typeFiltre = par.items.get< DefectSig<TypeFiltre>>().value        = frame.typeFiltre;

		int frame_order = par.items.get< DefectSig<Order>>().value        = frame.order;

		double frame_stopBandDb = par.items.get< DefectSig<StopBandDb>>().value = frame.stopBandDb;
		double frame_passBandRippleDb = par.items.get< DefectSig<PassBandRippleDb>>().value        = frame.passBandRippleDb;

		bool loop = true;
		int current_filtre = frame.typeFiltre;
		AdditionalParams typeLowFiltre(TypeLowFiltre, frame.typeFiltre, loop);
		AdditionalParams typeBandPassFiltre(TypeBandPassFiltre, frame.typeFiltre, loop);

		bool okButton = false;

		while(loop)
		{
			loop = false;
			switch(frame.typeFiltre)
			{
			case TypeLowFiltre:
				if(Dialog::Templ<NullType, AnalogFilterTable
					, TL::MkTlst<
					DefectSig<CutoffFrequency>
					, DefectSig<Order>
					, DefectSig<StopBandDb>
					, DefectSig<TypeFiltre>
					, DefectSig<CutoffFrequencyON>
					>::Result
					, 550
					, TL::MkTlst<DefOkBtn, CancelBtn
					, Dialog::NoButton<DefectSig<CutoffFrequency>>
					, Dialog::NoButton<DefectSig<CutoffFrequencyON>>
					, Dialog::NoButton<DefectSig<TypeFiltre>>
					, Dialog::NoButton<DefectSig<Order>>
					, Dialog::NoButton<DefectSig<StopBandDb> >
					>::Result
					, AdditionalParams
					>(par, &typeLowFiltre).Do(h, L"Настройки низкочастотного фильтра"))
				{
					frame.centerFrequency   = frame_centerFrequency;
					frame.widthFrequency    = frame_widthFrequency;
					okButton = true;
				}
				break;
			case TypeBandPassFiltre:
				if(Dialog::Templ<NullType, AnalogFilterTable
					, TL::MkTlst<
					DefectSig<CenterFrequency>
					, DefectSig<WidthFrequency>
					, DefectSig<Order>
					, DefectSig<PassBandRippleDb>
					, DefectSig<TypeFiltre>
					, DefectSig<CutoffFrequencyON>
					>::Result
					, 550
					, TL::MkTlst<DefOkBtn, CancelBtn
					, Dialog::NoButton<DefectSig<WidthFrequency>>
					, Dialog::NoButton<DefectSig<CenterFrequency>>

					, Dialog::NoButton<DefectSig<CutoffFrequencyON>>
					, Dialog::NoButton<DefectSig<TypeFiltre>>
					, Dialog::NoButton<DefectSig<PassBandRippleDb>>
					, Dialog::NoButton<DefectSig<Order>>
					>::Result
					, AdditionalParams
					>(par, &typeBandPassFiltre).Do(h, L"Настройки полосового фильтра"))
				{
					frame.cutoffFrequency   = frame_cutoffFrequency;
					okButton = true;
				}
				break;
			}
			Repaint<DefectSig>( e.viewers.get<TemplWindow<DefectSig>::Viewer>(), frame);
		}
		if(!okButton)
		{
			frame.centerFrequency   = frame_centerFrequency;
			frame.widthFrequency    = frame_widthFrequency;
			frame.cutoffFrequency   = frame_cutoffFrequency;
			frame.typeFiltre = frame_typeFiltre;
			frame.cutoffFrequencyON = frame_cutoffFrequencyON;
			frame.order    = frame_order;
			Repaint<DefectSig>( e.viewers.get<TemplWindow<DefectSig>::Viewer>(), frame);
		}
	}
}
