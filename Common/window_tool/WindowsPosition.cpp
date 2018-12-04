#include "window_tool/WindowsPosition.h"
#include <Shlwapi.h>
#include "ItemIni.h"
#include "templates/typelist.hpp"

#pragma warning(disable : 4996)
namespace {
bool TestWindowRect(RECT &inp)
{
	RECT r;
	GetWindowRect(GetDesktopWindow(), &r);
	return inp.left < r.right && inp.top < r.bottom;
}
}

#pragma warning(disable: 4996)

void WindowPosition::Get_(const char *n, RECT &r)
{
	wchar_t path[1024];
	ItemIni::GetPath(path);

	wchar_t name[256];
	mbstowcs(name, &n[6], dimention_of(name));
	r.left   = ItemIni::Get(name, L"X", CW_USEDEFAULT, path);
	r.top    = ItemIni::Get(name, L"Y", CW_USEDEFAULT, path);
	r.right  = ItemIni::Get(name, L"Width", CW_USEDEFAULT, path);
	r.bottom = ItemIni::Get(name, L"Height", CW_USEDEFAULT, path);
}
void WindowPosition::Set_(const char *n, RECT &r)
{
	wchar_t path[1024];
	ItemIni::GetPath(path);

	if(!TestWindowRect(r))
	{
		r.left   = CW_USEDEFAULT;
		r.top    = CW_USEDEFAULT;
		r.right  = CW_USEDEFAULT;
		r.bottom = CW_USEDEFAULT;
	}

	wchar_t name[256];
	mbstowcs(name, &n[6], dimention_of(name));

	ItemIni::Set(name, L"X"     , r.left  , path);
	ItemIni::Set(name, L"Y"     , r.top   , path);				 
	ItemIni::Set(name, L"Width" , r.right - r.left, path);
	ItemIni::Set(name, L"Height", r.bottom - r.top, path);
}