#pragma once
#include "window_tool/message.h"
#include "TEvent.h"
#include "tools_debug\DebugMess.h"

/**
* \brief Шаблон для чекбокса 
* \param T - шаблонный параметр пример использования смотри  TestCheckBox
*/
template<class T>class RadioBtnWidget : public T, public TEvent
{
public:
	HWND hWnd;
public:
	RadioBtnWidget(){}
	template<class Z>RadioBtnWidget(Z &z) : T(z){}
	/**
	* \brief выполнить метод в обработчике WM_CREATE
	* \param hOwner - смотри winapi
	* \param txt - текст около кнопки
	* \param bool T::Init(HWND) - инициировать состояние чекбокса(true - галочка включена, false - отключена)
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
	* \brief выполнить в обработчике WM_SIZE
	*/
	void Size(int left, int top, int width, int height)
	{
		MoveWindow(hWnd, left, top, width, height, true);
	}
//private:
	/**
	* \brief выполняется при снятии или установки галочки 
	* \param static void T::Command() - определить в шаблонном параметре
	*/
	void Do(TCommand &m)
	{
		T::Command(m);
	}
};

