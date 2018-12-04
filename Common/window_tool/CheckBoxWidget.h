#pragma once
#include "window_tool/message.h"
#include "TEvent.h"
#include "tools_debug\DebugMess.h"

/**
* \brief Шаблон для чекбокса 
* \param T - шаблонный параметр пример использования смотри  TestCheckBox
*/
template<class T>class CheckBoxWidget : public T, public TEvent
{
	HWND hWnd;
public:
	CheckBoxWidget(){}
	template<class Z>CheckBoxWidget(Z &z) : T(z){}
	/**
	* \brief выполнить метод в обработчике WM_CREATE
	* \param hOwner - смотри winapi
	* \param txt - текст около кнопки
	* \param bool T::Init(HWND) - инициировать состояние чекбокса(true - галочка включена, false - отключена)
	*/
	void Init(HWND hOwner, const wchar_t *txt)
	{
		hWnd = CreateWindow(L"button", txt
			, WS_CHILD | WS_VISIBLE | BS_AUTOCHECKBOX | WS_TABSTOP
			, 0, 0, 0, 0, hOwner, NULL, GetModuleHandle(NULL), NULL
			);
		SetWindowLongPtr(hWnd, GWLP_USERDATA, (LONG)this);
		Button_SetCheck(hWnd, T::Init(hWnd) ? BST_CHECKED : BST_UNCHECKED);
	}
	/**
	* \brief выполнить в обработчике WM_SIZE
	*/
	void Size(int left, int top, int width, int height)
	{
		MoveWindow(hWnd, left, top, width, height, true);
	}
	bool GetCheck(){return  BST_CHECKED == Button_GetCheck(hWnd);}
	void SetCheck(bool b){Button_SetCheck(hWnd, b ? BST_CHECKED : BST_UNCHECKED);}
private:
	/**
	* \brief выполняется при снятии или установки галочки 
	* \param static void T::Command(bool) - определить в шаблонном параметре
	*/
	void Do(TCommand &m)
	{
		T::Command(m, BST_CHECKED == Button_GetCheck(m.hControl));
	}
};

class CheckBoxHandler
{
	struct TObj{};
	TObj *o;
	void(TObj::*ptr)(bool);
protected:
	void Command(TCommand &m, bool b)
	{
		if(o && ptr) (o->*ptr)(b);
	}
	bool Init(HWND h)
	{
		bool b = false;
		if(o && ptr) (o->*ptr)(b);
		return b;
	}
public:
	CheckBoxHandler(): o(NULL), ptr(NULL){}
	template<class T>void SetHandler(T *t, void(T::*p)(bool))
	{
		o = (TObj *)t;
		ptr = (void(TObj::*)(bool))p
	}
};