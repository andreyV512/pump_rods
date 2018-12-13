#include "ResultViewer.h"
//#include "MessageText/StatusMessages.h"
#include "templates/templates.hpp"
#include "App/AppBase.h"
#include "window_tool\MenuAPI.h"
//#include "Windows/ViewersMenu.hpp"
#include "MessageText\StatusMessages.h"

using namespace Gdiplus;
ResultViewer::ResultViewer()
	: CommonViewer(&tchart, 1) 
    , tchart(backScreen)
	, viewerData(Singleton<DataItem::ResultData>::Instance())
	, count(0)
{
	chart = &tchart;

	tchart.items.get<BarSeriesNoFixed>().SetColorBarHandler(this, &ResultViewer::GetColorBar);
	tchart.items.get<BarSeriesNoFixed>().dataLength = dimention_of(viewerData.status);
	cursor.SetMouseMoveHandler(this, &ResultViewer::Draw);
	cursor.horizontalLine = false;	
}

bool ResultViewer::GetColorBar(int zone, double &data, unsigned &color)
{
	color = StatusColor()(viewerData.status[zone]);
	data = 90;
	return zone < count;
}

bool ResultViewer::Draw(TMouseMove &l, VGraphics &g)
{
	int x = 0;
	CoordCell(tchart, l.x, x, DataItem::output_buffer_size);

	int offs = int((double)x * Singleton<DeadAreaTable>::Instance().items.get<RodLenght>().value / DataItem::output_buffer_size);
	unsigned color = 0xffffffff;
	wchar_t txt[1024];
	StatusText()(viewerData.status[x], color, txt);
	wsprintf(label.buffer, L"<ff>смещение <ff00>%d <%x>%s"
		, offs
		, color
		, txt
		);
	label.Draw(g());
	return false;
}
