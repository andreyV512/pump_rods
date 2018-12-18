#pragma once
#include <Windows.h>

namespace Structure
{
	struct MedianFiltre
	{
		static void Do(HWND h);
	};

	struct FilterDlg
	{
		static void Do(HWND h);
	};

	struct CorrectionSensorDlg
	{
		static void Do(HWND h);
	};

	bool TestChangeParam(HWND h);
	void StoreParam(HWND);

	struct FrameWidthViewDlg
	{
		static void Do(HWND);
	};
}