#pragma  once
#include <Windows.h>

class WindowPosition
{
static void Get_(const char *, RECT &);
static void Set_(const char *, RECT &);
public:
	template<class T>static void Get(RECT &r)
	{
		Get_(typeid(T).name(), r);
	}
	template<class T>static void Set(RECT &r)
	{
		Set_(typeid(T).name(), r);
	}
	template<class T>static void Set(HWND h)
	{
		RECT r;
		GetWindowRect(h, &r);
		Set_(typeid(T).name(), r);
	}
};

template<class T>struct WindowPositionDlg
{
	static void Do(HWND h)
	{
		RECT r;
		GetWindowRect(h, &r);
		WindowPosition::Set<T>(r);
	}
};