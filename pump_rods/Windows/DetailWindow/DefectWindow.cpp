#include "DefectWindow.h"
#include "window_tool/MenuApi.h"
#include "CommonWindow/Common.h"
#include "DataItem\DataItem.h"
#include "Compute\Compute.h"

namespace DefectMenu
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

bool DefectWindow::Def::Draw(TMouseMove &l, VGraphics &g)
{
	//if(0 == count) return false;
	Parent::Draw(l, g);
	int x;
	CoordCell(tchart, l.x, x, DataItem::output_buffer_size);

	int offs = int((double)x * currentOffset / DataItem::output_buffer_size);

	//wsprintf(label.buffer, L"<ff>смещение <ff00>%d <ff>%s %s"
	//	, offs
	//	, Wchar_from<double, 2>(buffer[x])()
	//	, L"Mess(buffer[x], x)"
	//	);
	//label.Draw(g());
	owner->ChangeFrame(offs);
	
	return true;
}

LRESULT DefectWindow::operator()(TCreate &m)
{
	Menu<DefectMenu::menu_list>().Init(m.hwnd);
	DataItem::Defectoscope &item = Singleton<DataItem::Defectoscope>::Instance();
	ColorTable::TItems &color = Singleton<ColorTable>::Instance().items;
	ViewerCountTable::TItems &viewerCount = Singleton<ViewerCountTable>::Instance().items;

	Def &def = viewers.get<Def>();
	def.owner = this;
	memmove(def.buffer, item.outputData, sizeof(def.buffer));
	memmove(def.status, item.status, sizeof(def.status));

//	def.count = DataItem::output_buffer_size;
	def. deathZoneFirst = item.deathZoneFirst;
	def.deathZoneSecond = item.deathZoneSecond;
	def.threshSortDown = item.threshSortDown; 
	def.threshDefect = item.threshDefect;
	def.result = item.result;
	def.tchart.maxAxesX = item.currentOffset - 1;
	def.currentOffset = item.currentOffset;
	def.inputData = item.inputData;
//	def.lengthTube = Singleton<DeadAreaTable>::Instance().items.get<RodLenght>().value;
	def.tchart.items.get<BottomAxesMeters>().maxBorder = Singleton<DeadAreaTable>::Instance().items.get<RodLenght>().value;	
	def.threshSortDownColor = color.get<Clr<SortDown>>().value;
	def.threshDefectColor = color.get<Clr<Defect>>().value;
	def.deathZoneColor = color.get<Clr<DeathZone>>().value;
	def.nominalColor = color.get<Clr<Nominal>>().value;
	def.cursor.SetMouseMoveHandler(&def, & DefectWindow::Def::Draw);

	FrameViewer &frame =  viewers.get<FrameViewer>();
	frame.cursor.SetMouseMoveHandler(&frame, &FrameViewer::DrawFrame); 
	ChangeFrame(0);
	TL::foreach<viewers_list, Common::__create_window__>()(&viewers, &m.hwnd);	
	return 0;
}

void DefectWindow::ChangeFrame(int offsetDef)
{
	Def &def = viewers.get<Def>();
	ViewerCountTable::TItems &viewerCount = Singleton<ViewerCountTable>::Instance().items;
	DataItem::Defectoscope &item = Singleton<DataItem::Defectoscope>::Instance();
	FrameViewer &frame =  viewers.get<FrameViewer>();
	frame.count = viewerCount.get<DefectSig<ViewerCount>>().value;

	frame.medianFiltreLength = Singleton<MedianFiltreTable>::Instance().items.get<DefectSig<MedianFiltreWidth>>().value;
	frame.cutoffFrequency = Singleton<AnalogFilterTable>::Instance().items.get<DefectSig<CutoffFrequency>>().value;

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
		, tbuf//frame.buffer
		, tbuf_size//frame.buffer)
		, Singleton<L502ParametersTable>::Instance().items.get<DefectSig<ChannelSamplingRate>>().value
		);

	memmove(frame.buffer, &tbuf[offs_b], sizeof(frame.buffer));

	//BottomAxesMeters &bottomAxesMeters = frame.tchart.items.get<BottomAxesMeters>();
	//bottomAxesMeters.minBorder = offsetDef;
	//bottomAxesMeters.maxBorder = offsetDef + frame.count;
	frame.tchart.minAxesX = offsetDef;
	frame.tchart.maxAxesX = offsetDef + frame.count;

	double adcRange =  100.0 / DataItem::ADC_RANGE(Singleton<L502ParametersTable>::Instance().items.get<DefectSig<RangeL502>>().value);
	double koef = Singleton<KoeffSignTable>::Instance().items.get<DefectSig<KoeffSign>>().value;

	for(int i = 0; i < dimention_of(frame.buffer); ++i)
	{
		frame.buffer[i] *= adcRange * koef;
	}

	frame.delta = int((double)frame.count / dimention_of(frame.buffer));

	frame.count = frame.count;//dimention_of(frame.buffer);
	frame.nominalColor		= def.nominalColor		;

	DeadAreaTable::TItems &dead = Singleton<DeadAreaTable>::Instance().items;
	int rodLength = dead.get<RodLenght>().value;
	frame.deathZoneFirst	= int((double)dead.get<DefectSig<First<DeathZone>>>().value * item.currentOffset / rodLength); 
	frame.deathZoneSecond	= item.currentOffset -  int((double)dead.get<DefectSig<Second<DeathZone>>>().value * item.currentOffset / rodLength); 

	dprint("frame.deathZoneFirst %d frame.deathZoneSecond %d\n", frame.deathZoneFirst, frame.deathZoneSecond);

	frame.deathZoneColor	= def.deathZoneColor	; 	
	frame.threshDefect	 	= def.threshDefect	 	;
	frame.threshDefectColor	= def.threshDefectColor	;
	frame.threshSortDownColor = def.threshSortDownColor;
	frame.threshSortDown   	= def.threshSortDown   	;
	
	

	ThresholdsTable::TItems &tresh = Singleton<ThresholdsTable>::Instance().items;

	frame.tchart.items.get<FrameViewer::Border<SortDown>>().value = tresh.get<DefectSig<Thresh<SortDown>>>().value;
	frame.tchart.items.get<FrameViewer::Border<Defect>>().value = tresh.get<DefectSig<Thresh<Defect>>>().value;

	frame.tchart.items.get<FrameViewer::Border<SortDown>>().color  = frame.threshSortDownColor;
	frame.tchart.items.get<FrameViewer::Border<Defect>>().color  = frame.threshDefectColor;

	RepaintWindow(frame.hWnd);
}

void DefectWindow::operator()(TDestroy &m)
{
	delete this;
	SetWindowLongPtr(m.hwnd, GWLP_USERDATA, 0);
}

void DefectWindow::operator()(TSize &m)
{
	static const int height = 200;

	{
		Def &def = viewers.get<Def>();
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

void DefectWindow::operator()(TCommand &m)
{
	EventDo(m);
}

void DefectWindow::operator()(TMouseWell &l)
{
	TL::find<viewers_list, Common::__in_rect__>()(
		&viewers
		, &Common::__event_data__<TMouseWell, DefectWindow>(*this, l)
		);
}
//-----------------------------------------------------------------------------

