#include "StructWindow.h"
#include "window_tool/MenuApi.h"
#include "CommonWindow/Common.h"
#include "DataItem\DataItem.h"
#include "Compute\Compute.h"
#include "App/AppBase.h"

namespace StructMenu
{
	struct MainFile{};
	MENU_TEXT(L"Файл", TopMenu<MainFile>)

	struct MainExit
	{
		static void Do(HWND h)
		{
			TClose c = {h, WM_CLOSE, 0, 0};
			SendMessage(MESSAGE(c));
		}
	};

	MENU_ITEM(L"Выход", MainExit)

	template<>struct TopMenu<MainFile>
	{
		typedef TL::MkTlst<
			MenuItem<MainExit>
		>::Result list;
	};

	typedef TL::MkTlst<
		TopMenu<MainFile>
	>::Result menu_list;	
};

bool StructWindow::Str::Draw(TMouseMove &l, VGraphics &g)
{
	if(count > 0)
	{
		Parent::Draw(l, g);
		int x;
		CoordCell(tchart, l.x, x, DataItem::output_buffer_size);

		int offs = int((double)x * currentOffset / DataItem::output_buffer_size);

		owner->ChangeFrame(offs);
	}
	return true;
}

LRESULT StructWindow::operator()(TCreate &m)
{
	Menu<StructMenu::menu_list>().Init(m.hwnd);
	StructSig<DataItem::Buffer> &item = Singleton<StructSig<DataItem::Buffer>>::Instance();
	ColorTable::TItems &color = Singleton<ColorTable>::Instance().items;
	ViewerCountTable::TItems &viewerCount = Singleton<ViewerCountTable>::Instance().items;

	Str &str = viewers.get<Str>();
	str.owner = this;
	memmove(str.buffer, item.outputData, sizeof(str.buffer));
	memmove(str.status, item.status, sizeof(str.status));

	str. deathZoneFirst = item.deathZoneFirst;
	str.deathZoneSecond = item.deathZoneSecond;
	str.threshSortDown = item.threshSortDown; 
	str.threshDefect = item.threshDefect;
	str.result = item.result;
	str.tchart.maxAxesX = item.currentOffset - 1;
	str.currentOffset = item.currentOffset;
	str.inputData = item.inputData;
	str.tchart.items.get<BottomAxesMeters>().maxBorder = Singleton<DeadAreaTable>::Instance().items.get<RodLenght>().value;	
	str.threshSortDownColor = color.get<Clr<SortDown>>().value;
	str.threshDefectColor = color.get<Clr<Defect>>().value;
	str.deathZoneColor = color.get<Clr<DeathZone>>().value;
	str.nominalColor = color.get<Clr<Nominal>>().value;
	str.cursor.SetMouseMoveHandler(&str, & StructWindow::Str::Draw);
	str.count = DataItem::output_buffer_size;

	FrameViewer &frame =  viewers.get<FrameViewer>();
	frame.cursor.SetMouseMoveHandler(&frame, &FrameViewer::DrawFrame); 
	ChangeFrame(str.currentX);
	TL::foreach<viewers_list, Common::__create_window__>()(&viewers, &m.hwnd);	
	return 0;
}

