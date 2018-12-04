#pragma once
#include "window_tool/message.h"
#include "templates\typelist.hpp"
#include "Windows\MainWindow\MainWindow.h"
#include "FrameViewer.h"

class DefectWindow
{
public:
	class Def: public MainWindow::DefectoscopeViewer
	{
	public:
		typedef MainWindow::DefectoscopeViewer Parent; 
		void operator()(TRButtonDown &){};

		bool Draw(TMouseMove &, VGraphics &);
	};

	typedef TL::MkTlst<
		Def
		, FrameViewer
	>::Result viewers_list;
	TL::Factory<viewers_list> viewers;
	HWND hWnd;
	LRESULT operator()(TCreate &);
	void operator()(TDestroy &);
	void operator()(TSize &);
	void operator()(TCommand &);
	void operator()(TMouseWell &);
};