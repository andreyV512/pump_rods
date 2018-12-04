#pragma once
#include <windows.h>
#include <windowsx.h>
#include <Shellapi.h>
//---------------------------------------------------------------------------
#ifdef  _M_X64
#define NOUSED(type) type noused;
#else
#define NOUSED(type)
#endif
#pragma pack(2)
struct TMessage
{
  HWND hwnd; UINT uMsg; WPARAM wParam; LPARAM lParam;
};

#define __MESSAGE__(z) z.hwnd, z.uMsg, z.wParam, z.lParam
#define MESSAGE(z) __MESSAGE__((*(TMessage *)&z))

struct MessageProc
{
  virtual void operator()(LPARAM) = 0;
  virtual ~MessageProc(){}
};
struct Empty{};
typedef void (Empty::*EmptyMember)(TMessage &);

template<class T>class THandlers
{
public:
	typedef T type;
	Empty *o;
	long (Empty::*m)(T &);
	long operator()(T &t){if(NULL != o && NULL != m)return (o->*m)(t); return 1;}
};

#define WM_MESSAGE WM_USER + 8

struct TKeyDown
{
  HWND hwnd;
  UINT uMsg; 
  WPARAM VirtKey; 
  LPARAM KeyData;
};

struct TSize
{
	HWND hwnd;
	UINT message;
	WPARAM resizing;	
	WORD Width;
	WORD Height;
};

struct TPaint
{
  HWND hwnd;
  UINT uMsg;
  HDC hdc;
  LPARAM noUsed;
};

struct TActivate
{
	HWND hwnd;
	UINT uMsg;
	WORD fActive;
	WORD fMinimized;
	NOUSED(DWORD)
	HWND hwndPrevious;
};
struct TSysKeyDown
{
  HWND hwnd; UINT uMsg; WPARAM VirtKey; LPARAM KeyData;
};

struct TGetMinMaxInfo
{
	HWND hwnd;
	UINT uMsg;
	WPARAM noUsed;
	LPMINMAXINFO pMinMaxInfo;
};

struct TDestroy
{
	HWND hwnd;
	UINT uMsg;
	WPARAM wParam;
	LPARAM lParam;
};

struct TTimer
{
	HWND hwnd;
	UINT uMsg;
	WPARAM ID;
	VOID (CALLBACK *TimerProc)(HWND, UINT, UINT_PTR, DWORD);
};

struct TCreate
{
	HWND hwnd; 
    UINT uMsg; 
    WPARAM no_used; 
    CREATESTRUCT *create;
};
struct TClose
{
	HWND hwnd;
	UINT uMsg;
	WPARAM wParam;
	LPARAM lParam;
};
struct TCommand
{
	HWND hwnd;
	UINT uMsg;
	WORD id;
	WORD isAcselerator;
	NOUSED(DWORD);
	HWND hControl;
	TCommand(HWND hwnd, WORD id, WORD isAcselerator, HWND hControl)
		: hwnd(hwnd)
		, uMsg (WM_COMMAND)
		, id  (id)
		, isAcselerator(isAcselerator)
		, hControl 	   (hControl)
	{}
};
struct TNotify
{
  HWND hwnd;
  UINT message;
  WPARAM idCtrl; 
  LPNMHDR pnmh;
};
struct TUser
{
	HWND hwnd; 
    UINT uMsg; 
    void(*ptr)(void *);
    void *data;
};
struct TMouseMove
{
  HWND hwnd;
  UINT message;
  WPARAM flags; 
  WORD x;
  WORD y;
};
struct TLButtonDown
{
  HWND hwnd;
  UINT message;
  WPARAM flags; 
  WORD x;
  WORD y;
};
struct TRButtonDown
{
  HWND hwnd;
  UINT message;
  WPARAM flags; 
  WORD x;
  WORD y;
};
struct TLButtonUp
{
  HWND hwnd;
  UINT message;
  WPARAM flags; 
  WORD x;
  WORD y;
};
struct TMoving
{
	HWND hwnd;
    UINT message;
	WPARAM noused; 
	RECT *pRect;
};
struct TSizing
{
	HWND hwnd;
    UINT message;
	WPARAM noused; 
	RECT *pRect;
};
struct TRButtonUp
{
  HWND hwnd;
  UINT message;
  WPARAM flags; 
  WORD x;
  WORD y;
};
struct TLButtonDbClk
{
  HWND hwnd;
  UINT message;
  WPARAM flags; 
  WORD x;
  WORD y;
};
struct TMouseWell
{
	HWND hwnd;
	UINT message;
	struct
	{
		WORD lButton :1;
		WORD rButton :1;
		WORD shift   :1;
		WORD control :1;
		WORD mButton :1;
		WORD xButton1:1;
		WORD xButton2:1;
	} flags;
	short delta;
	NOUSED(DWORD)
	WORD x;
	WORD y;
};
struct TCopyData
{
	HWND hwnd;
	UINT message;
	HWND h;
	COPYDATASTRUCT *copyDataStruct;
};
struct TContextMenu
{
  HWND hwnd; UINT uMsg; WPARAM wParam; LPARAM lParam;
};

struct TDropFiles
{
	HWND hwnd;
	UINT message;
	HDROP hDrop;
	LPARAM noused; 
};
struct TInitDialog
{
  HWND hwnd; UINT uMsg; WPARAM wParam; LPARAM lParam;
};
#pragma pack()
//------------------------------------------------------------------------------------------------

