#pragma once 

class App
{
public:
	App();
	void Destroy();
	static const wchar_t *Salt(){return L"Mess";};
};