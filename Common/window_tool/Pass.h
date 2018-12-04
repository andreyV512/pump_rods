#pragma once
#include <Windows.h>

class PasswordDlg
{
	const wchar_t *pass;
	const wchar_t *title;
	HWND hEdit;
	static LRESULT CALLBACK Proc(HWND , UINT , WPARAM , LPARAM );
public:
	PasswordDlg(const wchar_t *, const wchar_t *);
	bool Do(HWND );
};

class TypesizePasswordDlg: public PasswordDlg
{
	static bool &Value();
public:
	TypesizePasswordDlg();
	bool Do(HWND );
};

class OptionPasswordDlg: public PasswordDlg
{
	static bool &Value();
public:
	OptionPasswordDlg();
	bool Do(HWND );
};