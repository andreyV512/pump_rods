#pragma once
#include "window_tool/message.h"
#include "templates\typelist.hpp"
#include "Windows\MainWindow\MainWindow.h"
#include "FrameViewer.h"

class StructWindow
{
public:
	class Str: public MainWindow::StructureViewer
	{
	public:
		typedef MainWindow::StructureViewer Parent; 
		StructWindow *owner;
		void operator()(TRButtonDown &){};
		bool Draw(TMouseMove &, VGraphics &);
	};

	typedef TL::MkTlst<
		Str
		, FrameViewer
	>::Result viewers_list;
	TL::Factory<viewers_list> viewers;
	HWND hWnd;
	LRESULT operator()(TCreate &);
	void operator()(TDestroy &);
	void operator()(TSize &);
	void operator()(TCommand &);
	void operator()(TMouseWell &);

	void ChangeFrame(int);
};