#pragma once
#include "L502\Device502.h"
#include "DataItem\DataItem.h"
#include "App\App.h"

template<class T>struct l502Run;
template<class T, template<class>class W>struct l502Run<W<T>>
{
	enum
	{
		ok
		, thread_error
		, buffer_overlapped
		, l502_read_error
	}status;
	int &currentOffset;
	bool terminate;
	HANDLE hThread;
	double (&inputData)[App::buffer_size];
	l502Run(T &t)
		: currentOffset(t.currentOffset)
		, terminate(false)
		, inputData(t.inputData)
		, status(ok)
		, hThread(NULL)
	{
		currentOffset = 0;
	}
	~l502Run()
	{
		terminate = true;
		if(NULL != hThread)WaitForSingleObject(hThread, INFINITE);
	}

	static DWORD WINAPI Proc(PVOID p)
	{
		((l502Run *)p)->Do();
		return 0;
	}
	
	unsigned operator()()
	{
		L502ParametersTable::TItems &param = Singleton<L502ParametersTable>::Instance().items;
		bool b = l502.SetupParams(
			&param.get<W<ChannelL502>>().value
			, &param.get<W<RangeL502>>().value
			, param.get<W<ChannelSamplingRate>>().value
			, 1
			); 
		if(b)
		{
			hThread = CreateThread(NULL, 0, Proc, this, 0, NULL);
		}
		else
		{
			return thread_error;
		}
		return ok;
	};

	void Do()
	{
		l502.Start();
		int err = 0;
		unsigned countData = 0;
		while(!terminate)
		{
			unsigned startChannel;
			if(0 == (err = l502.Read(startChannel, &inputData[currentOffset], countData)))
			{
				currentOffset += countData;
				if(currentOffset > dimention_of(inputData) - Device502::buffer_length)
				{
					status = buffer_overlapped;
					terminate = true;
					dprint("read ok %d\n", currentOffset);
				}
			}
			else
			{
				status = l502_read_error;
				terminate = true;
				dprint("read stop %d\n", err);
			}
		}
		l502.Stop();
	}
};