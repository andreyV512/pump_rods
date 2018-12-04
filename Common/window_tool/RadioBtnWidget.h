#pragma once
#include "window_tool/message.h"
#include "TEvent.h"
#include "tools_debug\DebugMess.h"

/**
* \brief ������ ��� �������� 
* \param T - ��������� �������� ������ ������������� ������  TestCheckBox
*/
template<class T>class RadioBtnWidget : public T, public TEvent
{
public:
	HWND hWnd;
public:
	RadioBtnWidget(){}
	template<class Z>RadioBtnWidget(Z &z) : T(z){}
	/**
	* \brief ��������� ����� � ����������� WM_CREATE
	* \param hOwner - ������ winapi
	* \param txt - ����� ����� ������
	* \param bool T::Init(HWND) - ������������ ��������� ��������(true - ������� ��������, false - ���������)
	*/
	void Init(HWND hOwner, const wchar_t *txt, bool initValue = false, bool group = false)
	{
		hWnd = CreateWindow(L"button", txt
			, WS_CHILD | WS_VISIBLE | BS_AUTORADIOBUTTON | WS_TABSTOP | (group ? WS_GROUP : 0)
			, 0, 0, 0, 0, hOwner, NULL, GetModuleHandle(NULL), NULL
			);
		SetWindowLongPtr(hWnd, GWLP_USERDATA, (LONG)this);
		Button_SetCheck(hWnd, T::Init(hWnd, initValue) ? BST_CHECKED : BST_UNCHECKED);
	}
	/**
	* \brief ��������� � ����������� WM_SIZE
	*/
	void Size(int left, int top, int width, int height)
	{
		MoveWindow(hWnd, left, top, width, height, true);
	}
//private:
	/**
	* \brief ����������� ��� ������ ��� ��������� ������� 
	* \param static void T::Command() - ���������� � ��������� ���������
	*/
	void Do(TCommand &m)
	{
		T::Command(m);
	}
};

