#pragma once

template<class T, class Table>struct OnCheckBox
{
protected:
	void Command(TCommand &m, bool b)
	{
		Singleton<Table>::Instance().items.get<T>().value = b;
		CBase base(ParametersBase().name());
		if(base.IsOpen())
		{
			UpdateWhere<Table>(Singleton<Table>::Instance(), base).ID(1).Execute();
		}
	}
	bool Init(HWND h)
	{		
		return Singleton<Table>::Instance().items.get<T>().value;
	}
};