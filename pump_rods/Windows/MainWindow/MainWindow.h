#pragma once
#include "window_tool/message.h"
#include "MainWindowToolBar.h"
#include "window_tool/SelectTypeSizeList.h"
#include "CommonWindow/CheckBox.hpp"
#include "window_tool/CheckBoxWidget.h"
#include "App/AppBase.h"
#include "CommonWindow/TopLabelViewer.h"
#include "Viewers.h"
#include "ResultViewer.h"

class MainWindow
{
public:
	MainWindowToolBar toolBar;
	SelectTypeSizeList select;
	typedef TL::MkTlst<
		  CheckBoxWidget<OnCheckBox<DefectSig<Check>, OnTheJobTable> >
		, CheckBoxWidget<OnCheckBox<StructSig<Check>, OnTheJobTable> >
	>::Result check_list;
	TL::Factory<check_list> checks;
	TopLabelViewer topLabelViewer;
	class DefectoscopeViewer: public ViewerData
	{
	public: 
		typedef ViewerData Parent;
		bool Draw(TMouseMove &, VGraphics &);
		void operator()(TRButtonDown &);
	};
	class StructureViewer: public ViewerData
	{
	public: 
		typedef ViewerData Parent;
		bool Draw(TMouseMove &, VGraphics &);
		void operator()(TRButtonDown &);
	};
	typedef TL::MkTlst<
		DefectoscopeViewer
		, StructureViewer
		, ResultViewer
	>::Result viewers_list;
	TL::Factory<viewers_list> viewers;
	HWND hWnd;
	HWND hStatusWindow;
	LRESULT operator()(TCreate &);
	void operator()(TDestroy &);
	void operator()(TSize &);
	void operator()(TCommand &);
//	void operator()(TGetMinMaxInfo &);
	void operator()(TClose &);
	void operator()(TMouseWell &);
	void operator()(TUser &);
//	void Destroy();
//	void EnableTool(bool);
//
	void StatusBar(int n, wchar_t *txt);
//	void UpdateWindow();
	void SetColor();
	void CleanChart(bool);
};