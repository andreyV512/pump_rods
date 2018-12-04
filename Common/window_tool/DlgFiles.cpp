#include "DlgFiles.h"
#include "templates/typelist.hpp"
//-------------------------------------------------------------------------------
SaveData::SaveData(HWND h, wchar_t *title)
{
	memset(&ofn, 0, sizeof(ofn));
	ofn.lStructSize = sizeof(ofn);
	ofn.hwndOwner = h;
	ofn.lpstrFile = sFile;
	sFile[0] = '\0';
	ofn.nMaxFile = dimention_of(sFile);
	ofn.nFilterIndex = 1;
	ofn.Flags = OFN_OVERWRITEPROMPT | OFN_NOCHANGEDIR | OFN_NONETWORKBUTTON;
	ofn.lpstrFilter = L"*.bz2\0*.bz2\0.dat\0*.dat\0*.*\0*.*\0";
	if(NULL != title)ofn.lpstrTitle = title;
	ofn.lpstrDefExt = L"dat";
}
//---------------------------------------------------------------------------------
bool SaveData::operator()()
{
	return 0 != GetSaveFileName(&ofn);
}
//-------------------------------------------------------------------------------
OpenData::OpenData(HWND h, wchar_t *title)
{
	memset(&ofn, 0, sizeof(ofn));
	ofn.lStructSize = sizeof(ofn);
	ofn.hwndOwner = h;
	ofn.lpstrFile = sFile;
	sFile[0] = '\0';
	ofn.nMaxFile = dimention_of(sFile);
	ofn.nFilterIndex = 1;
	ofn.Flags = OFN_CREATEPROMPT | OFN_HIDEREADONLY;
	ofn.lpstrFilter = L"*.bz2\0*.bz2\0*.dat\0*.dat\0*.*\0*.*\0";
	if(NULL != title)ofn.lpstrTitle = title;
	ofn.lpstrDefExt = L"dat";
}
//---------------------------------------------------------------------------------
bool OpenData::operator()()
{
	return 0 != GetOpenFileName(&ofn);
}
//------------------------------------------------------------------------------------
ColorDialog::ColorDialog(HWND h, wchar_t *title)
{
	ZeroMemory(&color, sizeof(color));
	color.hwndOwner = h;
	color.lStructSize = sizeof(color);
	color.Flags = CC_RGBINIT | CC_FULLOPEN;
	color.rgbResult = Result;
	color.lpCustColors = crCustColors;
}
//--------------------------------------------------------------------------------------
bool ColorDialog::operator()()
{
	return 0 != ChooseColor(&color);
}
//-----------------------------------------------------------------------------