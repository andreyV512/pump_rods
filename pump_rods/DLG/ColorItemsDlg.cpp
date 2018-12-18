#include "Dialogs.h"
#include <GdiPlus.h>
#include "tools_debug/DebugMess.h"
#include "window_tool/Dlgfiles.h"
#include "DlgTemplates/ParamDlgNew.h"
#include "App\App.h"

namespace{
	struct DefaultBtn;
	typedef Dialog::Templ<ParametersBase, ColorTable
		, ColorTable::items_list
		, 550
		, TL::MkTlst<OkBtn, CancelBtn, DefaultBtn>::Result 
	> ClrDlg;
	void __color_wm_paint(HWND hwnd, int color)
	{
		PAINTSTRUCT p;
		HDC hdc = BeginPaint(hwnd, &p);
		{		
			Gdiplus::Graphics g(hdc);
			RECT r;
			GetClientRect(hwnd, &r);
			g.FillRectangle(&Gdiplus::SolidBrush(Gdiplus::Color(color)), 0, 0, r.right, r.bottom);
		}
		EndPaint(hwnd, &p);
	}

	int __color_l_button_down(HWND hwnd)
	{
		ColorDialog c(hwnd);
		char d[4] = {-1};
		if(c())
		{
			*(int *)d = c.color.rgbResult | 0xff000000;
			char t = d[0];
			d[0] = d[2];
			d[2] = t;
			InvalidateRect(hwnd, NULL, true);
		}
		return *(int *)d;
	}

template<class T>struct __color__
{	
	static LRESULT CALLBACK Proc_(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
	{
		switch(msg)
		{
		case WM_PAINT:
			{
				T *t = (T *)GetWindowLongPtr(hwnd, GWLP_USERDATA);
				if(t)
				{
					__color_wm_paint(hwnd, t->value);
				}
			}
			return 0;
		case WM_LBUTTONDBLCLK:
		case WM_LBUTTONDOWN:
			{
				T *t = (T *)GetWindowLongPtr(hwnd, GWLP_USERDATA);
				if(t)
				{
			    t->value = __color_l_button_down(hwnd);
				}
			}
			return 0;
		}
		return CallWindowProc(OldWndProc, hwnd, msg, wParam, lParam);
	}
	static WNDPROC OldWndProc;
};

 template<class T>WNDPROC __color__<T>::OldWndProc;

template<class T>struct ColorButtonItem
{
	static const int DY = 25;
	HWND Init(HWND h, int &x, int &width, int &dy, T &t)
	{
		HWND hWnd = CreateWindow(L"button", L""
			, WS_VISIBLE | WS_CHILD | WS_TABSTOP
			,x, dy, 100, 30, h, (HMENU)0, (HINSTANCE)::GetModuleHandle(NULL), 0
			);
		__color__<T>::OldWndProc = (WNDPROC)GetWindowLongPtr(hWnd, GWLP_WNDPROC);
		SetWindowLongPtr(hWnd, GWLP_WNDPROC, (long ) __color__<T>::Proc_);
		SetWindowLongPtr(hWnd, GWLP_USERDATA, (long )&t);
		CreateWindow(L"static", ParamTitle<T>()()
			, WS_VISIBLE | WS_CHILD
			, 100 + 20, dy + 7, dlg_width, 20, h, 0, (HINSTANCE)::GetModuleHandle(NULL), NULL
			);
		dy += 35;

		return hWnd;
	}
};

#define DLG_SUB(n, txt)template<>struct DlgSubItems<n, int>: ColorButtonItem<n>{};\
template<>struct __data_from_widget__<Dialog::DlgItem2<n, ClrDlg>, int>\
{\
    int operator()(Dialog::DlgItem2<n, ClrDlg> &o)\
	{\
		return o.value.value;\
	}\
};\
DO_NOT_CHECK(n)\
PARAM_TITLE(n, txt)

DLG_SUB(Clr<Nominal  >, L"Норма")
DLG_SUB(Clr<DeathZone>, L"Мёртвая зона")
DLG_SUB(Clr<SortDown >, L"Понижение сорта")
DLG_SUB(Clr<Defect   >, L"Брак")
DLG_SUB(Clr<SensorOff>, L"Датчик отключён")

struct DefaultBtn
{
	static const int width = 160;
	static const int height = 30;
	static const int ID = IDRETRY;
	wchar_t *Title(){return L"Встроенные настройки";}
	template<class Owner>void BtnHandler(Owner &o, HWND h)
	{
		if(TestPassword<Owner::Base, Owner::Table>()(h))
		{
			CBase base(Owner::Base().name());
			if(base.IsOpen())
			{
				ColorTable color;
				UpdateWhere<ColorTable>(color, base).ID(1).Execute();
				CopyFactory()(Singleton<ColorTable>::Instance().items, color.items);
			}
			EndDialog(h, FALSE);
		}
	}
};

}
void ColorItemsDlg::Do(HWND h)
{
	ColorTable color;
	CopyFactory()(color.items, Singleton<ColorTable>::Instance().items);
	if(ClrDlg(color).Do(h, L"Цвета"))
	{
		CopyFactory()(Singleton<ColorTable>::Instance().items, color.items);
		App::UpdateViewers();
	}
}

