#pragma once 

template<class>struct On{};
template<class>struct Off{};

class App
{
public:
	App();
	void Destroy();
	static const wchar_t *Salt(){return L"Mess";};
};