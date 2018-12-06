#include "ResultViewer.h"
//#include "MessageText/StatusMessages.h"
#include "templates/templates.hpp"
#include "App/AppBase.h"
#include "window_tool\MenuAPI.h"
//#include "Windows/ViewersMenu.hpp"

using namespace Gdiplus;
ResultViewer::ResultViewer()
	: CommonViewer(&tchart, 1) 
    , tchart(backScreen)
	, viewerData(Singleton<DataItem::ResultData>::Instance())
{
	chart = &tchart;

	tchart.items.get<FixedGridSeries>().SetColorCellHandler(this, &ResultViewer::GetColorBar);
	cursor.SetMouseMoveHandler(this, &ResultViewer::Draw);
	cursor.horizontalLine = false;
}

bool ResultViewer::GetColorBar(unsigned sensor, int zone, double &data, unsigned &color)
{
	//color = StatusColor()(viewerData.status[zone]);
	return false;//zone < viewerData.currentOffsetZones - 1;
}

bool ResultViewer::Draw(TMouseMove &l, VGraphics &g)
{
	//int x = currentX;
	int x, y;
	tchart.CoordCell(l.x, l.y, x, y);

	bool drawZones =  x < viewerData.currentOffsetZones - 1;
	if(drawZones)
	{		
		unsigned color;
		bool b;
		wchar_t s[256] = L"test title";
		//StatusText()(viewerData.status[x], color, b, s);
		wsprintf(label.buffer, L"<ff>результат зона <ff>%d <%6x>%s"
			, 1 + x
			, color
			, s
			);
		label.Draw(g());
	}
	return drawZones;
}
