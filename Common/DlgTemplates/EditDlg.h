#pragma once
#include <windows.h>
class EditDlg
{
public:
	HWND hEdit;
	bool(*OkBtn)(HWND, wchar_t *);
	bool(*CancelBtn)(HWND, wchar_t *);
	static bool Dump(HWND, wchar_t *){return true;}
	wchar_t buf[128];
	EditDlg(HWND hWnd
		, wchar_t *title, wchar_t *data, bool(*ok)(HWND, wchar_t *) = Dump
		, bool(*cancel)(HWND, wchar_t *) = Dump);
	static LRESULT CALLBACK Proc(HWND h, UINT msg, WPARAM wParam, LPARAM lParam);
};
class EditDlgMultiLines
{
public:
	HWND *hEdit;
	bool(*OkBtn)(HWND, wchar_t **);
	bool(*CancelBtn)(HWND, wchar_t **);
	static bool Dump(HWND, wchar_t **){return true;}
	wchar_t **buf;
	wchar_t **title_text;
	int count;
	EditDlgMultiLines(int count, HWND hWnd
		, wchar_t *title, wchar_t **title_data, wchar_t **data, bool(*ok)(HWND, wchar_t **) = Dump
		, bool(*cancel)(HWND, wchar_t **) = Dump);
	~EditDlgMultiLines();
	static LRESULT CALLBACK Proc(HWND h, UINT msg, WPARAM wParam, LPARAM lParam);
};

class EditDlgComboBox
{
public:
	struct Handlers
	{		
		virtual void Init(HWND h) = 0;
        virtual bool OkBtn(wchar_t *) = 0;
    	virtual bool CancelBtn() = 0;
	};
public:
	HWND hEdit;	
	Handlers &handlers;
	wchar_t buf[128];
	EditDlgComboBox(HWND hWnd
		, wchar_t *title
		, Handlers &handlers 
		);
	static LRESULT CALLBACK Proc(HWND h, UINT msg, WPARAM wParam, LPARAM lParam);
};
class UpDownDlg
{
public:
	HWND hwndUpDown;
	int initValue;
	wchar_t buf[128];
	bool(*OkBtn)(HWND, wchar_t *);
	bool(*CancelBtn)(HWND, wchar_t *);
	static bool Dump(HWND, wchar_t *){return true;}
	UpDownDlg(HWND hWnd
		, wchar_t *title, int initValue = 1, bool(*ok)(HWND, wchar_t *) = Dump
		, bool(*cancel)(HWND, wchar_t *) = Dump);
	static LRESULT CALLBACK Proc(HWND h, UINT msg, WPARAM wParam, LPARAM lParam);
};
