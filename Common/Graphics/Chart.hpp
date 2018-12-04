#pragma once
#include "templates/typelist.hpp"

using namespace Gdiplus;
#pragma warning(disable : 4355)
template<class Chart, class List>class ChartDraw : public Chart
{
	template<class O, class P>struct __draw__{void operator()(O *o, P *){o->Draw();}};
public:
	typedef List items_list;
	TL::Factory<List> items;
public:
	ChartDraw(Gdiplus::Bitmap *&backScreen_): Chart(backScreen_), items((Chart &)*this){}
	void Draw(Gdiplus::Graphics &graph)
	{
		Chart::Draw(graph);
		TL::foreach<List, __draw__>()(&items, this);
	}
	template<class lst>void DrawItems(Gdiplus::Graphics &graph)
	{
		TL::foreach<lst, __draw__>()(&items, this);
	}
};