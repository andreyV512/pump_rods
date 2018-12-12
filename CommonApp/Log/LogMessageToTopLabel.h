#pragma once
#include <Windows.h>

class LogMessageToTopLabel
{
	HANDLE hTimer;
	void Do();
	static VOID CALLBACK WaitOrTimerCallback(PVOID lpParameter, BOOLEAN TimerOrWaitFired);
public:
	LogMessageToTopLabel();
	void Run();
	void Stop();
};