void StructWindow::ChangeFrame(int offsetDef)
{
	Str &def = viewers.get<Str>();
	ViewerCountTable::TItems &viewerCount = Singleton<ViewerCountTable>::Instance().items;
	StructSig<DataItem::Buffer> &item = Singleton<StructSig<DataItem::Buffer>>::Instance();
	FrameViewer &frame =  viewers.get<FrameViewer>();
	frame.count = viewerCount.get<StructSig<ViewerCount>>().value;
	frame.medianFiltreLength = Singleton<MedianFiltreTable>::Instance().items.get<StructSig<MedianFiltreWidth>>().value;
	frame.cutoffFrequency = Singleton<AnalogFilterTable>::Instance().items.get<StructSig<CutoffFrequency>>().value;

	static const int tbuf_size = 3 * dimention_of(frame.buffer) / 2;
	double tbuf[tbuf_size];

	int offs_b = tbuf_size / 3;
	int offs = int(offsetDef - offs_b * frame.delta);
	int frameWidth = 3 * frame.count / 2;
	if(offs < 0)
	{
		offs = 0;
		offs_b = 0;
		frameWidth = frame.count;
	}
	else if(offs + frame.count > item.currentOffset)
	{
		offs = item.currentOffset - frame.count;
	}

	Compute::Compute(
		item.inputData + offs
		, frameWidth
		, frame.cutoffFrequency
		, frame.medianFiltreLength
		, tbuf
		, tbuf_size
		, Singleton<L502ParametersTable>::Instance().items.get<StructSig<ChannelSamplingRate>>().value
		);

	memmove(frame.buffer, &tbuf[offs_b], sizeof(frame.buffer));

	frame.tchart.minAxesX = offsetDef;
	frame.tchart.maxAxesX = offsetDef + frame.count;

	double adcRange =  100.0 / DataItem::ADC_RANGE(Singleton<L502ParametersTable>::Instance().items.get<StructSig<RangeL502>>().value);
	double koef = Singleton<KoeffSignTable>::Instance().items.get<StructSig<KoeffSign>>().value;

	for(int i = 0; i < dimention_of(frame.buffer); ++i)
	{
		frame.buffer[i] *= adcRange * koef;
	}

	frame.delta = int((double)frame.count / dimention_of(frame.buffer));

	frame.count = frame.count;
	frame.nominalColor		= def.nominalColor;

	DeadAreaTable::TItems &dead = Singleton<DeadAreaTable>::Instance().items;
	int rodLength = dead.get<RodLenght>().value;
	frame.deathZoneFirst	= int((double)dead.get<StructSig<First<DeathZone>>>().value * item.currentOffset / rodLength); 
	frame.deathZoneSecond	= item.currentOffset -  int((double)dead.get<StructSig<Second<DeathZone>>>().value * item.currentOffset / rodLength); 

	dprint("frame.deathZoneFirst %d frame.deathZoneSecond %d\n", frame.deathZoneFirst, frame.deathZoneSecond);

	frame.deathZoneColor	= def.deathZoneColor	; 	
	frame.threshDefect	 	= def.threshDefect	 	;
	frame.threshDefectColor	= def.threshDefectColor	;
	frame.threshSortDownColor = def.threshSortDownColor;
	frame.threshSortDown   	= def.threshSortDown   	;
	
	ThresholdsTable::TItems &tresh = Singleton<ThresholdsTable>::Instance().items;

	frame.tchart.items.get<FrameViewer::Border<SortDown>>().value = tresh.get<StructSig<Thresh<SortDown>>>().value;
	frame.tchart.items.get<FrameViewer::Border<Defect>>().value = tresh.get<StructSig<Thresh<Defect>>>().value;

	frame.tchart.items.get<FrameViewer::Border<SortDown>>().color  = frame.threshSortDownColor;
	frame.tchart.items.get<FrameViewer::Border<Defect>>().color  = frame.threshDefectColor;

	RepaintWindow(frame.hWnd);
}

void StructWindow::operator()(TDestroy &m)
{
	delete this;
	SetWindowLongPtr(m.hwnd, GWLP_USERDATA, 0);
}

void StructWindow::operator()(TSize &m)
{
	static const int height = 200;

	{
		Str &def = viewers.get<Str>();
		TSize size = {def.hWnd, WM_SIZE, 0, m.Width, height};
		SendMessage(MESSAGE(size));
		MoveWindow(def.hWnd , 0, 0, size.Width, size.Height, true);
	}
	{
		FrameViewer &frame = viewers.get<FrameViewer>();
		TSize size = {frame.hWnd, WM_SIZE, height, m.Width, m.Height - height};
		SendMessage(MESSAGE(size));
		MoveWindow(frame.hWnd , 0, height, size.Width, size.Height, true);
	}
}

void StructWindow::operator()(TCommand &m)
{
	EventDo(m);
}

void StructWindow::operator()(TMouseWell &l)
{
	TL::find<viewers_list, Common::__in_rect__>()(
		&viewers
		, &Common::__event_data__<TMouseWell, StructWindow>(*this, l)
		);
}
//-----------------------------------------------------------------------------

